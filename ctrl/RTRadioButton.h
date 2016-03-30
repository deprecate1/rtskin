#pragma once


// CRTRadioButton

class CRTRadioButton : public CButton
{
	DECLARE_DYNAMIC(CRTRadioButton)

public:
	CRTRadioButton();
	virtual ~CRTRadioButton();
protected:
	static CBitmap* m_StaticBitmap[5];
	static UINT     m_StaticBitmapDrawMode[5];
	static BOOL     m_IsEnableSkin;
	static BOOL     m_IsBackTransparent;  

public:
	static void SetBitmap(CBitmap* StaticBitmap[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
	static void BackTransparent(BOOL Transparent = TRUE);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


