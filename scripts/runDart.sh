#!/bin/bash

## Run Dart tests
yes | cp -f ${SQLITE3DART_LIBRARY_PATH}/${SQLITE3DART_LIBRARY_NAME}.* ${TRAVIS_BUILD_DIR}/sqlite3dart/example
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TRAVIS_BUILD_DIR}/sqlite3dart/example
cd ${TRAVIS_BUILD_DIR}/sqlite3dart/example
pwd
ls -l
pub get
dart -v ./sqlite3dart_test.dart
