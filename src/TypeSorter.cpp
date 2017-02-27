#include "AutoFFI/TypeSorter.h"

#include "boost/graph/topological_sort.hpp"

namespace autoffi {

struct CycleDetector : public boost::default_dfs_visitor {
    bool& cycle;
    CycleDetector(bool& cycle): cycle(cycle) {}
    template <class Edge, class Graph>
    void back_edge(Edge e, Graph& g) {
      cycle = true;
    }
};

template<typename Graph>
bool has_cycles(Graph g) {
  bool cycle = false;
  CycleDetector cycleDetector(cycle);
  boost::depth_first_search(g, boost::visitor(cycleDetector));
  return cycleDetector.cycle;
}

void TypeSorter::addDependency(Type* a, Type* b) {
  auto u(getVertex(a)), v(getVertex(b));
  if (!boost::edge(u, v, g).second) {
    boost::add_edge(u, v, g);
    if (has_cycles(g)) {
      boost::remove_edge(u, v, g);
    }
  }
}

void TypeSorter::visitFunctionType(FunctionType* type) {
  addDependency(type, type->getReturnType());
  for (auto paramType: type->getParamTypes())
    addDependency(type, paramType);
}

void TypeSorter::visitQualType(QualType* type) {
  addDependency(type, type->getUnderlyingType());
}

void TypeSorter::visitPointerType(PointerType* type) {
  addDependency(type, type->getReferencedType());
}

void TypeSorter::visitArrayType(ArrayType* type) {
  addDependency(type, type->getElementType());
}

void TypeSorter::visitRecordType(RecordType* type) {
  for (auto field: type->getFields())
    addDependency(type, field.type);
}

//void TypeSorter::addType(Type* type) {
  //vertexMap.insert(VertexMap::value_type(type, boost::add_vertex(g)));
  //visit(type);
//}

void TypeSorter::sort() {
  std::deque<Graph::vertex_descriptor> sorted;
  boost::topological_sort(g, std::back_inserter(sorted));
  for (auto v: sorted) {
    push_back(g[v].type);
  }
}

}
