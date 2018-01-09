/*
 * DaraApMac.h
 *
 *  Created on: 2017Äê12ÔÂ12ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_DARA_DARAAPMAC_H_
#define INET_LINKLAYER_DARA_DARAAPMAC_H_

#include "inet/common/INETDefs.h"
#include "inet/common/PkKinds.h"
#include "inet/linklayer/dara/DaraHostPk_m.h"
#include "inet/linklayer/dara/DaraApPk_m.h"
#include <vector>
#include <set>
#include "inet/linklayer/common/ApMacBase.h"
using namespace std;

namespace inet
{

struct DaraHostPkInfo
{
    long pkId;
    int slotId;
    bool isClean;
};
class DaraApMac:public ApMacBase
{
private:
    int m_AckSlotNum;
    vector<DaraHostPkInfo> HostPksInfoVector;
    vector<DaraHostPkInfo> CleanPksInfoVector;

    set<int> PkIdSet;
    set<string> HostPathSet;

    long m_TotalReceivedPk=0;
    long m_RecoveriedPk=0;
    double m_Sdr=0;
    cMessage *m_SelfMsgHandleReq;

    simsignal_t m_PkTotalCountSignal;
    simsignal_t m_PkRecoveryCountSignal;
    simsignal_t m_SdrSignal;


protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleNonSelfMsg(cMessage *msg);
    void handleSelfMsg(cMessage *msg);
    void handleHostData();
    void setPkClean(int pkId);
    bool isPkCleanInThisSlot(int slotId,int pkId);
    void sendAck();
    void finish() override;

public:
    DaraApMac();
    ~DaraApMac();
};
}



#endif /* INET_LINKLAYER_DARA_DARAAPMAC_H_ */
