#!/bin/sh
mkdir -p protobuf/ && wget "https://github.com/google/protobuf/releases/download/v3.2.0/protobuf-cpp-3.2.0.tar.gz" -O - | tar -zx --directory protobuf/ --strip-components 1 && cd protobuf/ && mkdir -p out/ && cd out/ && cmake ../cmake -DCMAKE_BUILD_TYPE=Release && sudo make install
