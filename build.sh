##
## Use this script to facilitate the package building. 
##
## At terminal, goes to the sources main directory, and run this script.
## Syntax:
## 	build [<parameter>]
## Where <parameter> can be:
##	rm 		- Remove old builded files before building the package; after, it runs "dart" (see below).
##  dart 	- Only runs the Dart test at directory 'sqlite3dart/example'.
##
##  If <parameter> is not present, the script build the package and runs the Dart test.
##

## Find Dart program on environment variable PATH.
if [ -z "$DART_SDK" ]
then
	export DART_SDK = $(dirname $(dirname `which dart`))
fi 

## Check if CMake, C/C++ compilers are installed.
if [ -z `which cmake` ] || [ -z `which gcc` ] || [ -z `(which g++)` ] || [ -z `which make` ]
then
	echo "# Please, install gcc, g++ and make to build the library."
	exit 
fi

function runDart ()
{
	pushd $(pwd)/sqlite3dart/example
	local PREVIOUS_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
	
	dart -v sqlite3dart_test.dart	
	if [ $? -ne 0 ];
	then 
		echo "# Error running dart tests"
	fi
	export LD_LIBRARY_PATH=$PREVIOUS_LD_LIBRARY_PATH
	popd
	exit;
}

if [ "$1" = "dart" ]
then 
	runDart
	exit;
else
then
	rm -rf ./build;
fi

cmake -S . -Bbuild 
if [ $? -ne 0 ]
then
	echo "# Error running CMake"
	exit;
fi

## Build the package
(cd ./build && make)
if [ $? -ne 0 ]
then
	echo "# Error running Make"
	exit;
fi

yes | cp -rf ./build/libsqlite3dart_extension.so ./sqlite3dart/example
runDart
