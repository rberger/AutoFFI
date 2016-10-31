Transit
=======

> A language-agnostic AST for inter-program communication.

Tansit is a protocol specification and accompanying tools for extracting 
information from source code which is essential for communication between
different programs, most importantly between C/C++ programs and scripting
languages.

## Software using Transit

Transit doesn't do anything interesting out-of-the-box except for extracting
essential information from your source code. In order to harness its full
power, you will need to use one of the libraries which consume the Transit AST.

 - [bstruct](http://github.com/samvv/binstruct): tools for data serialization and deserialization
 - [AutoFFI](http://github.com/samvv/AutoFFI): tools for automagically connecting to a foreign ABI

<!--Transit is a library which allows you to query a program's structure within your-->
<!--own software, without relying on external compilers or parsers. Transit does the-->
<!--heavy lifting of selecting the right compiler for the job, and returns things-->
<!--such as a list of symbols and type information of a set of sources to your-->
<!--program.-->

