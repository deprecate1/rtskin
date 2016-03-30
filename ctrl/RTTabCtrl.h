#pragma once


// CRTTabCtrl

class CRTTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CRTTabCtrl)

public:
	CRTTabCtrl();
	virtual ~CRTTabCtrl();

protected:
	int   m_CurSel;
	static CBitmap* m_TabBitmap[5];
	static UINT     m_TabBitmapDrawMode[5];

	static BOOL     m_IsEnableSkin;
	BOOL            m_IsShowTabItem;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	static void SetTabBitmap(CBitmap* TabBitamp[], UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
	void GetClientRect(LPRECT lpRect);
	void ShowTabItem(BOOL IsVisable);
	LONG Add(int Position,LPCTSTR lpszItemText,int Image,CWnd* pItemWnd = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg void OnTcnSelchange();
	afx_msg void OnTcnSelchanging(int oldSel);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


