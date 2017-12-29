/*
 * LdHostMac.cc
 *
 *  Created on: 2017年6月2日
 *      Author: NETLAB
 */

#include "inet/linklayer/ld/LdHostMac.h"
namespace inet
{
Define_Module(LdHostMac);

LdHostMac::LdHostMac()
{
    m_SelfMsgReq=nullptr;
    m_SelfMsgData=nullptr;
    m_SelfMsgReqPks=nullptr;
    m_SelfMsgTimer=nullptr;
}

LdHostMac::~LdHostMac()
{
    cancelAndDelete(m_SelfMsgReq);
    cancelAndDelete(m_SelfMsgData);
    cancelAndDelete(m_SelfMsgReqPks);
    cancelAndDelete(m_SelfMsgTimer);
}

void LdHostMac::initialize()
{
    HostMacBase::initialize();
    m_ReqSlots=par("reqSlotNum");
    EV_DEBUG<<this->getFullName()<<" initialize parameters: "<<endl;
    EV_DEBUG<<"reqSlotNum: "<<m_ReqSlots<<endl;

    //注册信号量
    //排队等待时间
//    m_QueueTimeSignal=registerSignal("queueTime");
    m_Queuetime=0;
    //被丢弃的数据包个数
//    m_DiscardPkCountSignal=registerSignal("discardPkCount");
    //总共发送的数据包个数
//    m_SentPkCountSignal=registerSignal("sentPkCount");
    m_TotalTimeSignal=registerSignal("totaltime");
    m_Time1Signal=registerSignal("time1");
    m_Time2Signal=registerSignal("time2");
    m_Time3Signal=registerSignal("time3");
    m_Time4Signal=registerSignal("time4");
    m_Time5Signal=registerSignal("time5");
    m_Time6Signal=registerSignal("time6");
    m_Time7Signal=registerSignal("time7");
    m_Time8Signal=registerSignal("time8");
    m_Time9Signal=registerSignal("time9");
    m_Time10Signal=registerSignal("time10");
    m_RetransmitTimesSignal=registerSignal("retransmitTimes");
    m_RetransmitTimesSignal1=registerSignal("retransmitTimes1");
    m_RetransmitTimesSignal2=registerSignal("retransmitTimes2");
    m_RetransmitTimesSignal3=registerSignal("retransmitTimes3");
    m_RetransmitTimesSignal4=registerSignal("retransmitTimes4");
    m_RetransmitTimesSignal5=registerSignal("retransmitTimes5");
    m_RetransmitTimesSignal6=registerSignal("retransmitTimes6");
    m_RetransmitTimesSignal7=registerSignal("retransmitTimes7");
    m_RetransmitTimesSignal8=registerSignal("retransmitTimes8");
    m_RetransmitTimesSignal9=registerSignal("retransmitTimes9");
    m_RetransmitTimesSignal10=registerSignal("retransmitTimes10");

    m_Time=0;
    m_Time1=0;
    m_Time2=0;
    m_Time3=0;
    m_Time4=0;
    m_Time5=0;
    m_Time6=0;
    m_Time7=0;
    m_Time8=0;
    m_Time9=0;
    m_Time10=0;

    m_RetransmitTimes=0;

    m_SelfMsgData=new cMessage("SelfMsg-SendData");
    m_SelfMsgReqPks=new cMessage("SelfMsg-ReqPks");
    m_SelfMsgReq=new cMessage("SelfMsg-Request");
    m_SelfMsgTimer=new cMessage("SelfMsg-Timer");
    scheduleAt(0,m_SelfMsgReq);
}

void LdHostMac::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        //EV_DEBUG<<this->getFullName()<<" begins to handle self message!"<<endl;
        handleSelfMsg(msg);
    }
    else
    {
        //EV_DEBUG<<this->getFullName()<<" begins to handle non-self message!"<<endl;
        handleNonSelfMsg(msg);
    }
}

void LdHostMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgReq)
    {
        //EV_DEBUG<<"Handle "<<msg->getFullName()<<endl;
        scheduleAt(simTime()+m_SlotLength*m_SlotsNum,m_SelfMsgReq);

        if(!isOutOfCommunicationRange())
        {
            m_Time=m_SlotLength*m_SlotsNum;
            emit(m_TotalTimeSignal,m_Time);
            m_TimeTotal=m_TimeTotal+m_Time;
            switch(setDistanceFlag())
            {
                case 1:
                {
                    emit(m_Time1Signal,m_Time);
                    m_Time1=m_Time1+m_Time;
                    break;
                }
                case 2:
                {
                    emit(m_Time2Signal,m_Time);
                    m_Time2=m_Time2+m_Time;
                    break;
                }
                case 3:
                {
                    emit(m_Time3Signal,m_Time);
                    m_Time3=m_Time3+m_Time;
                    break;
                }
                case 4:
                {
                    emit(m_Time4Signal,m_Time);
                    m_Time4=m_Time4+m_Time;
                    break;
                }
                case 5:
                {
                    emit(m_Time5Signal,m_Time);
                    m_Time5=m_Time5+m_Time;
                    break;
                }
                case 6:
                {
                    emit(m_Time6Signal,m_Time);
                    m_Time6=m_Time6+m_Time;
                    break;
                }
                case 7:
                {
                    emit(m_Time7Signal,m_Time);
                    m_Time7=m_Time7+m_Time;
                    break;
                }
                case 8:
                {
                    emit(m_Time8Signal,m_Time);
                    m_Time8=m_Time8+m_Time;
                    break;
                }
                case 9:
                {
                    emit(m_Time9Signal,m_Time);
                    m_Time9=m_Time9+m_Time;
                    break;
                }
                case 10:
                {
                    emit(m_Time10Signal,m_Time);
                    m_Time10=m_Time10+m_Time;
                    break;
                }
                default:
                {
                    throw cRuntimeError("LD--Error location");
                }
            }

            if(m_IsReadyToRequest)
            {
                EV_DEBUG<<"Host is ready to request!"<<endl;
                sendReq();
            }
            else
            {
                EV_DEBUG<<"Host is not ready to request!"<<endl;
            }
        }
        else
        {
            m_AppQueue.clear();
        }
    }
    else
    {
        if(msg==m_SelfMsgData)
        {
            EV_DEBUG<<"Handle "<<msg->getFullName()<<endl;
            sendData();
        }
        else
        {
            if(msg->getKind()==PkKinds_Self_IRSA_Host_Data)
            {
                msg->setKind(PkKinds_Host_Request);
                sendDirect(msg,getPropagationSlots()*m_SlotLength,0,m_Ap->gate("in"));
            }
            else
            {
                if(msg==m_SelfMsgTimer)
                {
                    m_RetransmitTimes++;
                }
                else
                {
                    throw cRuntimeError("LdHostMac--Self message type error!");
                }
            }
        }
    }
}

void LdHostMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case PkKinds_Ap_ACK:
    {
        if(m_SelfMsgTimer->isScheduled())
        {
            cancelEvent(m_SelfMsgTimer);
            emit(m_RetransmitTimesSignal,m_RetransmitTimes);
            switch(setDistanceFlag())
            {
            case 1:
             {
                 emit(m_RetransmitTimesSignal1,m_RetransmitTimes);
                 break;
             }
             case 2:
             {
                 emit(m_RetransmitTimesSignal2,m_RetransmitTimes);
                 break;
             }
             case 3:
             {
                 emit(m_RetransmitTimesSignal3,m_RetransmitTimes);
                 break;
             }
             case 4:
             {
                 emit(m_RetransmitTimesSignal4,m_RetransmitTimes);
                 break;
             }
             case 5:
             {
                 emit(m_RetransmitTimesSignal5,m_RetransmitTimes);
                 break;
             }
             case 6:
             {
                 emit(m_RetransmitTimesSignal6,m_RetransmitTimes);
                 break;
             }
             case 7:
             {
                 emit(m_RetransmitTimesSignal7,m_RetransmitTimes);
                 break;
             }
             case 8:
             {
                 emit(m_RetransmitTimesSignal8,m_RetransmitTimes);
                 break;
             }
             case 9:
             {
                 emit(m_RetransmitTimesSignal9,m_RetransmitTimes);
                 break;
             }
             case 10:
             {
                 emit(m_RetransmitTimesSignal10,m_RetransmitTimes);
                 break;
             }
             default:
             {
                 throw cRuntimeError("LD--Error location");
             }
            }
            m_RetransmitTimes=0;
        }

        LdACK *ackPk=dynamic_cast<LdACK *>(msg);
        m_DataSlotIndex=ackPk->getSlotIndex();
        m_DataSlotNum=ackPk->getSlotNum();
        m_IsReadyToRequest=ackPk->getIsOver();
        delete ackPk;
        EV_DEBUG<<"**********ACK Information for "<<m_Parent->getFullPath()<<"***********"<<endl;
        EV_DEBUG<<"Slot Index: "<<m_DataSlotIndex<<endl;
        EV_DEBUG<<"Slot Number: "<<m_DataSlotNum<<endl;
        //TODO
        //m_DataSendTime=simTime()+m_SlotLength*(m_DataSlotIndex+2);
        m_DataSendTime=getNearestFrameHead()-m_SlotLength*(m_SlotsNum-m_ReqSlots)+m_SlotLength*m_DataSlotIndex;
        EV_DEBUG<<"Start time of sending data is: "<<m_DataSendTime<<endl;
        EV_DEBUG<<"Nearest frame time is: "<<getNearestFrameHead()<<endl;
        if(m_DataSendTime<simTime())
        {
            throw cRuntimeError("LdHostMac--DataSendTime is error!");
        }
        scheduleAt(m_DataSendTime,m_SelfMsgData);
        break;
    }
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
            //EV_DEBUG<<"The new arrival message has been inserted into AppQueue."<<endl;
        }
        break;
    }
    default:
    {
        throw cRuntimeError("LdHostMac--Non self message type error!");
    }

    }
}

