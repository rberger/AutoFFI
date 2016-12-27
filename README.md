AutoFFI
=======

AutoFFI is a way to call compiled code from higher-level languages, such as
scripting languages. This repository contains the compiler which compiles
C/C++/ObjectiveC down to a "language-agnostic" AST compatible with the C-family
of languages.

## Where is the source?

The source is currently held private, until a decent license and an appropriate
roadmap has been made.


## Installation

Grab the latest [release](https://github.com/AutoFFI/AutoFFI/releases) and add
the binaries somewhere to your `$PATH`, or use one of the platform-specific
installation instructions provided below.

### OS X

The easiest way to install the AutoFFI compiler is through [Homebrew Cask](https://caskroom.github.io/). **Note:** this is a [work in progress](https://github.com/caskroom/homebrew-cask/pull/26625)

```
$ brew cask install autoffi
```

## Usage

```
$ affc -json myprog.c > api.json
```

Generates a special AST from a set of sources. For more information, read the
[example](http://github.com/AutoFFI/AutoFFI/tree/master/example/).

## Limitations

### Garbage Collection

AutoFFI allows you to call low-level procedures _as is_, without any fancy
things such as garbage collection. In order to achieve that you will still have
to write your own code.

## Credits

 - Anthony Green for his marvellous [libffi](http://github.com/libffi/libffi)

