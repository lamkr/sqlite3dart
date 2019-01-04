import 'package:sqlite3dart/src/SqliteDataType.dart';

class SqliteText
    extends SqliteDataType
{
  final int size;
  final String value;

  SqliteText(this.value, this.size) : super(3, 'SQLITE_TEXT');

  @override
  String toString() {
    return '$size:$value';
  }
}

