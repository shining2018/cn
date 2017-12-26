/*
 * TdmaApMac.cc
 *
 *  Created on: 2017Äê6ÔÂ2ÈÕ
 *      Author: NETLAB
 */
#include "inet/linklayer/tdma/TdmaApMac.h"

namespace inet
{
Define_Module(TdmaApMac);

TdmaApMac::TdmaApMac()
{
    m_SelfMsgACK=nullptr;
}
TdmaApMac::~TdmaApMac()
{
    cancelAndDelete(m_SelfMsgACK);
}

void TdmaApMac::initialize()
{
    ApMacBase::initialize();

    m_ReqSlots=par("reqSlotNum");
    EV_DEBUG<<"reqSlots: "<<m_ReqSlots<<endl;

    m_MaxPropagationSlots=ceil((m_CommunicationRange/SPEED_OF_LIGHT)/m_SlotLength);


    m_SelfMsgACK=new cMessage("SelfMsg-ACK");
    scheduleAt(0+m_SlotLength*(m_ReqSlots-m_MaxPropagationSlots),m_SelfMsgACK);
}

void TdmaApMac::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        handleSelfMsg(msg);
    }
    else
    {
        handleNonSelfMsg(msg);
    }
}

void TdmaApMac::handleSelfMsg(cMessage *msg)
{
    sendACK();
    m_ReqQueue.clear();
    scheduleAt(simTime()+m_SlotLength*m_SlotNum,m_SelfMsgACK);
}

void TdmaApMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case PkKinds_Host_Data :
    {
        EV_DEBUG<<"Ap has received a data packet."<<endl;
        emit(m_PkCountSignal,1l);
        m_pkCount++;
        delete msg;
        //send(msg,"outToSink");
        break;
    }
    case PkKinds_Host_Request:
    {
        EV_DEBUG<<"Ap has received a request packet."<<endl;
        m_ReqQueue.insert(msg);
        printQueue();
        break;
    }
    default:
    {
        throw cRuntimeError("TdmaApMac--Non self message type error!");
    }
    }
}

void TdmaApMac::sendACK()
{
    int queueLength=m_ReqQueue.getLength();
    EV_DEBUG<<"There are "<<queueLength<<" hosts in ReqQueue."<<endl;

    if(queueLength!=0)
    {
        int avgSlotNum=(m_SlotNum-m_ReqSlots)/queueLength;
        int leftSlotNum=(m_SlotNum-m_ReqSlots)%queueLength;

        int slotIndex=m_ReqSlots;
        int slotNum=0;
        cModule *mdHost;
        for(int i=0;i<queueLength;i++)
        {
            TdmaACK *ACKPk=new TdmaACK();
            ACKPk->setKind(PkKinds_Ap_ACK);
            ACKPk->setSlotIndex(slotIndex);
            if(i<leftSlotNum)
            {
                slotNum=avgSlotNum+1;
            }
            else
            {
                slotNum=avgSlotNum;
            }
            ACKPk->setSlotNum(slotNum);

            slotIndex=slotIndex+slotNum;

            mdHost=getModuleByPath((dynamic_cast<TdmaReq *>(m_ReqQueue.get(i)))->getHostPath());

            sendDirect(ACKPk,0,0,mdHost->gate("in"));
        }
    }
}

void TdmaApMac::printQueue()
{
    EV_DEBUG<<"Elements in m_ReqQueue:"<<endl;
    for(int i=0;i<m_ReqQueue.getLength();i++)
    {
        EV_DEBUG<<i<<" of "<<m_ReqQueue.getLength()<<" : "<<(dynamic_cast<TdmaReq *>(m_ReqQueue.get(i)))->getHostPath()<<endl;
    }
}
}



