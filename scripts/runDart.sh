#!/bin/bash

## Run Dart tests
yes | cp -rf ${SQLITE3DART_LIBRARY_PATH}/${SQLITE3DART_LIBRARY_NAME}.* ${TRAVIS_BUILD_DIR}/example
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TRAVIS_BUILD_DIR}/example
cd ${TRAVIS_BUILD_DIR}/example
pwd
dart -v sqlite3dart_test.dart
#if [ $? -ne 0 ]
#then 
#	echo "# Error running dart tests"
#	exit 1
#fi
exit $?
