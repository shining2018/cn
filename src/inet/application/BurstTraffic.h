/*
 * BurstTraffic.h
 *
 *  Created on: 2017Äê6ÔÂ1ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_APPLICATION_BURSTTRAFFIC_H_
#define INET_APPLICATION_BURSTTRAFFIC_H_

#include "inet/common/INETDefs.h"

namespace inet
{
class BurstTraffic:public cSimpleModule
{
private:
    //parameters:
    simtime_t m_StartTime;
    uint m_PkLength;
    cPar *m_BurstInterval;
    cPar *m_PksPerBurst;

    //self msg
    cMessage *m_SelfmsgTimer;

    //signal
    simsignal_t m_PkCountSignal;

    //pk NO.
    long m_PkNo;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendBurst();
    void scheduleNextBurst(simtime_t lastTime);

public:
    BurstTraffic();
    virtual ~BurstTraffic();
};
}



#endif /* INET_APPLICATION_BURSTTRAFFIC_H_ */
