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
import 'dart:io';
import 'package:sqlite3dart/sqlite3dart.dart';

void main() async
{

  await test("sqlite_open", () async {
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    assert(handler != null);
    await sqlite3_close(handler);
    assert(fileExists(filepath));
    deleteFile(filepath);
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
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    await sqlite3_close(handler);
    assert(fileExists(filepath));
    deleteFile(filepath);
  } );

  /// TODO This test causes crash on native library. It is necessary add C/C++ exception.
  ///await test("sqlite_close with invalid handler", () async {
  /// int invalidHandler = -1;
  ///  try {
  ///    await sqlite3_close(invalidHandler);
  ///  }
  ///  catch(e)
  ///  {}
  ///} );

  await test("sqlite_exec to create table", () async {
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    String sql = 'CREATE TABLE IF NOT EXISTS myTable (id int, name text)';
    await sqlite3_exec(handler, sql);
    for( int i = 0; i < 10; i++ ) {
      sql = "insert into myTable values ($i, 'luciano $i')";
      await sqlite3_exec(handler, sql);
    }
    sql = "select * from myTable";
    sqlite3_exec(handler, sql).listen(
      (result) {
        print('result: ');
        print(result);
      },
      onDone: () async {
        print('ondeone');
        await sqlite3_close(handler);
        deleteFile(filepath);
      },
      onError: (e) => print('oi $e')
    );
  } );

  /*await test("sqlite_exec to insert row in table", () async {
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    String sql = 'CREATE TABLE IF NOT EXISTS myTable (id int, name text)';
    await sqlite3_exec(handler, sql);
    for( int i = 0; i < 10; i++ ) {
      sql = "insert into myTable values ($i, 'luciano $i')";
      await sqlite3_exec(handler, sql);
    }
    sql = "select * from myTable";
    sqlite3_exec(handler, sql)
        .listen((result)
    => print(result))
        .onDone(() async {
      await sqlite3_close(handler);
      deleteFile(filepath);
    } );
  } );*/

  /// TODO lista tabelas: SELECT name FROM my_db.sqlite_master WHERE type='table';

}

FutureOr test(String name, Function functionTest ) async {
  await functionTest();
  print('Test \'$name\' successfully.\n');
}

bool fileExists(String filepath) =>
  new File(filepath).existsSync();

void deleteFile(String filepath) =>
  new File(filepath).deleteSync();
