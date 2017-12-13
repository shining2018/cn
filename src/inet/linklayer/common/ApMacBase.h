/*
 * ApMacBase.h
 *
 *  Created on: 2017Äê6ÔÂ3ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_COMMON_APMACBASE_H_
#define INET_LINKLAYER_COMMON_APMACBASE_H_

#include "inet/common/INETDefs.h"

namespace inet
{
class ApMacBase : public cSimpleModule
{
protected:
    double m_CommunicationRange;
    simtime_t m_SlotLength;
    int m_SlotNum;

    simsignal_t m_PkCountSignal;
    long m_pkCount;
    double m_pkPerSecond;

protected:
    virtual void initialize() override;
    virtual void finish() override;

public:
    ApMacBase();
    ~ApMacBase();
};
}



#endif /* INET_LINKLAYER_COMMON_APMACBASE_H_ */
