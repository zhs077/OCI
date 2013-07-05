#include "ocidef.h"
namespace OCI
{


//#define  FMT			"DAY,MONTH DD,YYYY"
//#define  LANG			"American"
#define FMT "Month dd, YYYY, HH:MI A.M."
#define LANG "American"

	OCICursor::OCICursor(void)
	{
	}

	OCICursor::~OCICursor(void)
	{
		close();
	}
	OCICursor::OCICursor(OCI::OCIConnection *pConn)
	{
		assert(pConn != NULL);
		this->pConn = pConn;
		bOpen = false;
		stmthp = NULL;
		colhd = NULL;
		bindhp = NULL;

	}

	ub4 OCICursor::getRecordNums()
	{
		int numrows_ = 0;
		sword ret;
		//获取查询得到的记录行数
		moveLast();
		
		ret = OCIAttrGet((dvoid*)stmthp, (ub4)OCI_HTYPE_STMT, (dvoid*)&numrows_,
			(ub4*)0, (ub4)OCI_ATTR_ROW_COUNT, (OCIError*)pConn->errhp);
		moveFirst();
			
		return (numrows_);
	}
	const char* OCICursor::getColName(const int &colnumber)const
	{
		sword ret;
		text *colname = NULL;
		ub4	colnamesz;
		ret = OCIParamGet(stmthp,OCI_HTYPE_STMT,pConn->errhp,(dvoid**)&colhd,colnumber);
		ret = OCIAttrGet(colhd,OCI_DTYPE_PARAM,(dvoid**)&colname,&colnamesz,OCI_ATTR_NAME,pConn->errhp);
		return (const char*)colname;
	}

	ub2 OCICursor::getColType(const int &colnumber) const
	{
		//列的类型
		ub2	dtype = 0;
		sword ret;
		ret = OCIParamGet((dvoid*)stmthp,OCI_HTYPE_STMT,pConn->errhp,(dvoid**)&colhd,colnumber);
		ret = OCIAttrGet((dvoid*)colhd,OCI_DTYPE_PARAM,(dvoid*)&dtype,0,OCI_ATTR_DATA_TYPE,pConn->errhp);
		return dtype;
	}

	ub2 OCICursor::getColSize(const int& colnumber)const
	{

		ub2  colsize = (ub2) 0;
		sword ret;
		ret = OCIParamGet(stmthp,OCI_HTYPE_STMT,pConn->errhp,(dvoid**)&colhd,colnumber);
		ret = OCIAttrGet(colhd,OCI_DTYPE_PARAM,(dvoid*)&colsize,0,OCI_ATTR_DATA_SIZE,pConn->errhp);
		return colsize;
	}
	ub2 OCICursor::getColScale(const int& colnumber)const
	{
		ub2  colscale = (ub2) 0;
		sword ret;
		//得到第i列的参数
		ret = OCIParamGet(stmthp, OCI_HTYPE_STMT,pConn->errhp, (dvoid**)&colhd, colnumber);
		//获取第i列的具体属性
		ret = OCIAttrGet(colhd,OCI_DTYPE_PARAM,(dvoid *)&colscale, (ub4*)0, (ub4)OCI_ATTR_SCALE, pConn->errhp );


		return colscale;
	}

	ub4 OCICursor::getColNums()const
	{
		ub4  colnums = (ub4) 0;
		sword ret ;
		ret = OCIAttrGet(stmthp,OCI_HTYPE_STMT,&colnums,0,OCI_ATTR_PARAM_COUNT,pConn->errhp);
		return colnums;

	}
	void OCICursor::close()
	{
		if (bOpen)
		{
			OCIHandleFree(stmthp,OCI_HTYPE_STMT);
			free();
			bOpen = false;
		}
	}
	bool OCICursor::open()
	{
		//分配句子句柄
		sword ret;
		ret = OCIHandleAlloc((dvoid *)pConn->envhp,(dvoid **)&stmthp,OCI_HTYPE_STMT,0,0);

		bOpen = OCIException::checkError(pConn->errhp,ret);

		return bOpen;
	}


	bool  OCICursor::prepare(const char* stmt)
	{
		if (!bOpen)
		{
			open();
		}
		sword ret;
		ret = OCIStmtPrepare(stmthp,pConn->errhp,(text*)stmt,strlen(stmt),OCI_NTV_SYNTAX,OCI_DEFAULT);
		return OCIException::checkError(pConn->errhp,ret);

	}

	bool OCICursor::execute(const char* sql)
	{

		bool bRet = false;
		if( false == prepare(sql))
		{
			return false;
		}
		bRet = executeSql(1);
		return bRet;

	}
	bool OCICursor::query(const char* sql)
	{
		assert(sql);
		//bool bRet = false;

		if( false == prepare(sql))
		{
			return false;
		}

		if (!executeSql(0,OCI_STMT_SCROLLABLE_READONLY))
		{
			return false;

		}

		return  allocateCursor();

	}

