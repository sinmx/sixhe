//
//类名：CDBUtil	数据库操作类
//职责：用于数据库的相关操作：建表、删表、插入行、删除行，使用sqlite库
//

#pragma once;

#include "sqlite3.h"
#include <vector>
using namespace std;

enum  DB_DATA_TYPE	//数据库数据类型
{
	DB_DATA_TYPE_INT,		//整数
	DB_DATA_TYPE_STR,		//字符串
};

typedef struct _tagColumnItem  //列
{
	CString strName;	//列名
	DB_DATA_TYPE dataType;	//数据类型
	CString strValue;	//值，都用字符串表示，整数10就用"10"表示
	bool IsPrimaryKey;	//是否为主键

	_tagColumnItem()
	{
		dataType = DB_DATA_TYPE_INT;
		IsPrimaryKey = false;
	}

} COLUMN_ITEM, *PCOLUMN_ITEM;

//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
typedef bool (WINAPI *PEnumRecord)(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

class CDBUtil
{
public:
	CDBUtil();
	~CDBUtil();

	enum
	{
		ERROR_DBUtil_Success = 0,		//成功
		ERROR_DBUtil_Error = 1,			//错误
		ERROR_DBUtil_HasOpen = 2,		//已经打开
		ERROR_DBUtil_ToUtf8 = 3,		//转化为utf8时失败
		ERROR_DBUtil_HasNotOpen = 4,  //数据库尚未打开

		//sqlite lib库调用失败，返回值在其API基础上加上该基值
		ERROR_DBUtil_SQLiteLib_Base = 100,	
	};

public:
	//打开关闭数据库，strDBFile数据库文件完整路径，返回值0表示成功，其他值表示错误代码
	int Open(const CString& strDBFile);
	int Close();

	//建表删表，columnVec中的strValue无意义，返回值0表示成功，其他值表示错误代码
	int CreateTable(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);
	int DropTable(const CString& strTableName);

	//插入一行数据，columnVec中的dataType和IsPrimaryKey无意义
	//返回值0表示成功，其他值表示错误代码
	int Insert(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);

	//插入多行数据调用该函数效率更高，columnVec中的dataType和IsPrimaryKey无意义
	//返回值0表示成功，其他值表示错误代码
	int Insert(const CString& strTableName, const vector<vector<COLUMN_ITEM>>& columnVec);

	//删除数据，columnVec存放满足条件：指定列为指定值，如果没指定任何条件就删除整个表
	//返回值0表示成功，其他值表示错误代码
	int Delete(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);

	//查询满足whereVec的所有记录，查询过程中回调EnumRecord函数，将context及查询记录作为其参数
	//返回值0表示成功，其他值表示错误代码
	int Query(const CString& strTableName, const vector<COLUMN_ITEM>& whereVec, PEnumRecord EnumRecord, LPARAM context);

protected:
	static char* ToUtf8(const CString& str);		//转化成utf8编码格式
	static CString FromUtf8(const char* szStr); //将utf8转化成unicode或多字节字符集
	void OutputSqliteError(const CString& strPrefix, int nErrorCode);  //sqlite错误信息输出，可用debugview查看
	static int ExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName);  //sql执行回调函数
	CString ConstructInsertSqlStatement(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec);  //构造一条SQL语句

protected:
	sqlite3* m_pDBConnection;			//数据库连接
};