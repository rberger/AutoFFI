Building From Source
====================

Building from source is relatively complicated, which is why we advise you to
use one of the binaries. Not all platforms have precompiled binaries, though,
so it might be necessary to build your own.

## General instructions

The following instructions are applicable to multiple platforms. Read on if you
encounter any issues in the process.

We use Clang SVN version 4.0 to develop the application. The best way to install
Clang is through CMake, which is described [here](http://llvm.org/docs/CMake.html).

### Required dependencies

 - [Boost](http://www.boost.org/)
 - [LLVM 4.0](http://llvm.org/)
 - [Clang 4.0](https://clang.llvm.org/)

### UNIX-like systems

1. Checkout and build LLVM/Clang
2. Checkout the source and generate build files

  ```bash
  $ git clone https://github.com/AutoFFI/AutoFFI
  $ mkdir -p build/Release && cd build/Release
  $ cmake ... -DCMAKE_INSTALL_PREFIX=/usr/local
  ```
3. Compile and install

  ```bash
  $ make install
  ```

## Platform-specific notes 

### Linux

Nightly packages of LLVM and Clang can be installed via the 
[official repository](http://apt.llvm.org/). As has already been noted, we use
the 4.0 branch.

### OS X

OS X is shipped with LLVM and Clang, but it uses a custom version of the
software, which as far as we know isn't suitable for linking. Therefore, you
will need to acquire "clean" version of LLVM. In theory, you can install LLVM with
Homebrew, with `brew install llvm --HEAD --with-clang --with-rtti`. This hasn't
been tested, however, so if it doesn't work you will have to manually install
using SVN and CMake.

