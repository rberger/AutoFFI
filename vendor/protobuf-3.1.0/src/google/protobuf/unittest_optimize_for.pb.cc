// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_optimize_for.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include <google/protobuf/unittest_optimize_for.pb.h>

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protobuf_unittest {

namespace {

const ::google::protobuf::Descriptor* TestOptimizedForSize_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TestOptimizedForSize_reflection_ = NULL;
struct TestOptimizedForSizeOneofInstance {
  ::google::protobuf::int32 integer_field_;
  ::google::protobuf::internal::ArenaStringPtr string_field_;
}* TestOptimizedForSize_default_oneof_instance_ = NULL;
const ::google::protobuf::Descriptor* TestRequiredOptimizedForSize_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TestRequiredOptimizedForSize_reflection_ = NULL;
const ::google::protobuf::Descriptor* TestOptionalOptimizedForSize_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TestOptionalOptimizedForSize_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() {
  protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "google/protobuf/unittest_optimize_for.proto");
  GOOGLE_CHECK(file != NULL);
  TestOptimizedForSize_descriptor_ = file->message_type(0);
  static const int TestOptimizedForSize_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, i_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, msg_),
    PROTO2_GENERATED_DEFAULT_ONEOF_FIELD_OFFSET(TestOptimizedForSize_default_oneof_instance_, integer_field_),
    PROTO2_GENERATED_DEFAULT_ONEOF_FIELD_OFFSET(TestOptimizedForSize_default_oneof_instance_, string_field_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, foo_),
  };
  TestOptimizedForSize_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      TestOptimizedForSize_descriptor_,
      TestOptimizedForSize::internal_default_instance(),
      TestOptimizedForSize_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, _has_bits_),
      -1,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, _extensions_),
      TestOptimizedForSize_default_oneof_instance_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, _oneof_case_[0]),
      sizeof(TestOptimizedForSize),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptimizedForSize, _internal_metadata_));
  TestRequiredOptimizedForSize_descriptor_ = file->message_type(1);
  static const int TestRequiredOptimizedForSize_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestRequiredOptimizedForSize, x_),
  };
  TestRequiredOptimizedForSize_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      TestRequiredOptimizedForSize_descriptor_,
      TestRequiredOptimizedForSize::internal_default_instance(),
      TestRequiredOptimizedForSize_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestRequiredOptimizedForSize, _has_bits_),
      -1,
      -1,
      sizeof(TestRequiredOptimizedForSize),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestRequiredOptimizedForSize, _internal_metadata_));
  TestOptionalOptimizedForSize_descriptor_ = file->message_type(2);
  static const int TestOptionalOptimizedForSize_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptionalOptimizedForSize, o_),
  };
  TestOptionalOptimizedForSize_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      TestOptionalOptimizedForSize_descriptor_,
      TestOptionalOptimizedForSize::internal_default_instance(),
      TestOptionalOptimizedForSize_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptionalOptimizedForSize, _has_bits_),
      -1,
      -1,
      sizeof(TestOptionalOptimizedForSize),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestOptionalOptimizedForSize, _internal_metadata_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      TestOptimizedForSize_descriptor_, TestOptimizedForSize::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      TestRequiredOptimizedForSize_descriptor_, TestRequiredOptimizedForSize::internal_default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      TestOptionalOptimizedForSize_descriptor_, TestOptionalOptimizedForSize::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() {
  TestOptimizedForSize_default_instance_.Shutdown();
  delete TestOptimizedForSize_default_oneof_instance_;
  delete TestOptimizedForSize_reflection_;
  TestRequiredOptimizedForSize_default_instance_.Shutdown();
  delete TestRequiredOptimizedForSize_reflection_;
  TestOptionalOptimizedForSize_default_instance_.Shutdown();
  delete TestOptionalOptimizedForSize_reflection_;
}

void protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_unittest::protobuf_InitDefaults_google_2fprotobuf_2funittest_2eproto();
  ::google::protobuf::internal::GetEmptyString();
  TestOptimizedForSize_default_instance_.DefaultConstruct();
  TestOptimizedForSize_default_oneof_instance_ = new TestOptimizedForSizeOneofInstance();
  TestRequiredOptimizedForSize_default_instance_.DefaultConstruct();
  TestOptionalOptimizedForSize_default_instance_.DefaultConstruct();
  TestOptimizedForSize_default_instance_.get_mutable()->InitAsDefaultInstance();
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    ::protobuf_unittest::TestOptimizedForSize::internal_default_instance(),
    1234, 5, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterMessageExtension(
    ::protobuf_unittest::TestOptimizedForSize::internal_default_instance(),
    1235, 11, false, false,
    ::protobuf_unittest::TestRequiredOptimizedForSize::internal_default_instance());
  TestRequiredOptimizedForSize_default_instance_.get_mutable()->InitAsDefaultInstance();
  TestOptionalOptimizedForSize_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_once_);
void protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_once_,
                 &protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_impl);
}
void protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n+google/protobuf/unittest_optimize_for."
    "proto\022\021protobuf_unittest\032\036google/protobu"
    "f/unittest.proto\"\312\002\n\024TestOptimizedForSiz"
    "e\022\t\n\001i\030\001 \001(\005\022.\n\003msg\030\023 \001(\0132!.protobuf_uni"
    "ttest.ForeignMessage\022\027\n\rinteger_field\030\002 "
    "\001(\005H\000\022\026\n\014string_field\030\003 \001(\tH\000*\t\010\350\007\020\200\200\200\200\002"
    "2@\n\016test_extension\022\'.protobuf_unittest.T"
    "estOptimizedForSize\030\322\t \001(\0052r\n\017test_exten"
    "sion2\022\'.protobuf_unittest.TestOptimizedF"
    "orSize\030\323\t \001(\0132/.protobuf_unittest.TestRe"
    "quiredOptimizedForSizeB\005\n\003foo\")\n\034TestReq"
    "uiredOptimizedForSize\022\t\n\001x\030\001 \002(\005\"Z\n\034Test"
    "OptionalOptimizedForSize\022:\n\001o\030\001 \001(\0132/.pr"
    "otobuf_unittest.TestRequiredOptimizedFor"
    "SizeB\002H\002", 568);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "google/protobuf/unittest_optimize_for.proto", &protobuf_RegisterTypes);
  ::protobuf_unittest::protobuf_AddDesc_google_2fprotobuf_2funittest_2eproto();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_once_);
void protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_once_,
                 &protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto {
  StaticDescriptorInitializer_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto() {
    protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  }
} static_descriptor_initializer_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto_;

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TestOptimizedForSize::kIFieldNumber;
const int TestOptimizedForSize::kMsgFieldNumber;
const int TestOptimizedForSize::kIntegerFieldFieldNumber;
const int TestOptimizedForSize::kStringFieldFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TestOptimizedForSize::kTestExtensionFieldNumber;
#endif
::google::protobuf::internal::ExtensionIdentifier< ::protobuf_unittest::TestOptimizedForSize,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int32 >, 5, false >
  TestOptimizedForSize::test_extension(kTestExtensionFieldNumber, 0);
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TestOptimizedForSize::kTestExtension2FieldNumber;
#endif
::google::protobuf::internal::ExtensionIdentifier< ::protobuf_unittest::TestOptimizedForSize,
    ::google::protobuf::internal::MessageTypeTraits< ::protobuf_unittest::TestRequiredOptimizedForSize >, 11, false >
  TestOptimizedForSize::test_extension2(kTestExtension2FieldNumber, *::protobuf_unittest::TestRequiredOptimizedForSize::internal_default_instance());
TestOptimizedForSize::TestOptimizedForSize()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:protobuf_unittest.TestOptimizedForSize)
}

