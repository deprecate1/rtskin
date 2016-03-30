#pragma once


// CRTStatic

class CRTStatic : public CStatic
{
	DECLARE_DYNAMIC(CRTStatic)

public:
	CRTStatic();
	virtual ~CRTStatic();
protected:
	static CBitmap* m_StaticBitmap[5];
	static UINT     m_StaticBitmapDrawMode[5];
	static BOOL     m_IsEnableSkin;
	static BOOL     m_IsBackTransparent;  
public:
	static void SetBitmap(CBitmap* StaticBitmap[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE,BOOL BackTransparent = FALSE);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