void LdHostMac::sendReq()
{
    if(!m_AppQueue.isEmpty())
    {
        //generate a new req
        LdReq *reqPk=new LdReq();
        reqPk->setKind(PkKinds_Host_Request);
        reqPk->setHostId(m_Parent->getId());
        reqPk->setHostPath(m_Parent->getFullPath().c_str());
        //calculate request how many slots
        //TODO:改变MAX_REQUEST_SLOTS看它对性能的影响
        int reqSlotNum=m_AppQueue.getLength();
        if(reqSlotNum>MAX_REQUEST_SLOTS)
        {
            reqSlotNum=MAX_REQUEST_SLOTS;
        }
        reqPk->setRequestSlotNum(reqSlotNum);
        reqPk->setHostPosition(getCurrentPosition());
        int propagationSlots=getPropagationSlots();
        reqPk->setPropagationSlotNum(propagationSlots);

        //select which slots to send requests
        bool AvaiableSlots[m_ReqSlots];
        int avaiableSlotNum=m_ReqSlots-getMaxPropagationSlts()-propagationSlots;
        for(int i=0;i<m_ReqSlots;i++)
        {
            if(i<avaiableSlotNum)
            {
                AvaiableSlots[i]=true;
            }
            else
            {
                AvaiableSlots[i]=false;
            }
        }

        int rdSlotIndex;
        int repetitionNum=getRepetitionNum();

        vector<int> slotIndexVector;
        slotIndexVector.clear();
        for(int i=0;i<repetitionNum;i++)
        {
            rdSlotIndex=intrand(avaiableSlotNum);
            while(!AvaiableSlots[rdSlotIndex])
            {
                rdSlotIndex=intrand(avaiableSlotNum);
            }
            AvaiableSlots[rdSlotIndex]=false;
            slotIndexVector.push_back(rdSlotIndex);
        }


        reqPk->setReqId(reqPk->getId());
        reqPk->setNumOfRepetitions(repetitionNum);
        reqPk->setPoint(slotIndexVector);
        reqPk->setIsRecovery(false);

        scheduleAt(simTime()+m_SlotLength*(m_SlotsNum-0.01),m_SelfMsgTimer);

        for(int i=0;i<slotIndexVector.size();i++)
        {
            LdReq *dupReq=reqPk->dup();
            dupReq->setSlotIndex(slotIndexVector[i]);
            dupReq->setKind(PkKinds_Self_IRSA_Host_Data);
            scheduleAt(simTime()+slotIndexVector[i]*m_SlotLength,dupReq);
        }
    }
    else
    {
        EV_DEBUG<<"LdHostMac--AppQueue is empty, no need to request slots."<<endl;
    }
}

void LdHostMac::sendData()
{
    double probabilityofPkError=getPkErrorRate();
    for(int i=0;i<m_DataSlotNum;i++)
    {
        if(!m_AppQueueError.isEmpty())
        {
            cPacket *pk=dynamic_cast<cPacket *>(m_AppQueueError.pop());
            pk->setKind(PkKinds_Host_Data);
            pk->setSrcProcId(setDistanceFlag());

            m_Random=dblrand();
            if(m_Random>probabilityofPkError)
            {
                m_Queuetime=simTime()-pk->getArrivalTime();
                emit(m_SchedulingTimeSignal,m_Queuetime);
                emit(m_SentPkCountSignal,1l);
                sendDirect(pk,0,0,m_Ap->gate("in"));
            }
            else
            {
                m_AppQueueError.insert(pk);
            }
        }
        else
        {
            if(!m_AppQueue.isEmpty())
            {
                cPacket *pk=dynamic_cast<cPacket *>(m_AppQueue.pop());
                pk->setKind(PkKinds_Host_Data);
                pk->setSrcProcId(setDistanceFlag());

                m_Random=dblrand();
                if(m_Random>probabilityofPkError)
                {
                    m_Queuetime=simTime()-pk->getArrivalTime();
                    emit(m_SchedulingTimeSignal,m_Queuetime);
                    emit(m_SentPkCountSignal,1l);
                    sendDirect(pk,0,0,m_Ap->gate("in"));
                }
                else
                {
                    m_AppQueueError.insert(pk);
                }

            }
            else
            {
                EV_DEBUG<<"AppQueue is empty, no data could be sent."<<endl;
            }
        }

    }
}

int LdHostMac::getRepetitionNum()
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

int LdHostMac::setDistanceFlag()
{
    double dDistance=getDistance();
    int nReturn= ceil(10*dDistance/m_CommunicationRange);
    if(nReturn <=0)
    {
        nReturn =1;
    }
    if(nReturn>10)
    {
        nReturn=10;
    }
    return nReturn;
}

void LdHostMac::finish()
{
    recordScalar("Time1",m_Time1);
    recordScalar("Time2",m_Time2);
    recordScalar("Time3",m_Time3);
    recordScalar("Time4",m_Time4);
    recordScalar("Time5",m_Time5);
    recordScalar("Time6",m_Time6);
    recordScalar("Time7",m_Time7);
    recordScalar("Time8",m_Time8);
    recordScalar("Time9",m_Time9);
    recordScalar("Time10",m_Time10);
}
}

