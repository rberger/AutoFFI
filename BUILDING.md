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

### UNIX-like systems

1. Checkout and build LLVM/Clang
2. Checkout the source and generate build files

  ```bash
  $ git clone https://github.com/samvv/binstruct
  $ mkdir -p build/Release && cd build/Release
  $ cmake ... -DCMAKE_INSTALL_PREFIX=/usr/local
  ```
3. Compile and install

  ```bash
  $ make install
  ```

## Platform-specific notes 

### Linux

Nightly packages of LLVM and Clang can be installed via the [official repository](http://apt.llvm.org/).

## OS X

OS X is shipped with LLVM and Clang, but is uses a custom version of the
software, which as far as we know isn't suitable for linking. Therefore, you
will need to acquire "clean" version of LLVM. The easiest way to do so is via
Homebrew, with `brew install llvm --with-clang --with-rtti`. However, chances
are that the installed version of LLVM is too outdated. The next steps assume
that you need to build LLVM from source because of this reason.

