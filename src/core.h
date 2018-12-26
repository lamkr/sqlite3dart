#ifndef core_h
#define core_h

#include "sqlite3.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

Dart_Handle handleError(Dart_Handle handle);

Dart_Handle createException(const char* message);

sqlite3* getSqliteHandle(Dart_NativeArguments arguments);

const char* getString(Dart_NativeArguments arguments, int index);

int Sqlite3Callback(void *a_param, int argc, char **argv, char **column);

#endif
