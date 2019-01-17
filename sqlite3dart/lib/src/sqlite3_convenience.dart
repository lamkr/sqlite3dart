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

enum SqliteTransactionType {
	Deferred, Immediate, Exclusive
}

///
/// Convenient method to begin a transaction.
///
Future<bool> sqlite3_begin_transaction(int handle, [SqliteTransactionType transactionType]) {
  var completer = new Completer<bool>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_begin_transaction');
  args.insert(2, handle);
  args.insert(3, transactionType == null ? '' : transactionType.toString().split('.').last);
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
/// Convenient method to commit a transaction.
///
Future<bool> sqlite3_commit_transaction(int handle) {
  var completer = new Completer<bool>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_commit_transaction');
  args.insert(2, handle);
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
/// Convenient method to rollback a transaction.
///
Future<bool> sqlite3_rollback_transaction(int handle) {
  var completer = new Completer<bool>();
  var replyPort = new RawReceivePort();
  var args = new List();
  args.insert(0, replyPort.sendPort);
  args.insert(1, 'sqlite3_rollback_transaction');
  args.insert(2, handle);
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


