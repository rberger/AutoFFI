
/**
 * Transit AST serializer and deserializer using Google Protocol buffers.
 */

#include <iostream>

#include "AutoFFI/AST.h"
#include "AutoFFI/CountMap.hpp"
#include "AutoFFI/Format.h"
#include "autoffi.pb.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

namespace autoffi {

struct TypeToMessageConverter : public ConstTypeVisitor<TypeToMessageConverter> {

  CountMap<const Type*>& index;

  proto::Catalog& c;

  TypeToMessageConverter(proto::Catalog* out, CountMap<const Type*>& index): c(*out), index(index) {}

  void visitType(const Type* type) {
    throw std::runtime_error("unvisited type");
  }

  void visitFixedArrayType(const FixedArrayType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::ARRAY);
    message->set_size(type->getCount());
    message->set_elementtype(index.getID(type->getElementType()));
  }

  void visitPointerType(const PointerType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_referencedtype(index.getID(type->getReferencedType()));
    message->set_kind(proto::TypeKind::POINTER);
    if (type->isBlockPtr())
      message->set_isblockpointer(true);
  }

  void visitVariadicArrayType(const VariadicArrayType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::ARRAY);
    message->set_elementtype(index.getID(type->getElementType()));
  }

  void visitQualType(const QualType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::QUALIFIED);
    message->set_isconst(type->isConst());
    message->set_isvolatile(type->isVolatile());
    message->set_underlyingtype(index.getID(type->getUnderlyingType()));
  }

  void visitPrimitiveType(const PrimitiveType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_primitivekind((proto::PrimitiveKind)type->getPrimitiveKind()); // FIXME: this is a really ugly hack!
    message->set_kind(proto::TypeKind::PRIMITIVE);
  }

  void visitEnumType(const EnumType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::ENUM);
    for (auto& value: type->getValues()) { 
      auto valueMessage = message->add_value();
      valueMessage->set_name(value.name);
      valueMessage->set_value(value.value);
    }
  }


  void visitUnionType(const UnionType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::UNION);
    for (auto field: type->getFields()) {
      auto fieldMessage(message->add_field());
      fieldMessage->set_name(field.name);
      fieldMessage->set_type(index.getID(field.type));
    }
  }

  void visitStructType(const StructType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::STRUCT);
    for (auto field: type->getFields()) {
      auto fieldMessage(message->add_field());
      fieldMessage->set_name(field.name);
      fieldMessage->set_type(index.getID(field.type));
    }
  }

  void visitFunctionType(const FunctionType* type) {
    auto message(c.add_type());
    message->set_id(index.getID(type));
    message->set_kind(proto::TypeKind::FUNCTION);
    message->set_returntype(index.getID(type->getReturnType()));
    for (auto paramType: type->getParamTypes()) {
      message->add_paramtype(index.getID(paramType));
    }
  };

};

proto::Catalog* convert(Catalog& c) {
  autoffi::CountMap<const autoffi::Type*> typeIdx(c.getTypes());
  auto message = new proto::Catalog;
  autoffi::TypeToMessageConverter converter(message, typeIdx);
  for (auto type: c.getTypes())
    converter.visit(type);
  for (auto ex: c.getExports()) {
    auto exportmsg = message->add_export_();
    exportmsg->set_name(ex->getName());
    exportmsg->set_type(typeIdx.getID(ex->getType()));
  }
  return message;
};

// TODO: complete me
void convert(proto::Type& typemsg) {
  switch (typemsg.kind()) {
  case proto::TypeKind::QUALIFIED:
    break;
  case proto::TypeKind::PRIMITIVE:
    break;
  case proto::TypeKind::ENUM:
    break;
  case proto::TypeKind::FUNCTION:
    break;
  case proto::TypeKind::RECORD:
    break;
  case proto::TypeKind::ARRAY:
    break;
  case proto::TypeKind::POINTER:
    break;
  default:
    throw std::runtime_error("could not parse type");
  }  
}

void Format::read(Catalog& c, std::istream& in, ReadOptions& opts) {
  auto catalogmsg = new proto::Catalog;
  if (!catalogmsg->ParseFromIstream(&in))
    throw std::runtime_error("could parse catalog");
  //for (auto typemsg: catalogmsg->type()) {
    //c.addType(convert(typemsg));
  //}
  

};

void Format::write(Catalog& c, std::ostream& out, WriteOptions& opts) {

  auto catalogmsg = convert(c);

  switch (opts.format) {
  case WriteOptions::Format::JSON:  {
    google::protobuf::util::JsonPrintOptions printOptions;
    printOptions.add_whitespace = opts.whitespaces;
    std::string Out;
    google::protobuf::util::MessageToJsonString(*catalogmsg, &Out, printOptions);
    std::cout << Out;
    break;
  } case WriteOptions::Format::PROTOBUF:
    catalogmsg->SerializeToOstream(&out);
    break;
  case WriteOptions::Format::DEBUGSTR:
    std::cerr << catalogmsg->DebugString() << std::endl;
    break;
  }

}

}
