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
  /*test("sqlite_open", () async {
    int handler = await sqlite3_open('./database.db');
    assert(handler != null);
    print('handler=$handler');
    await sqlite3_close(handler);
  });

  test("sqlite_open with invalid path", () async {
    int handler = 0;
    try {
      handler = await sqlite3_open('??stranger path?? ./database.db');
      await sqlite3_close(handler);
      assert(true==false, "Would be throwed SqliteException");
    }
    catch(e)
    {}
  } );*/

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
  } );
}

FutureOr test(String name, Function functionTest ) async {
  await functionTest();
  print('Test \'$name\' successfully.\n');
}

