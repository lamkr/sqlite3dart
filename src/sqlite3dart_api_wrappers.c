// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_core.h"

void _sqlite3_open(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[2];
	const cstring filename = param->value.as_string;

	sqlite3 *db;
	int error = sqlite3_open(filename, &db);
	if (!error) {
		result->type = Dart_CObject_kInt64;
		result->value.as_int64 = (int64_t)db;
	}
	else {
		result->type = Dart_CObject_kString;
		result->value.as_string = (const cstring)sqlite3_errmsg(db);
		sqlite3_close(db);
	}
}

const WrapperFunction wrappersFunctionsList[] = {
	{ "_sqlite3_open", _sqlite3_open},
	{ NULL, NULL }
};

