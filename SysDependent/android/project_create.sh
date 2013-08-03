#this generates an ant based cli build of the android-jni project

export ANDROID_SDK=~/Software/adt/sdk
export PATH=$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools:$PATH

android update project --target 1 --name AEngine --path .

