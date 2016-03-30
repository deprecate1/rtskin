#pragma once

#include "RTScrollBar.h"

// CRTScrollControlContainer

class CRTScrollControlContainer : public CWnd
{
	DECLARE_DYNAMIC(CRTScrollControlContainer)

public:
	CRTScrollControlContainer();
	virtual ~CRTScrollControlContainer();

protected:
	CWnd *m_Control;
	CRTScrollBar m_VerScrollBar;
	CRTScrollBar m_HorScrollBar;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL Create(CWnd* pControl);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize(void);
	void SetVerPos(int pos, BOOL redraw = TRUE);
	void SetVerRange(int nmin, int nmax,BOOL redraw = TRUE);
	void SetHorPos(int pos, BOOL redraw = TRUE);
	void SetHorRange(int nmin, int nmax,BOOL redraw = TRUE);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL PassMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


