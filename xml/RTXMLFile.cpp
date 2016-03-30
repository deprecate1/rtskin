#include "StdAfx.h"
#include "RTXmlFile.h"
#include "RTFolder.h"

CRTXmlFile::CRTXmlFile(void)
{
	m_NewPosition = -1;
}

CRTXmlFile::~CRTXmlFile(void)
{
	Clear();
}

CRTXmlItem* CRTXmlFile::GetItem(LPCTSTR lpItemName)
{
	int nCount = (int)m_ItemList.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		CRTXmlItem *pItem = (CRTXmlItem *) m_ItemList.GetAt(i);
		if(pItem->ItemName == lpItemName)
			return pItem;
	}
	return NULL;
}

// 打开文件，读取数据，然后关闭文件
BOOL CRTXmlFile::Open(LPCTSTR lpFileName)
{
	if(lpFileName == NULL)return FALSE;
	m_FileName = lpFileName;
	FILE *file = fopen((LPCTSTR)m_FileName,"r");
	if(file == NULL)return FALSE;

	Clear();
	CString fileText = "";
	char buf[1025];
	while(!feof(file))
	{
		int nLen = (int)fread(buf,1,1024,file);
		if(nLen > 0)
		{
			buf[nLen] = NULL;
			fileText += buf;
		}
	}
	fclose(file);

	char *ptr = (char *)((LPCTSTR)fileText);
	//XTRACE("读取 配置文件 %s 内容是: \n", lpFileName);
	//XTRACE("*****************************************\n");
	//XTRACE("%s\n", ptr);
	//XTRACE("*****************************************\n");
	while(*ptr != '\0')
	{
		if(!ParseItem(ptr,&ptr))break;
	}
	return TRUE;
}

void CRTXmlFile::AddItem(CRTXmlItem* pItem)
{
	m_ItemList.Add(pItem);
}

BOOL CRTXmlFile::Save(LPCTSTR lpSaveFile)
{
	if(lpSaveFile != NULL)m_FileName = lpSaveFile;

	CString tmpFile = m_FileName + ".tmp";
	FILE *back = fopen((LPCTSTR)m_FileName,"r");

	//创建备份文件
	if(back != NULL)
	{
		FILE *tmp = fopen((LPCTSTR)tmpFile,"w");
		if(tmp == NULL)
		{
			fclose(back);
			return FALSE;
		}

		char buf[1024];
		int nread = 0;
		while(!feof(back))
		{
			nread = (int)fread(buf,1,1024,back);
			if(nread > 0)
			{
				fwrite(buf,1,nread,tmp);
			}
		}
		fclose(back);
		fclose(tmp);
	}

	FILE *file = fopen((LPCTSTR)m_FileName,"w");
	if(file == NULL)
	{
		char path[MAX_PATH];
		CRTFolder::GetFilePath(m_FileName,path);
		if(!CRTFolder::MakeDirs(path))
			return FALSE;
		
		file = fopen(m_FileName,"w");
		if(file == NULL)return FALSE;
	}

	CRTXmlItem *pItem = GetFirstItem();
	while(pItem != NULL)
	{
		RTXmlItemData* pData = pItem->ItemData;
		char wrbuf[1024];
		ZeroMemory(wrbuf,1024);
		strcpy(wrbuf,"<");
		strcat(wrbuf,pItem->ItemName);
		fwrite(wrbuf,1,strlen(wrbuf),file);
		while(pData != NULL)
		{
			ZeroMemory(wrbuf,1024);
			strcpy(wrbuf," ");
			strcat(wrbuf,pData->KeyName);
			strcat(wrbuf,"=");
			strcat(wrbuf,pData->KeyValue);
			fwrite(wrbuf,1,strlen(wrbuf),file);
			pData = pData->Next;
		}
		ZeroMemory(wrbuf,1024);
		strcpy(wrbuf,">\r\n");
		fwrite(wrbuf,1,strlen(wrbuf),file);
		pItem = GetNextItem();
	}
	fclose(file);
	DeleteFile(tmpFile);
	return TRUE;	
}

BOOL CRTXmlFile::ParseItem(char * ptr,char** out)
{
	char * start = ptr;
	while(*start != '\0' && *start != '<')
		start++;
	if(*start == '\0')
	{
		*out = start;
		return FALSE;
	}
	start ++;
	
	if(!SkipBlank(start,&start))
	{
		*out = start;
		return TRUE;
	}
	
	char *key = start;
	if(!FindBlank(start,&start))
	{
		*out = start;
		return TRUE;
	}

	char temp = *start;
	*start = '\0';
	CRTXmlItem * pItem = new CRTXmlItem(key);
	m_ItemList.Add(pItem);
	*start = temp;

	while(SkipBlank(start,&start))
	{
		if(!ParseKey(start,pItem,&start))
			break;
		if(*start == '>')break;
	}
	*out = start;
	return TRUE;
}

BOOL CRTXmlFile::ParseKey(char* ptr,CRTXmlItem *pItem,char** out)
{
	char *start = ptr;
	char *begin = ptr;
	char *end = ptr;

	if(!FindBlank(start,&start))
	{
		return FALSE;
	}
	*out = start;
	char temp = *start;
	*start = NULL;


    while(*end != '=' && *end != '\0')
	{
		end ++;
	}
	if(end == '\0')	
	{
        *start = temp;
		return TRUE;
	}
	*end = NULL;
	end ++;
	pItem->SetKeyValue(begin,end);
	end--;
	*end = '=';
	
	*start = temp;
	return TRUE;
}

BOOL CRTXmlFile::SkipBlank(char * ptr,char ** out)
{
	char *start = ptr;
	while(*start > 0 && *start < 33)
			start ++;

	*out = start;
	if(*start == '<' || *start == '>' || *start == '\0')
		return FALSE;
	else
		return TRUE;
}

BOOL CRTXmlFile::FindBlank(char* input, char ** out)
{
	char *start = input;
	while((*start <= 0 || *start >= 33) && *start != '>' && *start != '<')
		start ++;
	
	*out = start;
	if(*start == '<' || *start == '\0')
		return FALSE;
	else
		return TRUE;
	return 0;
}

void CRTXmlFile::Clear()
{
	int nCount = (int)m_ItemList.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		CRTXmlItem *pItem = (CRTXmlItem *) m_ItemList.GetAt(i);
		delete pItem;
	}
	m_ItemList.RemoveAll();
}

CRTXmlItem* CRTXmlFile::GetFirstItem()
{
	m_NewPosition = 0;
	
	if(m_ItemList.GetCount() <= 0)
		return NULL;

	return (CRTXmlItem*)m_ItemList.GetAt(m_NewPosition);
}

CRTXmlItem* CRTXmlFile::GetNextItem()
{
	m_NewPosition++;
	if(m_NewPosition > m_ItemList.GetCount() - 1)
	{
		return NULL;
	}
	return (CRTXmlItem *)m_ItemList.GetAt(m_NewPosition);
}
