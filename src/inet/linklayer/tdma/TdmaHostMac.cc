/*
 * TdmaHostMac.cc
 *
 *  Created on: 2017��6��1��
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
    //��ʼ��
    HostMacBase::initialize();
    //��ȡ����
    m_ReqSlots=par("reqSlotNum");

    //DEBUG
    EV_DEBUG<<this->getFullName()<<" initialize parameters: "<<endl;
    EV_DEBUG<<"reqSlotNum: "<<m_ReqSlots<<endl;

    //�Ŷ�ʱ���ʼ��
    m_QueueTime=0;

    //����Ϣ��ʼ��
    m_SelfMsgReq=new cMessage("SelfMsg-REQ");
    m_SelfMsgData=new cMessage("SelfMsg-DATA");
    //������Ϣ��0ʱ�̿�ʼ����
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
 * ��������Ϣ��
 * m_SelfMsgReq,m_SelfMsgData
 */
void TdmaHostMac::handleSelfMsg(cMessage *msg)
{
    if(msg==m_SelfMsgReq)
    {
        scheduleAt(simTime()+m_SlotLength*m_SlotsNum,m_SelfMsgReq);

        /*
         * �������ͨ�ŷ�Χ������������������ݶ���
         * ���򣬷����������ݰ�
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
        //�������ݰ�
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
     * �����������
     * setKind:���ݰ�����
     * setHostId:�ڵ�ID
     * setHostPath:�ڵ�·��
     * setHostPosition:�ڵ�λ��
     */
    TdmaReq *reqPk=new TdmaReq();
    reqPk->setKind(PkKinds_Host_Request);
    reqPk->setHostId(m_Parent->getId());
    reqPk->setHostPath(m_Parent->getFullPath().c_str());
    reqPk->setHostPosition(getCurrentPosition());

    //ֱ�ӷ��͸�AP,û���Ƕ������
    sendDirect(reqPk,getPropagationSlots()*m_SlotLength,0,m_Ap->gate("in"));
    EV_DEBUG<<m_Parent->getFullPath()<<" successfully sent a request packet. "<<endl;
}

/*
 * �������ݰ�
 * �ܹ��������ݵ�ʱ϶��=AP�����host��ʱ϶��-host�����ӳ�ʱ϶��
 */
void TdmaHostMac::sendData()
{
    int propagationSlots=getPropagationSlots();
    double probabilityofPkError=getPkErrorRate();
    for(int i=0;i<(m_dataSlotNum-propagationSlots);i++)
    {

        /*
         * �жϴ�����������Ƿ������ݰ�
         * if:��
         * else:û��
         */
        if(!m_AppQueueError.isEmpty())
        {
            EV_DEBUG<<"m_AppQueueError is not empty."<<endl;
            //�Ӷ����л�ȡ���ݰ�
            cPacket *pk=dynamic_cast<cPacket *>(m_AppQueueError.pop());
            pk->setKind(PkKinds_Host_Data);

            /*
             * �ɹ����ͺͷ���ʧ�ܵ����
             */
            m_Random=dblrand();
            //�ɹ�����
            if(m_Random>probabilityofPkError)
            {
                //emit(m_SchedulingTimeSignal,(simTime()-pk->getArrivalTime()));
                sendDirect(pk,m_Ap->gate("in"));
                emit(m_SentPkCountSignal,1l);
                EV_DEBUG<<"Host has sent a packet to AP."<<endl;
            }
            //����ʧ��
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
                //�Ӷ����л�ȡ���ݰ�
                cPacket *pk=dynamic_cast<cPacket *>(m_AppQueue.pop());
                pk->setKind(PkKinds_Host_Data);

                /*
                 * �ɹ����ͺͷ���ʧ�ܵ����
                 */
                m_Random=dblrand();
                //�ɹ�����
                if(m_Random>probabilityofPkError)
                {
                    //emit(m_SchedulingTimeSignal,(simTime()-pk->getArrivalTime()));
                    sendDirect(pk,m_Ap->gate("in"));
                    emit(m_SentPkCountSignal,1l);
                    EV_DEBUG<<"Host has sent a packet to AP."<<endl;
                }
                //����ʧ��
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



