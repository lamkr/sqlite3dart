/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';
import 'dart:isolate';

//import 'repository_core.dart';
import 'package:sqlite3dart/sqlite3dart.dart';

import 'SqliteException.dart';

///
/// Open a new database connection.
/// See [https://sqlite.org/c3ref/open.html]
///
Future<int> sqlite3_open(String filename) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'sqlite3_open_wrapper';
  args[2] = filename;
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
    if( result is String ) {
      completer.completeError(new SqliteException(result));
    }
    else
      completer.complete(result);
  };
  return completer.future;
}

///
/// Close a database connection.
/// See [https://sqlite.org/c3ref/close.html]
///
Future<void> sqlite3_close(int handler) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'sqlite3_close_wrapper';
  args[2] = handler;
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
    if( result is String ) {
      completer.completeError(new SqliteException(result));
    }
    else
      completer.complete(result);
  };
  return completer.future;
}

///
/// One-step query execution method.
/// See [https://sqlite.org/c3ref/exec.html]
///
Future<SqliteRow> sqlite3_exec(int handler, String sql) {
  var completer = new Completer<SqliteRow>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_exec_wrapper');
  args.insert(2, handler);
  args.insert(3, sql);
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
    if( result is String ) {
      completer.completeError(new SqliteException(result));
    }
    else
      completer.complete(result);
  };
  return completer.future;
}


class SqliteRow
{}
