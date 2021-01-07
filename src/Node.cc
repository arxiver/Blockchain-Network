#include "Node.h"

Define_Module(Node);

unsigned char Node::parityBits(const char *string)
{

    unsigned char checkBits = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        checkBits = checkBits ^ (unsigned char)string[i];
    }
    return checkBits;
}

bool Node::checkError(const char *string, const bits &checkBits)
{
    unsigned char checkChar = parityBits(string);
    unsigned long i = checkBits.to_ulong();
    unsigned char c = static_cast<unsigned char>(i);
    return c == checkChar;
}

std::string Node::randString(){
    int MAX_MSG_SIZE = 10;
    int ASCII_START = 97; // start at 65 for more range
    int ASCII_END = 122; // end at 128 for including symbols
    int msgSize = uniform(0,MAX_MSG_SIZE) + 3;
    std::string msg = "";
    for(int i=0;i<msgSize;++i){
        msg += (char)(int)uniform(ASCII_START,ASCII_END);
    }
    return msg;
}

std::vector<std::string> Node::split (const std::string &s) {
    std::vector<std::string> result;
    char delim = ' ';
    std::stringstream ss (s);
    std::string item;
    while (getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}

std::string Node::join(std::vector<std::string> vec){
    std::string joined = "";
    for(int i=0;i<vec.size(); i++){
        joined += vec[i];
        if(i != vec.size()-1){
            joined += " ";
        }
    }
    return joined;
}

void Node::organize(){

    //getParentModule()->par("peers").setStringValue("hello World");
    //std::string peersStr = getParentModule()->par("peers").stringValue();
    // std::vector<std::string> peers = split(peersStr);
    int n = getParentModule()->par("n").intValue();
    n = n%2 ? n-1 : n;
    std::vector<int> temp;
    for (int i=0;i<n;++i){
        temp.push_back(i);
    }
    std::vector<std::string> vec;
    for (int i=0;i<n;++i){
        int pick = (int)(uniform(0,temp.size()) + rand()) % temp.size();
        vec.push_back(std::to_string(temp[pick]));
        temp.erase(temp.begin()+pick,temp.begin()+pick+1);
        //EV<<vec[i]<<endl;
    }
    std::string peers = join(vec);
    getParentModule()->par("peers").setStringValue(peers);
}

void Node::findMyPeer(){
    peerIndex = -1;
    InitConnection = false;
    int n = getParentModule()->par("n").intValue();
    n = n%2 ? n-1 : n;
    std::string peersStr = getParentModule()->par("peers").stringValue();
    std::vector<std::string> peers = split(peersStr);
    for(int i=0;i<n-1;i+=2){
        if(peers[i] == std::to_string(getIndex())){
            peerIndex = std::atoi(peers[i+1].c_str());
            InitConnection = true;
        }
        else if(peers[i+1] == std::to_string(getIndex())){
            peerIndex = std::atoi(peers[i].c_str());
        }
    }
}

void Node::initialize()
{
    if (getIndex()==0){
        organize();
    }
    findMyPeer();
    EV<<getIndex()<<", "<<peerIndex<<endl;
    peerIndex = peerIndex > getIndex() ? peerIndex-1 : peerIndex;
    if (peerIndex == -1) return;

     //EV << ". Scheduled a new packet after " << interval << "s";
     //scheduleAt(simTime() + interval, new cMessage(""));
    //double interval1 = uniform(0,0.999); //exponential(1 / par("lambda").doubleValue());
    double interval = uniform(0,0.999); //exponential(1 / par("lambda").doubleValue());
    //double interval = InitConnection? 0.5 : 1;
    //EV <<getIndex()<<" "<<simTime() <<endl;
    //time
    scheduleAt(simTime() + interval, new cMessage("network"));
    windowSize = (1 << (int)par("m")) - 1;
    nextFrameToSend = 0;
    ackExpected = 0;
    framExpected = 0;
    nBuffered = 0;
    fileIterator = 0;
    terminate = false;
    readMessagesFile();
}

void Node::handleMessage(cMessage *msg)
{

    if(terminate) {
        clearTimeoutEvents();
        return;
    }
    if (msg->isSelfMessage())
    {
        if(!(strcmp(msg->getName(),"network"))){
            // TODO add framing here and rest of functionality,
            // make it as a wrapper function like modification.
            // Options, randString(); // std::to_string(nextFrameToSend);
            // std::string s = randString(); //std::to_string(nextFrameToSend);
            std::string s = messages[fileIterator];
            s = byteStuffing(s);
            modification(s, false);
            nBuffered++;
            fileIterator++;
            buffer.push_back(s);
            terminate = fileIterator == (messages.size()-1);
            MyMessage_Base *sendMsg = makeMessage(s, terminate);
            sendData(sendMsg, peerIndex, true);
            increment(nextFrameToSend);
        }
        else if(!(strcmp(msg->getName(), "timeout"))){
            nextFrameToSend = ackExpected;
            for(int i=0; i<nBuffered; ++i){
                MyMessage_Base *sendMsg = makeMessage(buffer[i], false);
                sendData(sendMsg, peerIndex, true);
                increment(nextFrameToSend);
            }
        }
    }
    else {
        MyMessage_Base *receivedMsg = check_and_cast<MyMessage_Base *>(msg);
        // if there is no error acknowledge it else discard it
        // checkError i.e. (if it error free)
        if (receivedMsg->getMType() == 1){
            terminate = true;
        }
        bool isErrorFree = checkError(receivedMsg->getMPayload(), receivedMsg->getCheckBits());
        if (isErrorFree && framExpected == receivedMsg->getSeqNum()){
            //EV<<"Received correctly"<<endl;
            byteDestuffing(receivedMsg->getMPayload());
            increment(framExpected);
            while(between(ackExpected,receivedMsg->getAck(),nextFrameToSend)){
                nBuffered--;
                buffer.erase(buffer.begin(),buffer.begin()+1);
                if (timers[ackExpected] != nullptr){
                    cancelAndDelete(timers[ackExpected]);
                    timers[ackExpected] = nullptr;
                }
                increment(ackExpected);
            }
.        }

    }
    if(nBuffered < windowSize){
        double interval = uniform(0,0.999);//exponential(1 / par("lambda").doubleValue());
        EV << ". Scheduled a new packet after " << interval << "s";
        scheduleAt(simTime() + interval, new cMessage("network"));
        //scheduleAt(simTime() + NETWORK_READY_INTERVAL, new cMessage("network"));
    }
    EV<<"---------------------------------"<<endl;
    EV<<"Node: "<< getIndex() <<","<<endl;
    EV<<"nextFrameToSend: "<< nextFrameToSend<<","<<endl;
    EV<<"ackExpected: "<< ackExpected<<","<<endl;
    EV<<"framExpected: "<< framExpected<<","<<endl;
    EV<<"nBuffered: "<< nBuffered<<","<<endl;
    EV<<"---------------------------------"<<endl;
}

MyMessage_Base* Node::makeMessage(std::string s, bool isLastMessage){
    MyMessage_Base *msg = new MyMessage_Base(s.c_str());
    msg->setSeqNum(nextFrameToSend);
    msg->setAck((framExpected+windowSize)%(windowSize+1));
    msg->setMPayload(s.c_str());
    msg->setMType(0);
    if (isLastMessage)
        msg->setMType(1);
    msg->setCheckBits(std::bitset<8>(parityBits(s.c_str())));
    if (timers[nextFrameToSend] != nullptr){
        cancelAndDelete(timers[nextFrameToSend]);
        timers[nextFrameToSend] = nullptr;
    }
    timers[nextFrameToSend] = new cMessage("timeout");
    scheduleAt(simTime() + TIMEOUT_INTERVAL, timers[nextFrameToSend]);
    return msg;
}

void Node::increment(int &a){
    // circular(x-1) =(x>0) ? (x--) : MAX_SEQ
    a = (a+1) % (windowSize+1);
    return;
}

bool Node::between(int a,int b,int c){
 // return true if a<=b<c circular; false otherwise
    return (((a<=b)&&(b<c)) || ((c<a)&&(a<=b)) || ((b<c) && (c<a)));
}

void Node::sendData(MyMessage_Base *msg, int dest, bool Pdelay){
    //first check whether to send or not  (loss)

    double rand =  uniform(0, 1) * 10;
    if(rand< par("lossRand").doubleValue()){
        EV << "Message lost OMG " << endl;
        return; //don't send anything
    }
    //(duplicate)
    bool dup = false;
    rand = uniform(0, 1) * 10;
    if(rand<par("duplicateRand").doubleValue()){
        EV << "Duplicate happened " << endl;
        dup = true;
    }
    // P(delay): [boolean] probability of delaying exist
    int delayRand = uniform(0, 1) * 10;
    if (delayRand >= par("delayRand").doubleValue() && Pdelay)
    {
        EV << "delaying message with "<< TIMEOUT_INTERVAL + 0.1 << " seconds " << endl;
        sendDelayed(msg, TIMEOUT_INTERVAL + 0.1, "outs", dest);
        if(dup)
            sendDelayed(msg->dup(), TIMEOUT_INTERVAL + 0.1 , "outs", dest);
    }
    else
    {
        send(msg, "outs", dest);
        if(dup)
          send(msg->dup(), "outs", dest);
    }
}

bool Node::modification(std::string &mypayload, bool Pmodify){
    // P(modify): [boolean] probability of modification exist
    int modificationRand = uniform(0, 1) * 10;
    if (modificationRand >= par("modificationRand").doubleValue() && Pmodify)
    {
        int randBit = uniform(0, 7); // random bit in a char
        unsigned char oneBitRandom = std::pow(2, randBit);
        int randByte = uniform(0, mypayload.length()); // random char

        mypayload[randByte] = (unsigned char)mypayload[randByte] ^ oneBitRandom;
        EV << "modifying message, modified bit = " << std::to_string(randBit) << ", modified char = " << std::to_string(randByte) << endl;
        return true;
    }
    return false;
}

void Node::readMessagesFile(){
    std::ifstream myReadFile(std::to_string(getIndex())+".txt");
    std::string msg;
    while (getline (myReadFile, msg)) {
        messages.push_back(msg);
    }
    myReadFile.close();
}

void Node::clearTimeoutEvents(){
    for (auto & t : timers){
        if (timers[t.first] != nullptr){
            cancelAndDelete(timers[t.first]);
            timers[t.first] = nullptr;
        }
    }
    return;
}

std::string Node::byteStuffing(std::string s)
{
    std::string result = "";
    char flag = 'f';
    char escape = 'e';

    for(int i=0;i<s.length();i++)
    {
        if(s[i]==flag||s[i]==escape)
            result+=escape;
        result+=s[i];
    }
    return result;
}

std::string Node::byteDestuffing(std::string s)
{
    std::string result = "";
    char escape = 'e';

    for(int i=0;i+1<s.length();i++)
    {
        if(s[i]==escape)
            i++;
        result+=s[i];
    }
    return result;
}

void Node::writeStatistics(){

}

void Node::gatherStatistics(){
/*  implementation for one statistic gathering function that
    calculates and prints the following for all the system nodes during a

    // schedule at (simTime() + 3 minutes)
    simulation run of the period (3 minutes):
    // Number of generated frames (would be same as text size of all sent messages size)
    � The total number of generated frames.
    // Number of dropped frames would be
    � The total number of dropped frames.
    // Number of retransmission (time out sent frames)
    � The total number of retransmitted frames.
*/
}

/*
 *
 *    double interval = exponential(1 / par("lambda").doubleValue());
      EV << ". Scheduled a new packet after " << interval << "s";
      scheduleAt(simTime() + interval, new cMessage(""));
 *
 *
 */


