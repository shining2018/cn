/*
 * BurstTraffic.cc
 *
 *  Created on: 2017Äê6ÔÂ1ÈÕ
 *      Author: NETLAB
 */
#include "BurstTraffic.h"
#include "inet/common/PkKinds.h"

namespace inet
{
Define_Module(BurstTraffic);

BurstTraffic::BurstTraffic()
{
    m_BurstInterval=nullptr;
    m_PksPerBurst=nullptr;

    m_SelfmsgTimer=nullptr;
}

BurstTraffic::~BurstTraffic()
{
    cancelAndDelete(m_SelfmsgTimer);
}

void BurstTraffic::initialize()
{
    //get parameter values from .ini file
    m_StartTime=par("startTime");
    m_PkLength=par("pkLength");
    m_BurstInterval=&par("burstInterval");
    m_PksPerBurst=&par("pksPerBurst");

    m_PkNo=0;

    m_SelfmsgTimer=new cMessage("SendTimer");

    //register signal
    m_PkCountSignal=registerSignal("pkCount");

    scheduleNextBurst(-1);

}

void BurstTraffic::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        if(msg==m_SelfmsgTimer)
        {
            sendBurst();
            scheduleNextBurst(simTime());
        }
        else
        {
            throw cRuntimeError("BurstTraffic--Self Message Type Error!");
        }
    }
    else
    {
        throw cRuntimeError("BurstTraffic--Non-self Message Type Error!");
    }
}

void BurstTraffic::sendBurst()
{
    long pksPerBurst=m_PksPerBurst->longValue();
    EV_DEBUG<<pksPerBurst<<" packets are generating."<<endl;
    for(int i=0;i<pksPerBurst;i++)
    {
        m_PkNo++;
        char pkName[40];
        sprintf(pkName,"App-%d",m_PkNo);
        cPacket *pk=new cPacket(pkName);
        pk->setKind(PkKinds_AppPk);
        pk->setBitLength(m_PkLength);
        emit(m_PkCountSignal,1l);
        send(pk,"out");
    }
}

void BurstTraffic::scheduleNextBurst(simtime_t lastTime)
{
    simtime_t nextTime;
    double burstInterval=m_BurstInterval->doubleValue();
    if(lastTime==-1)
    {
        nextTime=simTime()<=m_StartTime?m_StartTime:simTime();
        nextTime=nextTime+burstInterval;
    }
    else
    {
        nextTime=lastTime+burstInterval;
        if(nextTime<simTime())
        {
            throw cRuntimeError("BurstTraffic--next scheduling time is wrong!");
        }
    }
    EV_DEBUG<<"Last time is: "<<lastTime<<", interval is:"<<burstInterval<<", and next scheduling time is: "<<nextTime<<endl;

    scheduleAt(nextTime,m_SelfmsgTimer);
}


}



