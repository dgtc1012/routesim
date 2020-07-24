#include "messages.h"

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
    *this = rhs;
}

RoutingMessage & RoutingMessage::operator=(const RoutingMessage & rhs) {
    /* For now.  Change if you add data members to the struct */
    return *this;
}

#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "Generic RoutingMessage()";
    return os;
}
#endif

#if defined(LINKSTATE)
RoutingMessage::RoutingMessage(int src, int dst, int cost, int age){
    srcNode = src;
    dstNode = dst;
    linkCost = cost;
    linkAge = age;
}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "LinkState RoutingMessage()";
    return os;
}
#endif

#if defined(DISTANCEVECTOR)
RoutingMessage::RoutingMessage(int s, map<int, TopoLink> distVec){
    sender = s;
    distanceVector = distVec;
}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "DistanceVector RoutingMessage()";
    return os;
}
#endif
