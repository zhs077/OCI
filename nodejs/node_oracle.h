
#ifndef NODE_ORACLE_
#define NODE_ORACLE_

#define NODEJS
#include<v8.h>
#include<node.h>

using namespace v8;
#include<string>
#include<vector>
#include <tuple>
using namespace std;

#include "../core/ocidef.h"
using namespace OCI;


class OracleOperator :public node::ObjectWrap
{
	public:
		static void Init(Handle<Object>exports);
	private:
	OracleOperator();
	~OracleOperator();
	//同步操作
	static Handle<Value>New(const Arguments& args);
	static Handle<Value>ConnectSync(const Arguments& args);				//连接数据库
	static Handle<Value>QuerySync(const Arguments& args);				//查询
	static Handle<Value>ExecuteSync(const Arguments& args);				//执行
	static Handle<Value>CloseSync(const Arguments& args);
	static Handle<Value>BeginTran(const Arguments& args);
	static Handle<Value>RollBack(const Arguments& args);
	static Handle<Value>Commit(const Arguments& args);		
	static Handle<Value>GetError(const Arguments& args);

	//异步操作
	static Handle<Value>Connect(const Arguments& args);
	static Handle<Value>Query(const Arguments& args);
	static Handle<Value>Execute(const Arguments& args );

private:

	static OCIConnection connect_; //连接
	static OCICursor*  cursor_;		//游标操作数据库
	typedef std::tuple< OracleOperator*,Persistent<Function>,bool,vector<string>,string > BUNDLE;   //传递参数的数据结构
	
	//static vector<string> m_parameters;
	//static bool m_bRet;
private:
	static const char* ToCString(const String::Utf8Value& value);
	static string Replace(const string& orignStr, const string& oldStr, const vector<string>&vc );
	static string MergeSql(const Arguments&args);
	static void connect_doing_process(uv_work_t* req);    //在这里处理数据，会在一个新线程中处理
	static void connect_after_process(uv_work_t* req);    //在这里在 NodeJS 的主线程处理，调用 JS 的回调函数

	static void query_doing_process(uv_work_t* req);
	static void query_after_process(uv_work_t* req);		//查询线程

	static void execute_doing_process(uv_work_t* req);
	static void execute_after_process(uv_work_t* req);		//执行线程




		
};



#endif
