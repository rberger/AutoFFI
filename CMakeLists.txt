
cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(AutoFFI CXX C)

option(USE_LOCAL_PROTOBUF "Compile an embedded version of Google's protobuf library" OFF)
option(USE_LOCAL_BOOST "Compile an embedded version of the required boost libraries" OFF)
option(BUILD_TESTS "Enable building test targets and executables" ON)

#add_definitions(-stdlib=libc++)
#add_definitions(-flto)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

find_package(LLVM 3.9.1 REQUIRED CONFIG HINTS /usr/local/opt)
find_package(Clang REQUIRED)
if (USE_LOCAL_PROTOBUF)
  add_subdirectory(vendor/protobuf/cmake)
else()
  find_package(protobuf REQUIRED)
endif()
if (USE_LOCAL_BOOST)
  execute_process(COMMAND ./bootstrap.sh WORKING_DIR vendor/boost/tools/build)
  execute_process(COMMAND ./tools/build/b2 install --prefix=out/ --with-filesystem --with-program_options WORKING_DIR vendor/boost/)
  link_directories(vendor/boost/out/lib)
  include_directories(vendor/boost/out/include)
else()
  find_package(Boost REQUIRED COMPONENTS filesystem program_options)
endif()

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

add_library(autoffi STATIC
  src/TypeSorter.cpp
	src/Catalog.cpp
  src/SymbolTable.cpp
  src/AST.cpp
  src/ProtoFormat.cpp
  src/autoffi.pb.cc
)
add_library(autoffiClang STATIC src/clang.cpp src/exepath.cpp)
add_executable(affc src/generate.cpp)
if (USE_LOCAL_PROTOBUF)
  target_link_libraries(autoffi libprotobuf)
else()
  target_link_libraries(autoffi protobuf)
endif()
target_link_libraries(autoffiClang autoffi ${LLVM_LIBRARIES} ${CLANG_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(affc autoffiClang)

file(COPY vendor/libcxx/include DESTINATION "${CMAKE_BINARY_DIR}/libcxx")
file(COPY vendor/clang/include DESTINATION "${CMAKE_BINARY_DIR}/clang")

#file(GLOB TEST_FILES test/*.cpp)
#add_executable(transitTests ${TEST_FILES})
#target_link_libraries(transitTests transitClang gtest gtest_main)

install(TARGETS affc RUNTIME DESTINATION bin)

if (BUILD_TESTS)
  add_executable(ast_tests test/ast.cpp)
  add_subdirectory(vendor/googletest/googletest/)
  target_link_libraries(ast_tests gtest gtest_main autoffi)
  enable_testing()
  add_test(ast ast_tests)
  #add_test(example "cd ../../example && make")
  #add_test(chipmunk ./affc ../../test/assets/chipmunk/include/chipmunk/chipmunk.h)
endif()

