#!/bin/bash

# Download Dart SDK
export DART_SDK_FILENAME="dartsdk-macos-x64-release.zip"
export DART_SDK_CHANNEL="stable"
export DART_SDK_VERSION="2.7.0"
export DART_SDK="$HOME/dart-sdk"
export DART_SDK_URL="https://storage.googleapis.com/dart-archive/channels/${DART_SDK_CHANNEL}/release/${DART_SDK_VERSION}/sdk/${DART_SDK_FILENAME}"

mkdir -p ${HOME}/Downloads/
echo "--connect-timeout 15 --retry 5 -o ${HOME}/Downloads/${DART_SDK_FILENAME} -O https://storage.googleapis.com/dart-archive/channels/${DART_SDK_CHANNEL}/release/${DART_SDK_VERSION}/sdk/${DART_SDK_FILENAME}"
curl --connect-timeout 15 --retry 5 -o "${HOME}/Downloads/${DART_SDK_FILENAME}" -O ${URL%$'\r'}
unzip ${HOME}/Downloads/${DART_SDK_FILENAME} -d ${HOME}/
export PATH=$PATH:${DART_SDK}/bin
dart --version
