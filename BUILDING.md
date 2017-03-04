Building From Source
====================

Building from source is relatively complicated, which is why we advise you to
use one of the binaries. Not all platforms have precompiled binaries, though,
so it might be necessary to build your own.

## Linux 

### Debian and Ubuntu

You will need to install a more recent version of LLVM and Clang to get the code to compile. Nightly packages of LLVM and Clang can be installed via the 
[official repository](http://apt.llvm.org/). We use
the 3.9.1 branch. 

```
wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -
```

Next, add the package sources to APT. Choose one of the below.

```
# precise
deb http://apt.llvm.org/precise/ llvm-toolchain-precise-3.9 main
deb-src http://apt.llvm.org/precise/ llvm-toolchain-precise-3.9 main
# trusty
deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main
deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main
# wily
deb http://apt.llvm.org/wily/ llvm-toolchain-wily-3.9 main
deb-src http://apt.llvm.org/wily/ llvm-toolchain-wily-3.9 main
# xenial
deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-3.9 main
deb-src http://apt.llvm.org/xenial/ llvm-toolchain-xenial-3.9 main
# yakkety
deb http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-3.9 main
deb-src http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-3.9 main
# zetsy
deb http://apt.llvm.org/zesty/ llvm-toolchain-zesty-3.9 main
deb-src http://apt.llvm.org/zesty/ llvm-toolchain-zesty-3.9 main
```

The following assumes you have Ubuntu Xenial (16.04) LTS installed:

```
echo "deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main" | sudo tee -a /etc/apt/sources.list
echo "deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main" |
```

Finally, install LLVM and Clang:

```
sudo apt-get install clang-3.9 libclang-3.9-dev 
```

On recent versions of Ubuntu and Debian, it should be sufficient to install the following additional packages. If not, you will need to compile them from source.

```
sudo apt-get install protobuf-compiler libprotobuf-dev libboost-{filesystem,program-options}-dev libedit-dev libz-dev
```

One last step is to generate the protobuf translation units according to your specific version:


The instructions for installing AutoFFI then are pretty simple:

```bash
git clone https://github.com/AutoFFI/AutoFFI && cd AutoFFI/
protoc --cpp_out=src/ autoffi.proto
mkdir -p build/Release && cd build/Release
cmake ../../ -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make install
```

### Other Linux distros

Unfortunately, we don't have build instructions for other platforms. If you manage to get it working, please feel free to submit a pull request with appropriate instructions!


### OS X

OS X is shipped with LLVM and Clang, but it uses a custom version of the
software, which as far as we know isn't suitable for linking. Therefore,
you will need to acquire "clean" version of LLVM. Luckily, you can
install LLVM with Homebrew.

```
brew install llvm --with-clang --with-rtti
```

The commands for building are then as follows:

```bash
git clone https://github.com/AutoFFI/AutoFFI && cd AutoFFI/
protoc --cpp_out=src/ autoffi.proto
mkdir -p build/Release && cd build/Release
cmake ../../ -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make install
```

