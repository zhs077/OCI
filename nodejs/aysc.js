//异步调用

var oracle = require('./build/Release/oracle_win32');
var oracleoperator = new oracle.OracleOperator();

try{
	oracleoperator.connect('zhs','nodejs','nodejs',function(error){
		if(error){
			console.error(error);
			return;
		}
		
		
		
		//
		oracleoperator.beginTran();
		var sql='insert into test values(?,?,?)';
		oracleoperator.execute(sql,'2','郑华山','10',function(error){
			if(error){
			console.error(error);
			return;
		}
		oracleoperator.rollBack();
		});
		
		
		sql = 'select * from test';
		oracleoperator.query(sql,function(error,array){
			if(error){
			console.error(error);
			return;
			}
			else{
				for(var i=0; i < array.length;i++){
				console.log(array[i]);
			}
		}
		});
		
	});
}
catch(e){
	console.error(e);
}
finally{
}
