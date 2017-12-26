/*
 * TdmaApMac.h
 *
 *  Created on: 2017Äê6ÔÂ2ÈÕ
 *      Author: NETLAB
 */

#ifndef INET_LINKLAYER_TDMA_TDMAAPMAC_H_
#define INET_LINKLAYER_TDMA_TDMAAPMAC_H_

#include "inet/linklayer/tdma/TdmaACK_m.h"
#include "inet/linklayer/tdma/TdmaReq_m.h"
#include "inet/common/PkKinds.h"
#include "inet/linklayer/common/ApMacBase.h"

namespace inet
{
class TdmaApMac: public ApMacBase
{
private:
    //parameter
    int m_ReqSlots;

    int m_MaxPropagationSlots;


    cMessage *m_SelfMsgACK;
    //Req queue
    cQueue m_ReqQueue;


protected:
    void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void handleSelfMsg(cMessage *msg);
    void handleNonSelfMsg(cMessage *msg);
    void sendACK();
    void printQueue();

public:
    TdmaApMac();
    ~TdmaApMac();

};
}



#endif /* INET_LINKLAYER_TDMA_TDMAAPMAC_H_ */
