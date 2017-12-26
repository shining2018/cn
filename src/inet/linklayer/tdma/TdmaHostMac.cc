/*
 * TdmaHostMac.cc
 *
 *  Created on: 2017年6月1日
 *      Author: NETLAB
 */
#include "inet/linklayer/tdma/TdmaHostMac.h"
#include "inet/common/PkKinds.h"

namespace inet
{
Define_Module(TdmaHostMac);

TdmaHostMac::TdmaHostMac()
{
    m_SelfMsgReq=nullptr;
    m_SelfMsgData=nullptr;
}

TdmaHostMac::~TdmaHostMac()
{
    cancelAndDelete(m_SelfMsgReq);
    cancelAndDelete(m_SelfMsgData);
}

void TdmaHostMac::initialize()
{
    //初始化
    HostMacBase::initialize();
    //获取参数
    m_ReqSlots=par("reqSlotNum");

    //DEBUG
    EV_DEBUG<<this->getFullName()<<" initialize parameters: "<<endl;
    EV_DEBUG<<"reqSlotNum: "<<m_ReqSlots<<endl;

    //排队时间初始化
    m_QueueTime=0;

    //自消息初始化
    m_SelfMsgReq=new cMessage("SelfMsg-REQ");
    m_SelfMsgData=new cMessage("SelfMsg-DATA");
    //请求消息从0时刻开始发送
    scheduleAt(0,m_SelfMsgReq);
}

void TdmaHostMac::handleMessage(cMessage *msg)
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

/*
 * 处理自消息：
 * m_SelfMsgReq,m_SelfMsgData
 */
void TdmaHostMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgReq)
    {
        scheduleAt(simTime()+m_SlotLength*m_SlotsNum,m_SelfMsgReq);

        /*
         * 如果超出通信范围，不发送请求，清空数据队列
         * 否则，发送请求数据包
         */
        if(isOutOfCommunicationRange())
        {
            m_AppQueue.clear();
        }
        else
        {
            sendReq();
        }
    }
    else
    {
        //发送数据包
        if(msg==m_SelfMsgData)
        {
            sendData();
        }
        else
        {
            throw cRuntimeError("TdmaHostMac--Self message type error!");
        }
    }
}

void TdmaHostMac::handleNonSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
    case PkKinds_AppPk :
    {
        if(isAppQueueFull())
        {
            emit(m_DiscardPkCountSignal,1l);

            delete msg;

            EV_DEBUG<<"AppQueue is full, and the new arrival message has been deleted."<<endl;
        }
        else
        {
            msg->setArrivalTime(simTime());
            m_AppQueue.insert(msg);
            EV_DEBUG<<"The new arrival message has been inserted into AppQueue."<<endl;
        }

        break;
    }
    case PkKinds_Ap_ACK :
    {
        TdmaACK *ackPk=dynamic_cast<TdmaACK *>(msg);
        m_dataSlotIndex=ackPk->getSlotIndex();
        m_dataSlotNum=ackPk->getSlotNum();

        delete msg;

        //simtime_t dataSendTime=simTime()+m_SlotLength*(m_dataSlotIndex-(m_ReqSlots-2));
        simtime_t dataSendTime=getNearestFrameHead()-(m_SlotsNum-m_ReqSlots)*m_SlotLength+m_dataSlotIndex*m_SlotLength;
        scheduleAt(dataSendTime,m_SelfMsgData);
        break;
    }
    default:
    {
        throw cRuntimeError("TdmaHostMac--Non Self message type error!");
    }
    }
}

void TdmaHostMac::sendReq()
{
    /*
     * 生成请求包：
     * setKind:数据包类型
     * setHostId:节点ID
     * setHostPath:节点路径
     * setHostPosition:节点位置
     */
    TdmaReq *reqPk=new TdmaReq();
    reqPk->setKind(PkKinds_Host_Request);
    reqPk->setHostId(m_Parent->getId());
    reqPk->setHostPath(m_Parent->getFullPath().c_str());
    reqPk->setHostPosition(getCurrentPosition());

    //直接发送给AP,没考虑丢包情况
    sendDirect(reqPk,getPropagationSlots()*m_SlotLength,0,m_Ap->gate("in"));
    EV_DEBUG<<m_Parent->getFullPath()<<" successfully sent a request packet. "<<endl;
}

/*
 * 发送数据包
 * 能够发送数据的时隙数=AP分配给host的时隙数-host传播延迟时隙数
 */
void TdmaHostMac::sendData()
{
    int propagationSlots=getPropagationSlots();
    double probabilityofPkError=getPkErrorRate();
    for(int i=0;i<(m_dataSlotNum-propagationSlots);i++)
    {

        /*
         * 判断错误包缓存区是否有数据包
         * if:有
         * else:没有
         */
        if(!m_AppQueueError.isEmpty())
        {
            EV_DEBUG<<"m_AppQueueError is not empty."<<endl;
            //从队列中获取数据包
            cPacket *pk=dynamic_cast<cPacket *>(m_AppQueueError.pop());
            pk->setKind(PkKinds_Host_Data);

            /*
             * 成功发送和发送失败的情况
             */
            m_Random=dblrand();
            //成功发送
            if(m_Random>probabilityofPkError)
            {
                //emit(m_SchedulingTimeSignal,(simTime()-pk->getArrivalTime()));
                sendDirect(pk,m_Ap->gate("in"));
                emit(m_SentPkCountSignal,1l);
                EV_DEBUG<<"Host has sent a packet to AP."<<endl;
            }
            //发送失败
            else
            {
                EV_DEBUG<<"Host failed to send a packet, and store it to m_AppQueueError."<<endl;
                m_AppQueueError.insert(pk);
            }
        }
        else
        {
            if(!m_AppQueue.isEmpty())
            {
                //从队列中获取数据包
                cPacket *pk=dynamic_cast<cPacket *>(m_AppQueue.pop());
                pk->setKind(PkKinds_Host_Data);

                /*
                 * 成功发送和发送失败的情况
                 */
                m_Random=dblrand();
                //成功发送
                if(m_Random>probabilityofPkError)
                {
                    //emit(m_SchedulingTimeSignal,(simTime()-pk->getArrivalTime()));
                    sendDirect(pk,m_Ap->gate("in"));
                    emit(m_SentPkCountSignal,1l);
                    EV_DEBUG<<"Host has sent a packet to AP."<<endl;
                }
                //发送失败
                else
                {
                    EV_DEBUG<<"Host failed to send a packet, and store it to m_AppQueueError."<<endl;
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


}



