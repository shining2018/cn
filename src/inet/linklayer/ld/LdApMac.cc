/*
 * LdApMac.cc
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */
#include "inet/linklayer/ld/LdApMac.h"
namespace inet
{
Define_Module(LdApMac);

LdApMac::LdApMac()
{
    m_SelfMsgACK=nullptr;
}
LdApMac::~LdApMac()
{
    cancelAndDelete(m_SelfMsgACK);
}

void LdApMac::initialize()
{
    ApMacBase::initialize();
    //m_SlotLength=par("slotLength");
    //m_SlotNum=par("slotNum");
    m_ReqSlots=par("reqSlotNum");
    //m_CommunicationRange=par("communicationRange");
    m_MaxPropagationSlots=ceil((m_CommunicationRange/SPEED_OF_LIGHT)/m_SlotLength);

    m_PkTotalCountSignal=registerSignal("pkTotalCount");
    m_PkCount1Signal=registerSignal("pkCount1");
    m_PkCount2Signal=registerSignal("pkCount2");
    m_PkCount3Signal=registerSignal("pkCount3");
    m_PkCount4Signal=registerSignal("pkCount4");
    m_PkCount5Signal=registerSignal("pkCount5");
    m_PkCount6Signal=registerSignal("pkCount6");
    m_PkCount7Signal=registerSignal("pkCount7");
    m_PkCount8Signal=registerSignal("pkCount8");
    m_PkCount9Signal=registerSignal("pkCount9");
    m_PkCount10Signal=registerSignal("pkCount10");

    m_SelfMsgACK=new cMessage("SelfMsg-ACK");
    scheduleAt(0+m_SlotLength*(m_ReqSlots-m_MaxPropagationSlots+1),m_SelfMsgACK);
}

void LdApMac::handleMessage(cMessage *msg)
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

void LdApMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgACK)
    {
        scheduleAt(simTime()+m_SlotLength*m_SlotNum,m_SelfMsgACK);
        sendACK();
    }
    else
    {
        throw cRuntimeError("LdApMac--Self message type error!");
    }
}

void LdApMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case PkKinds_Host_Request:
    {
        LdReq *req=dynamic_cast<LdReq *>(msg);
        req->setTimestamp(simTime());
        m_LdReqVector.push_back(req);
        break;
    }
    case PkKinds_Host_Data:
    {
        emit(m_PkTotalCountSignal,1l);
        emit(m_PkCountSignal,1l);
        m_pkCount++;
        switch(msg->getSrcProcId())
        {
           case 1:
           {
               emit(m_PkCount1Signal,1l);
               break;
           }
           case 2:
           {
               emit(m_PkCount2Signal,1l);
               break;
           }
           case 3:
           {
               emit(m_PkCount3Signal,1l);
               break;
           }
           case 4:
           {
               emit(m_PkCount4Signal,1l);
               break;
           }
           case 5:
           {
               emit(m_PkCount5Signal,1l);
               break;
           }
           case 6:
           {
               emit(m_PkCount6Signal,1l);
               break;
           }
           case 7:
           {
               emit(m_PkCount7Signal,1l);
               break;
           }
           case 8:
           {
               emit(m_PkCount8Signal,1l);
               break;
           }
           case 9:
           {
               emit(m_PkCount9Signal,1l);
               break;
           }
           case 10:
           {
               emit(m_PkCount10Signal,1l);
               break;
           }
           default:
           {
               throw cRuntimeError("LD--AP receive error distance flag.");
               break;
           }
        }
        //send(msg,"outToSink");

        delete msg;
        break;
    }
    default:
    {
        throw cRuntimeError("LdApMac--Non self message type error!");
    }
    }
}

