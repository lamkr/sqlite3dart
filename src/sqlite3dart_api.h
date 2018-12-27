// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#ifndef sqlite3dart_api_h
#define sqlite3dart_api_h

#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_core.h"

extern Function syncFunctionsList[];

void sqlite3_threadsafe_(Dart_NativeArguments arguments);

void sqlite3_close_(Dart_NativeArguments arguments);

void sqlite3_libversion_number_(Dart_NativeArguments arguments);

//SQLITE_API int sqlite3_open(const cstring filename,   /* Database filename (UTF-8) */ sqlite3 **ppDb          /* OUT: SQLite db handle */);

void sqlite3_open_(Dart_NativeArguments arguments);

void sqlite3_exec_(Dart_NativeArguments arguments);

#endif
