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

	//��ȡ���̾����ļ��ļ�����·��
	const CString& GetImageFile() const {return m_strImageFile;}

	//�ж�ָ�����ļ��Ƿ����
	static BOOL IsFileExist(const CString& strFile);

	//����: ����Ŀ¼�������Ŀ¼δ���������ȴ�����
	//����: strPath		�������	��Ҫ������Ŀ¼·��
	//����ֵ: TRUE�ɹ�  FALSEʧ��
	static BOOL CreateFullDirectory(const CString& strPath);

	//���ļ�����·����ȡ�ļ���
	static CString ExtractFileName(const CString& strFile);

	//ȥ��·���еĵ�ţ�ȥ��ʧ�ܷ���false���ɹ�����true
	static bool RemoveDot(CString& strPathOrFile);

	//���ܣ��������·�� 
	//������strMainPathOrFile	�������	������յ�����·��
	//		strRelativePathOrFile	�������	Ҫ���������·��
	//����ֵ��strRelativePathOrFile ����� strMainPathOrFile�����·��
	static CString GetRelativePath(const CString& strMainPathOrFile, const CString& strRelativePathOrFile);

private:
	CString m_strImagePath;		//���̾����ļ�����·�������߽�β
	CString m_strImageFile;		//���̾����ļ�����·��
};