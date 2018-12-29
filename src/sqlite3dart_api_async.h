// SQLite3 for Dart
// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#ifndef sqlite3dart_api_async_h
#define sqlite3dart_api_async_h

#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_core.h"

extern Function asyncFunctionsList[];

void sqlite3_open_async_(Dart_NativeArguments arguments);

#endif
