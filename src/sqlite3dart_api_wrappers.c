// Copyright (c) 2018 Luciano Rodrigues (Brodi).
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
#include <inttypes.h>
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
	snprintf(_dynpointers[0].pointer, _dynpointers[0].size, "Exception:%s", message);
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

uint64_t _execRowIndex;
int sqlite3_exec_callback2(pointer parameter, int argc, cstring *argv, cstring *column) {
	CallbackParameter *param = (CallbackParameter*)parameter;

	uint64_t columnNameLength, columnValueLength, columnValueSizeLength;

	uint64_t rowLength = sizeOfInteger(_execRowIndex) + 1;
	for (int colIndex = 0; colIndex < argc; colIndex++) {
		columnNameLength = strlen(column[colIndex]);
		columnValueLength = strlen(argv[colIndex]);
		columnValueSizeLength = sizeOfInteger(columnValueLength);
		rowLength += columnNameLength + 1 + columnValueSizeLength + 1 + columnValueLength + 1;
	}

	new_dynp(&_dynpointers[0], rowLength);
	memset(_dynpointers[0].pointer, 0, _dynpointers[0].size);

	// Start string with index of the line.
	++_execRowIndex;
	snprintf(_dynpointers[0].pointer, _dynpointers[0].size, "%" PRIu64, _execRowIndex);

	char strSize[32];
	for (int colIndex = 0; colIndex < argc; colIndex++) {
		columnValueLength = strlen(argv[colIndex]);
		snprintf(strSize, sizeof(strSize), "%" PRIu64, columnValueLength);
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, column[colIndex]);
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, "=");
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, strSize);
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, ":");
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, argv[colIndex]);
		strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, ",");
	}
	strcat_s(_dynpointers[0].pointer, _dynpointers[0].size, "\n");

	Dart_CObject row;
	row.type = Dart_CObject_kString;
	row.value.as_string = _dynpointers[0].pointer;
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
	int error = sqlite3_exec(db, sql, sqlite3_exec_callback2, &parameter, &zErrMsg);
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
	int found = 0;
	if( argc == 1 ) {
		result.value.as_bool = strcmp(argv[0], tableName) == 0;
		found = 1;
	}
	Dart_PostCObject(param->replyPortId, &result);
	return found;
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

	snprintf(_dynpointers[0].pointer, _dynpointers[0].size, "select name from sqlite_master where type = 'table'");

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

void sqlite3_prepare_v2_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* paramReplyPortId = message->value.as_array.values[0];
	Dart_Port replyPortId = paramReplyPortId->value.as_send_port.id;

	Dart_CObject* paramHandle= message->value.as_array.values[2];
	int64_t handle = paramHandle->value.as_int64;
	if (isInvalidHandle(handle)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}

	sqlite3 *db = (sqlite3 *)handle;

	Dart_CObject* paramSqlStatement = message->value.as_array.values[3];
	const cstring sqlStatement = (const cstring)paramSqlStatement->value.as_string;
	
	int maxLength = -1;
	sqlite3_stmt *statementHandle = NULL;
	const char* unusedSqlStatement = NULL;
	int error = sqlite3_prepare_v2(db, sqlStatement, maxLength, &statementHandle, &unusedSqlStatement);
	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kInt64;
		result->value.as_int64 = (int64_t)statementHandle;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_step_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;

	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	int error = sqlite3_step(statementHandle);

	if (error == SQLITE_DONE) {
		result->type = Dart_CObject_kNull;
	}
	else if( error == SQLITE_ERROR ) {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
	else {
		result->type = Dart_CObject_kInt32;
		result->value.as_int32 = (int32_t)error;
	}
}

