import 'SqliteDataType.dart';

class SqliteFloat
    extends SqliteDataType<double>
{
  SqliteFloat(double value) : super(2, 'SQLITE_FLOAT', value);
}

