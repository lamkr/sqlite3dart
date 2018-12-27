library sqlite3dart;

import 'dart-ext:C:/projects/lamkr/sqlite3dart/build/Release/sqlite3dart_extension.dll';
//import 'dart-ext:../build/Release/sqlite3dart_extension';

void throwError() native "throwError_";
int sqlite3_threadsafe() native "sqlite3_threadsafe_";
int sqlite3_libversion_number() native "sqlite3_libversion_number_";
int sqlite3_close(int db) native "sqlite3_close_";
int sqlite3_open(String path) native "sqlite3_open_";
int sqlite3_exec(int db, String sql) native "sqlite3_exec_";

//Future<int> sqlite3_open_async(String path) native "sqlite3_open_async_";
SendPort sqlite3_open_async() native "sqlite3_open_async_";
