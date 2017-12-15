/*
 * HostMacBase.cc
 *
 *  Created on: 2017Äê6ÔÂ1ÈÕ
 *      Author: NETLAB
 */
#include "inet/linklayer/common/HostMacBase.h"

namespace inet
{
Define_Module(HostMacBase);

HostMacBase::HostMacBase()
{

}

HostMacBase::~HostMacBase()
{

}

void HostMacBase::initialize()
{
    findAp();
    findParentModule();
    initAllParameters();
    registerAllSignals();
}

void HostMacBase::findAp()
{
    m_Ap=getModuleByPath("ap");
    if(!m_Ap)
    {
        throw cRuntimeError("HostMacBase--Ap not found!");
    }
    else
    {
        EV_DEBUG<<this->getFullName()<<" found Ap: "<<m_Ap->getFullName()<<endl;
    }
}

void HostMacBase::findParentModule()
{
    m_Parent=getParentModule();
    if(!m_Parent)
    {
        throw cRuntimeError("HostMacBase--Parent module not found");
    }
    else
    {
        EV_DEBUG<<this->getFullName()<<"'s parent is: "<<m_Parent->getFullName()<<endl;
    }
}

void HostMacBase::initAllParameters()
{
    m_TxRate=par("txRate");
    m_CommunicationRange=par("communicationRange");
    m_SlotLength=par("slotLength");
    m_SlotsNum=par("slotNum");
    EV_DEBUG<<this->getFullName()<<" initialize parameters: "<<endl;
    EV_DEBUG<<"txRate: "<<m_TxRate<<endl;
    EV_DEBUG<<"communicationRange: "<<m_CommunicationRange<<endl;
    EV_DEBUG<<"slotLength: "<<m_SlotLength<<endl;
    EV_DEBUG<<"slotNum: "<<m_SlotsNum<<endl;
}

void HostMacBase::registerAllSignals()
{
    m_SchedulingTimeSignal=registerSignal("schedulingTime");
    m_DiscardPkCountSignal=registerSignal("discardPkCount");
    m_SentPkCountSignal=registerSignal("sentPkCount");
}

//void HostMacBase::handleMessage(cMessage *msg)
//{
//
//}

Coord HostMacBase::getCurrentPosition()
{
    cModule *mdMobility=m_Parent->getSubmodule("mobility");
    IMobility *mdMovingMobility=dynamic_cast<IMobility *>(mdMobility);
    return mdMovingMobility->getCurrentPosition();
}

Coord HostMacBase::getApPosition()
{
    cModule *mdMobilityOfAp=m_Ap->getSubmodule("mobility");
    IMobility *mdMovingMobilityOfAp=dynamic_cast<IMobility *>(mdMobilityOfAp);
    return mdMovingMobilityOfAp->getCurrentPosition();
}

double HostMacBase::getDistance()
{
    Coord cdHost=getCurrentPosition();
    Coord cdAp=getApPosition();
    Coord cdDistance=cdHost-cdAp;

    return sqrt(cdDistance.x*cdDistance.x+cdDistance.y*cdDistance.y+cdDistance.z*cdDistance.z);
}

int HostMacBase::getPropagationSlots()
{
    double dDistance=getDistance();
    simtime_t stPropagationTime=dDistance/SPEED_OF_LIGHT;
    return ceil(stPropagationTime/m_SlotLength);
}

int HostMacBase::getMaxPropagationSlts()
{
    simtime_t stMaxPropagationTime=m_CommunicationRange/SPEED_OF_LIGHT;
    return ceil(stMaxPropagationTime/m_SlotLength);
}

simtime_t HostMacBase::getNearestFrameHead()
{
    return ceil(simTime()/(m_SlotLength*m_SlotsNum))*m_SlotLength*m_SlotsNum;
}

bool HostMacBase::isOutOfCommunicationRange()
{
    if(getDistance()>m_CommunicationRange)
    {
        EV_DEBUG<<"Host is out of communication range."<<endl;
        return true;
    }
    else
    {
        EV_DEBUG<<"Host is in communication range."<<endl;
        return false;
    }
}

//TODO:need to modify
double HostMacBase::getPkErrorRate()
{
    return 0.001;
}

bool HostMacBase::isAppQueueFull()
{
    if(m_AppQueue.getLength()==QUEUE_CAPACITY)
    {
        EV_DEBUG<<"AppQueue is full!"<<endl;
        return true;
    }
    else
    {
        if(m_AppQueue.getLength()<QUEUE_CAPACITY)
        {
            return false;
        }
        else
        {
            throw cRuntimeError("AppQueue is out of bound.");
        }
    }
}
}



