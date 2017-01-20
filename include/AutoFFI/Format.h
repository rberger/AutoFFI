
#ifndef TRANSIT_JSON_FORMATTER_H_
#define TRANSIT_JSON_FORMATTER_H_

#include "AST.h"
#include "Catalog.h"

namespace autoffi {

struct WriteOptions {
  enum Format {
    JSON,
    PROTOBUF,
    DEBUGSTR
  };
  Format format = PROTOBUF;
  bool whitespaces = false;
};

struct ReadOptions {
  enum Format {
    JSON,
    PROTOBUF
  };
  Format format = PROTOBUF;
};

class Format {
public:
  void write(Catalog& c, std::ostream& out, WriteOptions& opts); 
  void read(Catalog& c, std::istream& in, ReadOptions& opts);
};

}

#endif

