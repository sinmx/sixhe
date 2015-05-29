///////////////////////////////////////////
//������CPathUtilEx	 ·��������
//ְ�𣺵�һʵ������ȡ��·����Ϣ����Щ��Ϣ�ǲ��ɱ�ģ���������ʱ��ȡ���������ʣ�����ͬ����
//		Ŀǰ��Ϣ��������ǰ���̾����ļ����ڵ�·��
////////////////////////////////////////////////////////

#pragma once

class CPathUtilEx
{
protected:  //��ֹʵ����
	CPathUtilEx();
	~CPathUtilEx();

public:
	//��ȡ��һʵ������
	static CPathUtilEx* Instance();

	//��ȡ���̾����ļ�����·�������߽�β
	const CString& GetImagePath() const {return m_strImagePath;}

	//�ж�ָ�����ļ��Ƿ����
	static BOOL IsFileExist(const CString& strFile);

	//����: ����Ŀ¼�������Ŀ¼δ���������ȴ�����
	//����: strPath		�������	��Ҫ������Ŀ¼·��
	//����ֵ: TRUE�ɹ�  FALSEʧ��
	static BOOL CreateFullDirectory(const CString& strPath);

	//�ļ�����·����ȥ���ļ�������ȡ·����β�������»���
	static CString GetFilePath(const CString& strFileFullPath);

	//·������������C:\Ruijie Network\1.txt��׷�Ӻ��Ϊ"C:\Ruijie Network\1.txt"
	static void AddQuotationMarks(CString& strPathOrFile);

	//���ܣ��������·�� 
	//������strMainPathOrFile	�������	������յ�����·��
	//		strRelativePathOrFile	�������	Ҫ���������·��
	//����ֵ��strRelativePathOrFile ����� strMainPathOrFile�����·��
	static CString GetRelativePath(const CString& strMainPathOrFile, const CString& strRelativePathOrFile);

	//ȥ��·���еĵ�ţ�ȥ��ʧ�ܷ���false���ɹ�����true
	static bool RemoveDot(CString& strPathOrFile);

	//���ܣ���ȡ��Ŀ¼
	//������strCurrentDirectory		�������		��ǰĿ¼��β�������»���
	//����ֵ�����ظ�Ŀ¼���մ���ʾû�и�Ŀ¼
	static CString GetParentDirectory(const CString& strCurrentDirectory);

	//��ȡָ���ļ����е���չ�������ļ�1.txt����ֵΪtxt
	static CString GetFileExtendName(const CString& strFileName);

private:
	CString m_strImagePath;		//���̾����ļ�����·�������߽�β
};