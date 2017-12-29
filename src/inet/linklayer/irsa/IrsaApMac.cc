/*
 * IrsaApMac.cc
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */
#include "inet/linklayer/irsa/IrsaApMac.h"
#include <algorithm>

namespace inet
{
Define_Module(IrsaApMac);

IrsaApMac::IrsaApMac()
{
    m_SelfMsgHandleHostData=nullptr;
}

IrsaApMac::~IrsaApMac()
{
    cancelAndDelete(m_SelfMsgHandleHostData);
}

void IrsaApMac::initialize()
{
    //m_SlotLength=par("slotLength");
    //m_SlotNum=par("slotNum");
    //m_PkTotalCountSignal=registerSignal("pkTotalCount");
    ApMacBase::initialize();
    m_SelfMsgHandleHostData=new cMessage("SelfMsg-HandleHostData");
    scheduleAt(0,m_SelfMsgHandleHostData);
}

void IrsaApMac::handleMessage(cMessage *msg)
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


//处理自消息
void IrsaApMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgHandleHostData)
    {
        EV_DEBUG<<"Next schedule time: "<<simTime()+m_SlotNum*m_SlotLength<<endl;
        scheduleAt(simTime()+m_SlotNum*m_SlotLength,m_SelfMsgHandleHostData);

        if(HostPksInfoVector.size()!=0)
        {
            handleHostData();
        }
    }
    else
    {
        throw cRuntimeError("IrsaApMac--Self message type error!");
    }
}

bool IrsaApMac::isPkClean(int pkID)
{
    bool isClean=false;
    for(vector<IrsaHostPkInfo>::iterator ite=CleanPksInfoVector.begin();ite!=CleanPksInfoVector.end();ite++)
    {
        if((*ite).pkId==pkID)
        {
            EV_DEBUG<<"find pk: "<<(*ite).pkId<<endl;
            isClean= true;
        }
    }
    return isClean;
}

