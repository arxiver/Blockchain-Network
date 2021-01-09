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

#ifndef __BLOCKCHAIN_NODE_H_
#define __BLOCKCHAIN_NODE_H_

#define NETWORK_READY_INTERVAL 0.6
#define TIMEOUT_INTERVAL 0.7
#define MODIFIABLE true
#define DELAYABLE false
#define LOSSABLE false
#define DUPLICTABLE false

#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <bitset>
#include <iostream>
#include <fstream>
#include "MyMessage_m.h"
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{

  protected:
    int windowSize;             // a.k.a. MAX_SEQ (2^m - 1)
    int peerIndex;              // Index of the peer of this node.
    bool InitConnection;        // Does this node has peer and would it start connection
    int nextFrameToSend;        // a.k.a. "S" out-bound stream, next frame outgoing
    int ackExpected;            // a.k.a. "SF" oldest frame unacknowledged
    int framExpected;           // a.k.a. "R" next frame expected on in-bound stream
    std::vector<std::string> buffer; // buffer array o messages
    int nBuffered;              // number of buffered packets/frames
    int fileIterator;
    bool iTerminate;
    bool peerTerminate;
    // Statistics variables
    int retransmittedCount = 0;
    int droppedCount = 0;
    int generatedCount = 0;
    int usefulSentCount = 0;
    bool reinitialize = false;
    std::vector<std::string> messages; // read all message from the file and hold inside
    std::unordered_map<int, cMessage*> timers;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void organize();
    void findMyPeer();
    std::string randString();
    MyMessage_Base* makeMessage(std::string s, bool modifiable, bool isLastMessage);
    std::vector<std::string> split (const std::string &s);
    std::string join(std::vector<std::string> vec);
    bool between(int a,int b,int c);
    void increment(int & a);
    unsigned char parityBits(const char * string);
    bool checkError(const char * string,const bits& checkBits);
    bool modification(std::string &mypayload, bool modifiable);
    void sendData(MyMessage_Base *msg, int dest, bool delayable, bool lossable, bool duplictable);
    void readMessagesFile();
    void clearTimeoutEvents();
    void printStatistics();
    void printStatisticsGeneral();
    void printState(std::string state,std::string msg);
    std::string byteStuffing(std::string s);
    std::string byteDestuffing(std::string s);
};


#endif
