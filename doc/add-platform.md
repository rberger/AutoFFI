Targeting a New Platform
========================

Currently, only OS X Sierra (10.6) is supported. However, it should be possible to target any platform.

Run the following code in your terminal to determine which compiler flags are set:

```bash
$ echo "//" | gcc -xc++ -E -v -
``` 

The resulting include directories should be added to
`./cmake/Modules/FindSTDLIB.cmake`. If you are unfamiliar with CMake syntax or
unsure how to proceed, feel free to open an
issue on GitHub.