	bool OCICursor::bindQuery(const char* sql)
	{
		assert(sql);
		//bool bRet = false;

		if (!executeSql(0,OCI_DEFAULT))
		{
			return false;

		}
		return  allocateCursor();

	}


	bool OCICursor::moveFirst(int rows/* =1 */)
	{

		sword ret;
		ret = OCIStmtFetch2(stmthp,pConn->errhp,rows,OCI_FETCH_FIRST,0,OCI_DEFAULT);
		return OCIException::checkError(pConn->errhp,ret);
	}
	bool OCICursor::moveNext(int rows/* =1 */)
	{

		sword ret;
		ret = OCIStmtFetch2(stmthp,pConn->errhp,rows,OCI_FETCH_NEXT,0,OCI_DEFAULT);
		return OCIException::checkError(pConn->errhp,ret);
	}

	bool OCICursor::moveLast(int rows/* =1 */)
	{

		sword ret;
		ret = OCIStmtFetch2(stmthp,pConn->errhp,rows,OCI_FETCH_LAST,0,OCI_DEFAULT);
		return OCIException::checkError(pConn->errhp,ret);
	}

	bool OCICursor::executeSql(ub4 iters,ub4 mode)
	{
		sword ret;
		ret = OCIStmtExecute(pConn->svchp,stmthp,pConn->errhp,iters,(ub4)0,(OCISnapshot *)NULL,(OCISnapshot *)NULL,mode);
		return OCIException::checkError(pConn->errhp,ret);

	}

	bool OCICursor::allocateCursor()
	{
		ub4			colType=0;			//列的类型
		ub4			colSize=0;			//列的长度
		ub4			colScale=0;			//小数点位数
		//int			iValue = 0;
		double		dValue=0.0;
		char*		pTmp = NULL;
		sword		ret;

		//free();//释放资源
		ub4  colNums = getColNums();
		ind.resize(colNums);
		defines.resize(colNums);
		colsType.resize(colNums);
		colsSize.resize(colNums);
		colsScale.resize(colNums);
		rowBuffers.resize(colNums);



		for (ub4 i = 1; i <= colNums; ++i)
		{

			colType = getColType(i);
			colSize = getColSize(i);
			colScale = getColScale(i);
			//const char* s = getColName(i);
			colsType[i-1] = colType;
			colsSize[i-1] = colSize;
			colsScale[i-1] = colScale;

			switch(colType)
			{

			case SQLT_CHR:
			case SQLT_AFC:
			case SQLT_VCS:

				rowBuffers[i-1] = (char*)new char[colSize+1];
				memset(rowBuffers[i-1],0,colSize+1);
				ret = OCIDefineByPos(stmthp,&defines[i-1],pConn->errhp,i,(dvoid*)rowBuffers[i-1],colSize+1,
					SQLT_STR,(dvoid*)&ind[i-1],0,0,OCI_DEFAULT);
				break;

			case SQLT_NUM:
			case SQLT_INT:
				if(colScale <=0)    //这里由于oracle的INTEGER类型为64位，采用double来表示
				{
					rowBuffers[i-1] = (double *)new double ;
					ret = OCIDefineByPos(stmthp,&defines[i-1],pConn->errhp,i,(dvoid*)rowBuffers[i-1],sizeof(double),
						SQLT_FLT,(dvoid*)&ind[i-1],0,0,OCI_DEFAULT);

				}
				else
				{

					rowBuffers[i-1] = (OCINumber *) new OCINumber;
					ret = OCIDefineByPos(stmthp,&defines[i-1],pConn->errhp,i,(dvoid*)rowBuffers[i-1],sizeof(OCINumber),
						SQLT_VNU,(dvoid*)&ind[i-1],0,0,OCI_DEFAULT);
				}
				break;

			case SQLT_IBDOUBLE:
			case SQLT_IBFLOAT:


				rowBuffers[i-1] = (double *) new double;
				ret = OCIDefineByPos(stmthp,&defines[i-1],pConn->errhp,i,
					(dvoid*)rowBuffers[i-1],8,
					4,(dvoid*)&ind[i-1],0,0,OCI_DEFAULT);
				break;

			case SQLT_DAT:

				rowBuffers[i-1] = (OCIDate*)new OCIDate;

				ret = OCIDefineByPos(stmthp,&defines[i-1],pConn->errhp,
					i,(dvoid*)rowBuffers[i-1],sizeof(OCIDate),colType,
					(dvoid*)&ind[i-1],0,0,OCI_DEFAULT);
				break;

			default:
				assert(false);
				break;




			}

			/*	if (ret != OCI_SUCCESS)
			{
			OCIException::checkError(pConn->errhp,ret);
			return false;
			}*/


		}

		return true;


	}




