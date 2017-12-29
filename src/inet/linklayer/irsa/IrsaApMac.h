/*
 * IrsaApMac.h
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_IRSA_IRSAAPMAC_H_
#define INET_LINKLAYER_IRSA_IRSAAPMAC_H_

#include "inet/common/INETDefs.h"
#include "inet/common/PkKinds.h"
#include "inet/linklayer/irsa/IrsaHostPk_m.h"
#include <vector>
#include "inet/linklayer/common/ApMacBase.h"
using namespace std;

namespace inet
{
struct IrsaHostPkInfo
{
    long pkId;
    int slotId;
    bool isClean;
};
class IrsaApMac:public ApMacBase
{
private:
    //参数
    //int m_SlotNum;
    //simtime_t m_SlotLength;

    vector<IrsaHostPk *> HostPksVector;
    vector<IrsaHostPk *> CleanPksVector;

    vector<IrsaHostPkInfo> HostPksInfoVector;
    vector<IrsaHostPkInfo> CleanPksInfoVector;

    cMessage *m_SelfMsgHandleHostData;

    //统计量
    //simsignal_t m_PkTotalCountSignal;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleNonSelfMsg(cMessage *msg);
    void handleSelfMsg(cMessage *msg);
    void handleHostData();
    bool isPkClean(int pkID);
    bool isPkCleanInThisSlot(int slotID,int pkID);


public:
    IrsaApMac();
    ~IrsaApMac();

};
}


#endif /* INET_LINKLAYER_IRSA_IRSAAPMAC_H_ */
