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

extern DynamicPointer _dynpointer;

bool isInvalidHandle(int64_t address) {
	/*
	  TODO This do not works correctly.
	  
	int64_t address32 = (int32_t)address;
	return address32 < 1;
	*/
	return false;
}

void mountException(Dart_CObject* result, const cstring message) {
	new_dynp(&_dynpointer, strlen("Exception:") + strlen(message) + 1);
	sprintf_s(_dynpointer.pointer, _dynpointer.size, "Exception:%s", message);
	result->type = Dart_CObject_kString;
	result->value.as_string = _dynpointer.pointer;
}

void sqlite3_open_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[2];
	const cstring filename = param->value.as_string;

	sqlite3 *db;
	int error = sqlite3_open(filename, &db);
	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kInt64;
		result->value.as_int64 = (int64_t) db;
	}
	else {
		mountException(result, (const cstring)sqlite3_errstr(error));
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
	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
		del_dynp(&_dynpointer);
	}
	else {
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

int _execRowIndex;
int execCallback(pointer parameter, int argc, cstring *argv, cstring *column) {
	Dart_Port replyPortId = *(Dart_Port*) parameter;
	Dart_CObject rowIndex, dataColumn;
	rowIndex.type = Dart_CObject_kInt64;
	rowIndex.value.as_int64 = ++_execRowIndex;
	Dart_PostCObject( replyPortId, &rowIndex );
	for( int colIndex = 0; colIndex < argc; colIndex++ ) {
		size_t length = strlen(column[colIndex]) + 1 + strlen(argv[colIndex]) + 1;
		new_dynp( &_dynpointer, length );
		sprintf_s(_dynpointer.pointer, _dynpointer.size, "%s=%s", column[colIndex], argv[colIndex]);
		dataColumn.type = Dart_CObject_kString;
		dataColumn.value.as_string = _dynpointer.pointer;
		Dart_PostCObject(replyPortId, &dataColumn);
	}
	return 0;
}

void sqlite3_exec_wrapper(Dart_CObject* message, Dart_CObject* result) {
	_execRowIndex = -1;

	Dart_CObject* paramReplyPortId = message->value.as_array.values[0];
	Dart_Port replyPortId = paramReplyPortId->value.as_send_port.id;

	Dart_CObject* param2 = message->value.as_array.values[2];
	int64_t address = param2->value.as_int64;
	if (isInvalidHandle(address)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}

	sqlite3 *db = (sqlite3 *)address;

	Dart_CObject* param3 = message->value.as_array.values[3];
	const cstring sql = (const cstring)param3->value.as_string;

	cstring zErrMsg = NULL;
	int error = sqlite3_exec(db, sql, execCallback, &replyPortId, &zErrMsg);
	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		mountException(result, zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

const WrapperFunction wrappersFunctionsList[] = {
	{ "sqlite3_open_wrapper", sqlite3_open_wrapper},
	{ "sqlite3_close_wrapper", sqlite3_close_wrapper},
	{ "sqlite3_exec_wrapper", sqlite3_exec_wrapper},
	{ NULL, NULL }
};

