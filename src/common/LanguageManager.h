///////////////////////////////////////////////
//������CLanguageManager	���Թ�����
//ְ�𣺵�һʵ������������ʱ�������ԣ�������ȡ������ͬ�������ڽ��桢��Ϣ�����ֵ���ʾ�����������ļ���Ϊunicode��ʽ
//////////////////////////////////////////////////////////

#pragma once

#include "SynObject.h"
#include <vector>
using namespace std;

typedef struct tagSectionUnit   //���ÿ������
{
	CString strSecName;				//����
	CDWordArray saLanguageID;		//��ϢID�б�
	CStringArray saLanguageText;	//��Ϣ�б�����ϢIDһһ��Ӧ

	tagSectionUnit()
	{
		//
	}

	//�������캯�����Ա�֧��vector
	tagSectionUnit(const tagSectionUnit& val) : strSecName(val.strSecName)		
	{
		saLanguageID.Copy(val.saLanguageID);
		saLanguageText.Copy(val.saLanguageText);
	}

	//���ظ�ֵ��������Ա�֧��vector
	tagSectionUnit& operator= (const tagSectionUnit& val)
	{
		if (this == &val)  //just me
		{
			return *this;
		}

		strSecName = val.strSecName;		
		saLanguageID.Copy(val.saLanguageID);
		saLanguageText.Copy(val.saLanguageText);
		return *this;
	}

}SECTION_UNIT, *PSECTION_UNIT;

class CLanguageManager
{
protected:  //��ֹʵ����
	CLanguageManager();
	virtual ~CLanguageManager();

public:
	enum eLanguage_ID
	{
		LANGUAGE_CHS_SIMPLE = 1,	// ��������
		LANGUAGE_ENG = 2,          // Ӣ��
	};

	enum  //�������
	{
		Error_Language_Manager_Success = 0,		//�ɹ�
		Error_Language_Manager_File_Not_Found = 1,		//�ļ�������
		Error_Language_Manager_File_Open_Fail = 2,		//�ļ���ʧ��
	};

public:
	//��ȡ��һʵ������
	static CLanguageManager* Instance();

	//���ܣ�������������
	//������nLanguageID		�������		eLanguage_IDö��ֵ
	//		strLanguageFile	�������		�����ļ�����·�����մ�ʱ����������ΪEXE����Ŀ¼�µ�uiCHS.ini, ��������ΪEXE����Ŀ¼�µ�uiENG.ini
	//����ֵ��0��ʾ�ɹ�������ֵ��ʾ�������
	int LoadLanguage(int nLanguageID, const CString& strLanguageFile = _T(""));

	//���ܣ���ȡָ������Ϣ����
	//������id		�������		��Ϣ��ID
	//����ֵ����ϢID��Ӧ����Ϣ����
	CString LoadString(int id);

	//���ܣ���ȡ��ǰʹ�õ�����
	//����ֵ��eLanguage_IDö��ֵ
	int GetLanguageID();

	//���ܣ�����ָ���ĶԻ�����⼰�ؼ��ı�
	//������dlg	�������	ָ���ĶԻ������
	//		dlgID	�������	ָ���ĶԻ���ID
	void UpdateDialogUIText(CDialog *dlg, UINT dlgID);

private:
	void CleanLanguage();		//��ռ��ص�����
	int ReadLanguage(const CString& strLanguageFile);	//��������	
	
private:
	int m_nLanguageID;		//��ǰʹ�õ�����, eLanguage_IDö��ֵ
	vector<SECTION_UNIT> m_vecSecList;		//������
	bool m_bLoadSuccess;			//��ʶ�Ƿ���سɹ�
	static CCS m_ccsSync;			//ͬ���ⲿ�ĵ���
};
