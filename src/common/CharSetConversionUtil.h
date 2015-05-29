////////////////////////////////////////////////////
//类名：CCharSetConversionUtil	字符集转换服务类
//职责：实现多字节、utf8、unicode字符集间转换
//////////////////////////////////////////////////////////

#pragma once

class CCharSetConversionUtil
{
public:
	//功能: 将unicode字符串转化为多字节字符串
	//参数: pUnicodeBuf		输入参数	unicode缓冲区
	//		nBufLen			输入参数    缓冲区的长度
	//返回值: 多字节串缓冲区\0结尾，调用者释放内存
	static char* WideCharToMultiByteEx(const unsigned char* pUnicodeBuf, const int nBufLen);

	//功能：将多字节字符串转化为unicode字符串
	//参数：pMBBuf		输入参数     多字节字符串缓冲区
	//		nBufLen		输入参数	 缓冲区的长度
	//返回值：unicode字符串缓冲区\0结尾，调用者释放内存
	static wchar_t* MultiByteToWideCharEx(const char* pMBBuf, const int nBufLen);

	//功能：将utf8编码转换为多字节
	//参数：pUtf8Buf  输入参数   utf8缓冲区
	//		nBufLen   输入参数   utf8缓冲区的长度
	//返回值：多字节串缓冲区\0结尾，调用者释放内存
	static char* Utf8ToMultiByte(const char* pUtf8Buf, const int nBufLen);

	//功能：将多字节串转换为utf8
	//参数：pMBBuf		输入参数		多字节串
	//		nBufLen		输入参数		多字节串缓冲区的长度
	//返回值：utf8缓冲区\0结尾，调用者释放内存
	static char* MultiByteToUtf8(const char* pMBBuf, const int nBufLen);

	//功能: 将unicode字符串转化为utf8编码格式字符串
	//参数: pUnicodeBuf		输入参数	unicode缓冲区
	//		nBufLen			输入参数    缓冲区的长度
	//返回值: utf8编码格式字符串缓冲区\0结尾，调用者释放内存
	static char* WideCharToUtf8(const unsigned char* pUnicodeBuf, const int nBufLen);

	//功能：将utf8编码格式字符串转化为unicode字符串
	//参数：pUtf8Buffer		输入参数     utf8编码格式字符串
	//		nBufLen			输入参数	 缓冲区的长度
	//返回值：unicode字符串缓冲区\0结尾，调用者释放内存
	static wchar_t* Utf8ToWideChar(const char* pUtf8Buffer, const int nBufLen);
};
