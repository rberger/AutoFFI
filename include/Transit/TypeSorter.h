
#ifndef TRANSIT_TYPE_SORT_H_
#define TRANSIT_TYPE_SORT_H_

#include <iostream> // for debugging only

#include "AST.h"
#include "CountMap.hpp"

#include <deque>
#include <list>

#include "boost/property_map/property_map.hpp"
#include "boost/range/size.hpp"
#include "boost/bimap.hpp"
#include "boost/graph/adjacency_list.hpp"

namespace transit {

struct vertex_info {
  Type* type;
};

class TypeSorter : std::deque<Type*>, public TypeVisitor<TypeSorter> {
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_info>;
  Graph g;
  using VertexMap = std::map<Type*, Graph::vertex_descriptor>;
  VertexMap m;
  using TypePropertyMap = boost::property_map<Graph, Type, vertex_info>;
  std::deque<Type*> sorted;
  void sort();
  void addDependency(Type* a, Type* b);
  inline Graph::vertex_descriptor getVertex(Type* type) { return m[type]; }
public:

  void visitFunctionType(FunctionType* type); 
  void visitPointerType(PointerType* type);
  void visitRecordType(RecordType* type);
  void visitArrayType(ArrayType* type);
  void visitQualType(QualType* type);

  using std::deque<Type*>::begin;
  using std::deque<Type*>::end;
  using std::deque<Type*>::size;

  template<typename Range>
  TypeSorter(Range range): g(boost::size(range)) {
     // generate graph vertices
     int v = 0;
     for (auto type: range) {
       m.emplace(type, v);
       g[v++].type = type;
     }
     // add edges
     for (auto type: range)
       visit(type);
     // perform topological sort
     sort();
  }
};

}

#endif

