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

package cn.inet.linklayer.ld;
import cn.inet.linklayer.common.ApMacBase;

simple LdApMac extends ApMacBase
{
	parameters:
	    @display("i=block/ifcard");
	    int reqSlotNum;
	    //int slotNum;
	    //double slotLength @unit("s");
	    //double communicationRange @unit(m);
	    
	    //统计
	    //统计总共接收到的数据包数
	    @signal[pkTotalCount](type=long);
	    @statistic[pkTotalCount](title="pkTotalCount";source=pkTotalCount;record=count;interpolationmode=node);
	    //统计距离标志分别1~10的数据包数
	    @signal[pkCount1](type=long);
	    @statistic[pkCount1](title="pkCount1";source=pkCount1;record=count;interpolationmode=node);
	    
	    @signal[pkCount2](type=long);
	    @statistic[pkCount2](title="pkCount2";source=pkCount2;record=count;interpolationmode=node);
	    
	    @signal[pkCount3](type=long);
	    @statistic[pkCount3](title="pkCount3";source=pkCount3;record=count;interpolationmode=node);
	    
	    @signal[pkCount4](type=long);
	    @statistic[pkCount4](title="pkCount4";source=pkCount4;record=count;interpolationmode=node);
	    
	    @signal[pkCount5](type=long);
	    @statistic[pkCount5](title="pkCount5";source=pkCount5;record=count;interpolationmode=node);
	    
	    @signal[pkCount6](type=long);
	    @statistic[pkCount6](title="pkCount6";source=pkCount6;record=count;interpolationmode=node);
	    
	    @signal[pkCount7](type=long);
	    @statistic[pkCount7](title="pkCount7";source=pkCount7;record=count;interpolationmode=node);
	    
	    @signal[pkCount8](type=long);
	    @statistic[pkCount8](title="pkCount8";source=pkCount8;record=count;interpolationmode=node);
	    
	    @signal[pkCount9](type=long);
	    @statistic[pkCount9](title="pkCount9";source=pkCount9;record=count;interpolationmode=node);
	    
	    @signal[pkCount10](type=long);
	    @statistic[pkCount10](title="pkCount10";source=pkCount10;record=count;interpolationmode=node);
	    @class(LdApMac);
	    
	//gates:
	    //input inFromAp;
}