
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(Transit CXX)

set(CMAKE_CXX_STANDARD 11)

#add_definitions(-stdlib=libc++)
add_definitions(-flto)

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

#add_definitions("-DDEFAULT_ARGS=\"${tool-default_args}\"")
configure_file(src/config.h.in "${CMAKE_SOURCE_DIR}/src/config.h")

include_directories(include/)

llvm_map_components_to_libnames(LLVM_LIBRARIES all)
add_definitions(${LLVM_DEFINITIONS})

add_library(transitClang src/clang.cpp)
add_executable(transc src/generate.cpp)
target_link_libraries(transitClang transit ${LLVM_LIBRARIES} ${CLANG_LIBRARIES})
target_link_libraries(affc transitClang)

#file(GLOB TEST_FILES test/*.cpp)
#add_executable(transitTests ${TEST_FILES})
#target_link_libraries(transitTests transitClang gtest gtest_main)

install(TARGETS transc RUNTIME DESTINATION bin)

