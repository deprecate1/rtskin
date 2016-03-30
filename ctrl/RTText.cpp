#include "StdAfx.h"
#include "rttext.h"

CRTText::CRTText(void)
{
}

CRTText::~CRTText(void)
{
}

BOOL CRTText::Compare(LPCTSTR String1,LPCTSTR String2,BOOL bCaseSensitive)
{
	char *ptr1 = (char*)String1;
	char *ptr2 = (char*)String2;

	if(ptr1 == ptr2)return TRUE;
	if(ptr1 == NULL)return FALSE;
	if(ptr2 == NULL)return FALSE;

	if(bCaseSensitive)
	{
		while(TRUE)
		{
			if(*ptr1 != *ptr2)return FALSE;
			if(*ptr1 == NULL)
			{
				if(*ptr2 != NULL)
					return FALSE;
				else
					return TRUE;
			}

			if(*ptr2 == NULL)return FALSE;

			ptr1++;
			ptr2++;
		}

	}
	else
	{
		while(TRUE)
		{
			if(*ptr1 != *ptr2)
			{
				if(*ptr1 >= 'a' && *ptr1 <= 'z')
				{
					if( (*ptr1 - 32) != *ptr2)
						return FALSE;
				}
				else if(*ptr1 > 'A' && *ptr1 < 'Z')
				{
					if( (*ptr1 + 32) != *ptr2)
						return FALSE;
				}
				else
				{
					return FALSE;
				}
			}
			if(*ptr1 == NULL)
			{
				if(*ptr2 != NULL)
					return FALSE;
				else
					return TRUE;
			}

			if(*ptr2 == NULL)return FALSE;

			ptr1++;
			ptr2++;
		}
	}
}
