#!/bin/bash

#ENV="schroot -p --"

export ANDROID_NDK=~/Software/adt/android-ndk-r9
export ANDROID_SDK=~/Software/adt/sdk
export PATH=$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools:$PATH

cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=9 ..
#make
cd ..

# $ENV ant debug

