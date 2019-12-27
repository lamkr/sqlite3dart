#!/bin/bash

export DART_SDK_FILENAME=dartsdk-macos-x64-release.zip
export DART_SDK_CHANNEL=stable
export DART_SDK_VERSION=2.7.0
export DART_SDK=${HOME}/dart-sdk

## Download Dart SDK
mkdir -p ${HOME}/Downloads/
curl --connect-timeout 15 --retry 5 -o ${HOME}/Downloads/${DART_SDK_FILENAME} -O https://storage.googleapis.com/dart-archive/channels/${DART_SDK_CHANNEL}/release/${DART_SDK_VERSION}/sdk/${DART_SDK_FILENAME}
unzip ${HOME}/Downloads/${DART_SDK_FILENAME} -d ${HOME}/
export PATH=$PATH:${DART_SDK}/bin
dart --version
