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
#include <bitset>
#include "MyMessage_m.h"
typedef std::bitset<8> bits;

using namespace omnetpp;
unsigned char CheckBits(const char * string);
bool CheckError(const char * string,const bits& checkBits);

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};



#endif
