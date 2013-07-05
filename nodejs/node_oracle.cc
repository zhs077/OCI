#define BUILDING_NODE_EXTENSION

#define NODEJS
#include"node_oracle.h"
#include"../third_party/CodingConv/encodeutil.h"
#include <uv.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;
//utf8 转 Unicode
OCIConnection OracleOperator:: connect_;
OCICursor* OracleOperator:: cursor_ = new OCICursor(&connect_);
//vector<string> OracleOperator::m_parameters;
//bool OracleOperator:: m_bRet = false;
//合并为正确的SQL语句
string OracleOperator::Replace( const string& orignStr, const string& oldStr, const vector<string>&vc ) 
{ 
    size_t pos = 0; 
    string tempStr = orignStr; 
    string::size_type newStrLen; 
    string::size_type oldStrLen = oldStr.length();
	int i=0;
	
    while(true) 
    { 
        pos = tempStr.find(oldStr, pos); 
        if (pos == string::npos) break; 
		string s=vc.at(i);
		s = "\'"+s +"\'";
		newStrLen= s.length(); 

        tempStr.replace(pos, oldStrLen, s);         
        pos += newStrLen;
		i++;

    } 

    return tempStr; 
}

string OracleOperator::MergeSql(const Arguments&args)
{
	String::Utf8Value str(args[0]);
	const char * pstr = ToCString(str);
	string s1 = pstr;
	#ifdef OS_WIN32
	s1 = encodeConv::CodingConv::utf82Ascii(pstr);
	#endif
	
	vector<string> vc;
	int length = args.Length();
	if (args[length-1]->IsFunction())
	{
		length -= 1;
	}
	
	for(int i = 0; i<length;i++)
	{
		String::Utf8Value str1(args[i+1]);
		const char * pstr_tmp = ToCString(str1);
		string tmp = pstr_tmp;
		#ifdef OS_WIN32
		tmp = encodeConv::CodingConv::utf82Ascii(pstr_tmp);
		#endif
		vc.push_back(tmp);
		
	}
	string sql=Replace(s1,"?",vc);
	
	return sql;


}


const char* OracleOperator::ToCString(const String::Utf8Value& value)
{
	return *value ? *value: "string conversion failed";
}

OracleOperator::OracleOperator()
{
}
OracleOperator::~OracleOperator()
{
}

void OracleOperator::Init(Handle<Object>exports)
{
	Local<FunctionTemplate>tp1 = FunctionTemplate::New(New);
	tp1->SetClassName(String::NewSymbol("OracleOperator"));
	tp1->InstanceTemplate()->SetInternalFieldCount(1);
	
	//属性
	
	tp1->PrototypeTemplate()->Set(String::NewSymbol("connectSync"),
		FunctionTemplate::New(ConnectSync)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("connect"),
		FunctionTemplate::New(Connect)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("querySync"),
		FunctionTemplate::New(QuerySync)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("query"),
		FunctionTemplate::New(Query)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("executeSync"),
		FunctionTemplate::New(ExecuteSync)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("execute"),
		FunctionTemplate::New(Execute)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("beginTran"),
		FunctionTemplate::New(BeginTran)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("rollBack"),
		FunctionTemplate::New(RollBack)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("commit"),
		FunctionTemplate::New(Commit)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("closeSync"),
		FunctionTemplate::New(CloseSync)->GetFunction());
	tp1->PrototypeTemplate()->Set(String::NewSymbol("getError"),
		FunctionTemplate::New(GetError)->GetFunction());
		
	
	Persistent<Function>constructor = Persistent<Function>::New(tp1->GetFunction());
	exports->Set(String::NewSymbol("OracleOperator"),constructor);
	
}

Handle<Value>OracleOperator::New(const Arguments& args)
{
	HandleScope scope;
	OracleOperator* oracle = new OracleOperator();
	oracle->Wrap(args.This());
	return args.This();
}


