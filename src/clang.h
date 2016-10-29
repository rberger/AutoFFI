
#ifndef BSTRUCT_H_
#define BSTRUCT_H_

#include <iostream> // for debugging only
#include <map>

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "boost/range.hpp"

using namespace clang;

struct QualTypeLT {
  bool operator()(const QualType& a, const QualType& b) const; 
};

class QualTypeIDMapper : std::map<const QualType, unsigned int, QualTypeLT> {
public:

  using std::map<const QualType, unsigned int, QualTypeLT>::size;

  template<typename It>
  QualTypeIDMapper(It begin, It end) {
    unsigned int i(0);
    for (auto& val: boost::make_iterator_range(begin, end)) {
      emplace(val, i++);
    }
  }

  unsigned int getID(const QualType& val);
  const QualType& getQualType(unsigned int id);

};

using NamedDeclSet = std::set<const NamedDecl*>;
using TypeSet = std::set<const QualType, QualTypeLT>;

class TypesCollector : public TypeVisitor<TypesCollector> {
protected:
  TypeSet types;
public:
  TypeSet& getTypes();
  inline void add(const QualType& q) { types.emplace(q); };
  void VisitType(const Type* type);
  void VisitBuiltinType(const BuiltinType* type);
  void VisitQualType(const QualType& type);
  void VisitTypedefType(const TypedefType* type);
  void VisitFunctionProtoType(const FunctionProtoType* type);
  void VisitRecordType(const RecordType* type);
};

class ScanOperation {
  NamedDeclSet decls;
public:
  boost::iterator_range<NamedDeclSet::iterator> getDecls();
  int scan(std::vector<std::string> sources, std::vector<const char*> compilerArgs = {});
};

const QualType getTypeForNamedDecl(const NamedDecl* decl);

#endif

