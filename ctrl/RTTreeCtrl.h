#pragma once
#include "RTScrollControlContainer.h"

// CRTTreeCtrl
enum TREECTRL_FINDTYPE {
	TREECTRL_FINDTYPE_BYTEXT,
	TREECTRL_FINDTYPE_BYDATA,
	TREECTRL_FINDTYPE_INVALID
};

class CRTTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CRTTreeCtrl)

public:
	CRTTreeCtrl();
	virtual ~CRTTreeCtrl();

protected:
	static CBitmap* m_BackBitmap[4];
	static UINT     m_BackBitmapDrawMode[4];
	static BOOL     m_IsEnableSkin;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	static void SetBackBitmap(CBitmap* pBack[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEanble = TRUE);
	HTREEITEM FindItem(enum TREECTRL_FINDTYPE findtype, const void *data);	// 查找接口：找到某项的指针
	HTREEITEM FindItem(enum TREECTRL_FINDTYPE findtype, HTREEITEM item, const void *data);
	int FindItemCount(const char *pText);
	int FindItemCount(HTREEITEM item, const char *pText);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


