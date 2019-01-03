// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#ifndef sqlite3dart_core_h
#define sqlite3dart_core_h

#include "sqlite3.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef string
typedef char* cstring;
#endif

#ifndef pointer
typedef void* pointer;
#endif

typedef struct TFunction {
	cstring name;
	Dart_NativeFunction nativeFunction;
} Function;

typedef struct DynamicPointer {
	uint64_t size;
	pointer pointer;
} DynamicPointer;

typedef void(*NativeFunction)(Dart_CObject* message, Dart_CObject* result);

typedef struct TWrapperFunction {
	cstring name;
	NativeFunction nativeFunction;
} WrapperFunction;

Dart_Handle handleError(Dart_Handle handle);

Dart_Handle createException(const cstring message);

sqlite3* getSqliteHandle(Dart_NativeArguments arguments);

const cstring getCString(Dart_NativeArguments arguments, int index);

int Sqlite3Callback(pointer a_param, int argc, cstring* argv, cstring* column);

bool isEmptyOrNull(const cstring str);
	
bool isNotEmptyOrNull(const cstring str);

DynamicPointer* new_dynp(DynamicPointer *dynpointer, size_t size);

DynamicPointer* del_dynp(DynamicPointer *dynpointer);

#endif
