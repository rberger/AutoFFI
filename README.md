AutoFFI
=======

[![Travis CI](https://travis-ci.org/AutoFFI/AutoFFI.svg?branch=master)](https://travis-ci.org/AutoFFI/AutoFFI) [![Build status](https://ci.appveyor.com/api/projects/status/9qug08mj43f6ytm9?svg=true)](https://ci.appveyor.com/project/samvv/autoffi)

AutoFFI is a way to call compiled code from higher-level languages, such as
scripting languages. This repository contains the compiler which compiles
C/C++/ObjectiveC down to a "language-agnostic" AST compatible with the C-family
of languages.

## Help Wanted

Do you like compilers? Do you know LLVM? Would you like to be able to call
C/C++/ObjectiveC code directly without the hassle of binding? Then help us out!
Check out [the issues](https://github.com/AutoFFI/AutoFFI/issues) to get an
idea of what needs to be done and file a pull request whenever you like.

## Installation

Grab the latest [release](https://github.com/AutoFFI/AutoFFI/releases) and add
the binaries somewhere to your `$PATH`, or use one of the platform-specific
installation instructions provided below.

### OS X

The easiest way to install the AutoFFI compiler is through [Homebrew](https://brew.sh/).

**Note:** this method is not supported yet

```
$ brew install autoffi
```

## Usage

```
$ affc myprog.c --include myprog.c --exclude > api.json
```

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

