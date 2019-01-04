/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';

import 'SqliteRow.dart';
import 'SqliteText.dart';

import 'SqliteException.dart';

bool isException(String message) =>
  message.startsWith('Exception:');

void completeIfException(Completer completer, String message) {
  if( isException(message) )
    completer.completeError(new SqliteException(message.substring(10)));
}

SqliteRow parseRow( String stringRow ) {
  if( isEmptyOrNull(stringRow) )
    return SqliteRow.empty;
  SqliteRow row = new SqliteRow(_parseRowId(stringRow));
  int startIndex = 0;
  while( startIndex < (stringRow.length-1) ) {
    int equalIndex = stringRow.indexOf("=", startIndex);
    String name = _parseName(stringRow, startIndex, equalIndex);
    int size = _parseSize(stringRow, equalIndex);
    String value = _parseValue(stringRow, size, equalIndex);
    //print('$name, $size, $value');
    int nextColIndex = _nextColIndex(stringRow, size, equalIndex);
    startIndex = nextColIndex;
    row.addColumn(name, SqliteText(value, size));
  }
  return row;
}

int _parseRowId(String stringRow) {
  return 0; //TOOD
}

bool isEmptyOrNull(String str) => str == null || str.length == 0;

String _parseName(String stringRow, int startIndex, int equalIndex) {
  return stringRow.substring(startIndex, equalIndex);
}

int _parseSize(String stringRow, int equalIndex ) {
  int ddotIndex = stringRow.indexOf(":", equalIndex);
  return int.parse(stringRow.substring(equalIndex+1, ddotIndex));
}

String _parseValue(String stringRow, int size, int equalIndex) {
  int startIndex = stringRow.indexOf(":", equalIndex) + 1;
  return stringRow.substring(startIndex, startIndex+size);
}

int _nextColIndex(String stringRow, int size, int equalIndex) {
  int nextColIndex = stringRow.indexOf(":", equalIndex)+1;
  return nextColIndex + size + 1;
}

