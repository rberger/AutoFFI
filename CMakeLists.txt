
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(bstruct CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(STDLIB)
find_package(LLVM 4.0 REQUIRED CONFIG)
find_package(CLANG REQUIRED)

function(initializer_list lst var)
  set(acc "{")
  set(first true)
  foreach(include_dir IN LISTS ${lst})
    if (first)
      set(first false)
    else()
      set(acc "${acc},")
    endif()
    set(acc "${acc} \"-isystem${include_dir}\"")
  endforeach()
  set(${var} "${acc} }" PARENT_SCOPE)
endfunction()

initializer_list(STDLIB_INCLUDE_DIRS tool_default_args)

#add_definitions("-DDEFAULT_ARGS=\"${tool-default_args}\"")
configure_file(src/config.h.in src/config.h)
include_directories("${CMAKE_BINARY_DIR}/src")

llvm_map_components_to_libnames(LLVM_LIBRARIES all)
add_definitions(${LLVM_DEFINITIONS})

add_executable(bsls src/list.cpp)
target_link_libraries(bsls ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
add_executable(bsgen src/generate.cpp)

