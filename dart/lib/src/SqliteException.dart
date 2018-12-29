/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

class SqliteException 
implements Exception 
{
	final String message;
	
	SqliteException(this.message);
	
	String toString() {
		return '$runtimeType: $message';
	}
}