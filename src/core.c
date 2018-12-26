// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "core.h"

Dart_Handle handleError(Dart_Handle handle) {
	if (Dart_IsError(handle)) {
		Dart_PropagateError(handle);
	}
	return handle;
}

sqlite3* getSqliteHandle(Dart_NativeArguments arguments) {
	Dart_Handle addressHandle = handleError(Dart_GetNativeArgument(arguments, 0));
	uint64_t address = 0;
	handleError(Dart_IntegerToUint64(addressHandle, &address));
	return (sqlite3*)address;
}

const char* getString(Dart_NativeArguments arguments, int index) {
	Dart_Handle stringHandle = handleError(Dart_GetNativeArgument(arguments, index));
	char* str = NULL;
	handleError(Dart_StringToCString(stringHandle, &str));
	return (const char*)str;
}

int Sqlite3Callback(void *a_param, int argc, char **argv, char **column) {
	for (int i = 0; i < argc; i++)
		printf("%s,\t", argv[i]);
	printf("\n");
	return 0;
}
