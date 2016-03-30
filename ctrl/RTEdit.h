#pragma once

#include "RTScrollControlContainer.h"

// CRTEdit

class CRTEdit : public CEdit
{
	DECLARE_DYNAMIC(CRTEdit)

public:
	CRTEdit();
	virtual ~CRTEdit();

protected:
	static CBitmap* m_BackBitmap[5];
	static UINT     m_BackBitmapDrawMode[5];
	static BOOL     m_bEnableSkin;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	static void SetBackBitmap(CBitmap* pBack[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
	afx_msg void OnPaint();
	afx_msg void OnEnChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void DrawText(void);
};


