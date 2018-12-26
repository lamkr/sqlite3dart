library sqlite3dart;

import 'dart-ext:sqlite3dart_extension';

void throwError() native "throwError_";
int sqlite3_threadsafe() native "sqlite3_threadsafe_";
int sqlite3_libversion_number() native "sqlite3_libversion_number_";
int sqlite3_close(int db) native "sqlite3_close_";
int sqlite3_open(String path) native "sqlite3_open_";
int sqlite3_exec(int db, String sql) native "sqlite3_exec_";

