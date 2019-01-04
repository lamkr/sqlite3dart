import 'SqliteDataType.dart';

class SqliteFloat
    extends SqliteDataType
{
  final double value;

  SqliteFloat(this.value) : super(2, 'SQLITE_FLOAT');
}

