
cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(AutoFFI CXX C)

option(USE_LOCAL_PROTOBUF "Compile an embedded version of Google's protobuf library" OFF)
option(USE_LOCAL_BOOST "Compile an embedded version of the required boost libraries" OFF)
option(BUILD_TESTS "Enable building test targets and executables" ON)

#add_definitions(-stdlib=libc++)
#add_definitions(-flto)

set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

find_package(LLVM 3.9.1 REQUIRED CONFIG HINTS /usr/local/opt)
find_package(Clang REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

include_directories(include/)

llvm_map_components_to_libnames(LLVM_LIBRARIES all)
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--start-group")
endif()

add_executable(affc src/generate.cpp)

#set_target_properties(affc PROPERTIES LINK_SEARCH_START_STATIC 1)
#set_target_properties(affc PROPERTIES LINK_SEARCH_END_STATIC 1)
#set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_library(autoffi STATIC
  src/TypeSorter.cpp
  src/Catalog.cpp
  src/SymbolTable.cpp
  src/AST.cpp
  src/ProtoFormat.cpp
  src/autoffi.pb.cc
)
add_library(autoffiClang STATIC src/clang.cpp src/exepath.cpp)
target_link_libraries(autoffi ${PROTOBUF_LIBRARIES})
target_link_libraries(autoffiClang autoffi ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
target_link_libraries(affc autoffiClang ${Boost_LIBRARIES} ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
 
file(COPY vendor/libcxx/include DESTINATION "${CMAKE_BINARY_DIR}/libcxx")
file(COPY vendor/clang/include DESTINATION "${CMAKE_BINARY_DIR}/clang")
add_custom_target(zip zip -r autoffi.zip affc libcxx clang DEPENDS affc)

install(TARGETS affc RUNTIME DESTINATION bin)

if (BUILD_TESTS)
  add_executable(ast_tests test/ast.cpp)
  add_subdirectory(vendor/googletest/googletest/)
  target_link_libraries(ast_tests gtest gtest_main autoffi)
  enable_testing()
  add_test(ast ast_tests)
  #add_test(example make WORKING_DIRECTORY example/)
  #add_test(chipmunk ./affc ../../test/assets/chipmunk/include/chipmunk/chipmunk.h)
endif()

