//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"
#include "MyMessage_m.h"
#include <iostream>
#include <fstream>
Define_Module(Node);


unsigned char CheckBits(const char * string){


    unsigned char checkBits= 0;
    for (int i = 0; i <  strlen(string); i++)
    {
        checkBits = checkBits ^ (unsigned char)string[i];
    }
    return checkBits;
}

bool CheckError(const char * string,const bits& checkBits){
    unsigned char checkChar=CheckBits(string);
    unsigned long i = checkBits.to_ulong();
    unsigned char c = static_cast<unsigned char>( i );
    return c==checkChar;
}

void Node::initialize()
{
    int n = par("n");
    int w = (1 << (int)par("m")) - 1;
    for (int i=0;i<n;++i){
        ack.push_back(0);
        nextFrame.push_back(0);
    }

    //double interval = exponential(1 / par("lambda").doubleValue());
    //scheduleAt(simTime() + interval, new cMessage(""));
    //double interval = exponential(1 / par("lambda").doubleValue());
    scheduleAt(simTime() + 0.1, new cMessage(""));
    std::ifstream MyReadFile(std::to_string(getIndex())+".txt");
    std::string myText;
    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
      // Output the text from the file
      EV << myText;
    }

    // Close the file
    MyReadFile.close();


    double interval = exponential(1 / par("lambda").doubleValue());
    scheduleAt(simTime() + interval, new cMessage(""));
}

void Node::handleMessage(cMessage *msg)
{
    EV << ack[0] <<" "<< getIndex() <<"\n";
    return ;
    if (true) { //msg == timeoutEvent
        // timeout expired, re-send packet and restart timer
        //send(currentPacket->dup(), "out");
        //scheduleAt(simTime() + timeout, timeoutEvent);
    }
    else if (true) {  // if acknowledgment received
        //cancel timeout, prepare to send next packet, etc.
        //cancelEvent(timeoutEvent);
        //...
    }
    else {
        //...
    if (msg->isSelfMessage()) { //Host wants to send

        int rand, dest;
        do { //Avoid sending to yourself
            rand = uniform(0, gateSize("outs"));
        } while(rand == getIndex());

        //Calculate appropriate gate number
        dest = rand;
        if (rand > getIndex())
            dest--;

        std::stringstream ss;
        ss << rand;


        // take input from user
        //std::cout<<"Enter Payload";
        //std::string strtmp;
        //std::cin >> strtmp;
        //const char* string = strtmp.c_str();
        //

        //payload
        const char* string ="Hello World";




        EV << "Sending "<< ss.str() <<" from source " << getIndex() << "\n";

        // Message Data
        MyMessage_Base * msg_Base = new MyMessage_Base(ss.str().c_str());
        msg_Base->setSeqNum(1);
        msg_Base->setMPayload(string);
        msg_Base->setCheckBits(std::bitset<8> ( CheckBits(string)) );




        //modification
        int modificationRand=uniform(0,1)*10;
        if(modificationRand>=par("modificationRand").doubleValue()){
            int randBit=uniform(0,7);// random bit in a char
            unsigned char oneBitRandom = std::pow(2,randBit);

            std::string mypayload=msg_Base->getMPayload();

            int randByte=uniform(0,mypayload.length());// random char

            mypayload[randByte]=(unsigned char)mypayload[randByte]^oneBitRandom;
            msg_Base->setMPayload(mypayload.c_str());

            //EV<<"modificationRand is "<<std::to_string(modificationRand)<<endl;
            EV<<"modifying message, modified bit = "<<std::to_string(randBit)<<", modified char = " <<std::to_string(randByte)<<endl;

        }
        //




        //delay
        int delayRand=uniform(0,1)*10;
        if(delayRand>=par("delayRand").doubleValue()){
            //EV<<"delayRand is "<<std::to_string(delayRand)<<endl;
            EV<<"delaying message with 1 sec "<<endl;
            sendDelayed(msg_Base,1, "outs", dest);
        }
        else{
            send(msg_Base, "outs", dest);
        }
        delete msg;

        double interval = exponential(1 / par("lambda").doubleValue());
        EV << ". Scheduled a new packet after " << interval << "s";
        scheduleAt(simTime() + interval, new cMessage(""));
    }
    else {
        //atoi functions converts a string to int
        //Check if this is the proper destination
        MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
        if (atoi(mmsg->getName()) == getIndex()){

            if(CheckError(mmsg->getMPayload(),mmsg->getCheckBits()))
            {
                bubble("Message received with no error");
            }
            else
            {
                bubble("Message received WITH ERROR");
                EV <<"Noisy Message received = " << mmsg->getMPayload()<<endl;;
            }
        }
        else
            bubble("Wrong destination");
        delete mmsg;
    }

}

/*
 *
 static bool between(seq_nr a, seq_nr b, seq_nr c){
 // return true if a<=b<c circulary; false otherwise
 return (((a<=b)&&(b<c)) || ((c<a)&&(a<=b)) || ((b<c) && (c<a)));
 }
 *
 *
 * */
