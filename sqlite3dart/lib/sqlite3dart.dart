/// SQLite3 for Dart.
/// Copyright (c) 2018 Luciano Rodrigues (Brodi).
/// Please see the AUTHORS file for details. 
/// All rights reserved. Use of this source code is governed by a MIT-style 
/// license that can be found in the LICENSE file.
///
/// {@category Database}
library sqlite3dart;

import 'dart:isolate';
import 'dart-ext:sqlite3dart_extension';

export 'src/sqlite3.dart';
export 'src/SqliteException.dart';

//export 'src/repository_core.dart';
///
/// Returns a [SendPort] which will communicate with provider's native function wrappers.
///
/// **This method must be implementend by all data providers.**
SendPort get_receive_port() native "get_receive_port";

/*
void throwError() native "throwError_";

int sqlite3_threadsafe() native "sqlite3_threadsafe_";
int sqlite3_libversion_number() native "sqlite3_libversion_number_";
int sqlite3_close(int db) native "sqlite3_close_";
int sqlite3_exec(int db, String sql) native "sqlite3_exec_";
//Future<int> sqlite3_open_async(String path) native "sqlite3_open_async_";
SendPort sqlite3_open_async() native "sqlite3_open_async_";
*/

/*
public abstract IDataSourceFactory {
	IDataSourceFactory url(String url);
	IDataSourceFactory username(String username);
	IDataSourceFactory password(String username);

	IDataSource build();
}

public abstract IDataSource {
	FutureOr<IDataSession> getSession
}

public void readme(String url, String user, String password) {
	// get the AoJ DataSourceFactory
	DataSourceFactory factory = DataSourceFactory.newFactory("com.oracle.adbaoverjdbc.DataSourceFactory");
	// get a DataSource and a Session
	try (DataSource ds = factory.builder()
				 .url(url)
				 .username(user)
				 .password(password)
				 .build();
				 Session conn = ds.getSession(t -> System.out.println("ERROR: " + t.getMessage()))) {
	 // get a TransactionCompletion
	 TransactionCompletion trans = conn.transactionCompletion();
	 // select the EMPNO of CLARK
	 CompletionStage<Integer> idF = conn.<Integer>rowOperation("select empno, ename from emp where ename = ? for update")
					 .set("1", "CLARK", AdbaType.VARCHAR)
					 .collect(Collector.of(
									 () -> new int[1], 
									 (a, r) -> {a[0] = r.at("empno").get(Integer.class); },
									 (l, r) -> null,
									 a -> a[0])
					 )
					 .submit()
					 .getCompletionStage();
	 // update CLARK to work in department 50
	 conn.<Long>rowCountOperation("update emp set deptno = ? where empno = ?")
					 .set("1", 50, AdbaType.INTEGER)
					 .set("2", idF, AdbaType.INTEGER)
					 .apply(c -> { 
						 if (c.getCount() != 1L) {
							 trans.setRollbackOnly();
							 throw new SqlException("updated wrong number of rows", null, null, -1, null, -1);
						 }
						 return c.getCount();
					 })
					 .onError(t -> t.printStackTrace())
					 .submit();
	 
	 conn.catchErrors();  // resume normal execution if there were any errors
	 conn.commitMaybeRollback(trans); // commit (or rollback) the transaction
	}  
	// wait for the async tasks to complete before exiting  
	ForkJoinPool.commonPool().awaitQuiescence(1, TimeUnit.MINUTES);
}*/

