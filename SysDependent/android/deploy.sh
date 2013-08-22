#!/bin/sh
make
ant debug
adb install -r bin/AEngine-debug.apk
