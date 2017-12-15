/*
 * DaraHostMac.h
 *
 *  Created on: 2017Äê12ÔÂ12ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_DARA_DARAHOSTMAC_H_
#define INET_LINKLAYER_DARA_DARAHOSTMAC_H_
#include "inet/linklayer/common/HostMacBase.h"
#include "inet/linklayer/dara/DaraHostPk_m.h"
#include "inet/common/PkKinds.h"
#include <vector>
using namespace std;

namespace inet
{
class DaraHostMac : public HostMacBase
{
private:
    int m_RepetitionRate;
    int m_AckSlotNum;
    cMessage *m_SelfMsgTimer;

protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    int getRepetitionNum();
    void sendData();

public:
    DaraHostMac();
    ~DaraHostMac();

};
}



#endif /* INET_LINKLAYER_DARA_DARAHOSTMAC_H_ */
