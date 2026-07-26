#!/bin/bash
export TARGET=i686-elf  # or your arch
export PREFIX=/opt/cross

# Assumes you already built Clang/LLVM
git clone https://github.com/llvm/llvm-project.git
cd llvm-project

mkdir build-libcxx && cd build-libcxx
cmake -G "Ninja" ../libcxx \
  -DCMAKE_INSTALL_PREFIX=$PREFIX/$TARGET \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DLIBCXX_ENABLE_SHARED=ON \
  -DLIBCXX_ENABLE_STATIC=ON \
  -DLIBCXX_ENABLE_THREADS=OFF \
  -DLIBCXX_ENABLE_EXCEPTIONS=ON \
  -DLIBCXX_CXX_ABI=libcxxabi \
  -DCMAKE_SYSROOT=/path/to/your/os/root

ninja
ninja install