void TestOptimizedForSize::InitAsDefaultInstance() {
  msg_ = const_cast< ::protobuf_unittest::ForeignMessage*>(
      ::protobuf_unittest::ForeignMessage::internal_default_instance());
  TestOptimizedForSize_default_oneof_instance_->integer_field_ = 0;
  TestOptimizedForSize_default_oneof_instance_->string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

TestOptimizedForSize::TestOptimizedForSize(const TestOptimizedForSize& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:protobuf_unittest.TestOptimizedForSize)
}

void TestOptimizedForSize::SharedCtor() {
  _cached_size_ = 0;
  msg_ = NULL;
  i_ = 0;
  clear_has_foo();
}

TestOptimizedForSize::~TestOptimizedForSize() {
  // @@protoc_insertion_point(destructor:protobuf_unittest.TestOptimizedForSize)
  SharedDtor();
}

void TestOptimizedForSize::SharedDtor() {
  if (has_foo()) {
    clear_foo();
  }
  if (this != &TestOptimizedForSize_default_instance_.get()) {
    delete msg_;
  }
}

void TestOptimizedForSize::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TestOptimizedForSize::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TestOptimizedForSize_descriptor_;
}

const TestOptimizedForSize& TestOptimizedForSize::default_instance() {
  protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<TestOptimizedForSize> TestOptimizedForSize_default_instance_;

TestOptimizedForSize* TestOptimizedForSize::New(::google::protobuf::Arena* arena) const {
  TestOptimizedForSize* n = new TestOptimizedForSize;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void TestOptimizedForSize::clear_foo() {
// @@protoc_insertion_point(one_of_clear_start:protobuf_unittest.TestOptimizedForSize)
  switch (foo_case()) {
    case kIntegerField: {
      // No need to clear
      break;
    }
    case kStringField: {
      foo_.string_field_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
      break;
    }
    case FOO_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = FOO_NOT_SET;
}


void TestOptimizedForSize::UnsafeMergeFrom(const TestOptimizedForSize& from) {
  MergeFrom(from);
}
void TestOptimizedForSize::Swap(TestOptimizedForSize* other) {
  if (other == this) return;
  InternalSwap(other);
}
void TestOptimizedForSize::InternalSwap(TestOptimizedForSize* other) {
  GetReflection()->Swap(this, other);}

::google::protobuf::Metadata TestOptimizedForSize::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TestOptimizedForSize_descriptor_;
  metadata.reflection = TestOptimizedForSize_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// TestOptimizedForSize

// optional int32 i = 1;
bool TestOptimizedForSize::has_i() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void TestOptimizedForSize::set_has_i() {
  _has_bits_[0] |= 0x00000001u;
}
void TestOptimizedForSize::clear_has_i() {
  _has_bits_[0] &= ~0x00000001u;
}
void TestOptimizedForSize::clear_i() {
  i_ = 0;
  clear_has_i();
}
::google::protobuf::int32 TestOptimizedForSize::i() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.i)
  return i_;
}
void TestOptimizedForSize::set_i(::google::protobuf::int32 value) {
  set_has_i();
  i_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.i)
}

