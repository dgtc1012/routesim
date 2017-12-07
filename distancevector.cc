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
    
    routing_table.linksToNeighbors[dest].cost = updatedLatency; //neighbor cost = new cost
    routing_table.distanceVector[dest].cost = -1; //trigger recalc
    routing_table.topo[dest][dest].cost = 0; //topo for dest to self is 0
    
    //check if this is an actual update and do it, if it is send msg to neighbors with new dist vector for them to update.
    if(checkUpdated()){
        SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
    }
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m /*<< " (ignored)"*/ << endl;
    //update table with new distance vector
    routing_table.topo[m->sender] = m->distanceVector;
    //iterate through the message distance vector
    map<int, TopoLink>::const_iterator msgItr;
    //tbh i dont remember where I was going with this but I dont think it does anything?
    for(msgItr = m->distanceVector.begin(); msgItr != m->distanceVector.end(); msgItr++){
        if(routing_table.distanceVector[msgItr->first].cost == -1){
            
        }
    }
    
    //check if this distance vector is new information, if so, send out updated distance vector to neighbors
    if(checkUpdated()){
        SendToNeighbors(new RoutingMessage(number, routing_table.distanceVector));
    }
}

void DistanceVector::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* DistanceVector::GetNextHop(Node *destination) { 
    //make temporary node n of destination edge
    Node *n = new Node(routing_table.nextStepTo[destination->GetNumber()], NULL, 0, 0);
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

bool DistanceVector::checkUpdated(){
    bool updated = false;
    
    //iterator for distance vectors
    map<int, TopoLink>::const_iterator distVec;
    //iterator for neighbors
    map<int, TopoLink>::const_iterator neighbor;
    
    //check all distance vectors
    for (distVec = routing_table.distanceVector.begin(); distVec != routing_table.distanceVector.end(); distVec++){
        //node number of current dist vector
        int curNode = distVec->first;
        
        if((unsigned)curNode == number){
            //if the current node is you, set cost to 0
            routing_table.distanceVector[curNode].cost = 0;
            continue;
        }
        
        int newMin = 21474783; //just the max value an int can be?
        int newPath = 0; //new path length
        int nextHop = -1; //next place you can go after this
        
        for(neighbor = routing_table.linksToNeighbors.begin(); neighbor != routing_table.linksToNeighbors.end(); neighbor++){
            int curNeighbor = neighbor->first; //go through neighbors
            
            if(routing_table.linksToNeighbors[curNeighbor].cost != -1 && routing_table.topo[curNeighbor][curNode].cost != -1){
                //new path cost is cost to neighbor plus the new cost from the new distvector if it was updated
                newPath = routing_table.linksToNeighbors[curNeighbor].cost + routing_table.topo[curNeighbor][curNode].cost;
                
                //if new path is less than min, new min is new path and the next hop is that neighbor
                if(newPath < newMin){
                    newMin = newPath;
                    nextHop = curNeighbor;
                }
            }
        }
        //if there was a min found and it isnt equal to the current value, replace it in the table
        if(newMin != 21474783 && newMin != routing_table.distanceVector[curNode].cost){
            routing_table.distanceVector[curNode].cost = newMin;
            routing_table.nextStepTo[curNode] = nextHop;
            updated = true;
        }
    }    
    
    
    return updated;
}

ostream & DistanceVector::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
