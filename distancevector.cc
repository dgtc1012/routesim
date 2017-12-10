#include "distancevector.h"
#include "context.h"

DistanceVector::DistanceVector(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

DistanceVector::DistanceVector(const DistanceVector & rhs) :
    Node(rhs)
{
    *this = rhs;
}

DistanceVector & DistanceVector::operator=(const DistanceVector & rhs) {
    Node::operator=(rhs);
    return *this;
}

DistanceVector::~DistanceVector() {}

/** Write the following functions.  They currently have dummy implementations **/
void DistanceVector::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;
    
    //get new link update command
    //get destination and latancy
    int dest = l->GetDest();
    int updatedLatency = l->GetLatency();
    
    //neighbor cost = new cost
    routing_table.linksToNeighbors[dest].cost = updatedLatency; 
    //save the old latency to modify table
    int oldLatency = routing_table.topo[dest][dest].cost;
    //set new latency
    routing_table.topo[dest][dest].cost = updatedLatency;
    
    //if there was no way to get to this node before, set the shortest path to this latency
    if(routing_table.distanceVector[dest].cost != -1){
        //there was already a link existing here, so iterate through its links with other nodes
        map<int, TopoLink>::const_iterator vectorItr;
        for(vectorItr = routing_table.topo[dest].begin(); vectorItr != routing_table.topo[dest].end(); vectorItr++){
            int toNode = vectorItr->first;
            if(routing_table.topo[dest][toNode].cost != -1 && toNode != dest){
                routing_table.topo[dest][toNode].cost -= oldLatency;
                routing_table.topo[dest][toNode].cost += updatedLatency;
            }
        }
    }
    
    bool updated = UpdateDistanceVector();
    //cerr << "distance vector updated " << updated << std::endl;
    if(updated){
        SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
    }
    
}

bool DistanceVector::UpdateDistanceVector(){
    bool updated = false;
    
    //iterator for distance vectors
    map<int, TopoLink>::const_iterator dest;
    //iterator for neighbors
    map<int, TopoLink>::const_iterator neighbor;
    map<int, int> mins;
    map<int, int> nextHops;
        
    for(neighbor = routing_table.linksToNeighbors.begin(); neighbor != routing_table.linksToNeighbors.end(); neighbor++){
        int viaNode = neighbor->first;
        for(dest = routing_table.topo[viaNode].begin(); dest != routing_table.topo[viaNode].end(); dest++){
            int toNode = dest->first;
            //cerr << "path from node " << number << " to node " << toNode << " via node " << viaNode << " costs " << routing_table.topo[viaNode][toNode].cost << std::endl;
            if((unsigned)toNode == number){
                //if the current node is you, set cost to 0
                routing_table.distanceVector[toNode].cost = 0;
            }
            else if(mins[toNode] == 0 && routing_table.topo[viaNode][toNode].cost != -1){
                //cerr << "no previous min, new min is " << routing_table.topo[viaNode][toNode].cost << endl;
                mins[toNode] = routing_table.topo[viaNode][toNode].cost;
                nextHops[toNode] = viaNode;
            }
            else if(routing_table.topo[viaNode][toNode].cost < mins[toNode] && routing_table.topo[viaNode][toNode].cost != -1){
                //cerr << "previous min was " << mins[toNode] << ", new min is " << routing_table.topo[viaNode][toNode].cost << endl;
                
                mins[toNode] = routing_table.topo[viaNode][toNode].cost;
                nextHops[toNode] = viaNode;
            }
        }
    }
    
    map<int, int>::const_iterator update;
    for(update = mins.begin(); update != mins.end(); update++){
        int toNode = update->first;
        if(routing_table.distanceVector[toNode].cost != mins[toNode] && mins[toNode] != 0){
            routing_table.distanceVector[toNode].cost = mins[toNode];
            routing_table.nextStepTo[toNode] = nextHops[toNode];
            updated = true;
        }
    }
    
    return updated;
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " from node " << m->sender << endl;
    
    int src = m->sender;
    
    map<int, TopoLink> newDistanceVector = m->distanceVector;
    map<int, TopoLink>::const_iterator newVecItr;
    
    /*for(newVecItr = newDistanceVector.begin(); newVecItr != newDistanceVector.end(); newVecItr++){
        cerr << "Node " << m->sender << " has a path to " << newVecItr->first << " with a cost of " << newVecItr->second.cost << std::endl;
    }*/
    
    int distToSrc = routing_table.topo[src][src].cost;
    
    for(newVecItr = newDistanceVector.begin(); newVecItr != newDistanceVector.end(); newVecItr++){
        int toNode = newVecItr->first;
        if(number != toNode){
            routing_table.topo[src][toNode].cost = distToSrc + newVecItr->second.cost;
            //cerr << "Updated Path from node " << number << " to node " << toNode << " via node " << src << " costs " << routing_table.topo[src][toNode].cost << std::endl;
        }
        else{
            routing_table.topo[src][toNode].cost = 0;
            //cerr << "Updated Path from node " << number << " to node " << toNode << " via node " << src << " costs " << routing_table.topo[src][toNode].cost << std::endl;
        }
    }
    
    bool updated = UpdateDistanceVector();
    //cerr << "distance vector updated " << updated << std::endl;
    if(updated){
        SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
    }
}

void DistanceVector::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* DistanceVector::GetNextHop(Node *destination) { 
    //make temporary node n of destination edge
    Node *n = new Node(routing_table.nextStepTo[destination->GetNumber()], NULL, 0, 0);
    cerr << std::endl << "next hop to " << destination->GetNumber() << " is " << routing_table.nextStepTo[destination->GetNumber()] << std::endl;
    //look for matching node to find the next hop to get to the destination
    Node *actual = context->FindMatchingNode(const_cast<Node *>(n));
    return actual;
    //return NULL;
}

Table* DistanceVector::GetRoutingTable() {
    //make copy of the routing_table, return pointer to copy
    Table *t = new Table(routing_table);
    return t;
    //return NULL;
}

ostream & DistanceVector::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
