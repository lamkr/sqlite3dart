/// SQLite3 for Dart
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details.
/// All rights reserved. Use of this source code is governed by a MIT-style
/// license that can be found in the LICENSE file.

/// **ATTENTION**
///
/// To run this sample test, execute the command:
/// $ dart -v sqlite3dart_test.dart
///
import 'dart:async';
import 'package:sqlite3dart/sqlite3dart.dart';

void main() async
{
  /*await test("sqlite_open", () async {
    int handler = await sqlite3_open('./database.db');
    assert(handler != null);
    print('handler=$handler');
    await sqlite3_close(handler);
  });

  await test("sqlite_open with invalid path", () async {
    int handler = 0;
    try {
      handler = await sqlite3_open('??stranger path?? ./database.db');
      await sqlite3_close(handler);
      assert(true==false, "Would be throwed SqliteException");
    }
    catch(e)
    {}
  } );

  await test("sqlite_close", () async {
    int handler = await sqlite3_open('./database.db');
    await sqlite3_close(handler);
  } );

  await test("sqlite_close with invalid handler", () async {
    int invalidHandler = -1;
    try {
      await sqlite3_close(invalidHandler);
    }
    catch(e)
    {}
  } );*/

  await test("sqlite_exec (without arguments) to create table", () async {
    int handler = await sqlite3_open('./database.db');
    String sql = 'CREATE TABLE IF NOT EXISTS myTable (id int, name text)';
    await sqlite3_exec(handler, sql);
    sql = "insert into myTable values (0, 'luciano')";
    await sqlite3_exec(handler, sql);
    sql = "select * from myTable";
    await sqlite3_exec(handler, sql);
    await sqlite3_close(handler);
  } );

}

FutureOr test(String name, Function functionTest ) async {
  await functionTest();
  print('Test \'$name\' successfully.\n');
}

