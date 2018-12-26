#
# CMakeList.txt : CMake project for sqlite3dart, include source and define
# When Dart is x64, use the command:
#	$ cmake -S <sqlite3dart-parh> -B <to-generate-files-path> -DCMAKE_GENERATOR_PLATFORM=x64 
#
# C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin

cmake_minimum_required (VERSION 3.12)

project(sqlite3dart_extension)

# Change path for your Dart SDK instalation path.
set(DART_SDK $ENV{DART_SDK})
# Change to 'FALSE' if Dart SDK installed is x86.
# TODO: extract version in 
#	C> dart --VERSION
#	Dart VM version: 2.2.0-dev.1.1 (Thu Dec 13 12:40:26 2018 +0100) on "windows_x64"
set(IS_DART_SDK_X64 TRUE)

# Add source to this project's executable.
IF(WIN32)
	FILE(GLOB SOURCES src/*.c src/sqlite3/*.c src/windows/*.c)
ELSE()
	FILE(GLOB SOURCES src/*.c src/sqlite3/*.c)
ENDIF()

include_directories( src/sqlite3 ${DART_SDK} )

add_library (sqlite3dart_extension SHARED ${SOURCES})

IF(IS_DART_SDK_X64)
	message ("Dart is x64 in ${DART_SDK}")
	set(CMAKE_BUILD_TYPE Release64)
	set(CMAKE_GENERATOR_PLATFORM x64)
ELSE()
	message ("Dart is x86 in ${DART_SDK}")
ENDIF()

IF(WIN32)
	FILE(GLOB SOURCES src/windows/*.c)
	add_compile_definitions(DART_SHARED_LIB)
	set(DART_LIB dart.lib)
	link_directories(${DART_SDK}/bin)
	target_link_libraries(sqlite3dart_extension ${DART_SDK}/bin/${DART_LIB})
ENDIF()
