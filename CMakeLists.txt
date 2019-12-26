#
# Copyright (c) 2018 Luciano Bonifacio Rodrigues.
#
# CMakeList.txt : CMake project for sqlite3dart, include source and define
# When Dart is x64, use the command:
#	$ cmake -S <sqlite3dart-parh> -B <to-generate-files-path> -DCMAKE_GENERATOR_PLATFORM=x64 
#
# Comments:
# C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin
# https://medium.com/aprendacpp/usando-cmake-para-compilar-c-c-abf52c7dcbc8

cmake_minimum_required (VERSION 3.10.2)

project(sqlite3dart_extension)

set(CMAKE_C_FLAGS "-v")

# It is necessary to specify the environment variable to instalation path of Dart SDK.
if(NOT DEFINED ENV{DART_SDK})
	message(FATAL_ERROR "°° Please, set environment variable DART_SDK to instalation path of Dart SDK.")
endif()

# Change to 'FALSE' if then Dart SDK installed is x86.
set(IS_DART_SDK_X64 TRUE)

# Add source to this project's executable.
IF(WIN32)
	FILE(GLOB SOURCES src/*.c src/*.cpp src/sqlite3/*.c src/windows/*.c)
ELSE()
	FILE(GLOB SOURCES src/*.c src/*.cpp src/sqlite3/*.c)
ENDIF()

include_directories (src src/sqlite3 $ENV{DART_SDK})

add_library (sqlite3dart_extension SHARED ${SOURCES})

IF(IS_DART_SDK_X64)
	set(dart_architecture "x64")
	set(CMAKE_BUILD_TYPE Release64)
	set(CMAKE_GENERATOR_PLATFORM x64)
ELSE()
	set(dart_architecture "x86")
ENDIF()
message ("°° Dart ${dart_architecture}, installed on $ENV{DART_SDK}")

IF(WIN32)
	FILE(GLOB SOURCES src/windows/*.c)
	add_compile_definitions(DART_SHARED_LIB)
	set(DART_LIB dart.lib)
# poderia usar assim: set(DART_LIB_PATH $ENV{DART_SDK}/bin/dart.lib)
# poderia usar assim: message(${DART_LIB_PATH})
	link_directories($ENV{DART_SDK}/bin)
	target_link_libraries(sqlite3dart_extension $ENV{DART_SDK}/bin/${DART_LIB})
ENDIF()
