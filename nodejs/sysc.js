//同步

var oracle = require('./build/Release/oracle_win32');
var oracleoperator = new oracle.OracleOperator();
try{
	var b = oracleoperator.connectSync('zhs','nodejs','nodejs');
	if( b == false){
		console.log(error);
	}
	var sql = 'select * from test';
	var array = oracleoperator.querySync(sql);
	for(var i=0; i < array.length;i++){
				console.log(array[i]);
				}
				
				
	 sql='insert into test values(?,?,?)';
	 oracleoperator.beginTran();
	 oracleoperator.executeSync(sql,13,'郑华山',13);
	  oracleoperator.executeSync(sql,13,'郑华山',13);
	   oracleoperator.executeSync(sql,13,'郑华山',13);
	    oracleoperator.executeSync(sql,13,'郑华山',13); oracleoperator.executeSync(sql,13,'郑华山',13);
		 oracleoperator.executeSync(sql,13,'郑华山',13); oracleoperator.executeSync(sql,13,'郑华山',13); oracleoperator.executeSync(sql,13,'郑华山',13);
     //oracleoperator.commitTran();
	 oracleoperator.rollBack();
	 
	 var oracleoperator2 = new oracle.OracleOperator();
	 b = oracleoperator.connectSync('orcl','system','zhs');
	 if( b == false){
		console.log(error);
	}
	
	
	 var sql = 'select * from test';
	var array = oracleoperator.querySync(sql);
	for(var i=0; i < array.length;i++){
				console.log(array[i]);
				}
	 
}
catch(e){
}
finally{
}