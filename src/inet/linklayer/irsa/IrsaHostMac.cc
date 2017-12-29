/*
 * IrsaHostMac.cc
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */

#include "inet/linklayer/irsa/IrsaHostMac.h"

namespace inet
{
Define_Module(IrsaHostMac);

IrsaHostMac::IrsaHostMac()
{
    m_SelfMsgData=nullptr;
}

IrsaHostMac::~IrsaHostMac()
{
    cancelAndDelete(m_SelfMsgData);
}

void IrsaHostMac::initialize()
{
    HostMacBase::initialize();

    m_SelfMsgData=new cMessage("SelMsg-SendData");
    scheduleAt(0,m_SelfMsgData);

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
    if(msg==m_SelfMsgData)
    {
        scheduleAt(simTime()+m_SlotsNum*m_SlotLength,m_SelfMsgData);
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
        if(msg->getKind()==PkKinds_Self_IRSA_Host_Data)
        {
            msg->setKind(PkKinds_Host_Data);
            //TODO:调度时间貌似不合理
            //emit(m_SchedulingTimeSignal,(simTime()-msg->getArrivalTime()));
            sendDirect(msg,getPropagationSlots()*m_SlotLength,0,m_Ap->gate("in"));
        }
        else
        {
            throw cRuntimeError("Irsa--Self message type error!");
        }
    }
}

void IrsaHostMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case PkKinds_AppPk:
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
        }
        break;
    }
    default:
    {
        throw cRuntimeError("Irsa--Non self message type error!");
    }
    }
}

void IrsaHostMac::sendData()
{
    int pkNum=getDataPkNum();
    EV_DEBUG<<m_Parent->getFullPath()<<" will send "<<pkNum<<" packets."<<endl;
    int propagationSlots=getPropagationSlots();
    double probabilityofPkError=getPkErrorRate();

    //an array to indicate slots
    bool AvaiableSlots[m_SlotsNum];
    for(int i=0;i<m_SlotsNum;i++)
    {
        if(i<(m_SlotsNum-propagationSlots-1))
        {
            AvaiableSlots[i]=true;
        }
        else
        {
            AvaiableSlots[i]=false;
        }
    }

    //assign slots
    int rdSlotIndex;
    //IrsaHostPk *irsaPk=new IrsaHostPk();
    int repetitionNum=0;
    vector<int> slotIndexVector;
    while((pkNum!=0)&&(m_AppQueue.getLength()!=0))
    {
        cPacket *appPk=dynamic_cast<cPacket *>(m_AppQueue.pop());
        IrsaHostPk *irsaPk=new IrsaHostPk();
        irsaPk->setName(appPk->getFullName());
        irsaPk->setPkId(appPk->getId());
        delete appPk;


        //caculate the number of repetitions
        repetitionNum=getRepetitionNum();

        EV_DEBUG<<"The "<<pkNum<<"th packet will send "<<repetitionNum<<" repetitions."<<endl;

        //vector<int> slotIndexVector;
        slotIndexVector.clear();
        for(int i=0;i<repetitionNum;i++)
        {
            rdSlotIndex=intrand(m_SlotsNum-propagationSlots);
            while(!AvaiableSlots[rdSlotIndex])
            {
                rdSlotIndex=intrand(m_SlotsNum-propagationSlots);
            }

            AvaiableSlots[rdSlotIndex]=false;
            slotIndexVector.push_back(rdSlotIndex+propagationSlots);
        }
        EV_DEBUG<<"These repetitions will be sent at slots: ";
        for(int i=0;i<slotIndexVector.size();i++)
        {
            EV_DEBUG<<slotIndexVector[i]<<" ";
        }
        EV_DEBUG<<endl;

        irsaPk->setHostId(m_Parent->getId());
        irsaPk->setHostPath(m_Parent->getFullPath().c_str());
        irsaPk->setNumOfRepetitions(repetitionNum);
        irsaPk->setPoint(slotIndexVector);
        irsaPk->setIsRecovery(false);
        irsaPk->setKind(PkKinds_Host_Data);

        m_Random=dblrand();
        if(m_Random>probabilityofPkError)
        {
            sendDirect(irsaPk,0,0,m_Ap->gate("in"));
            emit(m_SentPkCountSignal,1l);
        }
        else
        {
            EV_DEBUG<<"IRSA packet has lost."<<endl;
            delete irsaPk;
        }


//        for(int i=0;i<slotIndexVector.size();i++)
//        {
//            IrsaHostPk *dupPk=irsaPk->dup();
//            dupPk->setSlotIndex(slotIndexVector[i]);
//            dupPk->setKind(PkKinds_Self_IRSA_Host_Data);
//            scheduleAt(simTime()+slotIndexVector[i]*m_SlotLength,dupPk);
//            //dupPk->setKind(PkKinds_Host_Data);
//            //sendDirect(dupPk,(propagationSlots+slotIndexVector[i])*m_SlotLength,0,m_Ap->gate("in"));
//        }

        slotIndexVector.clear();

        //delete irsaPk;

        pkNum--;
    }
    //delete irsaPk;
}

int IrsaHostMac::getDataPkNum()
{
    int pkNum=0;
    if(m_AppQueue.isEmpty())
    {
        pkNum=0;
    }
    else
    {
        pkNum=13;
    }
    return pkNum;
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

}



