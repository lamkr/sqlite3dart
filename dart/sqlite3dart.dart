library sqlite3dart;

import 'dart:async';
import 'dart:isolate';

//import 'dart-ext:C:/projects/lamkr/sqlite3dart/build/Release/sqlite3dart_extension.dll';
//import 'dart-ext:../build/Release/sqlite3dart_extension';
import 'dart-ext:sqlite3dart_extension';

/// This method must be implementend by all data providers.
SendPort get_receive_port() native "get_receive_port";

void throwError() native "throwError_";

int sqlite3_threadsafe() native "sqlite3_threadsafe_";
int sqlite3_libversion_number() native "sqlite3_libversion_number_";
int sqlite3_close(int db) native "sqlite3_close_";

//int sqlite3_open(String path) native "sqlite3_open_";
Future<int> sqlite3_open(String path) {
  var completer = new Completer<int>();
  var replyPort = new RawReceivePort();
  var args = new List(3);
  args[0] = replyPort.sendPort;
  args[1] = 'sqlite3_open';
  args[2] = path;
  get_receive_port().send(args);
  replyPort.handler = (result) {
    replyPort.close();
	if( result is String )
		throw new Sqlite3Exception(result);
	completer.complete(result);
  };
  return completer.future;
}

int sqlite3_exec(int db, String sql) native "sqlite3_exec_";

//Future<int> sqlite3_open_async(String path) native "sqlite3_open_async_";
SendPort sqlite3_open_async() native "sqlite3_open_async_";


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

class Sqlite3Exception 
implements Exception 
{
	final String message;
	
	Sqlite3Exception(this.message);
	
	String toString() {
		return '$runtimeType: $message';
	}
}