
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(Truss CXX)

set(CMAKE_CXX_STANDARD 11)

#add_definitions(-stdlib=libc++)

find_package(STDLIB)
find_package(LLVM 4.0 REQUIRED CONFIG)
find_package(CLANG REQUIRED)

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

include_directories(include/)

#add_definitions("-DDEFAULT_ARGS=\"${tool-default_args}\"")
configure_file(src/config.h.in "${CMAKE_SOURCE_DIR}/src/config.h")

llvm_map_components_to_libnames(LLVM_LIBRARIES all)
add_definitions(${LLVM_DEFINITIONS})

add_library(trussAST src/ast.cpp)
add_library(trussProto src/proto.cpp src/ast.pb.cc)
add_library(trussClang src/clang.cpp)
add_library(truss src/null.cpp)
add_executable(tls src/list.cpp)
add_executable(tgen src/json.cpp)
target_link_libraries(trussClang ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
target_link_libraries(truss trussAST trussClang trussProto)
target_link_libraries(tls truss)
target_link_libraries(tgen truss)

install(TARGETS tls tgen RUNTIME DESTINATION bin)

