// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

Dart_NativeFunction ResolveName(Dart_Handle name,
	int argc,
	bool* auto_setup_scope);

DART_EXPORT Dart_Handle sqlite3win_extension_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, ResolveName, NULL);
	if (Dart_IsError(result_code)) {
		return result_code;
	}

	return Dart_Null();
}


Dart_Handle HandleError(Dart_Handle handle) {
	if (Dart_IsError(handle)) {
		Dart_PropagateError(handle);
	}
	return handle;
}


void SystemRand(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	srand((unsigned)time(NULL));
	Dart_Handle result = HandleError(Dart_NewInteger(rand()));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

void _sqlite3_libversion_number(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	int version = sqlite3_libversion_number();
	Dart_Handle result = HandleError(Dart_NewInteger(version));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

//SQLITE_API int sqlite3_open(const char *filename,   /* Database filename (UTF-8) */ sqlite3 **ppDb          /* OUT: SQLite db handle */);

void _sqlite3_open(Dart_NativeArguments arguments) {
	char *filename;
	Dart_EnterScope();
	Dart_Handle _filename = HandleError(Dart_GetNativeArgument(arguments, 0));
	/*Dart_Handle result;
	if (!Dart_IsString(_filename))
		result = HandleError(Dart_NewInteger(0));
	else
		result = HandleError(Dart_NewInteger(1));

	if (!Dart_IsString(_filename)) {
		return;
	}*/
	HandleError(Dart_StringToCString(_filename, &filename));
	puts(filename);

	Dart_Handle result;
	sqlite3 *db;
	int erro = sqlite3_open(filename, &db);
	if (!erro) {
		uint64_t address = (uint64_t)db;
		printf("open %llu %llx", address, address);
		result = HandleError(Dart_NewInteger(address));
	}
	else {
		puts(sqlite3_errmsg(db));
		sqlite3_close(db);
		result = HandleError(Dart_NewInteger(erro));
	}
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

int The_Callback(void *a_param, int argc, char **argv, char **column) {
	for (int i = 0; i < argc; i++)
		printf("%s,\t", argv[i]);
	printf("\n");
	return 0;
}

void _sqlite3_exec(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	Dart_Handle _db = HandleError(Dart_GetNativeArgument(arguments, 0));
	Dart_Handle _sql = HandleError(Dart_GetNativeArgument(arguments, 1));
	char *sql, *errmsg;

	uint64_t address = 0;

	HandleError(Dart_StringToCString(_sql, &sql));
	HandleError(Dart_IntegerToUint64(_db, &address));

	printf("exec %llu %llx \n%s", address, address, sql);

	sqlite3 *db = (sqlite3 *)address;

	int rc = sqlite3_exec(db, sql, The_Callback, 0, &errmsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL exec error: %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	rc = sqlite3_close(db);
	if (rc)
		fprintf(stderr, "SQL close error: %i\n", rc);
	Dart_Handle result = HandleError(Dart_NewInteger(rc));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

typedef struct TFunctionLookup {
	const char* name;
	Dart_NativeFunction function;
} FunctionLookup;


FunctionLookup function_list[] = {
	{"SystemRand", SystemRand},
	{"_sqlite3_libversion_number", _sqlite3_libversion_number},
	{"_sqlite3_open", _sqlite3_open},
	{"_sqlite3_exec", _sqlite3_exec},
	{NULL, NULL} };


FunctionLookup no_scope_function_list[] = {
  {"NoScopeSystemRand", SystemRand},
  {NULL, NULL}
};

Dart_NativeFunction ResolveName(Dart_Handle name,
	int argc,
	bool* auto_setup_scope) {
	if (!Dart_IsString(name)) {
		return NULL;
	}
	Dart_NativeFunction result = NULL;
	if (auto_setup_scope == NULL) {
		return NULL;
	}

	Dart_EnterScope();
	const char* cname;
	HandleError(Dart_StringToCString(name, &cname));

	for (int i = 0; function_list[i].name != NULL; ++i) {
		if (strcmp(function_list[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = function_list[i].function;
			break;
		}
	}

	if (result != NULL) {
		Dart_ExitScope();
		return result;
	}

	for (int i = 0; no_scope_function_list[i].name != NULL; ++i) {
		if (strcmp(no_scope_function_list[i].name, cname) == 0) {
			*auto_setup_scope = false;
			result = no_scope_function_list[i].function;
			break;
		}
	}

	Dart_ExitScope();
	return result;
}