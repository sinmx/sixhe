//
//������CDBUtil	���ݿ������
//ְ���������ݿ����ز���������ɾ�������С�ɾ���У�ʹ��sqlite��
//

#pragma once;

#include "sqlite3.h"
#include <vector>
using namespace std;

enum  DB_DATA_TYPE	//���ݿ���������
{
	DB_DATA_TYPE_INT,		//����
	DB_DATA_TYPE_STR,		//�ַ���
};

typedef struct _tagColumnItem  //��
{
	CString strName;	//����
	DB_DATA_TYPE dataType;	//��������
	CString strValue;	//ֵ�������ַ�����ʾ������10����"10"��ʾ
	bool IsPrimaryKey;	//�Ƿ�Ϊ����

	_tagColumnItem()
	{
		dataType = DB_DATA_TYPE_INT;
		IsPrimaryKey = false;
	}

} COLUMN_ITEM, *PCOLUMN_ITEM;

//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
typedef bool (WINAPI *PEnumRecord)(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

class CDBUtil
{
public:
	CDBUtil();
	~CDBUtil();

	enum
	{
		ERROR_DBUtil_Success = 0,		//�ɹ�
		ERROR_DBUtil_Error = 1,			//����
		ERROR_DBUtil_HasOpen = 2,		//�Ѿ���
		ERROR_DBUtil_ToUtf8 = 3,		//ת��Ϊutf8ʱʧ��
		ERROR_DBUtil_HasNotOpen = 4,  //���ݿ���δ��

		//sqlite lib�����ʧ�ܣ�����ֵ����API�����ϼ��ϸû�ֵ
		ERROR_DBUtil_SQLiteLib_Base = 100,	
	};

public:
	//�򿪹ر����ݿ⣬strDBFile���ݿ��ļ�����·��������ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Open(const CString& strDBFile);
	int Close();

	//����ɾ��columnVec�е�strValue�����壬����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int CreateTable(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);
	int DropTable(const CString& strTableName);

	//����һ�����ݣ�columnVec�е�dataType��IsPrimaryKey������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Insert(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);

	//����������ݵ��øú���Ч�ʸ��ߣ�columnVec�е�dataType��IsPrimaryKey������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Insert(const CString& strTableName, const vector<vector<COLUMN_ITEM>>& columnVec);

	//ɾ�����ݣ�columnVec�������������ָ����Ϊָ��ֵ�����ûָ���κ�������ɾ��������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Delete(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);

	//��ѯ����whereVec�����м�¼����ѯ�����лص�EnumRecord��������context����ѯ��¼��Ϊ�����
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Query(const CString& strTableName, const vector<COLUMN_ITEM>& whereVec, PEnumRecord EnumRecord, LPARAM context);

protected:
	static char* ToUtf8(const CString& str);		//ת����utf8�����ʽ
	static CString FromUtf8(const char* szStr); //��utf8ת����unicode����ֽ��ַ���
	void OutputSqliteError(const CString& strPrefix, int nErrorCode);  //sqlite������Ϣ���������debugview�鿴
	static int ExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName);  //sqlִ�лص�����
	CString ConstructInsertSqlStatement(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);  //����һ��SQL���

protected:
	sqlite3* m_pDBConnection;			//���ݿ�����
};