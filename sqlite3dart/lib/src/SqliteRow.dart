import 'dart:typed_data';

import 'SqliteBoolean.dart';

import 'SqliteBlob.dart';

import 'SqliteNull.dart';

import 'SqliteText.dart';

import 'SqliteFloat.dart';

import 'SqliteInteger.dart';

import 'SqliteDataType.dart';

class SqliteRow
{
  static SqliteRow get empty => new SqliteRow(-1);

  final int id;
  final Map<String, SqliteDataType> _columns;

  SqliteRow(this.id) : _columns = {};

  SqliteDataType addColumn(String name, dynamic value) {
    SqliteDataType dataType;
    if( value is bool ) {
      dataType = new SqliteBoolean(value);
    }
    else if( value is int )
      dataType = new SqliteInteger(value);
    else if( value is double )
      dataType = new SqliteFloat(value);
    else if( value is String  )
      dataType = new SqliteText(value);
    else if( value is Uint8List )
      dataType = new SqliteBlob(value);
    else //if( value == Null || value == null )
      dataType = new SqliteNull();
    _columns[name] = dataType;
    return dataType;
  }

  operator [](String columnName) => _columns[columnName];

  @override
  String toString() {
    return '$id: $_columns';
  }
}
