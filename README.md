AutoFFI
=======

[![Travis CI](https://travis-ci.org/AutoFFI/AutoFFI.svg?branch=master)](https://travis-ci.org/AutoFFI/AutoFFI) [![Build status](https://ci.appveyor.com/api/projects/status/9qug08mj43f6ytm9?svg=true)](https://ci.appveyor.com/project/samvv/autoffi)

AutoFFI is a way to call compiled code from higher-level languages, such as
scripting languages. This repository contains an analyser which generates a
file containing metadata. This metadata in turn is neccessary for automatically
connecting to a C library.

## Help Wanted

Do you like compilers? Do you know LLVM? Would you like to be able to call
C code directly without the hassle of binding? Then help us out!
Check out [the issues](https://github.com/AutoFFI/AutoFFI/issues) to get an
idea of what needs to be done and file a pull request whenever you like.

## Installation

### Linux

We provide binaries that are built by our build system whenever a new release
arrives. You can download them to the
[releases](https://github.com/AutoFFI/AutoFFI/releases) section. Simply symlink
the binaries somewhere into your `$PATH`.

**Warning:** the additional files must be placed near the executable so that
the analyser might be able to find them. Therefore, we recommend symlinking.

### OS X

The easiest way to install the AutoFFI compiler is through [Homebrew](https://brew.sh/).

**Note:** this method is not supported yet

```
$ brew install autoffi/autoffi/autoffi
```

## Usage

```
$ affc myprog.c --include myprog.c > api.json
```

 - **quiet**: do not emit Clang diagnostics while compiling
 - **include**: include exported symbols that reside within this file. Usually these are the header files of the project.
 - **include-hidden**: include hidden symbols in the generated output. If you
   plan to connect to the ABI, this is almost never what you want.
 - **include-protected**: include symbols that have been marked as protected.

Generates a special AST from a set of sources. For more information, read the
[example](http://github.com/AutoFFI/AutoFFI/tree/master/example/).

Options:

 - **include:** one or more paths of which the resulting AST will be generated
 - **exclude:** one or more path that will not be included in the generated AST

## Limitations

### Garbage Collection

AutoFFI allows you to call low-level procedures _as is_, without any fancy
things such as garbage collection. In order to achieve that you will still have
to write your own code.

## Credits

 - Anthony Green for his marvellous [libffi](http://github.com/libffi/libffi)

