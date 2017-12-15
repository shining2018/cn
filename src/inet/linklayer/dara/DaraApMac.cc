/*
 * DaraApMac.cc
 *
 *  Created on: 2017Äê12ÔÂ12ÈÕ
 *      Author: NETLAB
 */

#include "inet/linklayer/dara/DaraApMac.h"

namespace inet
{
Define_Module(DaraApMac);

DaraApMac::DaraApMac()
{
    m_SelfMsgHandleReq=nullptr;
}

DaraApMac::~DaraApMac()
{
    cancelAndDelete(m_SelfMsgHandleReq);
}

void DaraApMac::initialize()
{
    ApMacBase::initialize();
    m_AckSlotNum=par("ackSlotNum");
    m_SelfMsgHandleReq=new cMessage("SelfMsg-HandleHostReq");
    scheduleAt(0+m_SlotLength*(m_SlotNum-m_AckSlotNum),m_SelfMsgHandleReq);
}

void DaraApMac::handleMessage(cMessage *msg)
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

void DaraApMac::handleNonSelfMsg(cMessage *msg)
{
    if(msg->getKind()==PkKinds_Host_Request)
    {
        DaraHostPk *pk=dynamic_cast<DaraHostPk *>(msg);
        //output debug infomation
        EV_DEBUG<<"Ap received a packet: "<<msg->getFullPath()<<endl;
        EV_DEBUG<<"pk id: "<<pk->getPkId()<<endl;
        EV_DEBUG<<"slot index: "<<pk->getSlotIndex()<<endl;
        EV_DEBUG<<"number of repetitions: "<<pk->getNumOfRepetitions()<<endl;

        DaraHostPkInfo pkInfor={pk->getPkId(),pk->getSlotIndex(),false};

        PkIdSet.insert(pk->getPkId());
        //HostIdSet.insert(pk->getHostId());
        HostPathSet.insert(pk->getHostPath());

        HostPksInfoVector.push_back(pkInfor);

        delete pk;

    }
    else
    {
        EV_DEBUG<<"Ap received a wrong type message."<<endl;
    }
}

void DaraApMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgHandleReq)
    {
        scheduleAt(simTime()+m_SlotLength*m_SlotNum,m_SelfMsgHandleReq);

        if(HostPksInfoVector.size()!=0)
        {
            handleHostData();
            sendAck();
            PkIdSet.clear();
            HostPathSet.clear();
        }
    }
    else
    {
        throw cRuntimeError("DaraApMac--Self message type error!");
    }
}

void DaraApMac::setPkClean(int pkId)
{
    for(vector<DaraHostPkInfo>::iterator ite=HostPksInfoVector.begin();ite!=HostPksInfoVector.end();ite++)
    {
        if((*ite).pkId==pkId)
        {
            (*ite).isClean=true;
        }
    }
}

bool DaraApMac::isPkCleanInThisSlot(int slotId,int pkId)
{
    bool isClean=true;
    for(vector<DaraHostPkInfo>::iterator ite=HostPksInfoVector.begin();ite!=HostPksInfoVector.end();ite++)
    {
        if(((*ite).slotId==slotId)&&((*ite).pkId!=pkId))
        {
            isClean=isClean&&(*ite).isClean;
        }
    }
    return isClean;
}

void DaraApMac::handleHostData()
{
    CleanPksInfoVector.clear();
    int newCleanPk=-1;
    vector<DaraHostPkInfo>::iterator ite;
    int HostPksInfoVecSize=HostPksInfoVector.size();
    EV_DEBUG<<HostPksInfoVecSize<<" requests were received during this frame."<<endl;

    while(newCleanPk!=0)
    {
        newCleanPk=0;
        for(int i=0;i<HostPksInfoVecSize;i++)
        {
            if(HostPksInfoVector[i].isClean==false)
            {
                if(isPkCleanInThisSlot(HostPksInfoVector[i].slotId,HostPksInfoVector[i].pkId))
                {
                    HostPksInfoVector[i].isClean=true;
                    setPkClean(HostPksInfoVector[i].pkId);
                    CleanPksInfoVector.push_back(HostPksInfoVector[i]);
                    newCleanPk++;
                }
            }
        }
    }
    EV_DEBUG<<"Clean pkId: ";
    for(int i=0;i<CleanPksInfoVector.size();i++)
    {
        EV_DEBUG<<CleanPksInfoVector[i].pkId<<" ";
        emit(m_PkCountSignal,1l);
        m_pkCount++;
    }
    EV_DEBUG<<endl;

    m_TotalReceivedPk+=PkIdSet.size();
    m_RecoveriedPk+=CleanPksInfoVector.size();
    HostPksInfoVector.clear();
}

void DaraApMac::sendAck()
{
    vector<int> ack;
    for(int i=0;i<CleanPksInfoVector.size();i++)
    {
        ack.push_back(CleanPksInfoVector[i].pkId);
    }
    DaraApPk *pk=new DaraApPk();
    pk->setKind(PkKinds_Ap_ACK);
    pk->setAck(ack);
    for(set<string>::iterator ite=HostPathSet.begin();ite!=HostPathSet.end();ite++)
    {
        DaraApPk *dup=pk->dup();
        sendDirect(dup,0,0,getModuleByPath((*ite).c_str())->gate("in"));
    }
    delete pk;
}

void DaraApMac::finish()
{
    EV_DEBUG<<"Total received pk: "<<m_TotalReceivedPk<<endl;
    EV_DEBUG<<"Recoveried pk: "<<m_RecoveriedPk<<endl;

}
}


