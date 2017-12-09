#ifndef _table
#define _table

#include <iostream>
#include <map>

using namespace std;

struct TopoLink {
    TopoLink(): cost(-1), age(0) {}

    TopoLink(const TopoLink & rhs) {
        *this = rhs;
    }

    TopoLink & operator=(const TopoLink & rhs) {
        this->cost = rhs.cost;
        this->age = rhs.age;

        return *this;
    }

    int cost;
    int age;
};

// Students should write this class
class Table {
    private:
        //map < int, map < int, TopoLink > > topo;
    public:
        Table();
        Table(const Table &);
        Table & operator=(const Table &);

        ostream & Print(ostream &os) const;

        // Anything else you need
        map<int, map<int, TopoLink> > topo; //made this public bc im lazy, I should have made accessor methods

        #if defined(LINKSTATE)
            map<int, int> nextStepTo;
        #endif

        #if defined(DISTANCEVECTOR)
            map<int, TopoLink> linksToNeighbors; //map of links this node has to its neighbors
            map<int, TopoLink> distanceVector; //map of distance vectors
            map<int, int> nextStepTo; //tells you next node in your path to your destination - next link to take to get to dest
        #endif
};

inline ostream & operator<<(ostream &os, const Table & t) { return t.Print(os);}


#endif
