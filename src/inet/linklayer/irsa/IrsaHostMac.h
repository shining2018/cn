/*
 * IrsaHostMac.h
 *
 *  Created on: 2017Äê6ÔÂ2ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_IRSA_IRSAHOSTMAC_H_
#define INET_LINKLAYER_IRSA_IRSAHOSTMAC_H_

#include "inet/linklayer/common/HostMacBase.h"
#include "inet/linklayer/irsa/IrsaHostPk_m.h"
#include "inet/common/PkKinds.h"
#include <vector>
using namespace std;

namespace inet
{
class IrsaHostMac:public HostMacBase
{
private:
    cMessage *m_SelfMsgData;
    double m_Random;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    void handleSelfMsgSendData(cMessage *msg);
    void sendData();
    int getDataPkNum();
    int getRepetitionNum();
public:
    IrsaHostMac();
    ~IrsaHostMac();
};
}



#endif /* INET_LINKLAYER_IRSA_IRSAHOSTMAC_H_ */
