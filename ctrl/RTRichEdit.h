#pragma once

#include "RTScrollControlContainer.h"

// CRTRichEdit

class CRTRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRTRichEdit)

public:
	CRTRichEdit();
	virtual ~CRTRichEdit();

protected:
	static BOOL m_bEnableSkin;
	static CBitmap * m_BackBitmap[5];
	static UINT      m_BackBitmapDrawMode[5];

	CRTScrollControlContainer m_Container;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	static void EnableSkin(BOOL IsEnable = TRUE);
	static void SetBackBitmap(CBitmap* pBack[],UINT DrawMode[]);
protected:
	virtual void PreSubclassWindow();
};


