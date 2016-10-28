# - Try to find the standard libaries
# Once done this will define
#  STDLIB_FOUND - System has standard libraries
#  STDLIB_INCLUDE_DIRS - The standard libraries include directories
#  STDLIB_LIBRARIES - The libraries needed to use standard libraries
#  STDLIB_DEFINITIONS - Compiler switches required for using standard libraries

# TODO: for completeness sake, find the static and shared libraries

set(darwin_sierra_include_dirs 
  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include"
  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/4.2.1"
  "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include"
)

if(APPLE)
  EXEC_PROGRAM(uname ARGS -r OUTPUT_VARIABLE DARWIN_VERSION)
  if (DARWIN_VERSION EQUAL "16.0.0")
    set(STDLIB_INCLUDE_DIRS ${darwin_sierra_include_dirs})
  endif()
  # TODO: add header locations for older Darwin OSes;
elseif(UNIX)
  # TODO: add header locations for UNIX systems
elseif(WIN32)
  # TODO: add header locations for Windows systems
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set STDLIB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(stdlib DEFAULT_MSG
  STDLIB_INCLUDE_DIRS
  #STDLIB_LIBRARIES
  )

#mark_as_advanced(STDLIB_INCLUDE_DIRS STDLIB_LIBRARIES)

