// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <inttypes.h>
#include "sqlite3dart_api.h"
#include "sqlite3dart_api_async.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

//DEPRECATED
Function asyncFunctionsList[] = {
//	{ "sqlite3_close_port_", sqlite3_close_port_},
	{ "sqlite3_open_async_", sqlite3_open_async_},
	{ NULL, NULL }
};

void sqlite3_open_async( Dart_Port destPortId, Dart_CObject* message ) {
	printf("sqlite3_open_async: destPortId=%" PRId64 "\n", destPortId);

	Dart_Port reply_port_id = ILLEGAL_PORT;

	if (message->type != Dart_CObject_kArray || message->value.as_array.length != 2 )
	//if( message->type != Dart_CObject_kString || isEmptyOrNull(message->value.as_string) )
	{
		//TODO ERRO
			printf("sqlite3_open_async: message->type=%d \n", message->type);
	}

	puts("1");
	Dart_CObject* param0 = message->value.as_array.values[0];
	Dart_CObject* param1 = message->value.as_array.values[1];

	puts("2");
	Dart_Port replyPortId  = param0->value.as_send_port.id;
	printf("p0=%" PRId64 "\n", replyPortId);

	cstring filename = NULL;
	if(param1->type != Dart_CObject_kNull)
		filename = param1->value.as_string;
	printf("p1=%s\n", filename);
	Dart_CObject result;
	sqlite3 *db = NULL;

	puts("3");
	int error = sqlite3_open(filename, &db);
	puts("4");
	if (error==SQLITE_OK) {
		puts("5");
		result.type = Dart_CObject_kInt64;
		result.value.as_int64 = (int64_t) db;
	}
	else {
		const char* errmsg = sqlite3_errmsg(db);
		result.type = Dart_CObject_kTypedData;
		result.value.as_typed_data.type = Dart_TypedData_kUint8;
		result.value.as_typed_data.values = (uint8_t*) errmsg;
		result.value.as_typed_data.length = strlen(errmsg);
		printf("6 err: %s\n", errmsg);
		sqlite3_close(db);
	}
	puts("7");
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
	Dart_EnterScope();
	Dart_SetReturnValue(arguments, Dart_Null());
	Dart_Port servicePort = Dart_NewNativePort("sqlite3_open_async", sqlite3_open_async, true);
	printf("sqlite3_open_async_: destPortId=%" PRId64 "\n", servicePort);
	if (servicePort != ILLEGAL_PORT) {
		Dart_Handle sendPort = Dart_NewSendPort(servicePort);
		Dart_SetReturnValue(arguments, sendPort);
	}
	Dart_ExitScope();
}

//TODO
Dart_CObject newSqlite3Handler(sqlite3* db) {
	Dart_CObject sqlite3handler;
	sqlite3handler.type = Dart_CObject_kTypedData;
	//sqlite3handler.value.as_typed_data = NULL; // ? TODO
	return sqlite3handler;
}