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

package cn.inet.node;
import cn.inet.mobility.contract.IMobility;
import cn.inet.application.IApp;
import cn.inet.linklayer.contract.IHostMac;

module Host
{
    parameters:
        @display("i=device/uav_s");

        //mobility module
        string mobilityType = default("StationaryMobility");
        //application layer
        string appType = default("BurstTraffic");
        //mac layer
        string macType = default("TdmaHostMac");

        @networkNode();

    gates:
        input in @directIn;

    submodules:
        mobility: <mobilityType> like IMobility;
        app: <appType> like IApp;
        mac: <macType> like IHostMac {

            @display("p=117,111");
        }
    connections:
        in --> mac.inFromHost;
        app.out --> mac.inFromApp;
}
