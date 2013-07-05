#ifndef _OCIDEF_H_
#define  _OCIDEF_H_

#include "platform_config.h"
namespace OCI
{



	//�쳣��Ϣ
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

		static OCIException* createOCIExecption();					//����ģʽ
		static bool checkError(OCIError* pErr,sb2 status);			//���status�µĴ���
		static const char* getErrorMsg();							//��ȡ������Ϣ
	private:

		static sb2 errorCode;										//�������
		static char msg[512];										//������Ϣ
		static OCIException* pExecption;
	};
	//ORACLE ���ݿ�����
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

		bool connect(const char* dblink,const char* username,const char* password);  //�������ݿ�����dblinkΪ���ݿ�ʵ����
		void disconnect();															//�Ͽ�����	
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
		OCIEnv*			envhp;      //�������
		OCIError*		errhp;		//������
		OCIServer*		srvhp;		//������
		OCISvcCtx*		svchp;		//���������ľ��
		OCISession*		usrhp;		//�û��Ự���
		OCIDescribe*	dschp;		//�������
		OCITrans*		txnhp;		//������




	};


	//���ӳ�
	class OCIConPool
	{
	private:
		OCIConPool()
		{

		}
	public:
		enum{MAX_NUM= 20};

	private:
		std::vector<OCIConnection*>conns; //��������




	};




	



	//�α�
	class OCICursor
	{
	public:
		OCICursor(void);
		virtual ~OCICursor(void);
		OCICursor(OCIConnection* pConn);



	private:
		OCIConnection* pConn;						//����
		OCIStmt* stmthp;							//�����
		OCIParam* colhd;							//�о��
		OCIBind* bindhp;							//�󶨾��
		bool bOpen;									//�Ƿ��
		std::vector<void*>rowBuffers;				//�еĻ�����	
		std::vector<OCIDefine *> defines;			//����������
		std::vector <sb2>ind;						//��ָʾ������
		std::vector<int>colsType;					//����������
		std::vector<int>colsSize;					//�д�С����
		std::vector<int>colsScale;					//�е�С��λ������
		std::string strFieldValue;





	public:
		bool open();													//��
		void close();													//�ر�
		const char*getColName(const int& colnumber)const;				//�е�����
		ub2 getColType(const int& colnumber)const;						//�е�����
		ub2 getColSize(const int& colnumber)const;						//�еĳ���
		ub2 getColScale(const int& colnumber)const;						//С������λ��
		ub4 getColNums()const;											//�еĸ���
		ub4 getRecordNums();											//��ȡ��¼�ĸ���
		bool query(const char* sql);									//��ѯ���
		bool bindQuery(const char* sql);								//�󶨲�ѯ
		bool moveFirst(int rows=1);										//
		bool moveNext(int rows=1);										//��ȡ���
		bool moveLast(int rows=1);										
		const char* getFieldValue(const int& colnumber);				//��ȡ�ֶ�ֵ���Դ�����ʽ����
		bool execute(const char* sql);									//ִ�в���ɾ�����µ�
		bool beginTran();
		bool commitTran();
		bool rollbackTran();

		bool bindByPos(ub4 pos,void* value,sb4 valuesize,void*indicator,sb4 type);
		bool bindByName(const char* name,void* value,sb4 valuesize,void*indicator,sb4 type);
		bool prepare(const char* stmt);									//׼��SQL���


	private:

		bool executeSql(ub4 iters=0,ub4 mode=OCI_DEFAULT);				//ִ��SQL����ȡ�����
		bool allocateCursor();											//Ϊ�α����ռ�
		double oicNumberToReal(const OCINumber* ociNum);				//NUMBER����תΪdouble
		void free();													//�ͷ�rowBuffers3 new�������Դ


	};

}

#endif
