///////////////////////////////////////////
//类名：CPathUtilEx	 路径工具类
//职责：单一实例，读取各路径信息，这些信息是不可变的，程序启动时读取，后续访问，无需同步，
//		目前信息包括：当前进程镜像文件所在的路径
////////////////////////////////////////////////////////

#pragma once

class CPathUtilEx
{
protected:  //阻止实例化
	CPathUtilEx();
	~CPathUtilEx();

public:
	//读取单一实例对象
	static CPathUtilEx* Instance();

	//读取进程镜像文件所在路径，后划线结尾
	const CString& GetImagePath() const {return m_strImagePath;}

	//判读指定的文件是否存在
	static BOOL IsFileExist(const CString& strFile);

	//功能: 创建目录，如果父目录未创建，会先创建它
	//参数: strPath		输入参数	想要创建的目录路径
	//返回值: TRUE成功  FALSE失败
	static BOOL CreateFullDirectory(const CString& strPath);

	//文件完整路径中去除文件名，提取路径，尾部带有下划线
	static CString GetFilePath(const CString& strFileFullPath);

	//路径加上引号如C:\Ruijie Network\1.txt，追加后成为"C:\Ruijie Network\1.txt"
	static void AddQuotationMarks(CString& strPathOrFile);

	//功能：计算相对路径 
	//参数：strMainPathOrFile	输入参数	计算参照的完整路径
	//		strRelativePathOrFile	输入参数	要计算的完整路径
	//返回值：strRelativePathOrFile 相对于 strMainPathOrFile的相对路径
	static CString GetRelativePath(const CString& strMainPathOrFile, const CString& strRelativePathOrFile);

	//去除路径中的点号，去除失败返回false，成功返回true
	static bool RemoveDot(CString& strPathOrFile);

	//功能：获取父目录
	//参数：strCurrentDirectory		输入参数		当前目录，尾部带有下划线
	//返回值：返回父目录，空串表示没有父目录
	static CString GetParentDirectory(const CString& strCurrentDirectory);

	//读取指定文件名中的扩展名，如文件1.txt返回值为txt
	static CString GetFileExtendName(const CString& strFileName);

private:
	CString m_strImagePath;		//进程镜像文件所在路径，后划线结尾
};