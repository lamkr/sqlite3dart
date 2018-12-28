import 'dart:typed_data';
import 'dart:async';
import 'dart:isolate';
import 'sqlite3dart.dart';

void main() async
{
	var db = await run();
	print('db=$db');
}

SendPort _port;

Future run() async {
  var completer = new Completer();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'asd asd ./banco.db';
  _servicePort.send(args);
  replyPort.handler = (result) {
	print('(result)=$result ${result.runtimeType}');
	if( result is Uint8List ) {
		throw new Sqlite3Exception(new String.fromCharCodes(result));
	}
    replyPort.close();
    if (result != null) {
      completer.complete(result);
    } 
	else {
      completer.completeError(new Exception("Random array creation failed"));
    }
  };
  return completer.future;
}

SendPort get _servicePort {
  if (_port == null) {
    _port = sqlite3_open_async();
  }
  return _port;
}

class Sqlite3Exception 
implements Exception 
{
	final String message;
	
	Sqlite3Exception(this.message);
	
	String toString() {
		return '$runtimeType: $message';
	}
}