import 'SqliteDataType.dart';

import 'SqliteColumn.dart';

class SqliteRow
{
  static SqliteRow get empty => new SqliteRow(-1);

  final int id;
  final Map<String, SqliteColumn> columns;

  SqliteRow(this.id) : columns = {};

  SqliteColumn addColumn(String name, SqliteDataType value) {
    return columns[name] = new SqliteColumn(name, value);
  }

  @override
  String toString() {
    return '$id: $columns';
  }
}