Handle<Value>OracleOperator::ConnectSync(const Arguments&args)
{
	HandleScope scope;
	String::Utf8Value strDblink(args[0]);
	String::Utf8Value strUserName(args[1]);
	String::Utf8Value strPwd(args[2]);
	
	const char* pDblink = ToCString(strDblink);
	const char* pUsername = ToCString(strUserName);
	const char* pPwd = ToCString(strPwd);
	//cout<<pDblink<<endl;
	//cout<<pUsername<<endl;
	//cout<<pPwd<<endl;

	bool bRet = connect_.connect(pDblink,pUsername,pPwd);
	return scope.Close(Boolean::New(bRet));
}

 Handle<Value>OracleOperator::Connect(const Arguments& args)
 {
	 HandleScope scope;
	 if(!args[args.Length()-1]->IsFunction() || args.Length() != 4)
	 {
		 ThrowException(Exception::TypeError(String::New("Number of parameters is wrong or expect a function!")));
	 }
	
	 vector<string> parameters;
	 String::Utf8Value strDblink(args[0]);
	 String::Utf8Value strUserName(args[1]);
	 String::Utf8Value strPwd(args[2]);
	 const char* pDblink = ToCString(strDblink);
	 const char* pUsername = ToCString(strUserName);
	 const char* pPwd = ToCString(strPwd);
	 parameters.push_back(pDblink);
	 parameters.push_back(pUsername);
	 parameters.push_back(pPwd);
	 bool bRet = false;
	 OracleOperator *self = ObjectWrap::Unwrap<OracleOperator>(args.This());
	 Local<Function> callback = Local<Function>::Cast(args[args.Length()-1]);
	 uv_work_t* req = new uv_work_t;
	 uv_loop_t* loop = uv_default_loop();
	
	 req->data = new BUNDLE(self,Persistent<Function>::New(callback),bRet,parameters,"");
	 uv_queue_work(loop,req,connect_doing_process,connect_after_process);
	 self->Ref();
	 return Undefined();

	 
 }
Handle<Value>OracleOperator::QuerySync(const Arguments& args)
{
	HandleScope scope;
	//String::Utf8Value str(args[0]);
	//const char * pstr = ToCString(str);
	string sql = MergeSql(args);
	bool bRet = false;
	bRet = cursor_->query(sql.c_str());
	if(false == bRet)
	{
			return scope.Close(Boolean::New(false)); 
	}
	int recordNums = cursor_->getRecordNums();
	Local<Array> array = Array::New(recordNums);
	int size = cursor_->getColNums();
	bRet = cursor_->moveFirst();
	
	int j = 0;
	while(bRet)
	{
		Local<Object> obj = Object::New();
		for(int i=0;i< size;i++)
		{
			const char* pValue = cursor_->getFieldValue(i+1);
			const char* pColName = cursor_->getColName(i+1);
			//cout<<pColName<<endl;
			obj->Set(String::NewSymbol(pColName), 
			String::New(encodeConv::CodingConv::ascii2Utf8(pValue).c_str()));
			
		}
		array->Set(j,obj);
		bRet = cursor_->moveNext();
		j++;
			
	}
	return scope.Close(array);


}


Handle<Value>OracleOperator::Query(const Arguments& args)
{
	HandleScope scope;
	if(!args[args.Length()-1]->IsFunction())
	{
		ThrowException(Exception::TypeError(String::New("expect a function!")));
	}
	string sql = MergeSql(args);
	bool bRet = false;
	OracleOperator *self = ObjectWrap::Unwrap<OracleOperator>(args.This());
	Local<Function>callback = Local<Function>::Cast(args[args.Length()-1]);
	uv_work_t* req = new uv_work_t;
	uv_loop_t* loop = uv_default_loop();
	req->data = new BUNDLE(self,Persistent<Function>::New(callback),bRet,vector<string>(),sql);
	uv_queue_work(loop, req, query_doing_process, query_after_process);    //交给 libuv 处理
	//增加 JS 对象的引用计数
	self->Ref();
	return Undefined();
}

Handle<Value>OracleOperator::ExecuteSync(const Arguments&args)
{
	HandleScope scope;
	string sql = MergeSql(args);
	bool bRet = cursor_->execute(sql.c_str());
	

	return scope.Close(Boolean::New(bRet));



}

