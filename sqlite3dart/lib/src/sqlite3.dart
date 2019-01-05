/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';
import 'dart:io';
import 'dart:isolate';

//import 'repository_core.dart';
import 'package:sqlite3dart/sqlite3dart.dart';
import 'SqliteRow.dart';

import 'SqliteException.dart';
import 'sqlite3_core.dart';

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
      completeIfException(completer, result);
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
      completeIfException(completer, result);
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
Stream<SqliteRow> sqlite3_exec(int handler, String sql) {
  var controller = StreamController<SqliteRow>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_exec_wrapper');
  args.insert(2, handler);
  args.insert(3, sql);
  get_receive_port().send(args);
  replyPort.handler = (result) {
    if( result is int ) {
      //stdout.write('\nlinha $result: ');
    }
    else if(result is String) {
      if(isException(result)) {
        replyPort.close();
        controller.addError(new SqliteException(result.substring(10)));
        return;
      }
      //stdout.write('$result');
      SqliteRow row = parseExecRow(result);
      controller.add(row);
    }
    else if( result == null ) {
      //controller.add(str);
      replyPort.close();
      controller.close();
    }
  };
  return controller.stream;
}

Future<bool> sqlite3_table_exists(int handler, String tableName) {
  var completer = new Completer<bool>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_table_exists');
  args.insert(2, handler);
  args.insert(3, tableName );
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
    if( result is String ) {
      completeIfException(completer, result);
    }
    else
      completer.complete(result);
  };
  return completer.future;
}

