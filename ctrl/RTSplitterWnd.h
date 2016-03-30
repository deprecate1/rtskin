#pragma once


// CRTSplitterWnd

class CRTSplitterWnd : public CWnd
{
	DECLARE_DYNAMIC(CRTSplitterWnd)

public:
	CRTSplitterWnd();
	virtual ~CRTSplitterWnd();

protected:
	CRect m_SplitterRect;
	int   m_SplitterWidth;
	CSize   m_SplitterSize;
	BOOL  m_IsSelSplitter;
	CPoint m_MouseDownPoint;
	CWnd* m_Child[2];
	BOOL m_IsVertical;
	static CBitmap* m_SplitterBitmap[5];
	static UINT     m_SplitterBitmapDrawMode[5];
	static BOOL     m_IsEnableSkin;
	CImageList *m_ImageList;
	static HCURSOR     m_VerCursor;
	static HCURSOR     m_HorCursor;

	DECLARE_MESSAGE_MAP()
public:
	void SetVertical(CWnd* pLeft, CWnd* pRight, int LeftWidth = 200, int RightWidth = -1);
	void SetHorizoncal(CWnd* pTop, CWnd* pBottom, int TopWidth = 200, int BottomWidth = -1);

	static void EnableSkin(BOOL IsEanble = TRUE);
	static void RTSetCursor(HCURSOR hVerCursor,HCURSOR hHorCursor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void RTSize();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	BOOL Create(DWORD dwStyle,const RECT &rect,CWnd* pParentWnd,UINT nID,CCreateContext *pContext = NULL);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	static void SetBitmap(CBitmap* pBitmaps[], UINT DrawMode[]);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


