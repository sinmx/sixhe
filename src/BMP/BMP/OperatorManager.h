//
//COperatorManager	������־ģ�飬�ṩ������־��ط���
//

#pragma once

class COperatorManager
{
protected:
	COperatorManager(void);
	~COperatorManager(void);

public:
	//��ȡ��һʵ��
	static COperatorManager* Instance();

	//��ʼ��ģ��
	void Init();

	//������־
	CString GetBuyLog();
	bool ClearBuyLog();
	bool AppendBuyLog(const CString& strNewLog);

private:
	//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	bool InsertBuyLog(const CString& strLog);		//������ע��־�����ݿ��У�����������ݣ��滻֮
	CString AppendTime(const CString& strLog);		//��־����ǰ��׷��ʱ��

private:
	static CCS m_synCCs;			//����ͬ���ⲿ����
	CString m_strBuyLog;	//��ע������־
};