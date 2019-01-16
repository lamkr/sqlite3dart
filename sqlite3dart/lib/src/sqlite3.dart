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
Future<void> sqlite3_close(int handle) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'sqlite3_close_wrapper';
  args[2] = handle;
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
Stream<SqliteRow> sqlite3_exec(int handle, String sql) {
  var controller = StreamController<SqliteRow>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_exec_wrapper');
  args.insert(2, handle);
  args.insert(3, sql);
  get_receive_port().send(args);
  replyPort.handler = (result) {
    if(result is String) {
      if(isException(result)) {
        replyPort.close();
        controller.addError(new SqliteException(result.substring(10)));
        return;
      }
      SqliteRow row = parseExecRow(result);
      controller.add(row);
    }
    else if( result == null ) {
      replyPort.close();
      controller.close();
    }
  };
  return controller.stream;
}

///
/// Convenient method to check if table exists.
///
Future<bool> sqlite3_table_exists(int handle, String tableName) {
  var completer = new Completer<bool>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_table_exists');
  args.insert(2, handle);
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

///
/// TODO
///
Future<int> sqlite3_prepare_v2(int handle, String sqlStatement) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_prepare_v2_wrapper');
  args.insert(2, handle);
  args.insert(3, sqlStatement );
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
/// TODO
///
Future<int> sqlite3_step(int statement) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_step_wrapper');
  args.insert(2, statement);
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
/// TODO
///
Future<int> sqlite3_reset(int statement) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_reset_wrapper');
  args.insert(2, statement);
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
/// TODO
///
Future<void> sqlite3_finalize(int statement) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_finalize_wrapper');
  args.insert(2, statement);
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
/// TODO
///
Future<void> sqlite3_bind_int(int statement, int index, int value) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_bind_int_wrapper');
  args.insert(2, statement);
  args.insert(3, index);
  args.insert(4, value);
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
/// TODO
///
Future<void> sqlite3_bind_int64(int statement, int index, int value) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_bind_int64_wrapper');
  args.insert(2, statement);
  args.insert(3, index);
  args.insert(4, value);
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
/// TODO
///
Future<void> sqlite3_bind_double(int statement, int index, double value) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_bind_double_wrapper');
  args.insert(2, statement);
  args.insert(3, index);
  args.insert(4, value);
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
/// TODO
///
Future<void> sqlite3_bind_text(int statement, int index, String value) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_bind_text_wrapper');
  args.insert(2, statement);
  args.insert(3, index);
  args.insert(4, value);
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
/// TODO
///
Future<String> sqlite3_column_text(int statement, int index) {
  var completer = new Completer<String>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_column_text_wrapper');
  args.insert(2, statement);
  args.insert(3, index);
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
    completer.complete(result);
  };
  return completer.future;
}
