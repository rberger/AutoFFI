
#include "Transit/Catalog.h"

namespace transit {

Catalog::export_range Catalog::getExports() {
  return boost::make_iterator_range(exports.begin(), exports.end());
}

Catalog::type_range Catalog::getTypes() {
  return boost::make_iterator_range(typeSeq.begin(), typeSeq.end());
}

bool Catalog::addType(Type* type) {
  if (typeSet.emplace(type).second) {
    typeSeq.push_back(type);
    return true;
  }
  return false;
}

bool Catalog::addExport(Export* ex) {
  return exports.emplace(ex).second;
}

bool Catalog::hasType(Type* type) const {
  return typeSet.count(type) > 0;
}

bool Catalog::hasExport(Export* ex) const {
  return exports.count(ex) > 0;
}

bool Catalog::removeType(Type* type) {
  if (typeSet.erase(type) > 0) {
    typeSeq.erase(std::find(typeSeq.begin(), typeSeq.end(), type));
    return true;
  }
  return false;
}

bool Catalog::removeExport(Export* ex) {
  return exports.erase(ex) > 0;
}

//struct TypeCollector : public TypeVisitor<TypeCollector> { 
  //std::set<Type*> visited;
  //void visitFunctionType(FunctionType* f) {
    //visited.emplace(f);
    //visit(f->getReturnType());
    //for (auto paramType: f->getParamTypes())
      //visit(paramType);
  //}
//};

void Catalog::removeDanglingTypes() {
}

}

