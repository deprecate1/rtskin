#pragma once

typedef struct _tagRTXmlItemData
{
	CString KeyName;
	CString KeyValue;
	struct _tagRTXmlItemData* Next;
}RTXmlItemData;

class CRTXmlItem
{
public:
	CString ItemName;
private:
	RTXmlItemData * ItemData;
	friend class CRTXmlFile;
public:
	CRTXmlItem(LPCTSTR lpItemName)
	{
		if(lpItemName != NULL)ItemName = lpItemName;
		ItemData = NULL;
	}

	void SetKeyValue(LPCTSTR lpKeyName,DWORD KeyValue)
	{
		CString strKeyValue;
		strKeyValue.Format("%d",KeyValue);
		SetKeyValue(lpKeyName,strKeyValue);
	}

	void SetKeyValue(LPCTSTR lpKeyName,LPCTSTR lpKeyValue)
	{
		RTXmlItemData *pItem = ItemData;
		RTXmlItemData *pItemEnd = ItemData;
		while(pItem != NULL)
		{
			if(pItem->KeyName == lpKeyName)
			{
				pItem->KeyValue = lpKeyValue;
				return;
			}
            pItemEnd = pItem;
			pItem = pItem->Next;
		}

		if(pItem == NULL)
		{
			if(pItemEnd == NULL)
			{
				ItemData = new RTXmlItemData;
				ItemData->Next = NULL;
				ItemData->KeyName = lpKeyName;
				ItemData->KeyValue = lpKeyValue;
			}
			else
			{
				pItemEnd->Next = new RTXmlItemData;
				pItemEnd = pItemEnd->Next;
				pItemEnd->Next = NULL;
				pItemEnd->KeyName = lpKeyName;
				pItemEnd->KeyValue = lpKeyValue;
			}
				
		}
	}
	LPCTSTR GetKeyValue(LPCTSTR lpKeyName)
	{
		RTXmlItemData *ptr = ItemData;
		while(ptr != NULL)
		{
			if(ptr->KeyName == lpKeyName)
			{
				return (LPCTSTR)ptr->KeyValue;
			}
			ptr = ptr->Next;
		}
		return NULL;
	}

	DWORD GetKeyValueInt(LPCTSTR lpKeyName)
	{
		LPCTSTR lpKeyVlaue = GetKeyValue(lpKeyName);
		if(lpKeyVlaue == NULL)return 0;

		return atol(lpKeyVlaue);
	}

	BOOL GetKeyValueBool(LPCTSTR lpKeyName)
	{
		return (BOOL)GetKeyValueInt(lpKeyName);
	}
	~CRTXmlItem()
	{
		RTXmlItemData *temp = NULL;
		RTXmlItemData *ptr = ItemData;
		while(ptr != NULL)
		{
			temp = ptr;
			ptr = ptr->Next;
			delete temp;
		}
	}
};

class CRTXmlFile
{
public:
	CRTXmlFile(void);
	~CRTXmlFile(void);
private:
	CPtrArray m_ItemList;
	CString   m_FileName;
public:
	CRTXmlItem* GetItem(LPCTSTR lpItemName);
	BOOL Open(LPCTSTR lpFileName);
	BOOL Save(LPCTSTR lpSaveFile = NULL);
	void AddItem(CRTXmlItem* pItem);
	CRTXmlItem* GetFirstItem();
	CRTXmlItem* GetNextItem();
private:
	BOOL ParseItem(char * ptr,char** out);
	BOOL ParseKey(char* ptr,CRTXmlItem* pItem,char ** out);
	BOOL SkipBlank(char * ptr,char ** out);
	BOOL FindBlank(char* input, char ** out);
	int    m_NewPosition;
	void Clear();
};
