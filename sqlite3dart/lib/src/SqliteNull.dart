import 'SqliteDataType.dart';

class SqliteNull
    extends SqliteDataType<Object>
{
  SqliteNull() : super(5, 'SQLITE_NULL', null);
}

