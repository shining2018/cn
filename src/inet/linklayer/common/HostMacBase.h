/*
 * HostMacBase.h
 *
 *  Created on: 2017Äê6ÔÂ1ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_COMMON_HOSTMACBASE_H_
#define INET_LINKLAYER_COMMON_HOSTMACBASE_H_
#define QUEUE_CAPACITY 20000
#include "inet/common/INETDefs.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/mobility/contract/IMobility.h"

namespace inet
{
class HostMacBase:public cSimpleModule
{
protected:
    //parameters
    double m_TxRate;
    double m_CommunicationRange;
    simtime_t m_SlotLength;
    int m_SlotsNum;

    double m_pkErrorRate;

    //AP module
    cModule *m_Ap;

    //parent module
    cModule *m_Parent;

    //AppQueue
    cQueue m_AppQueue;

    //statistic
    simsignal_t m_SentPkCountSignal;
    simsignal_t m_DiscardPkCountSignal;
    simsignal_t m_SchedulingTimeSignal;

protected:
    virtual void initialize() override;
    //virtual void handleMessage(cMessage *msg) override;
    Coord getCurrentPosition();
    Coord getApPosition();
    double getDistance();
    int getPropagationSlots();
    int getMaxPropagationSlts();
    bool isOutOfCommunicationRange();
    bool isAppQueueFull();
    simtime_t getNearestFrameHead();
    double getPkErrorRate();

private:
    void findAp();
    void findParentModule();
    void initAllParameters();
    void registerAllSignals();


public:
    HostMacBase();
    ~HostMacBase();

};
}



#endif /* INET_LINKLAYER_COMMON_HOSTMACBASE_H_ */
