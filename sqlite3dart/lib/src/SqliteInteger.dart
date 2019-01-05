import 'SqliteDataType.dart';

class SqliteInteger
    extends SqliteDataType<int>
{
  SqliteInteger(int value) : super(1, 'SQLITE_INTEGER', value);
}

