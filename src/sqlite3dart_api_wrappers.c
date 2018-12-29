// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <setjmp.h>
#include "sqlite3dart_core.h"

#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

const cstring ERR_INVALID_HANDLE = "Invalid database handle.";

bool isInvalidHandle(int64_t address) {
	int64_t address32 = (int32_t)address;
	return address32 < 1;
}

void mountException(Dart_CObject* result, const cstring message) {
	result->type = Dart_CObject_kString;
	result->value.as_string = ERR_INVALID_HANDLE;
}

void sqlite3_open_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[2];
	const cstring filename = param->value.as_string;

	sqlite3 *db;
	int error = sqlite3_open(filename, &db);
	if (!error) {
		result->type = Dart_CObject_kInt64;
		result->value.as_int64 = (int64_t) db;
	}
	else {
		mountException(result, (const cstring)sqlite3_errstr(error));
		sqlite3_close(db);
	}
}

void sqlite3_close_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[2];
	int64_t address = param->value.as_int64;
	if (isInvalidHandle(address)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}

	sqlite3 *db = (sqlite3 *)address;
	int error = sqlite3_close(db);
	if (!error) {
		result->type = Dart_CObject_kNull;
	}
	else {
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

const WrapperFunction wrappersFunctionsList[] = {
	{ "sqlite3_open_wrapper", sqlite3_open_wrapper},
	{ "sqlite3_close_wrapper", sqlite3_close_wrapper},
	{ NULL, NULL }
};

