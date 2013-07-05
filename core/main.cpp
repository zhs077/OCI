

//#include "oci.h"
//#include "orl.h"
//#pragma comment(lib,"oci.lib")
//
#include <iostream>
using namespace std;
//#include "ocidef.h"
//using namespace OCI;

#include "ocidef.h"
using namespace OCI;






int main()
{

	//wstring s=L"1234你好";
	//string s2;

//	 detail::wchar_utf8(s.begin(),s.end(),s2.begin());
	//errorMsg = 0x004374d0 "ORA-24374: 在读取或执行并读取之前没有完成定义
	
	OCIConnection conn;
 	bool  b = conn.connect("orcl","system","zhs");
	const char* errorMsg = OCIException::getErrorMsg();
	//char* sql="select * from t_pd_zxcw_tp_point";
	char *sql = "select Test10 from TESTTABLE_OCI";
	OCICursor cursor(&conn);
	b =  cursor.query(sql);
	if(false == b)
	{
		errorMsg = OCIException::getErrorMsg();
	}
	//int xx = ptr->getColNums();

	//int type = cursor.getColType(1);
	//int type2 = cursor.getColType(2);
	int size = cursor.getColNums();
	cout<<"表student中的内容如下: "<<endl;
	//cursor.moveLast();
	while (cursor.moveNext())
	{
		for (int i=0; i<size; i++)
		{


			cout<<cursor.getFieldValue(i+1)<<","<<endl;
		

			
		}
		cout<<endl;
	}



	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9,TEST10)values (123, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello','2012-10-11 16:20:40')";

	b= cursor.execute(sql);

	if(false == b)
	{
		errorMsg = OCIException::getErrorMsg();
	}
	//插入
	b = cursor.beginTran();

	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (7, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (8, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	
		sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (9, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
			b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (10, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (11, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (12, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (13, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
		b = cursor.execute(sql);
	sql = "insert into testtable_oci (SID, SEQ, UNAME, INTRODUCE, TEST1, TEST2, TEST3, TEST4, TEST5, TEST8, TEST9)values (14, '1', 'zhs', 'fdf', 123.122, null, 123.456, 2.12, null, '', 'hello')";
	b = cursor.execute(sql);


	//cursor.rollbackTran();
	if (b)
	{
		cursor.commitTran();
	}
	else
	{
		cursor.rollbackTran();
	}



	//cursor.execute("delete from TESTTABLE_OCI ");


	cursor.execute("update TESTTABLE_OCI set test8='abcdefg' ");

	//绑定查询

	sql ="select UName,Introduce from TestTable_OCI where SID=:vSID";
	cursor.prepare(sql);

	int vSID=7;

	//b= cursor.bindByPos(1,(void*)&vSID, sizeof(vSID),NULL,SQLT_INT);

	b= cursor.bindByName("vSID",(void*)&vSID, sizeof(vSID),NULL,SQLT_INT);
	if (b == false)
	{
		errorMsg = OCIException::getErrorMsg();
	}
	b = cursor.bindQuery(sql);
	if (b == false)
	{
		errorMsg = OCIException::getErrorMsg();
	}
	
	size = cursor.getColNums();
	while (cursor.moveNext())
	{
		for (int i=0; i<size; i++)
		{


			cout<<cursor.getFieldValue(i+1)<<","<<endl;



		}
		cout<<endl;
	}

	
		//s[0] = OCIBindByPos(stmthpp, &bndhp, errhp, 1, (void*)&vSID, sizeof(vSID),
	//SQLT_INT, 0, 0, 0, 0, 0, OCI_DEFAULT);

	//OCIBindByPos()


	//cursor.moveFirst();
	//cursor.moveNext();

	//while (cursor.moveNext())
	//{
	//	for (int i=0; i<size; i++)
	//	{
	//		cout<<cursor.getFieldValue(i+1)<<","<<endl;
	//		

	//	}
	//	cout<<endl;
	//}


	/*int flag;
	std::cin>>flag;
	while(flag <= 100)
	{
		
		b = cursor.query(sql);
		if (b == false)
		{
			cout<<"查询失败"<<endl;
		}
		else
		{
			cout<<"查询成功"<<endl;

		}
		flag++;
	}
std::cin>>flag;*/
//	errorMsg = OCIException::getErrorMsg();
//
//
//
//	
//	int mode = OCI_THREADED |OCI_OBJECT;
//
//	sword ret;
//	ret = OCIInitialize(mode,(dvoid*)0,   (dvoid*(*)(dvoid *ctxp, size_t size))0, 
//		(dvoid*(*)(dvoid *ctxp, dvoid *memptr, size_t newsize))0, 
//		(void (*)(dvoid *ctxp, dvoid *memptr))0);
//	if (ret != OCI_SUCCESS)
//	{
//		cout<<"OCIInitialize error"<<endl;
//		return 0;
//	}
//	OCIEnv*			envhp;      //环境句柄
//	OCIError*		errhp;		//错误句柄
//	OCIServer*		srvhp;		//服务句柄
//	OCISvcCtx*		svchp;		//服务上下文句柄
//	OCISession*		usrhp;		//用户会话句柄
//	OCIDescribe*	dschp;		//描述句柄
//	OCITrans*		txnhp;		//事务句柄
//	OCIBind			*bndhp;      // 绑定句柄
//	ret = OCIEnvInit(&envhp,OCI_DEFAULT,0,0);
//	if (ret != OCI_SUCCESS)
//	{
//		cout<<"OCIEnvInit error"<<endl;
//		return 0;
//	}
//	ret = OCIHandleAlloc((dvoid*)envhp,(dvoid**)&errhp,OCI_HTYPE_ERROR,0,0);
//	ret = OCIHandleAlloc((dvoid*)envhp,(dvoid**)&srvhp,OCI_HTYPE_SERVER,0,0);
//	const char* dblink="zhs"; 
//	const char* username = "zxcw";
//	const char* password = "zxcw";
//	char msg[512]={0};
//	ret = OCIServerAttach(srvhp,errhp,(text*)dblink,strlen(dblink),OCI_DEFAULT);
//	if (ret != OCI_SUCCESS)
//	{
//		cout<<"OCIServerAttach error"<<endl;
//		return 0;
//
//	}
//	ret = OCIHandleAlloc((dvoid*)envhp,(dvoid**)&svchp,OCI_HTYPE_SVCCTX,0,0);
//	ret = OCIAttrSet((dvoid*)svchp,OCI_HTYPE_SVCCTX,(dvoid*)srvhp,0,OCI_ATTR_SERVER,errhp);
//	
//
//	ret = OCIHandleAlloc((dvoid*)envhp, (dvoid**)&usrhp, OCI_HTYPE_SESSION, 0, 0);
//
//	ret = OCIAttrSet((dvoid*)usrhp,OCI_HTYPE_SESSION,(dvoid*)username,strlen(username),OCI_ATTR_USERNAME,errhp);
//	ret = OCIAttrSet((dvoid*)usrhp,OCI_HTYPE_SESSION,(dvoid*)password,strlen(password),OCI_ATTR_PASSWORD,errhp);
//
//	//初始化描述句柄
//	ret = OCIHandleAlloc((dvoid*)envhp,(dvoid**)&dschp,OCI_HTYPE_DESCRIBE,0,0);
//
//	//开始会话
//
//	int status;
//	status = OCISessionBegin(svchp,errhp,usrhp,OCI_CRED_RDBMS,OCI_DEFAULT);
//	OCIErrorGet((dvoid *)errhp, (ub4)1, (text *)NULL, &status,
//		(unsigned char*)msg, (ub4)sizeof(msg), OCI_HTYPE_ERROR);
//
//	ret = OCIAttrSet((dvoid*)svchp, (ub4)OCI_HTYPE_SVCCTX, (dvoid*)usrhp, (ub4)0,(ub4)OCI_ATTR_SESSION, errhp);
//
//	ret = OCIHandleAlloc((dvoid *)envhp, (dvoid **)&txnhp, OCI_HTYPE_TRANS, 0, 0);
//	ret = OCIAttrSet((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)txnhp, 0,OCI_ATTR_TRANS, errhp);
//
//
//	
//
//	//开始sql语句
//
//	OCIStmt* stmthpp = NULL;
//	//创建一个句子句柄
//	//int ret;
//
//	ret = OCIHandleAlloc((dvoid *)envhp,(dvoid **)&stmthpp,OCI_HTYPE_STMT,0,0);
////	ret = OCIHandleAlloc((dvoid*)envhp,(dvoid**)&errhp,OCI_HTYPE_ERROR,0,0);
//
//
//	ret = OCIStmtPrepare(stmthpp,errhp,(text*)sql,strlen(sql),OCI_NTV_SYNTAX,OCI_DEFAULT);
//
//
//	OCIDefine* define1;
//	OCIParam* colhd;
//	char time[40]={0};
//	int point;
//	//ret = OCIDefineByPos(stmthpp,&define1,errhp,1,(dvoid*)time,strlen(time),SQLT_STR,(void*)&point,(ub2*)0,0,OCI_DEFAULT);
//
//	OCIDefine* defines[1];
//	sword indp[1];
//	int pointID;
//	char ch2[3][1024] = {0};
//	int s[5] = {0};
//
//	int vSID = 1;
//
//	char ch[3][1024] = {0};
//
//	char sql2[1024] = {0};
//
//	ub4 ub4RecordNo = 1;
//
//	sb4 sb4ErrorCode;
//
//	char sErrorMsg[1024] = {0};
//
//	OCIDefine *hDefine[2] = {NULL};
//
//	sb2 sb2Ind[2] = {0};                    // 所选择的列可能存在NULL，所以要用指示器变量
//
//	
//	//s[2] = OCIDefineByPos(stmthpp, &hDefine[1], errhp, 2, ch[1], sizeof(ch[1]), SQLT_STR, &sb2Ind[1], NULL, NULL, OCI_DEFAULT);
//	//ret = OCIDefineByPos(stmthpp,&defines[0],errhp,1,(dvoid*)ch2[0],sizeof(ch2[0]),SQLT_STR,&indp[0],0,0,OCI_DEFAULT);
//	ret = OCIStmtExecute(svchp,stmthpp,errhp,0,(ub4)0,(OCISnapshot *)NULL,(OCISnapshot *)NULL,(ub4)OCI_DEFAULT);
//	//ret = OCIDefineByPos(stmthpp,&define[0],errhp,1,(dvoid*)time,sizeof(time),SQLT_CHR,&indp[0],0,0,OCI_DEFAULT);
//	//cout<<msg<<endl;
//		s[1] = OCIDefineByPos(stmthpp, &hDefine[0], errhp, 1, ch[0], sizeof(ch[0]), SQLT_STR, &sb2Ind[0], NULL, NULL, OCI_DEFAULT);
//	
//		//列的名字
//		text* colname = NULL;
//		ub4	colnamesz_;
//
//		ret = OCIParamGet(stmthpp,OCI_HTYPE_STMT,errhp,(dvoid**)&colhd,1);
//		ret = OCIAttrGet(colhd,OCI_DTYPE_PARAM,(dvoid**)&colname,&colnamesz_,OCI_ATTR_NAME,errhp);
//
//
//		//列的长度
//		ub2  colsize_ = (ub2) 0;
//		ret = OCIParamGet(stmthpp,OCI_HTYPE_STMT,errhp,(dvoid**)&colhd,1);
//		ret = OCIAttrGet(colhd,OCI_DTYPE_PARAM,(dvoid*)&colsize_,0,OCI_ATTR_DATA_SIZE,errhp);
//
//
//		//列的类型
//		ub2	dtype = 0;
//		ret = OCIParamGet((dvoid*)stmthpp,OCI_HTYPE_STMT,errhp,(dvoid**)&colhd,1);
//		ret = OCIAttrGet((dvoid*)colhd,OCI_DTYPE_PARAM,(dvoid*)&dtype,0,OCI_ATTR_DATA_TYPE,errhp);
//
//		//记录的个数
//		ub4 recordnum = 0;
//		ret = OCIAttrGet((dvoid*)stmthpp,OCI_HTYPE_STMT,(dvoid*)&recordnum,0,OCI_ATTR_ROW_COUNT,errhp);
//
//		//列的个数
//		int rows;
//		ret = OCIAttrGet(stmthpp,OCI_HTYPE_STMT,&rows,0,OCI_ATTR_PARAM_COUNT,errhp);
//		ub2 dataLeng=0;;
//ret = OCIDefineByPos(stmthpp,&define1,errhp,1,(dvoid*)time,strlen(time)+1,SQLT_STR,NULL,&dataLeng,NULL,OCI_DEFAULT);
//string	fieldValue; //字段值
//fieldValue = (char*)time;
//ret = OCIStmtFetch(stmthpp,errhp,1,OCI_FETCH_NEXT,OCI_DEFAULT);
//OCIErrorGet((dvoid *)errhp, (ub4)1, (text *)NULL, &ret,
//			(unsigned char*)msg, (ub4)sizeof(msg), OCI_HTYPE_ERROR);
//ret = OCIDefineByPos(stmthpp,&define1,errhp,1,(dvoid*)time,strlen(time)+1,SQLT_STR,NULL,&dataLeng,NULL,OCI_DEFAULT);
//fieldValue = (char*)time;







		//开始查询


		



		//sprintf(sql2, "%s", "select UName,Introduce from TestTable_OCI where SID=:vSID");  

		//sprintf(sql2, "%s", "select UName,Introduce from TestTable_OCI" ); 
		//ret = OCIStmtPrepare(stmthpp, errhp, (text*)sql2, strlen(sql2), OCI_NTV_SYNTAX, OCI_DEFAULT);

		//s[1] = OCIDefineByPos(stmthpp, &hDefine[0], errhp, 1, ch[0], sizeof(ch[0]), SQLT_STR, &sb2Ind[0], NULL, NULL, OCI_DEFAULT);

		//s[2] = OCIDefineByPos(stmthpp, &hDefine[1], errhp, 2, ch[1], sizeof(ch[1]), SQLT_STR, &sb2Ind[1], NULL, NULL, OCI_DEFAULT);
		//for(vSID = 0; vSID < 12; vSID++)         // 循环绑定第一个输入变量以执行不同的sql语句

		//{

		//	s[0] = OCIBindByPos(stmthpp, &bndhp, errhp, 1, (void*)&vSID, sizeof(vSID), SQLT_INT, 0, 0, 0, 0, 0, OCI_DEFAULT);

		//	s[3] = OCIStmtExecute(svchp, stmthpp, errhp, 1, 0, NULL, NULL, OCI_DEFAULT);

		//	if(s[3] == OCI_SUCCESS)

		//	{

		//		printf("%d: ch[0] = (%d)%s\n", vSID, strlen(ch[0]), ch[0]);

		//		printf("ch[1] = (%d)%s\n\n", strlen(ch[1]), ch[1]);

		//	}

		//	else if(s[3] == OCI_NO_DATA)

		//	{

		//		printf("%d: There is NO_DATA\n\n", vSID);

		//	}

		//	else

		//	{

		//		if(OCIErrorGet(errhp, 1, NULL, &sb4ErrorCode, (OraText*)sErrorMsg, sizeof(sErrorMsg), OCI_HTYPE_ERROR) == OCI_SUCCESS)

		//			printf("%d: OCIStmtExecute: Error Message = %s\n", vSID, sErrorMsg);

		//		return -1;

		//	}

		//}




//HYConnection conn2;
//conn2.connect("zhs", "zxcw", "zxcw");
//if (conn2.isConnected()) cout<<"成功连接数据库"<<endl;
//else cout<<HYException::errMsg()<<endl;
//
//HYCursor cursor2(&conn2);
//try
//{
//	cursor2.query("select test7 from TESTTABLE_OCI");
//	int size = cursor2.numCols();
//	int cols = cursor2.numCols();
//	
//	cout<<"表student中的内容如下: "<<endl;
//	while (cursor2.fetch())
//	{
//		for (int i=0; i<size; i++)
//		{
//			cout<<cursor2.getFieldValue(i+1)<<", ";
//		}
//		cout<<endl;
//	}
//}
//catch(HYException& e)
//{
//	cout<<e.errMsg()<<endl;
//	return -1;
//}
//int i;
//cin>>i;


	return 0;
}
