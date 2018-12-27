// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_api.h"
#include "sqlite3dart_api_async.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

Function asyncFunctionsList[] = {
	{ "sqlite3_open_async_", sqlite3_open_async_},
	{ NULL, NULL }
};

void sqlite3_open_async( Dart_Port destPortId, Dart_Port replyPortId, Dart_CObject* message ) {
	if( message->type != Dart_CObject_kString || isEmptyOrNull(message->value.as_string) )
		;//TODO ERRO

	const cstring filename = message->value.as_string;
	Dart_CObject result;
	sqlite3 *db = NULL;

	int error = sqlite3_open(filename, &db);
	if (!error) {
		result.type = Dart_CObject_kInt64;
		result.value.as_int64 = (int64_t) db;
	}
	else {
		result.type = Dart_CObject_kUnsupported;
		result.value.as_string = sqlite3_errmsg(db);
		sqlite3_close(db);
	}
	Dart_PostCObject(replyPortId, &result);

		/*if (values != NULL) {
			Dart_CObject result;
			result.type = Dart_CObject::kUint8Array;
			result.value.as_byte_array.values = values;
			result.value.as_byte_array.length = length;
			Dart_PostCObject(reply_port_id, &result);
			free(values);
			// It is OK that result is destroyed when function exits.
			// Dart_PostCObject has copied its data.
			return;
		}*/
}

void sqlite3_open_async_(Dart_NativeArguments arguments) {
	Dart_SetReturnValue(arguments, Dart_Null());
	Dart_Port servicePort = Dart_NewNativePort("sqlite3_open_async", sqlite3_open_async, true);
	if (servicePort != -1) { // TODO kIllegalPort) {
		Dart_Handle sendPort = Dart_NewSendPort(servicePort);
		Dart_SetReturnValue(arguments, sendPort);
	}
}

Dart_CObject newSqlite3Handler(sqlite3* db) {
	Dart_CObject sqlite3handler;
	sqlite3handler.type = Dart_CObject_kTypedData;
	//sqlite3handler.value.as_typed_data = NULL; // ? TODO
	return sqlite3handler;
}