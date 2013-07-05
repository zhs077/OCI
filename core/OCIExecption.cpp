#include "ocidef.h"

namespace OCI
{


	char OCIException::msg[512] = {0};
	sb2 OCIException::errorCode = -1;
	OCIException* OCIException::pExecption = new OCIException();  //饿汉模式不需要加锁


	bool OCIException::checkError(OCIError* pErr,sb2 status)
	{

		errorCode = status;

		switch(errorCode)
		{

		case OCI_SUCCESS:
			strncpy_s(msg,512,"OCI_SUCCESS",strlen("OCI_SUCCESS"));
			break;
		case OCI_INVALID_HANDLE:
			strncpy_s(msg,512,"OCI_INVALID_HANDLE",strlen("OCI_INVALID_HANDLE"));
			break;
		case OCI_ERROR:
			OCIErrorGet(pErr,1,NULL,(sb4*)&errorCode,(text*)msg,sizeof(msg),OCI_HTYPE_ERROR);
			break;
		case OCI_STILL_EXECUTING:
			strncpy_s(msg,512,"OCI_STILL_EXECUTING",strlen("OCI_STILL_EXECUTING"));
			break;
		case OCI_NEED_DATA:
			strncpy_s(msg,512,"OCI_NEED_DATA",strlen("OCI_NEED_DATA"));
			break;
		case OCI_SUCCESS_WITH_INFO:
			strncpy_s(msg,512,"OCI_SUCCESS_WITH_INFO",strlen("OCI_SUCCESS_WITH_INFO"));
			break;
		case OCI_NO_DATA:
			strncpy_s(msg,512,"OCI_NO_DATA",strlen("OCI_NO_DATA"));
			break;
	
		default:
			break;

		}
		if (OCI_SUCCESS == status || OCI_SUCCESS_WITH_INFO == status)
		{
			return true;
		}





		return false;
	}


	const char* OCIException::getErrorMsg()
	{
		return msg;
	}

	OCIException*  OCIException::createOCIExecption()
	{
		return pExecption;
	}



}
