import 'dart:typed_data';

import 'SqliteDataType.dart';

class SqliteBlob
    extends SqliteDataType<Uint8List>
{
  SqliteBlob(Uint8List value) : super(4, 'SQLITE_BLOB', value);
}

