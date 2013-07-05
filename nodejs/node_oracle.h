
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
	//ͬ������
	static Handle<Value>New(const Arguments& args);
	static Handle<Value>ConnectSync(const Arguments& args);				//�������ݿ�
	static Handle<Value>QuerySync(const Arguments& args);				//��ѯ
	static Handle<Value>ExecuteSync(const Arguments& args);				//ִ��
	static Handle<Value>CloseSync(const Arguments& args);
	static Handle<Value>BeginTran(const Arguments& args);
	static Handle<Value>RollBack(const Arguments& args);
	static Handle<Value>Commit(const Arguments& args);		
	static Handle<Value>GetError(const Arguments& args);

	//�첽����
	static Handle<Value>Connect(const Arguments& args);
	static Handle<Value>Query(const Arguments& args);
	static Handle<Value>Execute(const Arguments& args );

private:

	static OCIConnection connect_; //����
	static OCICursor*  cursor_;		//�α�������ݿ�
	typedef std::tuple< OracleOperator*,Persistent<Function>,bool,vector<string>,string > BUNDLE;   //���ݲ��������ݽṹ
	
	//static vector<string> m_parameters;
	//static bool m_bRet;
private:
	static const char* ToCString(const String::Utf8Value& value);
	static string Replace(const string& orignStr, const string& oldStr, const vector<string>&vc );
	static string MergeSql(const Arguments&args);
	static void connect_doing_process(uv_work_t* req);    //�����ﴦ�����ݣ�����һ�����߳��д���
	static void connect_after_process(uv_work_t* req);    //�������� NodeJS �����̴߳������� JS �Ļص�����

	static void query_doing_process(uv_work_t* req);
	static void query_after_process(uv_work_t* req);		//��ѯ�߳�

	static void execute_doing_process(uv_work_t* req);
	static void execute_after_process(uv_work_t* req);		//ִ���߳�




		
};



#endif
