#pragma once

class CRTText
{
public:
	CRTText(void);
	~CRTText(void);

	static BOOL Compare(LPCTSTR String1,LPCTSTR lpString2,BOOL bCaseSensitive=FALSE);
};
