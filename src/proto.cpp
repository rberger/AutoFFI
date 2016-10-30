
#include "Transit/Analyser.h"
#include "ast.pb.h"

#include "llvm/Support/CommandLine.h"

using namespace llvm;

static cl::list<std::string> Files(cl::Positional, cl::desc("Files to parse"), cl::OneOrMore);

namespace transit {

proto::Type* convert(const Type* type) {
  switch (type->kind) {
  case Type::QUALIFIED: {
    auto message(convert(static_cast<const QualType*>(type)->underlyingType));
  } case Type::PRIMITIVE: {
    auto message(new proto::Type);
    message->set_kind(proto::Type::PRIMITIVE);
  } case Type::ENUM: {
    auto message(new proto::Type);
    message->set_kind(proto::Type::ENUM);
  } case Type::RECORD: {
    auto message(new proto::Type);
    switch(static_cast<const RecordType*>(type)->recordType) {
    case RecordType::STRUCT:
      message->set_kind(proto::Type::STRUCT);
    case RecordType::UNION:
      message->set_kind(proto::Type::UNION);
    };
  }
  }
}

}

int main(int argc, const char* argv[]) {

  auto sep(std::find_if(argv, argv+argc, [](const char* arg) {
    return !strcmp(arg, "--");
  }));

  std::vector<const char*> compilerArgs(sep == argv+argc ? sep : sep+1, argv+argc);

  cl::ParseCommandLineOptions(sep-argv, argv);

  transit::Analyser a;

  if (int res = a.scan(Files, compilerArgs))
    return res;
 
  return 0;
}

