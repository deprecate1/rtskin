#include "StdAfx.h"
#include "RTFolder.h"

CRTFolder::CRTFolder(void)
{
}

CRTFolder::~CRTFolder(void)
{
}

BOOL CRTFolder::MakeDirs(LPCTSTR lpPathName)
{
	if(CreateDirectory(lpPathName,NULL))
		return TRUE;

	char buf[MAX_PATH];
	GetFilePath(lpPathName,buf);
	if(!MakeDirs(buf))
		return FALSE;
	if(CreateDirectory(lpPathName,NULL))
		return TRUE;
	else
		return FALSE;
}

void CRTFolder::GetFilePath(LPCTSTR lpFileName, LPSTR lpPathBuf)
{
	ZeroMemory(lpPathBuf,MAX_PATH);
	strcpy(lpPathBuf,lpFileName);
	int nLen = (int)strlen(lpFileName) - 1;
	while(nLen >= 0)
	{
		if(lpPathBuf[nLen] == '\\')
		{
			lpPathBuf[nLen] = NULL;
			break;
		}
		nLen --;
	}
}

BOOL CRTFolder::MoveTo(LPCTSTR lpSrcDir, LPCTSTR lpDestDir)
{
	char SrcPathName[MAX_PATH];
	if(!GetFileName(lpSrcDir,SrcPathName))return FALSE;
	char DestFolder[MAX_PATH];
	ZeroMemory(DestFolder,MAX_PATH);
	strcpy(DestFolder,lpDestDir);
	strcat(DestFolder,"\\");
	strcat(DestFolder,SrcPathName);
	return MoveFile(lpSrcDir,DestFolder);
}

BOOL CRTFolder::DeleteFolder(LPCTSTR lpFolder)
{
	WIN32_FIND_DATA wfd;
	char FolderFind[MAX_PATH];
	ZeroMemory(FolderFind,MAX_PATH);
	strcpy(FolderFind,lpFolder);
	strcat(FolderFind,"\\*.*");
	HANDLE hFind = FindFirstFile(FolderFind,&wfd);
	if(hFind != NULL)
	{
		do
		{
			if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wfd.cFileName[0] == '.')continue;

				char NextFolder[MAX_PATH];
				ZeroMemory(NextFolder,MAX_PATH);
				strcpy(NextFolder,lpFolder);
				strcat(NextFolder,"\\");
				strcat(NextFolder,wfd.cFileName);
				DeleteFolder(NextFolder);
			}
			else
			{
				char NextFile[MAX_PATH];
				ZeroMemory(NextFile,MAX_PATH);
				strcpy(NextFile,lpFolder);
				strcat(NextFile,"\\");
				strcat(NextFile,wfd.cFileName);
				DeleteFile(NextFile);
			}
		}while(FindNextFile(hFind,&wfd));

		FindClose(hFind);
	}
	return RemoveDirectory(lpFolder);
}

BOOL CRTFolder::GetFileName(LPCTSTR lpFileName, char lpBuf[])
{
	char *ptr = NULL;
	int nLen = (int)strlen(lpFileName) - 1;
	if(lpFileName[nLen] == '\\')nLen --;
	while(nLen > 0)
	{
		if(lpFileName[nLen] == '\\')
		{
			ptr = (char*)(lpFileName + nLen + 1); 
			break;
		}
		nLen --;
	}
	if(nLen == 0)return FALSE;
	strcpy(lpBuf,ptr);
	return TRUE;
}

BOOL CRTFolder::ReName(LPCTSTR lpOldPath, LPCTSTR NewPath)
{
	char BasePath[MAX_PATH];
	ZeroMemory(BasePath,MAX_PATH);
	GetFilePath(lpOldPath,BasePath);
	strcat(BasePath,"\\");
	strcat(BasePath,NewPath);
	
	if(0 == rename(lpOldPath,BasePath))
		return TRUE;
	else
		return FALSE;
}