Handle<Value>OracleOperator::Execute(const Arguments&args)
{
	HandleScope scope;
	if(!args[args.Length()-1]->IsFunction())
	{
		ThrowException(Exception::TypeError(String::New("expect a function!")));
	}

	string sql = MergeSql(args);
	bool bRet = false;
	OracleOperator *self = ObjectWrap::Unwrap<OracleOperator>(args.This());
	Local<Function>callback = Local<Function>::Cast(args[args.Length()-1]);
	uv_work_t* req = new uv_work_t;
	uv_loop_t* loop = uv_default_loop();
	req->data = new BUNDLE(self,Persistent<Function>::New(callback),bRet,vector<string>(),sql);
	uv_queue_work(loop, req, execute_doing_process, connect_after_process);    //交给 libuv 处理

	//增加 JS 对象的引用计数
	self->Ref();
	return Undefined();

	
}
Handle<Value>OracleOperator::CloseSync(const Arguments&args)
{
	HandleScope scope;
	
	connect_.disconnect();
	if(cursor_ != NULL)
	{
		delete cursor_;
		cursor_ = NULL;
	}


	
	return scope.Close(Boolean::New(true));




}
Handle<Value>OracleOperator::BeginTran(const Arguments&args)
{
	HandleScope scope;
	bool bRet =cursor_->beginTran();
	return scope.Close(Boolean::New(bRet));
}
Handle<Value>OracleOperator::RollBack(const Arguments&args)
{
	HandleScope scope;
	bool bRet =cursor_->rollbackTran();
	return scope.Close(Boolean::New(bRet));
}
Handle<Value>OracleOperator::Commit(const Arguments&args)
{
	HandleScope scope;
	bool bRet =cursor_->commitTran();
	return scope.Close(Boolean::New(bRet));
}

Handle<Value>OracleOperator::GetError(const Arguments&args)
{
	HandleScope scope;
	const char* pError = OCIException::getErrorMsg();
	return scope.Close(String::New(encodeConv::CodingConv::ascii2Utf8(pError).c_str()));
}
void OracleOperator::connect_doing_process(uv_work_t *req)
{

	//处理数据，在这里不能处理 JS 对象，否者会出错

	BUNDLE& turtle = * static_cast<BUNDLE*>(req->data);
	OracleOperator* self = std::get<0>(turtle);
	vector<string> parameters = get<3>(turtle);
	//Arguments args =  get<5>(turtle); //不支持有问题
	//Handle<Value> value = Connect(args);
	//Handle<Boolean> b = value->ToBoolean();
	//bool bRet = b->Value();
	bool bRet  = self->connect_.connect(parameters[0].c_str(),parameters[1].c_str(),parameters[2].c_str());
	get<2>(turtle)= bRet;
}

void OracleOperator::connect_after_process(uv_work_t *req)
{
    HandleScope scope;
    BUNDLE* bundle = static_cast<BUNDLE*>(req->data);
    BUNDLE& turtle = *bundle;
    Persistent<Function>& callback = std::get<1>(turtle);
	bool bRet = get<2>(turtle);
	Local<Value>args[1];
	if (false == bRet)
	{
		const char * pError = OCIException::getErrorMsg();
		args[0] =String::New(encodeConv::CodingConv::ascii2Utf8(pError).c_str());
	}
	else
	{
		args[0]=(*Undefined());
	}
    TryCatch try_catch;    //用于捕获 JS 代码的错误
    //调用回调函数

    callback->Call(Context::GetCurrent()->Global(), 1, args);
    if(try_catch.HasCaught())
    {
        node::FatalException(try_catch);
    }
    std::get<0>(turtle)->Unref();    //释放 JS 对象
    callback.Dispose();    // Persistent 句柄需要自己释放资源
    delete bundle;
	bundle = NULL;
}


void OracleOperator::execute_doing_process(uv_work_t* req)
{
	BUNDLE& turtle = * static_cast<BUNDLE*>(req->data);

	OracleOperator* self = std::get<0>(turtle);
	string  sql = get<4>(turtle);
	//Arguments args =  get<5>(turtle); //不支持有问题
	//Handle<Value> value = Connect(args);
	//Handle<Boolean> b = value->ToBoolean();
	//bool bRet = b->Value();
	bool bRet  = self->cursor_->execute(sql.c_str());
	get<2>(turtle)= bRet;

}
void OracleOperator::query_doing_process(uv_work_t* req)
{

}

