#pragma once

class CRTFolder
{
public:
	CRTFolder(void);
	~CRTFolder(void);

	//创建目录，如果父目录不存在，创建父目录，成功回TRUE，失败返回FALSE
	static BOOL MakeDirs(LPCTSTR lpPathName);

	//返回指定文件或目录的父目录
	static void GetFilePath(LPCTSTR lpFileName, LPSTR lpPathBuf);
	static BOOL MoveTo(LPCTSTR lpSrcDir, LPCTSTR lpDestDir);
	static BOOL DeleteFolder(LPCTSTR lpFolder);
	static BOOL GetFileName(LPCTSTR lpFileName, char lpBuf[]);
	static BOOL ReName(LPCTSTR lpOldPath, LPCTSTR NewPath);
};