// optional .protobuf_unittest.ForeignMessage msg = 19;
bool TestOptimizedForSize::has_msg() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void TestOptimizedForSize::set_has_msg() {
  _has_bits_[0] |= 0x00000002u;
}
void TestOptimizedForSize::clear_has_msg() {
  _has_bits_[0] &= ~0x00000002u;
}
void TestOptimizedForSize::clear_msg() {
  if (msg_ != NULL) msg_->::protobuf_unittest::ForeignMessage::Clear();
  clear_has_msg();
}
const ::protobuf_unittest::ForeignMessage& TestOptimizedForSize::msg() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.msg)
  return msg_ != NULL ? *msg_
                         : *::protobuf_unittest::ForeignMessage::internal_default_instance();
}
::protobuf_unittest::ForeignMessage* TestOptimizedForSize::mutable_msg() {
  set_has_msg();
  if (msg_ == NULL) {
    msg_ = new ::protobuf_unittest::ForeignMessage;
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptimizedForSize.msg)
  return msg_;
}
::protobuf_unittest::ForeignMessage* TestOptimizedForSize::release_msg() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptimizedForSize.msg)
  clear_has_msg();
  ::protobuf_unittest::ForeignMessage* temp = msg_;
  msg_ = NULL;
  return temp;
}
void TestOptimizedForSize::set_allocated_msg(::protobuf_unittest::ForeignMessage* msg) {
  delete msg_;
  if (msg != NULL && msg->GetArena() != NULL) {
    ::protobuf_unittest::ForeignMessage* new_msg = new ::protobuf_unittest::ForeignMessage;
    new_msg->CopyFrom(*msg);
    msg = new_msg;
  }
  msg_ = msg;
  if (msg) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptimizedForSize.msg)
}

// optional int32 integer_field = 2;
bool TestOptimizedForSize::has_integer_field() const {
  return foo_case() == kIntegerField;
}
void TestOptimizedForSize::set_has_integer_field() {
  _oneof_case_[0] = kIntegerField;
}
void TestOptimizedForSize::clear_integer_field() {
  if (has_integer_field()) {
    foo_.integer_field_ = 0;
    clear_has_foo();
  }
}
::google::protobuf::int32 TestOptimizedForSize::integer_field() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.integer_field)
  if (has_integer_field()) {
    return foo_.integer_field_;
  }
  return 0;
}
void TestOptimizedForSize::set_integer_field(::google::protobuf::int32 value) {
  if (!has_integer_field()) {
    clear_foo();
    set_has_integer_field();
  }
  foo_.integer_field_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.integer_field)
}

// optional string string_field = 3;
bool TestOptimizedForSize::has_string_field() const {
  return foo_case() == kStringField;
}
void TestOptimizedForSize::set_has_string_field() {
  _oneof_case_[0] = kStringField;
}
void TestOptimizedForSize::clear_string_field() {
  if (has_string_field()) {
    foo_.string_field_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    clear_has_foo();
  }
}
const ::std::string& TestOptimizedForSize::string_field() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptimizedForSize.string_field)
  if (has_string_field()) {
    return foo_.string_field_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  return *&::google::protobuf::internal::GetEmptyStringAlreadyInited();
}
void TestOptimizedForSize::set_string_field(const ::std::string& value) {
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.string_field)
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestOptimizedForSize.string_field)
}
void TestOptimizedForSize::set_string_field(const char* value) {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(value));
  // @@protoc_insertion_point(field_set_char:protobuf_unittest.TestOptimizedForSize.string_field)
}
void TestOptimizedForSize::set_string_field(const char* value, size_t size) {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  foo_.string_field_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:protobuf_unittest.TestOptimizedForSize.string_field)
}
::std::string* TestOptimizedForSize::mutable_string_field() {
  if (!has_string_field()) {
    clear_foo();
    set_has_string_field();
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptimizedForSize.string_field)
  return foo_.string_field_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* TestOptimizedForSize::release_string_field() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptimizedForSize.string_field)
  if (has_string_field()) {
    clear_has_foo();
    return foo_.string_field_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  } else {
    return NULL;
  }
}
void TestOptimizedForSize::set_allocated_string_field(::std::string* string_field) {
  if (!has_string_field()) {
    foo_.string_field_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  clear_foo();
  if (string_field != NULL) {
    set_has_string_field();
    foo_.string_field_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        string_field);
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptimizedForSize.string_field)
}