bool IrsaApMac::isPkCleanInThisSlot(int slotID,int pkID)
{
    bool isClean=true;
    for(vector<IrsaHostPkInfo>::iterator ite=HostPksInfoVector.begin();ite!=HostPksInfoVector.end();ite++)
    {
        if(((*ite).slotId==slotID)&&((*ite).pkId!=pkID))
        {
            isClean=isClean&&(*ite).isClean;
        }
    }
    return isClean;
}
void IrsaApMac::handleHostData()
{
    CleanPksInfoVector.clear();
    int newCleanPk=-1;
    vector<IrsaHostPkInfo>::iterator ite;
    int pkNumInOneSlot=0;
    int HostPksInfoVecSize=HostPksInfoVector.size();
    while(newCleanPk!=0)
    {
        newCleanPk=0;
        for(int i=0;i<HostPksInfoVecSize;i++)
        {
            //首先判断是否已经解码出来
            if(isPkClean(HostPksInfoVector[i].pkId))
            {
                HostPksInfoVector[i].isClean=true;
            }
            //如果没有解码出来，看这个时隙能否解码出这个包
            else
            {
                if(isPkCleanInThisSlot(HostPksInfoVector[i].slotId,HostPksInfoVector[i].pkId))
                {
                    HostPksInfoVector[i].isClean=true;
                    CleanPksInfoVector.push_back(HostPksInfoVector[i]);
                    newCleanPk++;
                }
            }
        }
    }
    EV_DEBUG<<"Clean pks: ";
    for(int i=0;i<CleanPksInfoVector.size();i++)
    {
        EV_DEBUG<<CleanPksInfoVector[i].pkId<<" ";
        emit(m_PkCountSignal,1l);
        m_pkCount++;
    }
    EV_DEBUG<<endl;
    HostPksInfoVector.clear();
//    CleanPksVector.clear();
////    vector<IrsaHostPk *>::iterator ite;
////    ite=CleanPksVector.begin();
////    EV_DEBUG<<"The first element of cleanvector: "<<*ite<<endl;
////    try
////    {
////    for(ite=CleanPksVector.begin();ite!=CleanPksVector.end();++ite)
////    {
////            delete (*ite);
////    }
////    }
////    catch(exception e)
////    {
////        throw cRuntimeError("IRSA!");
////    }
//
////    vector<IrsaHostPk *>::iterator ite0;
////    for(ite0=CleanPksVector.begin();ite0!=CleanPksVector.end();ite0++)
////    {
////        delete *ite0;
////    }
////    CleanPksVector.clear();
//
//    //DEBUG
////    if(CleanPksVector.empty())
////    {
////        EV_DEBUG<<"NO ELEMENT!"<<endl;
////    }
////    else
////    {
////        for(int i=0;i<CleanPksVector.size();i++)
////        {
////            EV_DEBUG<<CleanPksVector[i]->getHostPath()<<endl;
////        }
////    }
//
//
//    int cleanPks=1;
//    simtime_t currentTimeStamp;
//
//    int PksVectorSize=HostPksVector.size();
//    EV_DEBUG<<"PksVectorSize: "<<PksVectorSize<<endl;
//    for(int itertimes=0;itertimes<PksVectorSize;itertimes++)
//    {
//        for(int i=0;i<PksVectorSize;i++)
//        {
//
//            if(HostPksVector[i]->getIsRecovery()==false)
//            {
//                EV_DEBUG<<"The ["<<i<<"] packet is not recovery."<<endl;
//                currentTimeStamp=HostPksVector[i]->getTimestamp();
//                for(int j=i+1;j<PksVectorSize;j++)
//                {
//                    if(HostPksVector[j]->getTimestamp()==currentTimeStamp)
//                    {
//                        cleanPks++;
//                        EV_DEBUG<<"cleanpks="<<cleanPks<<endl;
//                    }
//                }
//                if(cleanPks==1)
//                {
//
//                    for(int k=0;k<PksVectorSize;k++)
//                    {
//                        if(HostPksVector[k]->getPkId()==HostPksVector[i]->getPkId())
//                        {
//                            EV_DEBUG<<i<<"pkid="<<HostPksVector[i]->getPkId()<<endl;
//                            EV_DEBUG<<k<<"hostid="<<HostPksVector[k]->getHostId()<<endl;
//                            HostPksVector[k]->setIsRecovery(true);
//                            EV_DEBUG<<"The ["<<k<<"] packet is recoverying."<<endl;
//                        }
//                    }
//                    CleanPksVector.push_back(HostPksVector[i]);
//
//                }
//                else
//                {
//                    cleanPks=1;
//                }
//            }
//        }
//    }
//    //CleanPksVector.assign(HostPksVector.begin(),HostPksVector.end());
//    EV_DEBUG<<"Output pks: "<<endl;
//    EV_DEBUG<<"The size of cleandatavec is: "<<CleanPksVector.size()<<endl;
//    for(int i=0;i<CleanPksVector.size();i++)
//    {
//        EV_DEBUG<<"["<<i<<"]:"<<CleanPksVector[i]->getFullName()<<endl;
//        //IrsaHostPk *pk=new IrsaHostPk();
//        //pk=CleanPksVector[i];
//        //send(pk,"outToSink");
//        emit(m_PkCountSignal,1l);
//        m_pkCount++;
//    }
//
//    vector<IrsaHostPk *>::iterator ite;
//    for(ite=HostPksVector.begin();ite!=HostPksVector.end();ite++)
//    {
//        delete *ite;
//    }
//    HostPksVector.clear();
//
//    CleanPksVector.clear();
//
//
//
//    for(int i=0;i<HostPksVector.size();i++)
//    {
//        EV_DEBUG<<HostPksVector[i]->getHostPath()<<endl;
//    }

}



//处理非自消息
void IrsaApMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    //来自host的data
    case PkKinds_Host_Data:
    {
        IrsaHostPk *pk=dynamic_cast<IrsaHostPk *>(msg);
        int repetitionsNum=pk->getNumOfRepetitions();
        vector<int> slotIdVec=pk->getPoint();
        int vecSize=slotIdVec.size();
        if(repetitionsNum!=vecSize)
        {
            throw cRuntimeError("RepetitionNum!=VectorSize");
        }
        else
        {
            for(int i=0;i<vecSize;i++)
            {
                IrsaHostPkInfo pkInfo={pk->getPkId(),slotIdVec[i],false};
                EV_DEBUG<<"pkID: "<<pkInfo.pkId<<endl;
                EV_DEBUG<<"slotID: "<<pkInfo.slotId<<endl;
                HostPksInfoVector.push_back(pkInfo);
            }
        }
        delete pk;

        //pk->setTimestamp();
//
//        HostPksVector.push_back(pk);
//        EV_DEBUG<<"Ap received a packet: "<<pk->getFullName()<<endl;

        break;
    }
    default:
    {
        EV_DEBUG<<"Ap received a wrong type message."<<endl;
        break;
    }

    }
}


}




