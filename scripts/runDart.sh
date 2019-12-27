#!/bin/bash

## Run Dart tests
pushd ${HOME}
yes | cp -rf ./build/libsqlite3dart_extension.so ./sqlite3dart/example
pushd $(pwd)/sqlite3dart/example
export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
dart -v sqlite3dart_test.dart
if [ $? -ne 0 ];
then 
	echo "# Error running dart tests"
	exit 1
fi
popd
exit 0
