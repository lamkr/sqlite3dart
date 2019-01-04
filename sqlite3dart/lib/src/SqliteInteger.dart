import 'SqliteDataType.dart';

class SqliteInteger
    extends SqliteDataType
{
  final int value;

  SqliteInteger(this.value) : super(1, 'SQLITE_INTEGER');
}

