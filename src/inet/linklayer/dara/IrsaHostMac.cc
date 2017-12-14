/*
 * IrsaHostMac.cc
 *
 *  Created on: 2017年12月14日
 *      Author: NETLAB
 */
#include "inet/linklayer/dara/IrsaHostMac.h"
#include "inet/linklayer/dara/DaraApPk_m.h"

namespace inet
{
Define_Module(IrsaHostMac);

IrsaHostMac::IrsaHostMac()
{
    m_SelfMsgTimer=nullptr;
}

IrsaHostMac::~IrsaHostMac()
{
    cancelAndDelete(m_SelfMsgTimer);
}

void IrsaHostMac::initialize()
{
    HostMacBase::initialize();

    m_AckSlotNum=par("ackSlotNum");

    m_SelfMsgTimer=new cMessage("SelfMsg-Timer");
    scheduleAt(0,m_SelfMsgTimer);
}

void IrsaHostMac::handleMessage(cMessage *msg)
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

void IrsaHostMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgTimer)
    {
        scheduleAt(simTime()+m_SlotLength*m_SlotsNum,m_SelfMsgTimer);
        if(!isOutOfCommunicationRange())
        {
            sendData();
        }
        else
        {
            m_AppQueue.clear();
        }
    }
    else
    {
        if(msg->getKind()==PkKinds_Self_Dara_Host_Data)
        {
            msg->setKind(PkKinds_Host_Request);
            sendDirect(msg,getPropagationSlots()*m_SlotLength,0,m_Ap->gate("in"));
        }
        else
        {
            throw cRuntimeError("Dara--Self message type error!");
        }
    }
}

void IrsaHostMac::handleNonSelfMsg(cMessage *msg)
{
    if(msg->getKind()==PkKinds_AppPk)
    {
        if(isAppQueueFull())
        {
            emit(m_DiscardPkCountSignal,1l);
            delete msg;
            EV_DEBUG<<"The new arrival message has been deleted."<<endl;
        }
        else
        {
            msg->setArrivalTime(simTime());
            m_AppQueue.insert(msg);
            EV_DEBUG<<"The length of AppQueue is "<<m_AppQueue.getLength()<<endl;
        }
    }
    else
    {
        if(msg->getKind()==PkKinds_Ap_ACK)
        {
            DaraApPk *pk=dynamic_cast<DaraApPk *>(msg);
            vector<int> ack=pk->getAck();
            EV_DEBUG<<"ACK INFOMATION: ";
            for(int i=0;i<ack.size();i++)
            {
                EV_DEBUG<<ack[i]<<" ";
            }
            EV_DEBUG<<endl;

            delete pk;
        }
    }
}

int IrsaHostMac::getRepetitionNum()
{
    //0.5631x3+0.0436x3+0.3933x5
    int repetitionNum=0;
    double rd=dblrand();
    if(rd<0.5631)
    {
        repetitionNum=2;
    }
    else
    {
        if(rd<0.6067)
        {
            repetitionNum=3;
        }
        else
        {
            repetitionNum=5;
        }
    }
    return repetitionNum;
}

void IrsaHostMac::sendData()
{
    if(m_AppQueue.getLength()!=0)
    {
        int propagationSlot=getPropagationSlots();
        //初始化一个数组，记录哪些时隙可以发送请求副本，
        //初始化时，都可以发送
        bool AvaiableSlots[m_SlotsNum-m_AckSlotNum];
        for(int i=0;i<(m_SlotsNum-m_AckSlotNum);i++)
        {
            if(i<propagationSlot)
            {
                AvaiableSlots[i]=false;
            }
            else
            {
                AvaiableSlots[i]=true;
            }
        }

        //得到 需要发多少个副本
        int repetitionNum=getRepetitionNum();

        //以下过程得到时隙索引（即副本在哪些时隙到达基站）
        //并将这些索引保存在slotIndexVector中
        int rdSlotIndex;
        vector<int> slotIndexVector;
        slotIndexVector.clear();

        for(int i=0;i<repetitionNum;i++)
        {
            rdSlotIndex=intrand(m_SlotsNum-m_AckSlotNum);
            while(!AvaiableSlots[rdSlotIndex])
            {
                rdSlotIndex=intrand(m_SlotsNum);
            }
            AvaiableSlots[rdSlotIndex]=false;
            slotIndexVector.push_back(rdSlotIndex);
        }
        EV_DEBUG<<"These slot indexs are choosen:";
        for(int i=0;i<slotIndexVector.size();i++)
        {
            EV_DEBUG<<slotIndexVector[i]<<" ";
        }
        EV_DEBUG<<endl;

        cPacket *appPk=dynamic_cast<cPacket *>(m_AppQueue.pop());
        DaraHostPk *daraPk=new DaraHostPk();
        daraPk->setPkId(daraPk->getId());
        daraPk->setHostId(m_Parent->getId());
        daraPk->setHostPath(m_Parent->getFullPath().c_str());
        daraPk->setNumOfRepetitions(repetitionNum);
        daraPk->setPoint(slotIndexVector);
        daraPk->setIsRecovery(false);
        delete appPk;

        for(int i=0;i<slotIndexVector.size();i++)
        {
            DaraHostPk *dupPk=daraPk->dup();
            dupPk->setSlotIndex(slotIndexVector[i]);
            dupPk->setKind(PkKinds_Self_Dara_Host_Data);
            scheduleAt(simTime()+(slotIndexVector[i]-propagationSlot)*m_SlotLength,dupPk);
            EV_DEBUG<<"A repetion will be sent in "<<(simTime()+(slotIndexVector[i]-getPropagationSlots())*m_SlotLength)<<
                    ", and the propagation delay is "<<propagationSlot*m_SlotLength<<", so the pk will be arrived at "<<
                    (simTime()+(slotIndexVector[i])*m_SlotLength)<<endl;
        }

        delete daraPk;
        slotIndexVector.clear();
    }
}
}



