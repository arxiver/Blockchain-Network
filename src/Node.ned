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

package blockchain;

//
// TODO auto-generated module
//
simple Node
{
    parameters:
        int m = default(2);
        double delayRand = default(1.0);
        double duplicateRand = default(1.0);
        double lossRand = default(1.0);
        double modificationRand = default(1.0);
        double TIMEOUT_INTERVAL = default(0.7);
        double STATS_INTERVAL = default(180);
        double REINIT_INTERVAL = default(30);
        double NETWORK_INTERVAL = default(0.5);
        bool MODIFIABLE = default(false);
        bool DELAYABLE = default(false);
        bool LOSSABLE = default(false);
        bool DUPLICTABLE = default(false);
    gates:
        input ins[];
        output outs[];
}
