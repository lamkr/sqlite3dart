import 'SqliteDataType.dart';

class SqliteBoolean
    extends SqliteDataType<bool>
{
  SqliteBoolean(bool value) : super(6, 'SQLITE_BOOLEAN', value);
}

