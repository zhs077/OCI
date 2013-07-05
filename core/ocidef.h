#ifndef _OCIDEF_H_
#define  _OCIDEF_H_

#include "platform_config.h"
namespace OCI
{



	//异常信息
	class OCIException
	{
	private:
		OCIException(void)
		{

		}
	public:

		~OCIException(void)
		{

		}

		static OCIException* createOCIExecption();					//单例模式
		static bool checkError(OCIError* pErr,sb2 status);			//检查status下的错误
		static const char* getErrorMsg();							//获取错误信息
	private:

		static sb2 errorCode;										//错误代码
		static char msg[512];										//错误信息
		static OCIException* pExecption;
	};
	//ORACLE 数据库连接
	class OCIConnection
	{
	public:
		OCIConnection(void):connected(false),envhp(NULL),errhp(NULL),
			srvhp(NULL),svchp(NULL),usrhp(NULL),dschp(NULL),txnhp(NULL)
		{
		}
		~OCIConnection(void)
		{
			disconnect();	
		};

		bool connect(const char* dblink,const char* username,const char* password);  //连接数据库其中dblink为数据库实例名
		void disconnect();															//断开连接	
		bool isConnected() 
		{ 
			return connected; 
		}

		const char* getErrorMsg() const
		{
			return OCIException::getErrorMsg();
		}

	private:
		bool connected;
	public:
		OCIEnv*			envhp;      //环境句柄
		OCIError*		errhp;		//错误句柄
		OCIServer*		srvhp;		//服务句柄
		OCISvcCtx*		svchp;		//服务上下文句柄
		OCISession*		usrhp;		//用户会话句柄
		OCIDescribe*	dschp;		//描述句柄
		OCITrans*		txnhp;		//事务句柄




	};


	//连接池
	class OCIConPool
	{
	private:
		OCIConPool()
		{

		}
	public:
		enum{MAX_NUM= 20};

	private:
		std::vector<OCIConnection*>conns; //连接数组




	};




	



	//游标
	class OCICursor
	{
	public:
		OCICursor(void);
		virtual ~OCICursor(void);
		OCICursor(OCIConnection* pConn);



	private:
		OCIConnection* pConn;						//连接
		OCIStmt* stmthp;							//语句句柄
		OCIParam* colhd;							//列句柄
		OCIBind* bindhp;							//绑定句柄
		bool bOpen;									//是否打开
		std::vector<void*>rowBuffers;				//行的缓冲区	
		std::vector<OCIDefine *> defines;			//定义句柄数组
		std::vector <sb2>ind;						//空指示符数组
		std::vector<int>colsType;					//列类型数组
		std::vector<int>colsSize;					//列大小数组
		std::vector<int>colsScale;					//列的小数位数数组
		std::string strFieldValue;





	public:
		bool open();													//打开
		void close();													//关闭
		const char*getColName(const int& colnumber)const;				//列的名字
		ub2 getColType(const int& colnumber)const;						//列的类型
		ub2 getColSize(const int& colnumber)const;						//列的长度
		ub2 getColScale(const int& colnumber)const;						//小数点后的位数
		ub4 getColNums()const;											//列的个数
		ub4 getRecordNums();											//获取记录的个数
		bool query(const char* sql);									//查询语句
		bool bindQuery(const char* sql);								//绑定查询
		bool moveFirst(int rows=1);										//
		bool moveNext(int rows=1);										//获取结果
		bool moveLast(int rows=1);										
		const char* getFieldValue(const int& colnumber);				//获取字段值，以串的形式返回
		bool execute(const char* sql);									//执行插入删除更新等
		bool beginTran();
		bool commitTran();
		bool rollbackTran();

		bool bindByPos(ub4 pos,void* value,sb4 valuesize,void*indicator,sb4 type);
		bool bindByName(const char* name,void* value,sb4 valuesize,void*indicator,sb4 type);
		bool prepare(const char* stmt);									//准备SQL语句


	private:

		bool executeSql(ub4 iters=0,ub4 mode=OCI_DEFAULT);				//执行SQL、获取结果集
		bool allocateCursor();											//为游标分配空间
		double oicNumberToReal(const OCINumber* ociNum);				//NUMBER类型转为double
		void free();													//释放rowBuffers3 new分配的资源


	};

}

#endif
