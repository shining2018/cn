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

package cn.inet.linklayer.common;
import cn.inet.linklayer.contract.IHostMac;

simple HostMacBase like IHostMac
{
    parameters:
        @display("i=block/ifcard");
        //transmitting rate
        double txRate @unit(bps);
        //length of each slot
        double slotLength @unit("s");
        //number of slots in one frame
        int slotNum;
        //communication range
        double communicationRange @unit(m);
        
        //signal & statistic
        @signal[sentPkCount](type=long);
        @statistic[sentPkCount](title="Count of sent packets";source=sentPkCount;record=count;interpolationmode=node);
        @signal[discardPkCount](type=long);
        @statistic[discardPkCount](title="Count of Discarded packets";source=discardPkCount;record=count;interpolationmode=node);
        @signal[schedulingTime](type=simtime_t);
        @statistic[schedulingTime](title="Scheduling time";source=schedulingTime;record=mean);
	    
	    @class(HostMacBase);
	
	gates:
	    input inFromApp;
	    input inFromHost;
}
