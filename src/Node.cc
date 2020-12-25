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
