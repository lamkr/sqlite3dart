# CMakeList.txt : CMake project for sqlite3dart, include source and define
cmake_minimum_required (VERSION 3.8)

message(STATUS "CMAKE_SIZEOF_VOID_P=" ${CMAKE_SIZEOF_VOID_P}) 
message ("${CMAKE_SIZEOF_VOID_P}")

#set (CMAKE_GENERATOR_PLATFORM "x64") # Visual Studio 15 2017 Win64

project(sqlite3dart_extension)

# Change path for your Dart SDK instalation path
SET(DART_SDK $ENV{DART_SDK})

message ("${DART_SDK}")

include_directories (${DART_SDK})

# Add source to this project's executable.
IF(MSVC)
	IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
		message ("x64")
		set (PROJECTNAME_ARCHITECTURE "x64")
		set (CMAKE_GENERATOR_PLATFORM "x64") # Visual Studio 15 2017 Win64
	ENDIF()
	add_compile_definitions(DART_SHARED_LIB)
	link_directories("${DART_SDK}/bin")
	add_library (sqlite3dart_extension SHARED "windows/sqlite3dart_extension_dllmain_win.cpp" "sqlite3dart_extension.cpp" "sqlite3.c")	
	target_link_libraries(sqlite3dart_extension dart.lib)
ENDIF()

IF(UNIX)
	message ("is unix")
ENDIF()

