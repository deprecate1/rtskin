#pragma once


// CRTStatusBar

class CRTStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CRTStatusBar)

public:
	CRTStatusBar();
	virtual ~CRTStatusBar();

protected:
	static CBitmap* m_StatusBitmap[5];
	static UINT m_StatusBitmapDrawMode[5];
	static BOOL m_IsEnableSkin;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	static void SetBitmap(CBitmap*  StatusBitmap[], UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
	BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);
};


