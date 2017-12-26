/*
 * TdmaHostMac.h
 *
 *  Created on: 2017��6��1��
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
    //����������Ϣ��ʱ϶����
    int m_ReqSlots;
    //��������ж��Ƿ񶪰�
    double m_Random;

    /*
     * ����Ϣ��
     * m_SelfMsgReq�����������
     * m_SelfMsgData���������ݰ�
     */
    cMessage *m_SelfMsgReq;
    cMessage *m_SelfMsgData;

    /*
     * Ap ACK��Ϣ���ݣ�
     * m_dataSlotIndex��host�������ݵ���ʼʱ϶
     * m_dataSlotNum��host�������ݵ�ʱ϶����
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
