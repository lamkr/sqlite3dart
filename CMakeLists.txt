# CMakeList.txt : CMake project for sqlite3dart, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(sqlite3dart_extension)

SET(DART_SDK $DART_SDK)

message ("$DART_SDK")

add_compile_definitions(DART_SHARED_LIB)

include_directories ($DART_SDK)

# Add source to this project's executable.
IF(WIN32)
	set(CMAKE_GENERATOR_PLATFORM=x64) # Visual Studio 15 2017 [arch]
	link_directories("$(DART_SDK)/bin")
	add_library (sqlite3dart_extension "windows/sqlite3dart_extension_dllmain_win.cpp" "sqlite3dart_extension.cpp" "sqlite3.c")	
	target_link_libraries(sqlite3dart_extension dart.lib)
ENDIF()

IF(UNIX)
	message ("is unix")
ENDIF()


# TODO: Add tests and install targets if needed.
