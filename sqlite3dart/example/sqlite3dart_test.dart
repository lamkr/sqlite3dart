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
import 'sqlite3dart_core_for_test.dart';
import 'package:sqlite3dart/sqlite3dart.dart';
import 'package:sqlite3dart/src/SqliteBoolean.dart';
import 'package:sqlite3dart/src/SqliteRow.dart';

const int ROWCOUNT = 10;

void main() async
{
  test("test boolean data type", () {
    Assert( new SqliteBoolean(true).value );
    Assert( new SqliteBoolean(false).value == false );
    Assert( new SqliteBoolean(null).value == null );
    Assert( new SqliteBoolean(null).isNull );
  });

  await test("sqlite_open", () async {
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    assert(handler != null);
    await sqlite3_close(handler);
    Assert(fileExists(filepath));
    deleteFile(filepath);
  });

  await test("sqlite_open with invalid path", () async {
    int handler = 0;
    try {
      handler = await sqlite3_open('??stranger path?? ./database.db');
      await sqlite3_close(handler);
      Assert(true==false, "Would be throwed SqliteException");
    }
    catch(e)
    {}
  } );

  await test("sqlite_close", () async {
    String filepath = './database.db';
    int handler = await sqlite3_open(filepath);
    await sqlite3_close(handler);
    Assert(fileExists(filepath));
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

  await test("sqlite_exec: creates table", () async {
    String filepath = './database.db', tableName = 'myTable';
    int handler = await sqlite3_open(filepath);
    String sql = 'create table $tableName (id int, name text)';
    await sqlite3_exec(handler, sql);
    bool tableExists = await sqlite3_table_exists(handler, tableName);
    Assert( tableExists );
    await sqlite3_close(handler);
    deleteFile(filepath);
  } );

  const int ROWCOUNT = 10;

  await test("sqlite_exec: inserts $ROWCOUNT rows in table", () async {
    Stream stream;
    String sql;
    String filepath = './database.db', tableName = 'myTable';

    int handler = await sqlite3_open(filepath);

    sql = 'create table $tableName (id int, name text)';
    await sqlite3_exec(handler, sql).drain();
    /*
    stream = sqlite3_exec(handler, sql);
    await for (var result in stream)
    {}*/

    sql = "";
    for( int i = 0; i < ROWCOUNT; i++ ) {
      sql += "insert into myTable values ($i, 'test_6: It is the row $i');";
    }
    await sqlite3_exec(handler, sql).drain();
    /*
    stream = sqlite3_exec(handler, sql);
    await for (var result in stream)
    {}*/

    sql = "select * from myTable";
    int rowCount = 0;
    stream = sqlite3_exec(handler, sql);
    await for (SqliteRow sqliteRow in stream) {
      rowCount++;
    }

    await sqlite3_close(handler);

    deleteFile(filepath);
    Assert(rowCount == ROWCOUNT);
  } );


  await test("sqlite_exec: check count(*) == 10 inserted rows in table", () async {
    Stream stream;
    String sql;
    String filepath = './database.db', tableName = 'myTable';

    int handler = await sqlite3_open(filepath);

    sql = 'create table $tableName (id int, name text)';
    await sqlite3_exec(handler, sql).drain();
    /*stream = sqlite3_exec(handler, sql);
    await for (var result in stream) {
      print('test_7 sqlite3_exec: create table= $result');
    }*/

    sql = "";
    for( int i = 0; i < ROWCOUNT; i++ ) {
      sql += "insert into myTable values ($i, 'test_6: It is the row $i');";
    }
    await sqlite3_exec(handler, sql).drain();
    /*stream = sqlite3_exec(handler, sql);
    await for (var result in stream) {
      print('test_7 sqlite3_exec: inserts= $result');
    }*/

    int rowCount = 0;
    sql = "select count(*) from myTable";
    stream = sqlite3_exec(handler, sql);
    await for (SqliteRow sqliteRow in stream) {
      rowCount = int.parse( sqliteRow['count(*)'].value );
    }

    await sqlite3_close(handler);

    deleteFile(filepath);
    Assert(rowCount == ROWCOUNT);
  } );

}

