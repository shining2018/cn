/*
 * LdApMac.h
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_LD_LDAPMAC_H_
#define INET_LINKLAYER_LD_LDAPMAC_H_
#include "inet/common/INETDefs.h"
#include "inet/common/PkKinds.h"
#include "inet/linklayer/ld/LdReq_m.h"
#include "inet/linklayer/ld/LdACK_m.h"
#include <vector>
#include "inet/linklayer/common/ApMacBase.h"
using namespace std;

namespace inet
{
class LdApMac: public ApMacBase
{
private:
    //parameter
    int m_ReqSlots;
    //int m_SlotNum;
    //simtime_t m_SlotLength;
    //double m_CommunicationRange;
    int m_MaxPropagationSlots;

    vector<LdReq *> m_LdReqVector;
    vector<LdReq *> m_LdCleanReqVector;
    cMessage *m_SelfMsgACK;

    //统计量
    simsignal_t m_PkTotalCountSignal;
    //信号量，统计收到的数据包数，以及各个距离档收到的数据包
    simsignal_t m_PkCount1Signal;
    simsignal_t m_PkCount2Signal;
    simsignal_t m_PkCount3Signal;
    simsignal_t m_PkCount4Signal;
    simsignal_t m_PkCount5Signal;
    simsignal_t m_PkCount6Signal;
    simsignal_t m_PkCount7Signal;
    simsignal_t m_PkCount8Signal;
    simsignal_t m_PkCount9Signal;
    simsignal_t m_PkCount10Signal;

protected:
    void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    void sendACK();
    void handleReqRepetitions();
    void sortReqVec(vector<LdReq *> &vec);
    void outputVec(vector<LdReq *> &vec);
    int getMaxAvaSlots(int hostPropagationSlots);


public:
    LdApMac();
    ~LdApMac();

};
}


#endif /* INET_LINKLAYER_LD_LDAPMAC_H_ */