bool TestOptimizedForSize::has_foo() const {
  return foo_case() != FOO_NOT_SET;
}
void TestOptimizedForSize::clear_has_foo() {
  _oneof_case_[0] = FOO_NOT_SET;
}
TestOptimizedForSize::FooCase TestOptimizedForSize::foo_case() const {
  return TestOptimizedForSize::FooCase(_oneof_case_[0]);
}
inline const TestOptimizedForSize* TestOptimizedForSize::internal_default_instance() {
  return &TestOptimizedForSize_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TestRequiredOptimizedForSize::kXFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

TestRequiredOptimizedForSize::TestRequiredOptimizedForSize()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:protobuf_unittest.TestRequiredOptimizedForSize)
}

void TestRequiredOptimizedForSize::InitAsDefaultInstance() {
}

TestRequiredOptimizedForSize::TestRequiredOptimizedForSize(const TestRequiredOptimizedForSize& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:protobuf_unittest.TestRequiredOptimizedForSize)
}

void TestRequiredOptimizedForSize::SharedCtor() {
  _cached_size_ = 0;
  x_ = 0;
}

TestRequiredOptimizedForSize::~TestRequiredOptimizedForSize() {
  // @@protoc_insertion_point(destructor:protobuf_unittest.TestRequiredOptimizedForSize)
  SharedDtor();
}

void TestRequiredOptimizedForSize::SharedDtor() {
}

void TestRequiredOptimizedForSize::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TestRequiredOptimizedForSize::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TestRequiredOptimizedForSize_descriptor_;
}

const TestRequiredOptimizedForSize& TestRequiredOptimizedForSize::default_instance() {
  protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<TestRequiredOptimizedForSize> TestRequiredOptimizedForSize_default_instance_;

TestRequiredOptimizedForSize* TestRequiredOptimizedForSize::New(::google::protobuf::Arena* arena) const {
  TestRequiredOptimizedForSize* n = new TestRequiredOptimizedForSize;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void TestRequiredOptimizedForSize::UnsafeMergeFrom(const TestRequiredOptimizedForSize& from) {
  MergeFrom(from);
}
void TestRequiredOptimizedForSize::Swap(TestRequiredOptimizedForSize* other) {
  if (other == this) return;
  InternalSwap(other);
}
void TestRequiredOptimizedForSize::InternalSwap(TestRequiredOptimizedForSize* other) {
  GetReflection()->Swap(this, other);}

::google::protobuf::Metadata TestRequiredOptimizedForSize::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TestRequiredOptimizedForSize_descriptor_;
  metadata.reflection = TestRequiredOptimizedForSize_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// TestRequiredOptimizedForSize

// required int32 x = 1;
bool TestRequiredOptimizedForSize::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void TestRequiredOptimizedForSize::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
void TestRequiredOptimizedForSize::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
void TestRequiredOptimizedForSize::clear_x() {
  x_ = 0;
  clear_has_x();
}
::google::protobuf::int32 TestRequiredOptimizedForSize::x() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestRequiredOptimizedForSize.x)
  return x_;
}
void TestRequiredOptimizedForSize::set_x(::google::protobuf::int32 value) {
  set_has_x();
  x_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest.TestRequiredOptimizedForSize.x)
}

inline const TestRequiredOptimizedForSize* TestRequiredOptimizedForSize::internal_default_instance() {
  return &TestRequiredOptimizedForSize_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TestOptionalOptimizedForSize::kOFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

TestOptionalOptimizedForSize::TestOptionalOptimizedForSize()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:protobuf_unittest.TestOptionalOptimizedForSize)
}

void TestOptionalOptimizedForSize::InitAsDefaultInstance() {
  o_ = const_cast< ::protobuf_unittest::TestRequiredOptimizedForSize*>(
      ::protobuf_unittest::TestRequiredOptimizedForSize::internal_default_instance());
}

TestOptionalOptimizedForSize::TestOptionalOptimizedForSize(const TestOptionalOptimizedForSize& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:protobuf_unittest.TestOptionalOptimizedForSize)
}

void TestOptionalOptimizedForSize::SharedCtor() {
  _cached_size_ = 0;
  o_ = NULL;
}