void LdApMac::sendACK()
{
    if(m_LdReqVector.size()!=0)
    {
        //decode clean requests in LdCleanReqVector
        handleReqRepetitions();
        //assign slots
        if(m_LdCleanReqVector.empty())
        {
            EV_DEBUG<<"Request Vector is empty, no ACK packet is to send."<<endl;
        }
        else
        {
            sortReqVec(m_LdCleanReqVector);
            EV_DEBUG<<"Sorted Clean Request Vector: "<<endl;
            outputVec(m_LdCleanReqVector);

            //reqVecLength：申请时隙的节点个数
            const int reqVecLength=m_LdCleanReqVector.size();

            //slotNum，保存给每个host分配多少时隙
            int slotNum[reqVecLength];
            for(int i=0;i<reqVecLength;i++)
            {
                slotNum[i]=0;
            }
            const int dataSlotNum=m_SlotNum-m_ReqSlots;
            int avaiableDataSlotNum=m_SlotNum-m_ReqSlots;

            //给所有host分配时隙
            //bFinish是否分配结束
            bool bFinish=false;
            bool bTemp;
            while(!bFinish)
            {
                EV_DEBUG<<"Start a loop, bFinish="<<bFinish<<endl;
                for(int i=0;i<reqVecLength;i++)
                {
                    //如果当前分配的时隙数小于申请的时隙数
                    if(slotNum[i]< m_LdCleanReqVector[i]->getRequestSlotNum())
                    {
                        //如果当前分配的时隙数小于能够分配的最大时隙数
                        if(slotNum[i]<=(dataSlotNum-getMaxAvaSlots(m_LdCleanReqVector[i]->getPropagationSlotNum())))
                        {
                            //如果还有可用的时隙，那么就分配
                            if(avaiableDataSlotNum>0)
                            {
                                slotNum[i]++;
                                avaiableDataSlotNum--;
                                EV_DEBUG<<"A slot is allocated to "<<i<<", and total slots for it are: "<<slotNum[i]<<endl;
                                EV_DEBUG<<"nAvaiableDataSlotNum= "<<avaiableDataSlotNum<<endl;
                            }
                            //如果没有，那么就结束
                            else
                            {
                                bFinish=true;
                                EV_DEBUG<<"No data slots available."<<endl;
                            }
                        }
                        else
                        {
                            m_LdCleanReqVector[i]->setIsFinish(true);
                            //EV_DEBUG<<"slotNum["<<i<<"]==nDataSlotNum-nPropagationSlotNum."<<endl;
                        }
                    }
                    else
                    {
                        m_LdCleanReqVector[i]->setIsFinish(true);
                        EV_DEBUG<<"slotNum["<<i<<"]==reqSlotNum"<<endl;
                    }
                }

                //检查是否都已经分配结束
                bTemp=m_LdCleanReqVector[0]->getIsFinish();
                for(int i=1;i<reqVecLength;i++)
                {
                    bTemp=bTemp&&m_LdCleanReqVector[i]->getIsFinish();
                }

                if(bTemp || bFinish)
                {
                    bFinish=true;
                    EV_DEBUG<<"Allocation finishes."<<endl;
                }
            }


            int arrivalSlotIndex[reqVecLength];
            for(int i=0;i<reqVecLength;i++)
            {
                arrivalSlotIndex[i]=0;
            }
            EV_DEBUG<<"The arrival slot index:  ";
            for(int i=0;i<reqVecLength;i++)
            {
                if(i==0)
                {

                    arrivalSlotIndex[i]= getMaxAvaSlots(m_LdCleanReqVector[i]->getPropagationSlotNum());
                    EV_DEBUG<<arrivalSlotIndex[i]<<", ";
                }
                else
                {
                    arrivalSlotIndex[i]=arrivalSlotIndex[i-1]+slotNum[i-1];
                    if(arrivalSlotIndex[i]<m_LdCleanReqVector[i]->getPropagationSlotNum())
                    {
                        arrivalSlotIndex[i]=m_LdCleanReqVector[i]->getPropagationSlotNum();
                    }
                    EV_DEBUG<<arrivalSlotIndex[i]<<", ";
                }
            }
            EV_DEBUG<<endl;

            int slotIndex[reqVecLength];
            EV_DEBUG<<"********The arrival slot index:  ";
            for(int i=0;i<reqVecLength;i++)
            {
                slotIndex[i]=arrivalSlotIndex[i];
                EV_DEBUG<<slotIndex[i]<<", ";
                if(slotIndex[i]<0)
                {
                    throw cRuntimeError("slotIndex<0, slotIndex=%d,arrivalSlotIndex=%d,PropagationSlotNum=%d",slotIndex[i],arrivalSlotIndex[i],m_LdCleanReqVector[i]->getPropagationSlotNum());
                }
            }
            EV_DEBUG<<endl;

            EV_DEBUG<<"********The transmit slot number:  ";
            for(int i=0;i<reqVecLength;i++)
            {
                if(slotNum[i]<0)
                {
                    throw cRuntimeError("slotNum<0, slotNum==%d",slotNum[i]);
                }
                EV_DEBUG<<slotNum[i]<<", ";
            }
            EV_DEBUG<<endl;

//            int slotIndex[reqVecLength];
//            EV_DEBUG<<"********The start transmit slot index:  ";
//            for(int i=0;i<reqVecLength;i++)
//            {
//                slotIndex[i]=arrivalSlotIndex[i]-m_LdCleanReqVector[i]->getPropagationSlotNum();
//                EV_DEBUG<<slotIndex[i]<<", ";
//                if(slotIndex[i]<0)
//                {
//                    throw cRuntimeError("slotIndex<0, slotIndex=%d,arrivalSlotIndex=%d,PropagationSlotNum=%d",slotIndex[i],arrivalSlotIndex[i],m_LdCleanReqVector[i]->getPropagationSlotNum());
//                }
//            }
//            EV_DEBUG<<endl;
//
//            EV_DEBUG<<"********The transmit slot number:  ";
//            for(int i=0;i<reqVecLength;i++)
//            {
//                if(slotNum[i]<0)
//                {
//                    throw cRuntimeError("slotNum<0, slotNum==%d",slotNum[i]);
//                }
//                EV_DEBUG<<slotNum[i]<<", ";
//            }
//            EV_DEBUG<<endl;

            //更新m_reqVec
            vector<LdReq *> temp;
            int tempReqSlot;
            for(int i=0;i<reqVecLength;i++)
            {
                tempReqSlot=m_LdCleanReqVector[i]->getRequestSlotNum()-slotNum[i];
                m_LdCleanReqVector[i]->setRequestSlotNum(tempReqSlot);
                //m_LdCleanReqVector[i]->setPropagationSlotNum()=m_LdCleanReqVector[i]->getPropagationSlotNum()-slotNum[i];
                if(m_LdCleanReqVector[i]->getRequestSlotNum()!=0)
                {
                    temp.push_back(m_LdCleanReqVector[i]);
                }
            }

            cModule *mdHost;
            for(int i=0;i<reqVecLength;i++)
            {
                EV_DEBUG<<"AP send an ack packet to "<<m_LdCleanReqVector[i]->getHostPath()<<endl;
                LdACK *ackPk=new LdACK("ack");
                ackPk->setKind(PkKinds_Ap_ACK);
                ackPk->setSlotIndex(slotIndex[i]);
                ackPk->setSlotNum(slotNum[i]);

                if(m_LdCleanReqVector[i]->getRequestSlotNum()==0)
                {
                    ackPk->setIsOver(true);
                }
                else
                {
                    ackPk->setIsOver(false);
                }
                mdHost=getModuleByPath(m_LdCleanReqVector[i]->getHostPath());
                sendDirect(ackPk,0,0,mdHost->gate("in"));
            }


            if(m_LdCleanReqVector.size()!=temp.size())
            {
                m_LdCleanReqVector.clear();
                m_LdCleanReqVector=temp;
            }
            EV_DEBUG<<"Clean request vector after an ACK: "<<endl;
            outputVec(m_LdCleanReqVector);
        }
    }
}

