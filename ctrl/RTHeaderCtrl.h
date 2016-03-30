#pragma once


// CRTHeaderCtrl

class CRTHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CRTHeaderCtrl)

public:
	CRTHeaderCtrl();
	static void SetHeaderBitmap(CBitmap* pHeader[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEanble = TRUE);
	virtual ~CRTHeaderCtrl();

protected:
	static UINT m_HeaderBitmapDrawMode[5];
	static CBitmap* m_HeaderBitmap[5];
	static BOOL m_IsEanbleRTStyle;
protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


