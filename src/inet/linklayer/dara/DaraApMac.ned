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

package cn.inet.linklayer.dara;

import cn.inet.linklayer.common.ApMacBase;

simple DaraApMac extends ApMacBase
{
	parameters:
	    @display("i=block/ifcard");
	    int ackSlotNum;
	    
	    @signal[pkTotalCount](type=long);
	    @statistic[pkTotalCount](title="pkTotalCount";source=pkTotalCount;record=count;interpolationmode=node);
	    @signal[pkRecoveryCount](type=long);
	    @statistic[pkRecoveryCount](title="pkRecoveryCount";source=pkRecoveryCount;record=count;interpolationmode=node);
	    @signal[sdr](type=double);
	    @statistic[sdr](title="sdr";source=sdr;record=last;interpolationmode=node);
	    @class(DaraApMac);
}
