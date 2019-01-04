// Copyright (c) 2018 Luciano Rodrigues (Brodi).
// Please see the AUTHORS file for details. 
// All rights reserved. Use of this source code is governed by a MIT-style 
// license that can be found in the LICENSE file.
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include "sqlite3dart_core.h"

#define RECEIVE_PORT_NAME	"lamkr.sqlite3dart.receivePort"
#define DYNPOINTER_DEFAULT_SIZE (10*1024)

extern const WrapperFunction wrappersFunctionsList[];

const int MAX_DYN_POINTERS = 2;
DynamicPointer *_dynpointers;

Dart_NativeFunction resolveFunctionName(Dart_Handle name, int argc, bool* auto_setup_scope);
void get_receive_port(Dart_NativeArguments arguments);
void messageHandler(Dart_Port receivePort, Dart_CObject* message);

const Function functionsList[] = {
	{ "get_receive_port", get_receive_port },
	{ NULL, NULL }
};

Dart_Port _receivePort = ILLEGAL_PORT;

DART_EXPORT Dart_Handle sqlite3dart_extension_Init(Dart_Handle parent_library) {
	_dynpointers = malloc(sizeof(DynamicPointer)*MAX_DYN_POINTERS); 
	memset(_dynpointers, 0, sizeof(DynamicPointer)*MAX_DYN_POINTERS);
	for( int i = 0; i < MAX_DYN_POINTERS; i++ )
		new_dynp(&_dynpointers[i], DYNPOINTER_DEFAULT_SIZE);
	//printf("sqlite3dart_extension_Init: _dynpointer { %d, %X }\n", (int) _dynpointer.size, _dynpointer.pointer);

	if (Dart_IsError(parent_library)) {
		return parent_library;
	}
	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, resolveFunctionName, NULL);
	if (Dart_IsError(result_code)) {
		return result_code;
	}
	return Dart_Null();
}

Dart_NativeFunction resolveFunctionName(Dart_Handle name, int argc, bool* auto_setup_scope) {
	if (!Dart_IsString(name)) {
		return NULL;
	}

	if (auto_setup_scope == NULL) {
		return NULL;
	}

	Dart_EnterScope();
	cstring cname;
	handleError(Dart_StringToCString(name, &cname));

	Dart_NativeFunction result = NULL;
	for (int i = 0; functionsList[i].name != NULL; ++i) {
		if (strcmp(functionsList[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = functionsList[i].nativeFunction;
			break;
		}
	}

	if (result != NULL) {
		Dart_ExitScope();
		return result;
	}

	Dart_ExitScope();
	return result;
}


void get_receive_port(Dart_NativeArguments arguments) {
	Dart_EnterScope();
	Dart_SetReturnValue(arguments, Dart_Null());

	if (_receivePort == ILLEGAL_PORT) {
		_receivePort = Dart_NewNativePort(RECEIVE_PORT_NAME, messageHandler, true);
	}

	if (_receivePort != ILLEGAL_PORT) {
		Dart_Handle sendPort = Dart_NewSendPort(_receivePort);
		Dart_SetReturnValue(arguments, sendPort);
	}

	Dart_ExitScope();
}

void messageHandler(Dart_Port receivePort, Dart_CObject* message) {
	Dart_Port reply_port_id = ILLEGAL_PORT;

	if( message->type != Dart_CObject_kArray || message->value.as_array.length < 2)
	{
		//TODO ERRO
		printf("messageHandler: message->type=%d \n", message->type);
	}

	Dart_CObject* paramReplyPortId = message->value.as_array.values[0];
	Dart_CObject* paramFunctionName = message->value.as_array.values[1];

	Dart_Port replyPortId = paramReplyPortId->value.as_send_port.id;
	const cstring functionName = paramFunctionName->value.as_string;

	Dart_CObject result;
	for( int i = 0; wrappersFunctionsList[i].name != NULL; ++i ) {
		if( strcmp(wrappersFunctionsList[i].name, functionName) == 0 ) {
			wrappersFunctionsList[i].nativeFunction(message, &result);
			break;
		}
	}
	Dart_PostCObject(replyPortId, &result);
}

