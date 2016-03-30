#pragma once


// CRTListCtrl
#include "RTHeaderCtrl.h"
#include "RTScrollControlContainer.h"

class CRTListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CRTListCtrl)
	
public:
	CRTListCtrl();
	virtual ~CRTListCtrl();
	CRTScrollControlContainer    m_Container;
protected:
	CRTHeaderCtrl m_HeaderCtrl;
	
	static CBitmap* m_BackBitmap[5];
	static UINT     m_BackBitmapDrawMode[5];
	static BOOL m_bIsEnableSkin;
	BOOL   m_bDraging;
	BOOL   m_bPainting;
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	static void SetBackBitmap(CBitmap* pBitmap[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//void MoveWindow(LPCRECT rect);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


