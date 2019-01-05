abstract class SqliteDataType<T>
{
  final int id;
  final String name;
  final T _value;
  final bool isNull;

  SqliteDataType(this.id, this.name, this._value) : isNull = (_value == null);

  T get value => _value;

  @override
  String toString() => '$name=${isNull ? null : value}';
}

