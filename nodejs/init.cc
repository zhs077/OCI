
#define BUILDING_NODE_EXTENSION
#define NODEJS
#include<node.h>
using namespace v8;
#include"node_oracle.h"


void InitAll(Handle<Object>exports)
{
	OracleOperator::Init(exports);
	
}

NODE_MODULE(oracle_win32,InitAll)