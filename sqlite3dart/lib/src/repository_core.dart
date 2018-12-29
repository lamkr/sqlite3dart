/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.

import 'dart:isolate';

import 'dart-ext:sqlite3dart_extension';

/// Returns a [SendPort] which will communicate with provider's native function wrappers.
///
/// **This method must be implementend by all data providers.**
SendPort get_receive_port() native "get_receive_port";

