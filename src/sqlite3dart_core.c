// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_core.h"

Dart_Handle handleError(Dart_Handle handle) {
	if (Dart_IsError(handle)) {
		Dart_PropagateError(handle);
	}
	return handle;
}

Dart_Handle createException(const cstring message) {
	Dart_Handle apiError = Dart_NewApiError(message);
	return Dart_NewUnhandledExceptionError(apiError);
}

sqlite3* getSqliteHandle(Dart_NativeArguments arguments) {
	Dart_Handle addressHandle = Dart_GetNativeArgument(arguments, 0);
	uint64_t address = 0;
	handleError(Dart_IntegerToUint64(addressHandle, &address));
	return (sqlite3*)address;
}

const cstring getCString(Dart_NativeArguments arguments, int index) {
	Dart_Handle stringHandle = handleError(Dart_GetNativeArgument(arguments, index));
	cstring str = NULL;
	handleError(Dart_StringToCString(stringHandle, &str));
	return (const cstring) str;
}

int Sqlite3Callback(pointer a_param, int argc, cstring *argv, cstring *column) {
	for (int i = 0; i < argc; i++)
		printf("%s,\t", argv[i]);
	printf("\n");
	return 0;
}

bool isEmptyOrNull(const cstring str) {
	return str == NULL|| strlen(str) == 0;
}

bool isNotEmptyOrNull(const cstring str) {
	return !isEmptyOrNull(str);
}

DynamicPointer* new_dynp(DynamicPointer *dynpointer, size_t size) {
	if (dynpointer->pointer == NULL) {
		dynpointer->size = size;
		dynpointer->pointer = malloc(dynpointer->size);
		memset(dynpointer->pointer, 0, dynpointer->size);
	}
	else if( size > dynpointer->size ) {
		pointer newPointer = realloc(dynpointer->pointer, size);
		if (newPointer) {
			dynpointer->size = size;
			dynpointer->pointer = newPointer;
			memset(dynpointer->pointer, 0, dynpointer->size);
		}
	}
	return dynpointer;
}

DynamicPointer* del_dynp(DynamicPointer *dynpointer) {
	if (dynpointer && dynpointer->pointer) {
		free(dynpointer->pointer);
		dynpointer->size = 0;
		dynpointer->pointer = NULL;
	}
	return dynpointer;
}

