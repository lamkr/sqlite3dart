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

Function noScopeFunctionsList[] = {
	{ NULL, NULL }
};

Dart_NativeFunction resolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

DART_EXPORT Dart_Handle sqlite3dart_extension_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, resolveName, NULL);
	if (Dart_IsError(result_code)) {
		return result_code;
	}

	return Dart_Null();
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
