/// SQLite3 for Dart
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details.
/// All rights reserved. Use of this source code is governed by a MIT-style
/// license that can be found in the LICENSE file.

import 'dart:typed_data';
import 'dart:async';
import 'dart:isolate';
import 'package:sqlite3dart/sqlite3dart.dart';
import 'package:test/test.dart';

void main() async
{
  //test("sqlite_open", () async {
    print('oi s');
    int handler = await sqlite3_open('./database.db');
    assert(handler != null);
    print('handler=$handler');
  //});

  test("sqlite_open with invalid path", () async {
    try {
      int handler = await sqlite3_open('stranger path! ./database.db');
      print('handler=$handler / sem erros');
    }
    catch(e) {
      print(e.toString());
      assert(e);
    }
  } );
}

void test(String name, Function functionTest ) {
  functionTest();
}

