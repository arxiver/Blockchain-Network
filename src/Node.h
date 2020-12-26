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

#include <omnetpp.h>
#include <vector>
#include "MyMessage_m.h"
using namespace omnetpp;
typedef enum {frame_arrival, cksum_err, timeout, network_layer_ready} event_type;
#include <bitset>
#include "MyMessage_m.h"
typedef std::bitset<8> bits;

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{

  protected:
    int w;
    // REPLACE IT WITH S,SF,SL
    std::vector<int> ack;
    std::vector<int> nextFrame;
    // REPLACE IT WITH VECTOR OF STRINGS
    std::vector<std::vector<MyMessage_Base> > windowFrame;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    unsigned char parityBits(const char * string);
    bool checkError(const char * string,const bits& checkBits);
};



#endif
