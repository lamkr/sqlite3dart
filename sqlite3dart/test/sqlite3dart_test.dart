/// SQLite3 for Dart
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details.
/// All rights reserved. Use of this source code is governed by a MIT-style
/// license that can be found in the LICENSE file.

import 'dart:typed_data';
import 'dart:async';
import 'dart:io';
import 'dart:isolate';
import 'package:sqlite3dart/sqlite3dart.dart';
import "package:path/path.dart";
import "package:test/test.dart";

void main() async
{
  test("sqlite_open", () async {
    int handler = await sqlite3_open('./database.db');
    expect(handler, isNotNull);
  });

  test("sqlite_open with invalid path", () {
    expect(() async => await sqlite3_open('./database.db'),
        //throwsA(new isInstanceOf<SqliteException>()));
        throwsA(const TypeMatcher<SqliteException>()));
  });
}

