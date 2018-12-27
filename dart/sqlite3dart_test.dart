import 'dart:async';
import 'dart:isolate';
import 'sqlite3dart.dart';

void main() 
{
	await run();
}

Future<int> run() async {
  SendPort port = await sqlite3_open_async('c:/users/f0031997/projects/sqlite3dart/banco.db');
  var completer = new Completer();
  var replyPort = new RawReceivePort();
  var args = 'c:/users/f0031997/projects/sqlite3dart/banco.db';
    args[0] = seed;
    args[1] = length;
    args[2] = replyPort.sendPort;
    _servicePort.send(args);
    replyPort.handler = (result) {
      replyPort.close();
      if (result != null) {
        completer.complete(result);
      } else {
        completer.completeError(new Exception("Random array creation failed"));
      }
    };
    return completer.future;
  print('db=$db');
}