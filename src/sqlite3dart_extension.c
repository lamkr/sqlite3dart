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

#define RECEIVE_PORT_NAME	"lamkr.sqlite3dart.receivePort"

Function noScopeFunctionsList[] = {
	{ NULL, NULL }
};

Dart_Port _receivePort = ILLEGAL_PORT;

Dart_NativeFunction resolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

void createReceivePort();
void messageHandler(Dart_Port receivePort, Dart_CObject* message);

DART_EXPORT Dart_Handle sqlite3dart_extension_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, resolveName, NULL);
	if (Dart_IsError(result_code)) {
		return result_code;
	}

	createReceivePort();

	return Dart_Null();
}

void show(pointer p) {
	char *pc = p;
	for (int i = 0; i < 100; i++) {
		if (pc[i] < 32 || pc[i] > 127)
			printf("%d ", pc[i]);
		else
			printf("%c ", pc[i]);
	}
	printf("\n");
}

void oi() {
	printf("Oi!");
}

void get_receive_port(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	if (_receivePort == ILLEGAL_PORT) {
		pointer p = Dart_GetNativeIsolateData(arguments);

		show(p);
		printf("get_receive_port: p=%lld\n", (uint64_t) p);

		Dart_SetReturnValue(arguments, Dart_Null());
		_receivePort = Dart_NewNativePort(RECEIVE_PORT_NAME, messageHandler, true);
		printf("get_receive_port: _receivePort=%lld\n", _receivePort);
	}
	if (_receivePort != ILLEGAL_PORT) {
		Dart_Handle sendPort = Dart_NewSendPort(_receivePort);
		Dart_SetReturnValue(arguments, sendPort);
	}
	Dart_ExitScope();
}

void createReceivePort() {
	_receivePort = Dart_NewNativePort(RECEIVE_PORT_NAME, messageHandler, true);
	printf("createReceivePort: receivePort=%lld\n", _receivePort);
}

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
		result->value.as_string = (const cstring) sqlite3_errmsg(db);
		sqlite3_close(db);
	}
}

Function2 functionsList[] = {
	{ "sqlite3_open", _sqlite3_open},
	{ NULL, NULL }
};


void messageHandler(Dart_Port receivePort, Dart_CObject* message) {
	printf("sqlite3_open_async: receivePort=%lld\n", receivePort);

	Dart_Port reply_port_id = ILLEGAL_PORT;

	if (message->type != Dart_CObject_kArray || message->value.as_array.length != 3)
	{
		//TODO ERRO
		printf("messageHandler: message->type=%d \n", message->type);
	}

	Dart_CObject* paramReplyPortId = message->value.as_array.values[0];
	Dart_CObject* paramFunctionName = message->value.as_array.values[1];

	Dart_Port replyPortId = paramReplyPortId->value.as_send_port.id;
	printf("p0=%lld\n", replyPortId);

	const cstring functionName = paramFunctionName->value.as_string;
	printf("p1=%s\n", functionName);

	Dart_CObject result;
	for( int i = 0; functionsList[i].name != NULL; ++i ) {
		if( strcmp(functionsList[i].name, functionName) == 0 ) {
			printf("calling %s\n", functionsList[i].name);
			functionsList[i].nativeFunction(message, &result);
			printf("called\n");
			break;
		}
	}

	Dart_PostCObject(replyPortId, &result);
}

Dart_NativeFunction resolveName(Dart_Handle name, int argc, bool* auto_setup_scope) {
	if (!Dart_IsString(name)) {
		return NULL;
	}

	Dart_NativeFunction result = NULL;
	if (auto_setup_scope == NULL) {
		return NULL;
	}

	Dart_EnterScope();
	cstring cname;
	handleError(Dart_StringToCString(name, &cname));

	for (int i = 0; syncFunctionsList[i].name != NULL; ++i) {
		if (strcmp(syncFunctionsList[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = syncFunctionsList[i].nativeFunction;
			break;
		}
	}

	for (int i = 0; asyncFunctionsList[i].name != NULL; ++i) {
		if (strcmp(asyncFunctionsList[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = asyncFunctionsList[i].nativeFunction;
			break;
		}
	}

	if (result != NULL) {
		Dart_ExitScope();
		return result;
	}

	for (int i = 0; noScopeFunctionsList[i].name != NULL; ++i) {
		if (strcmp(noScopeFunctionsList[i].name, cname) == 0) {
			*auto_setup_scope = false;
			result = noScopeFunctionsList[i].nativeFunction;
			break;
		}
	}

	Dart_ExitScope();
	return result;
}
