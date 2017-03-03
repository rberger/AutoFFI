#!/bin/sh
set -e
export CLANG_VER=3.9
ls -la /usr/bin/*-$CLANG_VER | grep llvm | awk '{print $9}' | while read LLVM_CMD; do
  SHORT_CMD=$(echo "$LLVM_CMD" | sed "s%.*/%%;s/-$CLANG_VER//")
  sudo update-alternatives --install "/usr/bin/$SHORT_CMD" "$SHORT_CMD" "$LLVM_CMD" 1
done
