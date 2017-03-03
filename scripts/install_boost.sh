#!/bin/sh
DIR=$1
BOOST_ARCHIVE_URL="https://downloads.sourceforge.net/project/boost/boost/1.63.0/boost_1_63_0.tar.gz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fboost%2Ffiles%2Fboost%2F1.63.0%2F&ts=1488300925&use_mirror=netcologne"
BOOST_DIR="$DIR/vendor/boost"
wget $BOOST_ARCHIVE_URL -O "$DIR/boost.tar.gz" -N \
  && mkdir -p "$BOOST_DIR" \
  && tar -xf "$DIR/boost.tar.gz" --strip 1 --directory "$BOOST_DIR"  \
  && cd "$BOOST_DIR/tools/build" && ./bootstrap.sh \
  && cd ../../ && ./tools/build/b2 install -d0 --prefix=out/ --with-filesystem --with-program_options
rm -rf "$DIR/boost.tar.gz"
