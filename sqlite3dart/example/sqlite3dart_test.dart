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
import 'package:async/async.dart';
import 'package:sqlite3dart/sqlite3dart.dart';
import 'package:sqlite3dart/src/SqliteBoolean.dart';
import 'package:sqlite3dart/src/SqliteRow.dart';

const int ROWCOUNT = 10;

test_6() async {
  String filepath = './database.db', tableName = 'myTable';

  print('test_6 sqlite3_open');
  int handler = await sqlite3_open(filepath);

  print('test_6 sqlite3_exec');
  String sql = 'create table $tableName (id int, name text)';
  StreamQueue<SqliteRow> queue
    = new StreamQueue<SqliteRow>(sqlite3_exec(handler, sql));

  if(await queue.hasNext)
    await queue.next;

  print('test_6 sqlite3_close');
  await sqlite3_close(handler);

  print('test_6 delete:$filepath');
  deleteFile(filepath);

  /*sql = "";
  for( int i = 0; i < ROWCOUNT; i++ ) {
    sql += "insert into myTable values ($i, 'test_6: It is the row $i');";
    //await sqlite3_exec(handler, sql);
  }
  await sqlite3_exec(handler, sql);

  sql = "select * from myTable";
  int rowCount = 0;
  sqlite3_exec(handler, sql)
    .listen((sqliteRow) {
      print(sqliteRow);
      rowCount++;
    })
    .onDone(() async
    {
      print('rowCount=$rowCount');
      await sqlite3_close(handler);
      print('closed');
      deleteFile(filepath);
      Assert(rowCount == ROWCOUNT);
      print("Test 'sqlite_exec: inserts $ROWCOUNT rows in table' successfully");
    } );*/
}

test_7() async {
  String filepath = './database.db', tableName = 'myTable';
  print('test_7 $filepath');
  deleteFile(filepath);

  int handler = await sqlite3_open(filepath);
  print('test_6 sqlite3_open');

  String sql = 'create table $tableName (id int, name text)';
  await sqlite3_exec(handler, sql);
  print('test_6 sqlite3_exec');

  sql = "";
  for( int i = 0; i < ROWCOUNT; i++ ) {
    sql += "insert into myTable values ($i, 'test_7: It is the row $i')";
  }

  StreamQueue<SqliteRow> queue
    = new StreamQueue<SqliteRow>(sqlite3_exec(handler, sql));

  sql = "select count(*) from myTable";
  int rowCount = 0;
  sqlite3_exec(handler, sql)
    .listen((sqliteRow) {
      print('lisyen');
      print(sqliteRow);
      rowCount = sqliteRow['count'].value;
    })
    .onDone(() async
    {
      print('donw');
      await sqlite3_close(handler);
      await deleteFileAsync(filepath);
      Assert(rowCount == ROWCOUNT);
      print("Test 'sqlite_exec: check count(*) == 10 inserted rows in table' successfully");
    } );

}

void main() async
{
  await test_6();
  await test_7();
/*
  await test("test boolean data type", () async {
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
    String filepath = './database.db', tableName = 'myTable';
    int handler = await sqlite3_open(filepath);
    String sql = 'create table $tableName (id int, name text)';
    await sqlite3_exec(handler, sql);

    sql = "";
    for( int i = 0; i < ROWCOUNT; i++ ) {
      sql += "insert into myTable values ($i, 'It is the row $i');";
      //await sqlite3_exec(handler, sql);
    }
    await sqlite3_exec(handler, sql);

    sql = "select * from myTable";
    int rowCount = 0;
    sqlite3_exec(handler, sql)
        .listen((sqliteRow) {
          print(sqliteRow);
          rowCount++;
        })
        .onDone(() async {
          print('rowCount=$rowCount');
          await sqlite3_close(handler);
          print('closed');
          deleteFile(filepath);
          Assert(rowCount == ROWCOUNT);
        } );
  } );


  await test("sqlite_exec: check count(*) == 10 inserted rows in table", () async {
    String filepath = './database.db', tableName = 'myTable';
    print(filepath);
    deleteFile(filepath);
    int handler = await sqlite3_open(filepath);
    String sql = 'create table $tableName (id int, name text)';
    await sqlite3_exec(handler, sql);
    for( int i = 0; i < ROWCOUNT; i++ ) {
      sql = "insert into myTable values ($i, 'It is the row $i')";
      await sqlite3_exec(handler, sql);
    }
    sql = "select count(*) from myTable";
    int rowCount = 0;
    await sqlite3_exec(handler, sql)
        .listen((sqliteRow) {
          print('lisyen');
          print(sqliteRow);
          rowCount = sqliteRow['count'].value;
        })
        .onDone(() async {
          print('donw');
          await sqlite3_close(handler);
          await deleteFileAsync(filepath);
          Assert(rowCount == ROWCOUNT);
      } );
  } );
*/
}

Future test(String name, Function functionTest ) async {
  return functionTest();
}

void Assert(bool isValid, [String message='']) {
  if( !isValid )
    throw Exception('Condition is not valid. $message');
}

bool fileExists(String filepath) =>
  new File(filepath).existsSync();

void deleteFile(String filepath) {
  bool notDeleted = false;
  do {
    try {
      new File(filepath).deleteSync();
    }
    catch(e) {
      if( ! e.toString().contains('The system cannot find the file') )
        notDeleted = true;
    }
  } while(notDeleted);
}

Future<FileSystemEntity> deleteFileAsync(String filepath) async {
  return new File(filepath).delete();
}

	/*
https://www.burkharts.net/apps/blog/fundamentals-of-dart-streams/
https://medium.com/flutter-community/reactive-programming-streams-bloc-6f0d2bd2d248
	*/