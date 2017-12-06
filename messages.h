#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;

    // Anything else you need

    #if defined(LINKSTATE)
    #endif
    #if defined(DISTANCEVECTOR)
        //content of routing message, contains node sending the message and distanceVector to be sent
        RoutingMessage(int s, map<int, TopoLink>);
        int sender; //node sending message
        map<int, TopoLink> distanceVector; //distance vector
    #endif
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif
