/*
 * ApMacBase.cc
 *
 *  Created on: 2017Äê6ÔÂ3ÈÕ
 *      Author: NETLAB
 */
#include "inet/linklayer/common/ApMacBase.h"

namespace inet
{
Define_Module(ApMacBase);

ApMacBase::ApMacBase()
{

}

ApMacBase::~ApMacBase()
{

}

void ApMacBase::initialize()
{
    m_SlotLength=par("slotLength");
    m_SlotNum=par("slotNum");
    m_CommunicationRange=par("communicationRange");
    EV_DEBUG<<this->getFullPath()<<" initialize parameters: "<<endl;
    EV_DEBUG<<"slotLength: "<<m_SlotLength<<endl;
    EV_DEBUG<<"slotNum: "<<m_SlotNum<<endl;
    EV_DEBUG<<"communicationRange: "<<m_CommunicationRange<<endl;


    m_PkCountSignal=registerSignal("pkCount");
    m_pkPerSecond=0;
    m_pkCount=0;
}

void ApMacBase::finish()
{
    //m_pkPerSecond=m_pkCount/simTime();
    recordScalar("pkPerSecond", m_pkCount/simTime());
}

}



