/*
 * TdmaHostMac.h
 *
 *  Created on: 2017年6月1日
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_TDMA_TDMAHOSTMAC_H_
#define INET_LINKLAYER_TDMA_TDMAHOSTMAC_H_

#include "inet/linklayer/common/HostMacBase.h"
#include "inet/linklayer/tdma/TdmaReq_m.h"
#include "inet/linklayer/tdma/TdmaACK_m.h"

namespace inet
{
class TdmaHostMac:public HostMacBase
{
private:
    //发送请求消息的时隙个数
    int m_ReqSlots;
    //随机数，判断是否丢包
    double m_Random;

    /*
     * 自消息：
     * m_SelfMsgReq：发送请求包
     * m_SelfMsgData：发送数据包
     */
    cMessage *m_SelfMsgReq;
    cMessage *m_SelfMsgData;

    /*
     * Ap ACK消息内容：
     * m_dataSlotIndex：host发送数据的起始时隙
     * m_dataSlotNum：host发送数据的时隙个数
     */
    int m_dataSlotIndex;
    int m_dataSlotNum;

    simtime_t m_QueueTime;

protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    void sendReq();
    void sendData();

public:
    TdmaHostMac();
    ~TdmaHostMac();
};
}



#endif /* INET_LINKLAYER_TDMA_TDMAHOSTMAC_H_ */
