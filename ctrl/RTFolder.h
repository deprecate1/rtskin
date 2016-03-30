#pragma once

class CRTFolder
{
public:
	CRTFolder(void);
	~CRTFolder(void);

	//����Ŀ¼�������Ŀ¼�����ڣ�������Ŀ¼���ɹ���TRUE��ʧ�ܷ���FALSE
	static BOOL MakeDirs(LPCTSTR lpPathName);

	//����ָ���ļ���Ŀ¼�ĸ�Ŀ¼
	static void GetFilePath(LPCTSTR lpFileName, LPSTR lpPathBuf);
	static BOOL MoveTo(LPCTSTR lpSrcDir, LPCTSTR lpDestDir);
	static BOOL DeleteFolder(LPCTSTR lpFolder);
	static BOOL GetFileName(LPCTSTR lpFileName, char lpBuf[]);
	static BOOL ReName(LPCTSTR lpOldPath, LPCTSTR NewPath);
};