	const char* OCICursor::getFieldValue(const int& colnumber)
	{

		assert(colnumber >= 1);
		ub4 coltype = colsType[colnumber-1]; 
		ub4 colsize = colsSize[colnumber-1]; 
		ub4 colScale = colsScale[colnumber-1];
		int indi = ind[colnumber-1];//是否为NULL标志
		char			str[1000]={0};
		double			dValue;
		//int				iValue;
		//ub4				length;
		OCIDate *date;
		//int ret;
		switch(coltype)
		{

		case SQLT_CHR:
		case SQLT_AFC:
		case SQLT_VCS:

			if( -1 == indi)
			{	
				return "";	
			}

			strFieldValue = (const char*)rowBuffers[colnumber-1];

			return (const char*)rowBuffers[colnumber-1];


		case SQLT_NUM:
		case SQLT_INT:

			if( -1 == indi)
			{	
				return "0";	
			}


			if(colScale <=0)
			{
				 dValue = *(const double*)rowBuffers[colnumber -1];
				sprintf_s(str,sizeof(str),"%.f",dValue);                  //这里由于oracle的INTEGER类型为64位，采用double来表示

			}
			else  //针对Number有精度的情况
			{
				dValue = oicNumberToReal((OCINumber*)rowBuffers[colnumber-1]); //最多支持小数点六位 2013-7-3
				sprintf_s(str,sizeof(str),"%lf",dValue);
			}

			break;


		case SQLT_IBDOUBLE:
		case SQLT_IBFLOAT:

			if( -1 == indi)
			{	
				return "0";	
			}

			dValue =  *((const double*)rowBuffers[colnumber-1]);
			sprintf_s(str,sizeof(str),"%lf",dValue);

			break;

		case SQLT_DAT:
			if( -1 == indi)
			{	
				return "";	
			}


			
			date = (OCIDate *)rowBuffers[colnumber-1];
		
			sprintf_s(str,sizeof(str),"%d-%02d-%02d %02d:%02d:%02d",date->OCIDateYYYY,date->OCIDateMM,date->OCIDateDD
						,date->OCIDateTime.OCITimeHH,date->OCIDateTime.OCITimeMI,date->OCIDateTime.OCITimeSS);

		
		/*	length = sizeof(str);
			ret  = OCIDateToText(pConn->errhp,(OCIDate*)rowBuffers[colnumber-1],(text*)FMT,sizeof(FMT)-1,(text*)LANG,sizeof(LANG)-1
				,&length,(text*)str);
			OCIException::checkError(pConn->errhp,ret);*/
			/*ret  = OCIDateToText(pConn->errhp,(OCIDate*)rowBuffers[colnumber-1],NULL,sizeof(FMT)-1,NULL,sizeof(LANG)-1
				,&length,(text*)str);
			OCIException::checkError(pConn->errhp,ret);*/

			break;
		default:

			break;


		}

		strFieldValue = str;
		return strFieldValue.c_str();


	}


	bool OCICursor::beginTran()
	{

		return OCIException::checkError(pConn->errhp,
			OCITransStart(pConn->svchp,pConn->errhp,60,OCI_TRANS_NEW));


	}
	bool OCICursor::commitTran()
	{
		return OCIException::checkError(pConn->errhp,
			OCITransCommit(pConn->svchp,pConn->errhp,0));

	}
	bool OCICursor::rollbackTran()
	{
		return OCIException::checkError(pConn->errhp,
			OCITransRollback(pConn->svchp,pConn->errhp,0));

	}

	bool  OCICursor::bindByPos(ub4 pos,void* value,sb4 valuesize,void*indicator,sb4 type)
	{
		return OCIException::checkError(pConn->errhp,OCIBindByPos(stmthp,&bindhp,pConn->errhp,pos,
			(void*)value,valuesize,type,indicator,0,0,0,0,OCI_DEFAULT));
	}

	bool OCICursor::bindByName(const char* name,void* value,sb4 valuesize,void*indicator,sb4 type)
	{
		return OCIException::checkError(pConn->errhp,OCIBindByName(stmthp,&bindhp,pConn->errhp
			,(text*)name,-1,(void*)value,valuesize,type,indicator,0,0,0,0,OCI_DEFAULT));

	}
	double OCICursor::oicNumberToReal(const OCINumber* ociNum)
	{
		sword ret;
		double tempDouble = 0;
		ret = OCINumberToReal(pConn->errhp, ociNum, 
			(uword)sizeof(double), (dvoid *)&tempDouble);



		return tempDouble;
	}

	void OCICursor::free()
	{
		std::vector<void *>::iterator it = rowBuffers.begin();
		for (it; it != rowBuffers.end();++it)
		{
			delete *it;
			*it = NULL;
		}
		rowBuffers.clear();
		ind.clear();
		colsScale.clear();
		colsSize.clear();
		colsType.clear();
		defines.clear();
	}

}
