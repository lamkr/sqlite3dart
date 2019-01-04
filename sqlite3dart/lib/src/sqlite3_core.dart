/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:async';

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
    row.addColumn(name, SqliteText(), size, value);
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

class SqliteDataType
{
  final int id;
  SqliteDataType(this.id);
}

class SqliteInteger extends SqliteDataType {
  final String name = 'SQLITE_INTEGER';
  SqliteInteger() : super(1);
}

class SqliteFloat extends SqliteDataType {
  final String name = 'SQLITE_FLOAT';
  SqliteFloat() : super(2);
}

class SqliteText extends SqliteDataType {
  final String name = 'SQLITE_TEXT';
  SqliteText() : super(3);
}

class SqliteBlob extends SqliteDataType {
  final String name = 'SQLITE_BLOB';
  SqliteBlob() : super(4);
}

class SqliteNull extends SqliteDataType {
  final String name = 'SQLITE_NULL';
  SqliteNull() : super(5);

  @override
  String toString() => name;
}

class SqliteColumn
{
  final String name;
  final SqliteDataType type;
  final int size;
  final dynamic value;

  SqliteColumn(this.name, this.type, this.size, this.value);

  @override
  String toString() => '$name($type, $size)=$value';
}

class SqliteRow
{
  static SqliteRow get empty => new SqliteRow(-1);

  final int id;
  final Map<String, SqliteColumn> columns;

  SqliteRow(this.id) : columns = {};

  SqliteColumn addColumn(String name, SqliteDataType type, int size, dynamic value) {
    return columns[name] = new SqliteColumn(name, type, size, value);
  }
}