void LdApMac::handleReqRepetitions()
{
    vector<LdReq *> cleanReqVector;
    cleanReqVector.clear();

    int cleanReqs=1;
    simtime_t currentTimeStamp;

    EV_DEBUG<<"********Ap received "<<m_LdReqVector.size()<<" requests during this frame:"<<endl;
    outputVec(m_LdReqVector);

    for(int itertimes=0;itertimes<m_LdReqVector.size();itertimes++)
    {
        for(int i=0;i<m_LdReqVector.size();i++)
        {
            if(m_LdReqVector[i]->getIsRecovery()==false)
            {
                currentTimeStamp=m_LdReqVector[i]->getTimestamp();
                for(int j=i+1;j<m_LdReqVector.size();j++)
                {
                    if(m_LdReqVector[j]->getTimestamp()==currentTimeStamp)
                    {
                        cleanReqs++;
                    }
                }
                if(cleanReqs==1)
                {
                    for(int k=0;k<m_LdReqVector.size();k++)
                    {
                        if(m_LdReqVector[k]->getReqId()==m_LdReqVector[i]->getReqId())
                        {
                            m_LdReqVector[k]->setIsRecovery(true);
                        }
                    }
                    cleanReqVector.push_back(m_LdReqVector[i]);
                }
                else
                {
                    cleanReqs=1;
                }
            }
        }
    }

    for(int i=0;i<cleanReqVector.size();i++)
    {
        m_LdCleanReqVector.push_back(cleanReqVector[i]);
    }
    m_LdReqVector.clear();
    cleanReqVector.clear();

    EV_DEBUG<<"Clean Request Vector:"<<endl;
    outputVec(m_LdCleanReqVector);
}

void LdApMac::sortReqVec(vector<LdReq *> &vec)
{
    LdReq *ReqTemp;
    for(int i=0;i<vec.size();i++)
    {
        for(int j=i;j<vec.size()-1;j++)
        {
            if(vec[j]->getPropagationSlotNum()>vec[j+1]->getPropagationSlotNum())
            {
                ReqTemp=vec[j];
                vec[j]=vec[j+1];
                vec[j+1]=ReqTemp;
            }
        }
    }
}

void LdApMac::outputVec(vector<LdReq *> &vec)
{
    for(int i=0;i<vec.size();i++)
    {
        EV_DEBUG<<"Request ID: "<<vec[i]->getReqId()<<endl;
        EV_DEBUG<<"Host: "<<vec[i]->getHostPath()<<endl;
        EV_DEBUG<<"Repetitions: "<<vec[i]->getNumOfRepetitions()<<endl;
        EV_DEBUG<<"IS Recovery: "<<vec[i]->getIsRecovery()<<endl;
        EV_DEBUG<<"Time stamp: "<<vec[i]->getTimestamp()<<endl;
        EV_DEBUG<<"****************"<<endl;
    }
}

//考虑可以提前发送，那么节点最多可以利用多少个时隙
int LdApMac::getMaxAvaSlots(int hostPropagationSlots)
{
    simtime_t t= m_CommunicationRange/SPEED_OF_LIGHT;
    int maxSlots=ceil(t/m_SlotLength);
    if(hostPropagationSlots*2<=maxSlots)
    {
        return 0;
    }
    else
    {
        return (hostPropagationSlots*2-maxSlots);
    }
}

}



