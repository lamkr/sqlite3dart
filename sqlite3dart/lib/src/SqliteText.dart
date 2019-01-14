import 'SqliteDataType.dart';

class SqliteText
    extends SqliteDataType<String>
{
  SqliteText(String value) : super(3, 'SQLITE_TEXT', value);

  @override
  String toString() => "$name=${isNull ? null : '$value'}";
}

