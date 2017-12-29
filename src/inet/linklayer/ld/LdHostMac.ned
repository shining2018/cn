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
import cn.inet.linklayer.common.HostMacBase;

simple LdHostMac extends HostMacBase
{
	parameters:
	    @display("i=block/ifcard");
	    
	    int reqSlotNum;

	    //统计
//	    //成功发送出去的包平均排队时间
//	    @signal[queueTime](type=simtime_t);
//	    @statistic[queueTime](title="queueTime";source=queueTime;record=mean);
//	    //丢弃的包的个数
//	    @signal[discardPkCount](type=long);
//	    @statistic[discardPkCount](title="discardPkCount";source=discardPkCount;record=count;interpolationmode=node);	    
//	    //统计发送了多少个数据包
//	    @signal[sentPkCount](type=long);
//	    @statistic[sentPkCount](title="pkCount";source=sentPkCount;record=count;interpolationmode=node);

	    //记录在各个位置呆的时间
	    @signal[totaltime](type=simtime_t);
	    @statistic[totaltime](title="totaltime";source=totaltime;record=count);
	    @signal[time1](type=simtime_t);
	    @statistic[time1](title="time1";source=time1;record=count);
	    @signal[time2](type=simtime_t);
	    @statistic[time2](title="time2";source=time2;record=count);
	    @signal[time3](type=simtime_t);
	    @statistic[time3](title="time3";source=time3;record=count);
	    @signal[time4](type=simtime_t);
	    @statistic[time4](title="time4";source=time4;record=count);
	    @signal[time5](type=simtime_t);
	    @statistic[time5](title="time5";source=time5;record=count);
	    @signal[time6](type=simtime_t);
	    @statistic[time6](title="time6";source=time6;record=count);
	    @signal[time7](type=simtime_t);
	    @statistic[time7](title="time7";source=time7;record=count);
	    @signal[time8](type=simtime_t);
	    @statistic[time8](title="time8";source=time8;record=count);
	    @signal[time9](type=simtime_t);
	    @statistic[time9](title="time9";source=time9;record=count);
	    @signal[time10](type=simtime_t);
	    @statistic[time10](title="time10";source=time10;record=count);
	    
	    @signal[retransmitTimes](type=long);
	    @statistic[retransmitTimes](title="retransmitTimes";source=retransmitTimes;record=vector);
	    @signal[retransmitTimes1](type=long);
	    @statistic[retransmitTimes1](title="retransmitTimes1";source=retransmitTimes1;record=vector);
	    @signal[retransmitTimes2](type=long);
	    @statistic[retransmitTimes2](title="retransmitTimes2";source=retransmitTimes2;record=vector);
	    @signal[retransmitTimes3](type=long);
	    @statistic[retransmitTimes3](title="retransmitTimes3";source=retransmitTimes3;record=vector);
	    @signal[retransmitTimes4](type=long);
	    @statistic[retransmitTimes4](title="retransmitTimes4";source=retransmitTimes4;record=vector);
	    @signal[retransmitTimes5](type=long);
	    @statistic[retransmitTimes5](title="retransmitTimes5";source=retransmitTimes5;record=vector);
	    @signal[retransmitTimes6](type=long);
	    @statistic[retransmitTimes6](title="retransmitTimes6";source=retransmitTimes6;record=vector);
	    @signal[retransmitTimes7](type=long);
	    @statistic[retransmitTimes7](title="retransmitTimes7";source=retransmitTimes7;record=vector);
	    @signal[retransmitTimes8](type=long);
	    @statistic[retransmitTimes8](title="retransmitTimes8";source=retransmitTimes8;record=vector);
	    @signal[retransmitTimes9](type=long);
	    @statistic[retransmitTimes9](title="retransmitTimes9";source=retransmitTimes9;record=vector);
	    @signal[retransmitTimes10](type=long);
	    @statistic[retransmitTimes10](title="retransmitTimes10";source=retransmitTimes10;record=vector);			    
	    @class(LdHostMac);
}
