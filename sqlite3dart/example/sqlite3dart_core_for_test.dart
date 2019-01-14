/// SQLite3 for Dart
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details.
/// All rights reserved. Use of this source code is governed by a MIT-style
/// license that can be found in the LICENSE file.

import 'dart:async';
import 'dart:io';

FutureOr test(String name, Function functionTest ) async {
  await functionTest();
  print("'$name' test successful!\n");
}

void Assert(bool isValid, [String message='']) {
  if( !isValid )
    throw Exception('Condition is not valid. $message');
}

bool fileExists(String filepath) =>
    new File(filepath).existsSync();

void deleteFile(String filepath) {
  bool notDeleted = false;
  do {
    try {
      new File(filepath).deleteSync();
    }
    catch(e) {
      if( ! e.toString().contains('The system cannot find the file') )
        notDeleted = true;
    }
  } while(notDeleted);
}

Future<FileSystemEntity> deleteFileAsync(String filepath) async {
  return new File(filepath).delete();
}

/*
https://www.burkharts.net/apps/blog/fundamentals-of-dart-streams/
https://medium.com/flutter-community/reactive-programming-streams-bloc-6f0d2bd2d248
	*/