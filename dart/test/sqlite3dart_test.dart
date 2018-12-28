import "package:test/test.dart";
import 'dart:typed_data';
import 'dart:async';
import 'dart:isolate';
import 'package:lamkr/sqlite3dart/sqlite3dart.dart';

void main() async
{
    setUpAll(() {
	});

	group("some_Group_Name", () 
	{	
	   test("sqlite_open", () async { 
	     int handler = await sqlite3_open('./database.db')
		 expect(handler, isNotNUll); 
	   });  

	   test("sqlite_open with invalid path", () { 
		 expect(() async => await sqlite3_open('./database.db'), throwsA(new isInstanceOf<SqliteException>()));
	   }); 
	})
}

Future run2() async {
  var completer = new Completer();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'oi';
  args[2] = 'Luciano';
  _servicePort.send(args);
  replyPort.handler = (result) {
	print('(result)=$result ${result.runtimeType}');
    replyPort.close();
  };
  return completer.future;
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
    _port = get_receive_port();
  }
  return _port;
}

