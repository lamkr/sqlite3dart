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
  test("sqlite_open", () async {
    int handler = await sqlite3_open('./database.db');
    assert(handler != null);
    print('handler=$handler');
  });

  test("sqlite_open with invalid path", () async {
    try {
      await sqlite3_open('stranger path! ./database.db');
      assert(true==false, "Would be throwed SqliteException");
    }
    catch(e)
    {}
  } );
}

FutureOr test(String name, Function functionTest ) async {
  await functionTest();
  print('Test \'$name\' successfully.\n');
}

