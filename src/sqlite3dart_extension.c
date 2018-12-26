// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "core.h"
//#include "sqlite3.h"
//#include "include/dart_api.h"
//#include "include/dart_native_api.h"

typedef struct TFunctionLookup {
	const char* name;
	Dart_NativeFunction function;
} FunctionLookup;

Dart_NativeFunction resolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

DART_EXPORT Dart_Handle sqlite3dart_extension_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, resolveName, NULL);
	if (Dart_IsError(result_code)) {
		return result_code;
	}

	return Dart_Null();
}

void sqlite3_threadsafe_(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	bool isThreadSafe = sqlite3_threadsafe() != 0;
	Dart_Handle result = handleError(Dart_NewBoolean(isThreadSafe));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

void sqlite3_close_(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	sqlite3 *db = getSqliteHandle(arguments);
	int error = sqlite3_close(db);
	Dart_Handle result = !error ? Dart_Null : createException(sqlite3_errstr(error));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

void sqlite3_libversion_number_(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	int version = sqlite3_libversion_number();
	Dart_Handle result = handleError(Dart_NewInteger(version));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

//SQLITE_API int sqlite3_open(const char *filename,   /* Database filename (UTF-8) */ sqlite3 **ppDb          /* OUT: SQLite db handle */);

void sqlite3_open_(Dart_NativeArguments arguments) {
	Dart_EnterScope();

	const char *filename = getString(arguments, 0);

	Dart_Handle result;
	sqlite3 *db;
	int error = sqlite3_open(filename, &db);
	if( ! error ) {
		uint64_t address = (uint64_t) db;
		result = Dart_NewInteger(address);
	}
	else {
		result = createException(sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

void sqlite3_exec_(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	sqlite3* db = getSqliteHandle(arguments);
	const char *sql = getString(arguments, 1);
	char *errmsg;

	//TODO

	int rc = sqlite3_exec(db, sql, Sqlite3Callback, 0, &errmsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL exec error: %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	rc = sqlite3_close(db);
	if (rc)
		fprintf(stderr, "SQL close error: %i\n", rc);
	Dart_Handle result = handleError(Dart_NewInteger(rc));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

FunctionLookup functionList[] = {
	{"sqlite3_threadsafe_", sqlite3_threadsafe_},
	{"sqlite3_libversion_number_", sqlite3_libversion_number_},
	{"sqlite3_close_", sqlite3_close_},
	{"sqlite3_open_", sqlite3_open_},
	{"sqlite3_exec_", sqlite3_exec_},
	{NULL, NULL}
};

FunctionLookup noScopeFunctionList[] = {
	{NULL, NULL}
};

Dart_NativeFunction resolveName(Dart_Handle name, int argc, bool* auto_setup_scope) {
	if (!Dart_IsString(name)) {
		return NULL;
	}
	Dart_NativeFunction result = NULL;
	if (auto_setup_scope == NULL) {
		return NULL;
	}

	Dart_EnterScope();
	const char* cname;
	handleError(Dart_StringToCString(name, &cname));

	for (int i = 0; functionList[i].name != NULL; ++i) {
		if (strcmp(functionList[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = functionList[i].function;
			break;
		}
	}

	if (result != NULL) {
		Dart_ExitScope();
		return result;
	}

	for (int i = 0; noScopeFunctionList[i].name != NULL; ++i) {
		if (strcmp(noScopeFunctionList[i].name, cname) == 0) {
			*auto_setup_scope = false;
			result = noScopeFunctionList[i].function;
			break;
		}
	}

	Dart_ExitScope();
	return result;
}

