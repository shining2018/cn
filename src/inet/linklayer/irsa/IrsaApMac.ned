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

package cn.inet.linklayer.irsa;
import cn.inet.linklayer.common.ApMacBase;

simple IrsaApMac extends ApMacBase
{
	parameters:
	    //double slotLength @unit("s");
	    //int slotNum;
	    @display("i=block/ifcard");
	    	    
	    //统计
	    //统计总共接收到的数据包数
	    //@signal[pkTotalCount](type=long);
	    //@statistic[pkTotalCount](title="pkTotalCount";source=pkTotalCount;record=count;interpolationmode=node);
	
	    
	    @class(IrsaApMac);	
	//gates:
	    //input inFromAp;
}