void sqlite3_reset_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;

	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	int error = sqlite3_reset(statementHandle);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else  {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_finalize_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* paramReplyPortId = message->value.as_array.values[0];
	Dart_Port replyPortId = paramReplyPortId->value.as_send_port.id;

	Dart_CObject* paramStatement = message->value.as_array.values[2];
	int64_t statement = paramStatement->value.as_int64;

	sqlite3_stmt *statementHandle = (sqlite3_stmt *) statement;

	int error = sqlite3_finalize(statementHandle);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_bind_int_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;
	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	param = message->value.as_array.values[3];
	int32_t index = param->value.as_int32;

	param = message->value.as_array.values[4];
	int32_t value = param->value.as_int32;

	int error = sqlite3_bind_int(statementHandle, index, value);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_bind_int64_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;
	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	param = message->value.as_array.values[3];
	int32_t index = param->value.as_int32;

	param = message->value.as_array.values[4];
	int64_t value = param->value.as_int64;

	int error = sqlite3_bind_int64(statementHandle, index, value);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_bind_double_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;
	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	param = message->value.as_array.values[3];
	int32_t index = param->value.as_int32;

	param = message->value.as_array.values[4];
	double value = param->value.as_double;

	int error = sqlite3_bind_double(statementHandle, index, value);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_bind_text_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;
	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	param = message->value.as_array.values[3];
	int32_t index = param->value.as_int32;

	param = message->value.as_array.values[4];
	const cstring value = (const cstring)param->value.as_string;

	int error = sqlite3_bind_text(statementHandle, index, value, -1, NULL);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		//mountException(result, (const cstring)sqlite3_errmsg(db));
		mountException(result, (const cstring)sqlite3_errstr(error));
	}
}

void sqlite3_column_text_wrapper(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t statement = param->value.as_int64;
	sqlite3_stmt *statementHandle = (sqlite3_stmt *)statement;

	param = message->value.as_array.values[3];
	int32_t index = param->value.as_int32;
	
	const unsigned char* value = sqlite3_column_text(statementHandle, index);

	result->type = Dart_CObject_kString;
	result->value.as_string = (char*) value;
}

void sqlite3_begin_transaction(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t address = param->value.as_int64;
	if (isInvalidHandle(address)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}
	sqlite3 *db = (sqlite3 *)address;

	param = message->value.as_array.values[3];
	const char* transactionType = param->value.as_string;

	cstring zErrMsg = NULL;
	char sql[64];
	snprintf(sql, sizeof(sql), "begin %s transaction", transactionType);

	int error = sqlite3_exec(db, sql, NULL, NULL, NULL);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		mountException(result, zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

void sqlite3_commit_transaction(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t address = param->value.as_int64;
	if (isInvalidHandle(address)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}
	sqlite3 *db = (sqlite3 *)address;

	int error = sqlite3_exec(db, "commit", NULL, NULL, NULL);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		cstring zErrMsg = NULL;
		mountException(result, zErrMsg);
		sqlite3_free(zErrMsg);
	}
}


// TODO missing parameters. See https://www.sqlite.org/lang_transaction.html
void sqlite3_rollback_transaction(Dart_CObject* message, Dart_CObject* result) {
	Dart_CObject* param = message->value.as_array.values[0];
	Dart_Port replyPortId = param->value.as_send_port.id;

	param = message->value.as_array.values[2];
	int64_t address = param->value.as_int64;
	if (isInvalidHandle(address)) {
		mountException(result, ERR_INVALID_HANDLE);
		return;
	}
	sqlite3 *db = (sqlite3 *)address;

	int error = sqlite3_exec(db, "rollback transaction", NULL, NULL, NULL);

	if (error == SQLITE_OK) {
		result->type = Dart_CObject_kNull;
	}
	else {
		cstring zErrMsg = NULL;
		mountException(result, zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

const WrapperFunction wrappersFunctionsList[] = {
	{ "sqlite3_open_wrapper", sqlite3_open_wrapper},
	{ "sqlite3_close_wrapper", sqlite3_close_wrapper},
	{ "sqlite3_exec_wrapper", sqlite3_exec_wrapper},
	{ "sqlite3_table_exists", sqlite3_table_exists},
	{ "sqlite3_prepare_v2_wrapper", sqlite3_prepare_v2_wrapper},
	{ "sqlite3_step_wrapper", sqlite3_step_wrapper },
	{ "sqlite3_reset_wrapper", sqlite3_reset_wrapper },
	{ "sqlite3_finalize_wrapper", sqlite3_finalize_wrapper },
	{ "sqlite3_bind_int_wrapper", sqlite3_bind_int_wrapper },
	{ "sqlite3_bind_int64_wrapper", sqlite3_bind_int64_wrapper },
	{ "sqlite3_bind_double_wrapper", sqlite3_bind_double_wrapper },
	{ "sqlite3_bind_text_wrapper", sqlite3_bind_text_wrapper },
	{ "sqlite3_column_text_wrapper", sqlite3_column_text_wrapper },
	{ "sqlite3_begin_transaction", sqlite3_begin_transaction },
	{ "sqlite3_commit_transaction", sqlite3_commit_transaction },
	{ "sqlite3_rollback_transaction", sqlite3_rollback_transaction },
	{ NULL, NULL }
};

