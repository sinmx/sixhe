////////////////////////////////////////////////////
//������CCharSetConversionUtil	�ַ���ת��������
//ְ��ʵ�ֶ��ֽڡ�utf8��unicode�ַ�����ת��
//////////////////////////////////////////////////////////

#pragma once

class CCharSetConversionUtil
{
public:
	//����: ��unicode�ַ���ת��Ϊ���ֽ��ַ���
	//����: pUnicodeBuf		�������	unicode������
	//		nBufLen			�������    �������ĳ���
	//����ֵ: ���ֽڴ�������\0��β���������ͷ��ڴ�
	static char* WideCharToMultiByteEx(const unsigned char* pUnicodeBuf, const int nBufLen);

	//���ܣ������ֽ��ַ���ת��Ϊunicode�ַ���
	//������pMBBuf		�������     ���ֽ��ַ���������
	//		nBufLen		�������	 �������ĳ���
	//����ֵ��unicode�ַ���������\0��β���������ͷ��ڴ�
	static wchar_t* MultiByteToWideCharEx(const char* pMBBuf, const int nBufLen);

	//���ܣ���utf8����ת��Ϊ���ֽ�
	//������pUtf8Buf  �������   utf8������
	//		nBufLen   �������   utf8�������ĳ���
	//����ֵ�����ֽڴ�������\0��β���������ͷ��ڴ�
	static char* Utf8ToMultiByte(const char* pUtf8Buf, const int nBufLen);

	//���ܣ������ֽڴ�ת��Ϊutf8
	//������pMBBuf		�������		���ֽڴ�
	//		nBufLen		�������		���ֽڴ��������ĳ���
	//����ֵ��utf8������\0��β���������ͷ��ڴ�
	static char* MultiByteToUtf8(const char* pMBBuf, const int nBufLen);

	//����: ��unicode�ַ���ת��Ϊutf8�����ʽ�ַ���
	//����: pUnicodeBuf		�������	unicode������
	//		nBufLen			�������    �������ĳ���
	//����ֵ: utf8�����ʽ�ַ���������\0��β���������ͷ��ڴ�
	static char* WideCharToUtf8(const unsigned char* pUnicodeBuf, const int nBufLen);

	//���ܣ���utf8�����ʽ�ַ���ת��Ϊunicode�ַ���
	//������pUtf8Buffer		�������     utf8�����ʽ�ַ���
	//		nBufLen			�������	 �������ĳ���
	//����ֵ��unicode�ַ���������\0��β���������ͷ��ڴ�
	static wchar_t* Utf8ToWideChar(const char* pUtf8Buffer, const int nBufLen);
};
