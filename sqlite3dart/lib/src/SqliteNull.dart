import 'SqliteDataType.dart';

class SqliteNull
    extends SqliteDataType
{
  final Object value;

  SqliteNull() : value=null, super(5, 'SQLITE_NULL');
}

