/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';

import 'SqliteRow.dart';

import 'SqliteException.dart';

bool isException(String message) =>
  message.startsWith('Exception:');

void completeIfException(Completer completer, String message) {
  if( isException(message) )
    completer.completeError(new SqliteException(message.substring(10)));
}

SqliteRow parseExecRow( String execRow ) {
  if( isEmptyOrNull(execRow) )
    return SqliteRow.empty;
  // Parse row id.
  int startIndex = execRow.indexOf(",");
  int rowId = int.parse(execRow.substring(0, startIndex));
  SqliteRow row = new SqliteRow(rowId);

  ++startIndex;
  while( startIndex < (execRow.length-1) ) {
    int equalIndex = execRow.indexOf("=", startIndex);
    String name = _parseName(execRow, startIndex, equalIndex);
    int size = _parseSize(execRow, equalIndex);
    String value = _parseValue(execRow, size, equalIndex);
    //print('$name, $size, $value');
    int nextColIndex = _nextColIndex(execRow, size, equalIndex);
    startIndex = nextColIndex;
    row.addColumn(name, value);
  }
  return row;
}

bool isEmptyOrNull(String str) => str == null || str.length == 0;

String _parseName(String execRow, int startIndex, int equalIndex) {
  return execRow.substring(startIndex, equalIndex);
}

int _parseSize(String execRow, int equalIndex ) {
  int ddotIndex = execRow.indexOf(":", equalIndex);
  return int.parse(execRow.substring(equalIndex+1, ddotIndex));
}

String _parseValue(String execRow, int size, int equalIndex) {
  int startIndex = execRow.indexOf(":", equalIndex) + 1;
  return execRow.substring(startIndex, startIndex+size);
}

int _nextColIndex(String execRow, int size, int equalIndex) {
  int nextColIndex = execRow.indexOf(":", equalIndex)+1;
  return nextColIndex + size + 1;
}

