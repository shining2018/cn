/*
 * LdHostMac.h
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_LD_LDHOSTMAC_H_
#define INET_LINKLAYER_LD_LDHOSTMAC_H_
#define MAX_REQUEST_SLOTS 2000
#include "inet/linklayer/common/HostMacBase.h"
#include "inet/common/PkKinds.h"
#include "inet/linklayer/ld/LdReq_m.h"
#include "inet/linklayer/ld/LdACK_m.h"
#include <vector>
using namespace std;

namespace inet
{
class LdHostMac:public HostMacBase
{
private:
    int m_ReqSlots;
    //indicate weather to send a request packet
    bool m_IsReadyToRequest=true;

    int m_DataSlotIndex=-1;
    int m_DataSlotNum=-1;

    double m_Random;

    simtime_t m_DataSendTime;

    cMessage *m_SelfMsgReq;
    cMessage *m_SelfMsgReqPks;
    cMessage *m_SelfMsgData;
    cMessage *m_SelfMsgTimer;

    //统计量
//    simsignal_t m_DiscardPkCountSignal;
//    simsignal_t m_SentPkCountSignal;
//    simsignal_t m_QueueTimeSignal;
    simtime_t m_Queuetime;
    //在某个区域的时间
    simtime_t m_Time;
    simsignal_t m_TotalTimeSignal;
    simsignal_t m_Time1Signal;
    simsignal_t m_Time2Signal;
    simsignal_t m_Time3Signal;
    simsignal_t m_Time4Signal;
    simsignal_t m_Time5Signal;
    simsignal_t m_Time6Signal;
    simsignal_t m_Time7Signal;
    simsignal_t m_Time8Signal;
    simsignal_t m_Time9Signal;
    simsignal_t m_Time10Signal;

    simsignal_t m_RetransmitTimesSignal;
    simsignal_t m_RetransmitTimesSignal1;
    simsignal_t m_RetransmitTimesSignal2;
    simsignal_t m_RetransmitTimesSignal3;
    simsignal_t m_RetransmitTimesSignal4;
    simsignal_t m_RetransmitTimesSignal5;
    simsignal_t m_RetransmitTimesSignal6;
    simsignal_t m_RetransmitTimesSignal7;
    simsignal_t m_RetransmitTimesSignal8;
    simsignal_t m_RetransmitTimesSignal9;
    simsignal_t m_RetransmitTimesSignal10;

    simtime_t m_TimeTotal;
    simtime_t m_Time1;
    simtime_t m_Time2;
    simtime_t m_Time3;
    simtime_t m_Time4;
    simtime_t m_Time5;
    simtime_t m_Time6;
    simtime_t m_Time7;
    simtime_t m_Time8;
    simtime_t m_Time9;
    simtime_t m_Time10;

    long m_RetransmitTimes;


protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    void sendReq();
    void sendData();
    int getRepetitionNum();
    int setDistanceFlag();
    virtual void finish() override;

public:
    LdHostMac();
    ~LdHostMac();
};
}


#endif /* INET_LINKLAYER_LD_LDHOSTMAC_H_ */
