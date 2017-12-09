#include "linkstate.h"
#include "context.h"
#include <set>

LinkState::LinkState(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

LinkState::LinkState(const LinkState & rhs) :
    Node(rhs)
{
    *this = rhs;
}

LinkState & LinkState::operator=(const LinkState & rhs) {
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState() {}


/** Write the following functions.  They currently have dummy implementations **/
void LinkState::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;
    
    //get src, dst, and latency info from new link 
    int src = l->GetSrc();
    int dst = l->GetDest();
    int latency = l->GetLatency();
    //assign latency info to table
    routing_table.topo[src][dst].cost = latency;
    //increment age of link info
    routing_table.topo[src][dst].age++;
    //run dijkstra
    Dijkstra();
    //send new link info to neighbors
    SendToNeighbors(new RoutingMessage(src, dst, latency, routing_table.topo[src][dst].age));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
    //get src, dst, latency and age from msg
    int src = m->srcNode;
    int dst = m->dstNode;
    int latency = m->linkCost;
    int newAge = m->linkAge;
    //check if you already have info about this link or if this info is newer than what you have
        //then updated the routing table
    if(routing_table.topo[src][dst].cost == -1 || routing_table.topo[src][dst].age < newAge){
        routing_table.topo[src][dst].cost = latency;
        routing_table.topo[src][dst].age = newAge;
        //run dijkstra() again, note that it has changed and send the message you recieved to your neighbors
        Dijkstra();
        SendToNeighbors(m);
    }
    
}

void LinkState::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* LinkState::GetNextHop(Node *destination) { 
    //get the next hop from the nextStepTo array.
    int hop = destination->GetNumber();
    //get node num of destination, look for this number in the nextStepTo array
    while(routing_table.nextStepTo[hop] != number){
        hop = routing_table.nextStepTo[hop];
    }
    //make temp node of the corresponding next step, find matching node
    Node *h = new Node(hop, NULL, 0, 0);
    Node *actual = context->FindMatchingNode(const_cast<Node *>(h));
    return actual;
}

Table* LinkState::GetRoutingTable() {
    Table* t = new Table(routing_table);
    return t;
}

void LinkState::Dijkstra(){
    map<int, int> distance;
    map<int, map<int, TopoLink> >::const_iterator topoItr;
    map<int, int>::const_iterator distItr;
    map<int, int> previous;
    set<int> visited;
    int tableSize = routing_table.topo.size();
    
    for (topoItr = routing_table.topo.begin(); topoItr != routing_table.topo.end(); topoItr++) {
        routing_table.topo[topoItr->first][topoItr->first].cost = 0;
    }
    
    //init distance and prev
    for(int i = 0; i < tableSize; i++){
        distance[i] = 21474783;
        previous[i] = -1;
    }
    
    distance[number] = 0; //distance to self is 0
    previous[number] = number;
    
    for(int i = 0; i < tableSize - 1; i++){
        int u = -1;
        int min = 21474783;
        
        for(topoItr = routing_table.topo.begin(); topoItr != routing_table.topo.end(); topoItr++){
            if(visited.count(topoItr->first) < 1 && distance[topoItr->first] <= min){
                min = distance[topoItr->first];
                u = topoItr->first;
            }
        }
        
        visited.insert(u);
        for(topoItr = routing_table.topo.begin(); topoItr != routing_table.topo.end(); topoItr++){
            int v = topoItr->first;
            if(visited.count(v) < 1 && routing_table.topo[u][v].cost != -1 && distance[u] != 21474783 && distance[u] + routing_table.topo[u][v].cost < distance[v]){
                distance[v] = distance[u] + routing_table.topo[u][v].cost;
                previous[v] = u;
            }
        }
    }
    routing_table.nextStepTo = previous;
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