void OracleOperator::query_after_process(uv_work_t* req)
{
	HandleScope scope;
	BUNDLE* bundle = static_cast<BUNDLE*>(req->data);
	BUNDLE& turtle = *bundle;
	Persistent<Function>&callback = get<1>(turtle);
	string sql = get<4>(turtle);
	bool bRet = cursor_->query(sql.c_str());
	TryCatch trycath;
	Local<Value>args[2];
	if (bRet == false)
	{
		const char* pError = OCIException::getErrorMsg();
		args[0] = String::New(encodeConv::CodingConv::ascii2Utf8(pError).c_str());
		args[1] = *(Undefined());

	}
	else
	{
		args[0]=*Undefined();
		int recordNums = cursor_->getRecordNums();
		Local<Array> array = Array::New(recordNums);
		int size = cursor_->getColNums();
		bRet = cursor_->moveFirst();

		int j = 0;
		while(bRet)
		{
			Local<Object> obj = Object::New();
			for(int i=0;i< size;i++)
			{
				const char* pValue = cursor_->getFieldValue(i+1);
				const char* pColName = cursor_->getColName(i+1);
				//cout<<pColName<<endl;
				obj->Set(String::NewSymbol(pColName), 
					String::New(encodeConv::CodingConv::ascii2Utf8(pValue).c_str()));

			}
			array->Set(j,obj);
			bRet = cursor_->moveNext();
			j++;

		}

		args[1] = array;

	}
	

	callback->Call(Context::GetCurrent()->Global(),2,args);
	if (trycath.HasCaught())
	{
		node::FatalException(trycath);
	}
	get<0>(turtle)->Unref();
	callback.Dispose();
	delete bundle;
	bundle = NULL;
	
	

	

}
//void OracleOperator::after_process(uv_work_t *req)
//
//{
//
//	HandleScope scope;
//
//	BUNDLE* bundle = static_cast<BUNDLE*>(req->data);
//
//	BUNDLE& turtle = *bundle;
//
//	Persistent<Function>& callback = std::get<2>(turtle);
//	//const Arguments& args2 = std::get<3>(turtle);
//
//	/*String::Utf8Value strDblink(args2[0]);
//	String::Utf8Value strUserName(args2[1]);
//	String::Utf8Value strPwd(args2[2]);
//
//	const char* pDblink = ToCString(strDblink);
//	const char* pUsername = ToCString(strUserName);
//	const char* pPwd = ToCString(strPwd);
//	cout<<"pDblink"<<pDblink<<endl;
//	cout<<pUsername<<endl;
//	cout<<pPwd<<endl;*/
//	/*Handle<Value>ret;
//	ret =ConnectSync(args2);
//	Handle<Boolean> bRet = ret->ToBoolean();
//
//	Local<Value> args[1];
//	if (bRet->Value() ==false)
//	{
//	const char * pError = OCIException::getErrorMsg();
//
//	args[0] =String::New(encodeConv::CodingConv::ascii2Utf8(pError).c_str());
//	cout<<pError<<endl;
//
//	}
//	else
//	{
//	cout<<"Undefined"<<endl;
//	args[0]=(*Undefined());
//	}
//	*/
//
//
//
//	//Arguments args2;
//	//ConnectSync(args);
//	Local<Value> args[] = { String::New("zjs") };
//
//	TryCatch try_catch;    //用于捕获 JS 代码的错误
//
//
//
//	//在 Addon 中调用 console 对象打印日志
//
//	Handle<Value> handle;
//
//	Handle<Object> global = Context::GetCurrent()->Global();
//
//	handle = global->Get(String::New("console"));
//
//	if(handle->IsObject())
//
//	{
//
//		Handle<Object> console = Handle<Object>::Cast(handle);
//
//		handle = console->Get(String::New("log"));
//
//		if(handle->IsFunction())
//
//		{
//
//			Handle<Function> log = Handle<Function>::Cast(handle);
//
//			Handle<Value> args[] = { String::New("doing_process call console.log") };
//
//			log->Call(global, 3, args);
//
//		}
//
//	}
//
//
//
//	//调用回调函数
//
//	callback->Call(Context::GetCurrent()->Global(), 1, args);
//
//
//
//	if(try_catch.HasCaught())
//
//	{
//
//		node::FatalException(try_catch);
//
//	}
//
//
//
//	std::get<0>(turtle)->Unref();    //释放 JS 对象
//
//	callback.Dispose();    // Persistent 句柄需要自己释放资源
//
//	delete bundle;
//
//}