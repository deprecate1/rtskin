#pragma once


// CRTScrollBar

class CRTScrollBar : public CScrollBar
{
	DECLARE_DYNAMIC(CRTScrollBar)

public:
	CRTScrollBar();
	virtual ~CRTScrollBar();
	typedef enum _ScrollBarSizeArea
	{
		AreaNone = 0,
		AreaTop = 1,
		AreaBottom = 2,
		AreaLeft = 1,
		AreaRight = 2
	}ScrollBarSizeArea;

	void DrawHorizontal(CDC* pDC);
	void DrawVertical(CDC* pDC);
	
	static void EnableSkin(BOOL IsEnable = TRUE);
	static void SetHorizontalBitmap(CBitmap* pHorizontal[],UINT DrawMode[]);
	static void SetVerticalBitmap(CBitmap* pVertical[],UINT DrawMode[]);
	static void SetVerticalUpArrowBitmap(CBitmap* pVerticalUpArrowBitmap[],UINT DrawMode[]);
	static void SetVerticalDownArrowBitmap(CBitmap* pVerticalDownArrowBitmap[],UINT DrawMode[]);
	static void SetHorizontalLeftArrowBitmap(CBitmap* pHorizontalLeftArrowBitmap[],UINT DrawMode[]);
	static void SetHorizontalRightArrowBitmap(CBitmap* pHorizontalRightArrowBitmap[],UINT DrawMode[]);
	void SetSizeArea(ScrollBarSizeArea Area = CRTScrollBar::AreaNone);
	

protected:	
	CPoint m_MouseDownPoint;
	CRect m_ThumbRect;
	CRect m_LeftArrowRect;
	CRect m_RightArrowRect;
	CRect m_TopArrowRect;
	CRect m_BottomArrowRect;
	CRect m_AreaRect;
	CRect m_DragThumbRect;

	BOOL m_bMouseDownArrowForback;
	BOOL m_bMouseDownArrowForwad;
	int  m_nThumbStart;
	double m_dbThumbInterval;
	double m_dbThumbRemainder;
	int  m_nThumbTrackPos;
	BOOL m_bDragging;
	BOOL m_bMouseDown;
	ScrollBarSizeArea  m_SizeArea;
	int  m_MouseDownTime;
	
	static BOOL   m_IsEnableSkin;
	friend class CRTInternalScrollBar;
	static CBitmap* m_ScrollBarVerticalBitmap[5];
	static CBitmap* m_ScrollBarHorizontalBitmap[5];
	static UINT     m_ScrollBarVerticalBitmapDrawMode[5];
	static UINT     m_ScrollBarHorizontalBitmapDrawMode[5];

	static CBitmap* m_ScrollBarUpArrowBitmap[5];
	static CBitmap* m_ScrollBarLeftArrowBitmap[5];
	static CBitmap* m_ScrollBarDownArrowBitmap[5];
	static CBitmap* m_ScrollBarRightArrowBitmap[5];

	static UINT     m_ScrollBarUpArrowBitmapDrawMode[5];
	static UINT     m_ScrollBarLeftArrowBitmapDrawMode[5];
	static UINT     m_ScrollBarDownArrowBitmapDrawMode[5];
	static UINT     m_ScrollBarRightArrowBitmapDrawMode[5];

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void SetRange(int nmin, int nmax,BOOL bRedraw = TRUE);
	void SetPos(int pos, BOOL redraw = TRUE);
};


