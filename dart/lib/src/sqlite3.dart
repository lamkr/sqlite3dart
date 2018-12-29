/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';
import 'dart:isolate';

import 'repository_core.dart';
import 'SqliteException.dart';

///
/// Open a new database connection.
/// {@see https://sqlite.org/c3ref/open.html}
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
      throw new SqliteException(result);
    }
	  completer.complete(result);
  };
  return completer.future;
}


