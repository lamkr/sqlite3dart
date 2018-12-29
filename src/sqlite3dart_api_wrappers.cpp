// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <excpt.h>
#include "sqlite3dart_core.h"

#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

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
		result->type = Dart_CObject_kString;
		result->value.as_string = (const cstring) sqlite3_errmsg(db);
		sqlite3_close(db);
	}
}

void sqlite3_close_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[2];
	printf("sqlite3_close_wrapper: %lld\n", param->value.as_int64);
	puts("1");
	sqlite3 *db = (sqlite3 *) param->value.as_int64;
	int error = 0;
	/*TRY {
		puts("2");
		error = sqlite3_close(db);
		printf("sqlite3_close_wrapper: error %d\n", error);
	}
	CATCH {
		puts("excecao\n");
		error = 1;
	}
	ETRY;*/
	__try {
		puts("2");
		error = sqlite3_close(db);
		printf("sqlite3_close_wrapper: error %d\n", error);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		puts("excecao\n");
		error = 1;
	}
	if (!error) {
		result->type = Dart_CObject_kNull;
	}
	else {
		result->type = Dart_CObject_kString;
		result->value.as_string = (const cstring)sqlite3_errstr(error);
	}
	puts("fim");

}

const WrapperFunction wrappersFunctionsList[] = {
	{ "sqlite3_open_wrapper", sqlite3_open_wrapper},
	{ "sqlite3_close_wrapper", sqlite3_close_wrapper},
	{ NULL, NULL }
};

