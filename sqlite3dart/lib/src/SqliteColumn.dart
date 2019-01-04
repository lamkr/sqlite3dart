import 'SqliteDataType.dart';

class SqliteColumn
{
  final String name;
  final SqliteDataType value;

  SqliteColumn(this.name, this.value);

  @override
  String toString() => '$name=$value';
}

