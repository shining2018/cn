/*
 * DaraIrsaHostMac.h
 *
 *  Created on: 2017Äê12ÔÂ27ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_DARA_DARAIRSAHOSTMAC_H_
#define INET_LINKLAYER_DARA_DARAIRSAHOSTMAC_H_

#include "inet/linklayer/common/HostMacBase.h"
#include "inet/linklayer/dara/DaraHostPk_m.h"
#include "inet/common/PkKinds.h"
#include <vector>
using namespace std;

namespace inet
{
class DaraIrsaHostMac : public HostMacBase
{
private:
    int m_AckSlotNum;
    int m_RepetitionRate;
    cMessage *m_SelfMsgTimer;



protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    int getRepetitionNum();
    void sendData();


public:
    DaraIrsaHostMac();
    ~DaraIrsaHostMac();
};
}



#endif /* INET_LINKLAYER_DARA_DARAIRSAHOSTMAC_H_ */
