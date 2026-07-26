#!/bin/bash
set -e

# === Configuration ===
TARGET=i686-terrarium     # or your custom target triplet
PREFIX=/opt/cross         # where your cross-compiler and libc will live
BUILD_DIR=build-newlib
SRC_DIR=newlib            # path to newlib source
DEST_FS=~/Terrarium/root  # OS root filesystem

# === Step 1: Build newlib ===
mkdir -p $BUILD_DIR
cd $BUILD_DIR

../$SRC_DIR/configure \
    --target=$TARGET \
    --prefix=$PREFIX \
    --disable-newlib-supplied-syscalls

make -j$(nproc)
make install

cd ..

# === Step 2: Copy headers and libraries to your OS filesystem ===
mkdir -p $DEST_FS/usr/include
mkdir -p $DEST_FS/lib

cp -r $PREFIX/$TARGET/include/* $DEST_FS/usr/include/
cp $PREFIX/$TARGET/lib/libc.a $DEST_FS/lib/
cp $PREFIX/$TARGET/lib/libm.a $DEST_FS/lib/
