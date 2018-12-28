// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_api.h"

void get_receive_port(Dart_NativeArguments arguments);

Function syncFunctionsList[] = {
	{ "sqlite3_threadsafe_", sqlite3_threadsafe_},
	{ "sqlite3_libversion_number_", sqlite3_libversion_number_ },
	{ "sqlite3_close_", sqlite3_close_ },
	{ "sqlite3_open_", sqlite3_open_ },
	{ "sqlite3_exec_", sqlite3_exec_ },
	{ "get_receive_port", get_receive_port },
	{ NULL, NULL }
};

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
	Dart_Handle result = !error ? Dart_Null() : createException((const cstring) sqlite3_errstr(error));
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

//SQLITE_API int sqlite3_open(const cstring filename,   /* Database filename (UTF-8) */ sqlite3 **ppDb          /* OUT: SQLite db handle */);

void sqlite3_open_(Dart_NativeArguments arguments) {
	Dart_EnterScope();

	const cstring filename = getCString(arguments, 0);

	Dart_Handle result;
	sqlite3 *db;
	int error = sqlite3_open(filename, &db);
	if( ! error ) {
		uint64_t address = (uint64_t) db;
		result = Dart_NewInteger(address);
	}
	else {
		result = createException((const cstring) sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

void sqlite3_exec_(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	Dart_Handle result;
	cstring errorMessage = NULL;

	sqlite3* db = getSqliteHandle(arguments);
	const cstring sql = getCString(arguments, 1);

	//TODO

	int rc = sqlite3_exec(db, sql, Sqlite3Callback, 0, &errorMessage);
	if (rc != SQLITE_OK) {
		result = createException(errorMessage);
		sqlite3_free(errorMessage);
	}

	rc = sqlite3_close(db);
	if (rc)
		fprintf(stderr, "SQL close error: %i\n", rc);
	result = handleError(Dart_NewInteger(rc));
	Dart_SetReturnValue(arguments, result);
	Dart_ExitScope();
}

