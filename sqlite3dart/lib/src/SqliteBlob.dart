import 'dart:typed_data';

import 'SqliteDataType.dart';

class SqliteBlob
    extends SqliteDataType
{
  final int size;
  final Uint8List value;

  SqliteBlob(this.value, this.size) : super(4, 'SQLITE_BLOB');
}

