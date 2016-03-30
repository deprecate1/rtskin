#pragma once


// CRTToolBar

class CRTToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CRTToolBar)

public:
	CRTToolBar();
	virtual ~CRTToolBar();
	BOOL LoadToolBar (UINT nIDResource);
	BOOL InsertControl (int nIndex, CWnd& Ctrl);
	void TrackPopupMenu (int nID, CMenu* pMenu);
	static BOOL EnableSkin(BOOL IsEnable = TRUE);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	static BOOL      m_IsEnableSkin;
	static CBitmap * m_ToolBarBitamp[4];
	static UINT      m_ToolBarBitmapDrawMode[4];
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BYTE m_bCheckVisibility;
	CSize CalcDynamicLayout (int nLength, DWORD dwMode);
	void OnButtonDropDown (NMHDR* lpnmhdr, LRESULT* plResult);
public:
	afx_msg void OnPaint();
	static BOOL SetBitmap(CBitmap* ToolBarBitmap[],UINT DrawMode[]);
	BOOL LoadTrueColorToolBar(int  nBtnWidth, UINT uToolBarNormal, UINT uToolBarDisable);
	BOOL SetTrueColorToolBar(UINT uToolBarType, UINT uToolBar, int nBtnWidth);
};


#define DROPDOWN(item) ((WORD)-item)

#define IS_STDBTN(button)        (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_BUTTON)
#define IS_CHECKBTN(button)      (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_CHECK)
#define IS_GROUPBTN(button)      (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_CHECKGROUP)
#define IS_DROPDOWN(button)      (((button).fsStyle&TBSTYLE_DROPDOWN)==TBSTYLE_DROPDOWN)
#define IS_SEPARATOR(button)     (((button).fsStyle&TBSTYLE_SEP)&&((button).idCommand==0))
#define IS_CONTROL(button)       (((button).fsStyle&TBSTYLE_SEP)&&((button).idCommand!=0))
#define IS_CHECKED(button)       (((button).fsState&TBSTATE_CHECKED)==TBSTATE_CHECKED)
#define IS_ENABLED(button)       (((button).fsState&TBSTATE_ENABLED)==TBSTATE_ENABLED)
#define IS_INDETERMINATE(button) (((button).fsState&TBSTATE_INDETERMINATE)==TBSTATE_INDETERMINATE)
#define IS_PRESSED(button)       (((button).fsState&TBSTATE_ENABLED)==TBSTATE_PRESSED)
#define IS_VISIBLE(button)       (((button).fsState&TBSTATE_HIDDEN)==0)
#define IS_WRAP(button)          (((button).fsState&TBSTATE_WRAP)==TBSTATE_WRAP)
