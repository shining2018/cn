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
import cn.inet.linklayer.contract.IApMac;

simple ApMacBase like IApMac
{
	parameters:
	    @display("i=block/ifcard");
	    double slotLength @unit("s");
	    int slotNum;
	    double communicationRange @unit(m);
	    
	    @signal[pkCount](type=long);
	    @statistic[pkCount](title="pkCount";source=pkCount;record=count;interpolationmode=node);
	    
	    @class(ApMacBase);
	
	gates:
	    input inFromAp;
	    //output outToSink;
}
