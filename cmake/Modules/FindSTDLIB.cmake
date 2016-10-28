# - Try to find the standard libaries
# Once done this will define
#  STDLIB_FOUND - System has standard libraries
#  STDLIB_INCLUDE_DIRS - The standard libraries include directories
#  STDLIB_FRAMEWORKS_DIRS - The standard libraries framework directories
#  STDLIB_LIBRARIES - The libraries needed to use standard libraries
#  STDLIB_DEFINITIONS - Compiler switches required for using standard libraries

# TODO: for completeness sake, find the static and shared libraries

set(darwin_sierra_include_dirs 
  "/usr/local/include"
  "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../include/c++/v1"

  # We are using a more modern version of the compiler, need to use the corresponding libs
  #"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/8.0.0/include"
  "/usr/local/lib/clang/4.0.0/include"

  "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include"
  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include"
)

set(darwin_sierra_frameworks_dirs
  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks"
)

if(APPLE)
  EXEC_PROGRAM(uname ARGS -r OUTPUT_VARIABLE DARWIN_VERSION)
  if (DARWIN_VERSION EQUAL "16.0.0")
    set(STDLIB_INCLUDE_DIRS ${darwin_sierra_include_dirs})
    set(STDLIB_FRAMEWORKS_DIRS ${darwin_sierra_frameworks_dirs})
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

