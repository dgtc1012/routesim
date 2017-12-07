#include "table.h"

Table::Table() {
    topo.clear();
}

Table::Table(const Table & rhs) {
    *this = rhs;
}

Table & Table::operator=(const Table & rhs) {
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;
    
    #if defined(LINKSTATE)
        nextStepTo = rhs.nextStepTo;
        wasChanged = rhs.wasChanged;
    #endif
    #if defined(DISTANCEVECTOR)
        linksToNeighbors = rhs.linksToNeighbors;
        distanceVector = rhs.distanceVector;
        nextStepTo = rhs.nextStepTo;
    #endif
    return *this;
}

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  os << "Generic Table()";
  return os;
}
#endif

#if defined(LINKSTATE)
ostream & Table::Print(ostream &os) const
{
  os << "LinkState Table()";
  
  map<int, map<int, TopoLink> >::const_iterator tableRow = topo.begin();
  while(tableRow != topo.end()){
      map<int, TopoLink>::const_iterator tableCol = (*tableRow).second.begin();
      while(tableCol != (*tableRow).second.end()){
        os << "src = " << (*tableRow).first << " dest = " << (*tableCol).first << " cost = " << (*tableCol).second.cost << " age = " << (*tableCol).second.age << std::endl;
        tableCol++;
      }
      tableRow++;
  }
  
  return os;
}
#endif

#if defined(DISTANCEVECTOR)
ostream & Table::Print(ostream &os) const
{
  os << std::endl << "DistanceVector Table()" << std::endl;
  
  map<int, TopoLink>::const_iterator i = distanceVector.begin();
  while(i != distanceVector.end()){
      os << "Node: " << (*i).first << " Distance: " << (*i).second.cost << std::endl;
      i++;
  }
  
  return os;
}
#endif