TestOptionalOptimizedForSize::~TestOptionalOptimizedForSize() {
  // @@protoc_insertion_point(destructor:protobuf_unittest.TestOptionalOptimizedForSize)
  SharedDtor();
}

void TestOptionalOptimizedForSize::SharedDtor() {
  if (this != &TestOptionalOptimizedForSize_default_instance_.get()) {
    delete o_;
  }
}

void TestOptionalOptimizedForSize::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TestOptionalOptimizedForSize::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TestOptionalOptimizedForSize_descriptor_;
}

const TestOptionalOptimizedForSize& TestOptionalOptimizedForSize::default_instance() {
  protobuf_InitDefaults_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<TestOptionalOptimizedForSize> TestOptionalOptimizedForSize_default_instance_;

TestOptionalOptimizedForSize* TestOptionalOptimizedForSize::New(::google::protobuf::Arena* arena) const {
  TestOptionalOptimizedForSize* n = new TestOptionalOptimizedForSize;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void TestOptionalOptimizedForSize::UnsafeMergeFrom(const TestOptionalOptimizedForSize& from) {
  MergeFrom(from);
}
void TestOptionalOptimizedForSize::Swap(TestOptionalOptimizedForSize* other) {
  if (other == this) return;
  InternalSwap(other);
}
void TestOptionalOptimizedForSize::InternalSwap(TestOptionalOptimizedForSize* other) {
  GetReflection()->Swap(this, other);}

::google::protobuf::Metadata TestOptionalOptimizedForSize::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TestOptionalOptimizedForSize_descriptor_;
  metadata.reflection = TestOptionalOptimizedForSize_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// TestOptionalOptimizedForSize

// optional .protobuf_unittest.TestRequiredOptimizedForSize o = 1;
bool TestOptionalOptimizedForSize::has_o() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void TestOptionalOptimizedForSize::set_has_o() {
  _has_bits_[0] |= 0x00000001u;
}
void TestOptionalOptimizedForSize::clear_has_o() {
  _has_bits_[0] &= ~0x00000001u;
}
void TestOptionalOptimizedForSize::clear_o() {
  if (o_ != NULL) o_->::protobuf_unittest::TestRequiredOptimizedForSize::Clear();
  clear_has_o();
}
const ::protobuf_unittest::TestRequiredOptimizedForSize& TestOptionalOptimizedForSize::o() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest.TestOptionalOptimizedForSize.o)
  return o_ != NULL ? *o_
                         : *::protobuf_unittest::TestRequiredOptimizedForSize::internal_default_instance();
}
::protobuf_unittest::TestRequiredOptimizedForSize* TestOptionalOptimizedForSize::mutable_o() {
  set_has_o();
  if (o_ == NULL) {
    o_ = new ::protobuf_unittest::TestRequiredOptimizedForSize;
  }
  // @@protoc_insertion_point(field_mutable:protobuf_unittest.TestOptionalOptimizedForSize.o)
  return o_;
}
::protobuf_unittest::TestRequiredOptimizedForSize* TestOptionalOptimizedForSize::release_o() {
  // @@protoc_insertion_point(field_release:protobuf_unittest.TestOptionalOptimizedForSize.o)
  clear_has_o();
  ::protobuf_unittest::TestRequiredOptimizedForSize* temp = o_;
  o_ = NULL;
  return temp;
}
void TestOptionalOptimizedForSize::set_allocated_o(::protobuf_unittest::TestRequiredOptimizedForSize* o) {
  delete o_;
  o_ = o;
  if (o) {
    set_has_o();
  } else {
    clear_has_o();
  }
  // @@protoc_insertion_point(field_set_allocated:protobuf_unittest.TestOptionalOptimizedForSize.o)
}

inline const TestOptionalOptimizedForSize* TestOptionalOptimizedForSize::internal_default_instance() {
  return &TestOptionalOptimizedForSize_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf_unittest

// @@protoc_insertion_point(global_scope)
