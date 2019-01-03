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

#define QUOTEME(n) #n

const cstring ERR_INVALID_HANDLE = "Invalid database handle.";

extern DynamicPointer* _dynpointers;

typedef struct TCallbackParameter {
	Dart_Port replyPortId;
	pointer data;
} CallbackParameter;

bool isInvalidHandle(int64_t address) {
	/*
	  TODO This do not works correctly.
	  
	int64_t address32 = (int32_t)address;
	return address32 < 1;
	*/
	return false;
}

void mountException(Dart_CObject* result, const cstring message) {
	new_dynp(&_dynpointers[0], strlen("Exception:") + strlen(message) + 1);
	sprintf_s(_dynpointers[0].pointer, _dynpointers[0].size, "Exception:%s", message);
	result->type = Dart_CObject_kString;
	result->value.as_string = _dynpointers[0].pointer;
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
	}
	else {
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

int _execRowIndex;
int sqlite3_exec_callback(pointer parameter, int argc, cstring *argv, cstring *column) {
	CallbackParameter *param = (CallbackParameter*)parameter;
	Dart_CObject rowIndex, dataColumn, row;
	rowIndex.type = Dart_CObject_kInt64;
	rowIndex.value.as_int64 = ++_execRowIndex;
	Dart_PostCObject( param->replyPortId, &rowIndex );
	uint64_t rowLength = 0, columnNameLength, columnValueLength;
	char strSize[32]; 
	memset(_dynpointers[0].pointer, 0, _dynpointers[0].size);
	for( int colIndex = 0; colIndex < argc; colIndex++ ) {
		columnNameLength = strlen(column[colIndex]);
		columnValueLength = strlen(argv[colIndex]);
		size_t sizeLength = sprintf_s(strSize, sizeof(strSize), "%llu:", columnValueLength);
		rowLength += columnNameLength + 1 + sizeLength + 1 + columnValueLength +1 + 1;
		new_dynp( &_dynpointers[0], rowLength );
		strcat(_dynpointers[0].pointer, column[colIndex]);
		strcat(_dynpointers[0].pointer, "=");
		strcat(_dynpointers[0].pointer, strSize);
		strcat(_dynpointers[0].pointer, ":");
		strcat(_dynpointers[0].pointer, argv[colIndex]);
		strcat(_dynpointers[0].pointer, ",");
		dataColumn.type = Dart_CObject_kString;
		dataColumn.value.as_string = _dynpointers[0].pointer;
		puts(_dynpointers[0].pointer);
	}
	row.type = Dart_CObject_kString;
	Dart_PostCObject(param->replyPortId, &row);
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
	CallbackParameter parameter = { replyPortId, NULL};
	int error = sqlite3_exec(db, sql, sqlite3_exec_callback, &parameter, &zErrMsg);
	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		mountException(result, zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

int sqlite3_table_exists_callback(pointer parameter, int argc, cstring *argv, cstring *column) {
	CallbackParameter *param = (CallbackParameter*)parameter;
	const cstring tableName = (const cstring) param->data;
	Dart_CObject result;
	result.type = Dart_CObject_kBool;
	result.value.as_bool = false;
	if( argc == 1 ) {
		result.value.as_bool = strcmp(argv[0], tableName) == 0;
	}
	Dart_PostCObject(param->replyPortId, &result);
	return 0;
}

void sqlite3_table_exists(Dart_CObject* message, Dart_CObject* result) {
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
	const cstring tableName = (const cstring)param3->value.as_string;

	sprintf_s(_dynpointers[0].pointer, _dynpointers[0].size, "select name from sqlite_master where type='table' and name='%s'", tableName);

	cstring zErrMsg = NULL;
	CallbackParameter parameter = { replyPortId, tableName };
	int error = sqlite3_exec(db, _dynpointers[0].pointer, sqlite3_table_exists_callback, &parameter, &zErrMsg);
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
	{ "sqlite3_table_exists", sqlite3_table_exists},
	{ NULL, NULL }
};

