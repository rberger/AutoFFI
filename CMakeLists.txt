
cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(AutoFFI CXX C)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_TESTS "Enable building test targets and executables" ON)

#add_definitions(-stdlib=libc++)
add_definitions(-flto)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(LLVM 3.9.1 REQUIRED CONFIG HINTS /usr/local/opt)
find_package(Clang REQUIRED)
find_package(protobuf REQUIRED)
#find_package(Protobuf REQUIRED HINT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/protobuf-3.1.0")

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CLANG_INCLUDE_DIRS})

function(initializer_list lst var)
  set(acc "{")
  set(first true)
  foreach(val IN LISTS ${lst})
    if (first)
      set(first false)
    else()
      set(acc "${acc},")
    endif()
    set(acc "${acc} \"${val}\"")
  endforeach()
  set(${var} "${acc} }" PARENT_SCOPE)
endfunction()

foreach(include_dir IN LISTS STDLIB_INCLUDE_DIRS)
  list(APPEND arg_list "-isystem${include_dir}")
endforeach()
foreach(frameworks_dir IN LISTS STDLIB_FRAMEWORKS_DIRS)
  list(APPEND arg_list "-F${frameworks_dir}")
endforeach()

initializer_list(arg_list tool_default_args)

#add_definitions("-DDEFAULT_ARGS=\"${tool-default_args}\"")
configure_file(src/config.h.in "${CMAKE_SOURCE_DIR}/src/config.h")

include_directories(include/)

llvm_map_components_to_libnames(LLVM_LIBRARIES all)
add_definitions(${LLVM_DEFINITIONS})

add_library(autoffi 
	src/Catalog.cpp
  src/SymbolTable.cpp
  src/AST.cpp
  src/ProtoFormat.cpp
  src/TypeSorter.cpp
  src/autoffi.pb.cc
)
add_library(autoffiClang src/clang.cpp)
add_executable(affc src/generate.cpp)
target_link_libraries(autoffi protobuf)
target_link_libraries(autoffiClang autoffi ${LLVM_LIBRARIES} ${CLANG_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(affc autoffiClang)

file(COPY vendor/libcxx/include DESTINATION "${CMAKE_BINARY_DIR}/libcxx")
file(COPY vendor/clang/include DESTINATION "${CMAKE_BINARY_DIR}/clang")
file(COPY vendor/glibc/include DESTINATION "${CMAKE_BINARY_DIR}/glibc")

#file(GLOB TEST_FILES test/*.cpp)
#add_executable(transitTests ${TEST_FILES})
#target_link_libraries(transitTests transitClang gtest gtest_main)

install(TARGETS affc RUNTIME DESTINATION bin)

if (BUILD_TESTS)
  enable_testing()
  add_test(chipmunk ./affc ../../test/assets/chipmunk/include/chipmunk/chipmunk.h)
endif()

