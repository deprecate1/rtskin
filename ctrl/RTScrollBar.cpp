// RTScrollBar.cpp : 实现文件
//

#include "stdafx.h"
#include "RTScrollBar.h"
#include "RTDraw.h"

// CRTScrollBar

IMPLEMENT_DYNAMIC(CRTScrollBar, CScrollBar)
CRTScrollBar::CRTScrollBar()
{
	m_bMouseDown = FALSE;
	m_bMouseDownArrowForback = FALSE;
	m_bMouseDownArrowForwad = FALSE;
	m_bDragging = FALSE;
	m_nThumbStart = 0;
	m_dbThumbRemainder = 0.0f;
	m_dbThumbInterval = 0.0f;
}

CRTScrollBar::~CRTScrollBar()
{
}

CBitmap* CRTScrollBar::m_ScrollBarUpArrowBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTScrollBar::m_ScrollBarLeftArrowBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTScrollBar::m_ScrollBarDownArrowBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTScrollBar::m_ScrollBarRightArrowBitmap[5] = {NULL,NULL,NULL,NULL,NULL};

UINT     CRTScrollBar::m_ScrollBarUpArrowBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTScrollBar::m_ScrollBarLeftArrowBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTScrollBar::m_ScrollBarDownArrowBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTScrollBar::m_ScrollBarRightArrowBitmapDrawMode[5] = {0,0,0,0,0};

CBitmap* CRTScrollBar::m_ScrollBarHorizontalBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTScrollBar::m_ScrollBarVerticalBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTScrollBar::m_ScrollBarHorizontalBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTScrollBar::m_ScrollBarVerticalBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTScrollBar::m_IsEnableSkin = TRUE;

BEGIN_MESSAGE_MAP(CRTScrollBar, CScrollBar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CRTScrollBar 消息处理程序


void CRTScrollBar::OnPaint()
{
	if(!m_IsEnableSkin)return CScrollBar::OnPaint();

	CPaintDC dc(this); 
	DWORD style = GetStyle();
	if( (style & SBS_VERT) == SBS_VERT)
	{
		DrawVertical(&dc);
	}
	else
	{
		DrawHorizontal(&dc);
	}
}

void CRTScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_IsEnableSkin)return CScrollBar::OnLButtonDown(nFlags, point);

	m_MouseDownTime = 0;
	SetCapture();
	DWORD style = GetStyle();
	CWnd *pParent = GetParent();

	if((style & SBS_VERT)==SBS_VERT)
	{
		if(m_ThumbRect.PtInRect(point))
		{
			m_bMouseDown = true;
			m_MouseDownPoint.SetPoint(point.x - m_ThumbRect.left,point.y - m_ThumbRect.top);

			CClientDC dc(this);
			DrawVertical(&dc);
		}
		else if(m_BottomArrowRect.PtInRect(point))
		{
			m_bMouseDownArrowForwad = true;
			pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),(LPARAM)m_hWnd);
			CClientDC dc(this);
			DrawVertical(&dc);

			SetTimer(2,100,NULL);
		}
		else if(m_TopArrowRect.PtInRect(point))
		{
			m_bMouseDownArrowForback = true;
			pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),(LPARAM)m_hWnd);
			CClientDC dc(this);
			DrawVertical(&dc);
			
			SetTimer(2,100,NULL);
		}
		else if(m_AreaRect.PtInRect(point))
		{	
			if(pParent != NULL)
			{
				SCROLLINFO sif;
				sif.cbSize = sizeof(sif);
				GetScrollInfo(&sif);
			
				if(point.y < m_ThumbRect.top)
				{
					m_bMouseDownArrowForback = TRUE;
					pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,0),(LPARAM)m_hWnd);
				}
				else
				{
					m_bMouseDownArrowForwad = TRUE;
					pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,0),(LPARAM)m_hWnd);
				}
			}
			
			SetTimer(1,100,NULL);
		}
	}
	else
	{
		if(m_ThumbRect.PtInRect(point))
		{
			m_bMouseDown = true;
			m_MouseDownPoint.SetPoint(point.x - m_ThumbRect.left,point.y - m_ThumbRect.top);
		
			CClientDC dc(this);
			DrawHorizontal(&dc);
		}
		else if(m_RightArrowRect.PtInRect(point))
		{
			m_bMouseDownArrowForwad = true;
			pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,0),(LPARAM)m_hWnd);
			CClientDC dc(this);
			DrawHorizontal(&dc);
			
			SetTimer(2,100,NULL);
		}
		else if(m_LeftArrowRect.PtInRect(point))
		{
			m_bMouseDownArrowForback = true;
			pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,0),(LPARAM)m_hWnd);
			CClientDC dc(this);
			DrawHorizontal(&dc);
			
			SetTimer(2,100,NULL);
		}
		else if(m_AreaRect.PtInRect(point))
		{
			if(pParent != NULL)
			{
				SCROLLINFO sif;
				ZeroMemory(&sif,sizeof(sif));
				sif.cbSize = sizeof(sif);
				GetScrollInfo(&sif,SIF_PAGE|SIF_RANGE|SIF_POS|SIF_TRACKPOS);

				if(point.x < m_ThumbRect.left)
				{
					m_bMouseDownArrowForback = TRUE;
					pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,0),(LPARAM)m_hWnd);
				}
				else
				{
					m_bMouseDownArrowForwad = TRUE;
					pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,0),(LPARAM)m_hWnd);
				}
			}

			SetTimer(1,100,NULL);
		}
	}
}

void CRTScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_IsEnableSkin)return CScrollBar::OnLButtonUp(nFlags, point);
	ReleaseCapture();
	
	DWORD style = GetStyle();
	CWnd *pParent = GetParent();

	if(m_bDragging)
	{
		m_bDragging = FALSE;
		CPoint mousePoint(m_ThumbRect.left + m_MouseDownPoint.x,m_ThumbRect.top + m_MouseDownPoint.y);
		if( (style & SBS_VERT) == SBS_VERT)
		{
			if(mousePoint.y != point.y)
			{
				int move = point.y - mousePoint.y;
				double pos = m_ThumbRect.top + move - m_AreaRect.top;
				double height = m_AreaRect.Height() - m_ThumbRect.Height();
				double per = pos / height;
				SCROLLINFO sif;
				GetScrollInfo(&sif);
				int newpos = (int)((sif.nMax - sif.nMin) * per);
				if(newpos < sif.nMin)newpos = sif.nMin;
				if(newpos > sif.nMax)newpos = sif.nMax;
				if(pParent != NULL)
				{
					pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,newpos),(LPARAM)m_hWnd);
				}
			}
		}
		else
		{
			if(mousePoint.x == point.x)
			{
				int move = point.x - mousePoint.x;
				double pos = m_ThumbRect.left + move - m_AreaRect.left;
				double width = m_AreaRect.Width() - m_ThumbRect.Width();
				double per = pos / width;
				SCROLLINFO sif;
				GetScrollInfo(&sif);
				int newpos = (int)((sif.nMax - sif.nMin) * per);
				if(newpos < sif.nMin)newpos = sif.nMin;
				if(newpos > sif.nMax)newpos = sif.nMax;
				if(pParent != NULL)
				{
					pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBPOSITION,newpos),(LPARAM)m_hWnd);
				}
			}
		}
	}
	
	if(m_bMouseDown || m_bMouseDownArrowForback || m_bMouseDownArrowForwad)
	{
		m_bMouseDown = FALSE;
		m_bMouseDownArrowForback = FALSE;
		m_bMouseDownArrowForwad = FALSE;
        KillTimer(2);
		KillTimer(1);
		CClientDC dc(this);
		if((style & SBS_VERT) == SBS_VERT)
			DrawVertical(&dc);
		else
			DrawHorizontal(&dc);
	}
}

void CRTScrollBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_IsEnableSkin)return CScrollBar::OnMouseMove(nFlags, point);
    
	CWnd *pParent = GetParent();
	if(pParent == NULL)return;

	if(m_bMouseDown)m_bDragging = TRUE;
	if(!m_bDragging)return;

	CPoint mousePoint(m_ThumbRect.left + m_MouseDownPoint.x,m_ThumbRect.top + m_MouseDownPoint.y);
	DWORD style = GetStyle();

	if( (style & SBS_VERT) == SBS_VERT)
	{
		if(mousePoint.y == point.y)return;

		int move = point.y - mousePoint.y;
		double pos = m_ThumbRect.top + move - m_AreaRect.top;
		double height = m_AreaRect.Height() - m_ThumbRect.Height();
		double per = pos / height;
		SCROLLINFO sif;
		GetScrollInfo(&sif);
		int newpos = (int)((sif.nMax - sif.nMin) * per);
		if(newpos < sif.nMin) newpos = sif.nMin;
		if(newpos > sif.nMax) newpos = sif.nMax;
		sif.nTrackPos = newpos;
		if(m_nThumbTrackPos != newpos)
		{
			m_nThumbTrackPos = newpos;
			CClientDC dc(this);
			DrawVertical(&dc);
			pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBTRACK,sif.nTrackPos),(LPARAM)m_hWnd);
		}
	}
	else
	{
		if(mousePoint.x == point.x)return;

		int move = point.x - mousePoint.x;
		double pos = m_ThumbRect.left + move - m_AreaRect.left;
		double width = m_AreaRect.Width() - m_ThumbRect.Width();
		double per = pos / width;
		SCROLLINFO sif;
		GetScrollInfo(&sif);
		int newpos = (int)((sif.nMax - sif.nMin) * per);
		if(newpos < sif.nMin) newpos = sif.nMin;
		if(newpos > sif.nMax) newpos = sif.nMax;
		sif.nTrackPos = newpos;

		if(m_nThumbTrackPos != newpos)
		{
			m_nThumbTrackPos = newpos;
			CClientDC dc(this);
			DrawHorizontal(&dc);
			pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_THUMBTRACK,sif.nTrackPos),(LPARAM)m_hWnd);
		}
	}
}

void CRTScrollBar::OnTimer(UINT nIDEvent)
{
	if(!m_IsEnableSkin)return CScrollBar::OnTimer(nIDEvent);
	
	
	if(m_MouseDownTime < 3)
	{
		m_MouseDownTime++;
		return;
	}
	CWnd* pParent = GetParent();

	DWORD style = GetStyle();
	SCROLLINFO sif;
	GetScrollInfo(&sif);
	if( (style & SBS_VERT) == SBS_VERT)
	{
		if(nIDEvent == 2)
		{
			if(m_bMouseDownArrowForback)
			{
				pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),(LPARAM)m_hWnd);
			}
			else if(m_bMouseDownArrowForwad)
			{
				pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),(LPARAM)m_hWnd);
			}
		}
		else if(nIDEvent == 1)
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			if(!m_ThumbRect.PtInRect(point))
			{
				if(m_bMouseDownArrowForback)
				{
					pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,0),(LPARAM)m_hWnd);
				}
				else if(m_bMouseDownArrowForwad)
				{
					pParent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,0),(LPARAM)m_hWnd);
				}
			}
		}
	}
	else
	{
		if(nIDEvent == 2)
		{
			if(m_bMouseDownArrowForback)
			{
				pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,0),(LPARAM)m_hWnd);
			}
			else if(m_bMouseDownArrowForwad)
			{
				pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,0),(LPARAM)m_hWnd);
			}
		}
		else if(nIDEvent == 1)
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			if(!m_ThumbRect.PtInRect(point))
			{
				if(m_bMouseDownArrowForback)
				{
					pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,0),(LPARAM)m_hWnd);
				}
				else if(m_bMouseDownArrowForwad)
				{
					pParent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,0),(LPARAM)m_hWnd);
				}
			}
		}
	}
}

void CRTScrollBar::DrawHorizontal(CDC *pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CMemDC memDC(pDC, &clientRect);
	
	BOOL IsWindowEnable = IsWindowEnabled();
	//draw left arrow of scrollbar
	m_LeftArrowRect.SetRect(clientRect.left,clientRect.top,clientRect.left + clientRect.Height(),clientRect.bottom);
	
	if(m_SizeArea == AreaLeft)
	{
		CRTDraw::RTDrawBitmap(&memDC,&m_LeftArrowRect,m_ScrollBarHorizontalBitmap[BMP_BACK],m_ScrollBarHorizontalBitmapDrawMode[BMP_BACK]);
		m_LeftArrowRect.OffsetRect(clientRect.Height() + 1,0);
	}
	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_LeftArrowRect,m_ScrollBarLeftArrowBitmap[BMP_DISABLE],m_ScrollBarLeftArrowBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bMouseDownArrowForback)
			CRTDraw::RTDrawBitmap(&memDC,&m_LeftArrowRect,m_ScrollBarLeftArrowBitmap[BMP_DOWN],m_ScrollBarLeftArrowBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_LeftArrowRect,m_ScrollBarLeftArrowBitmap[BMP_NORMAL],m_ScrollBarLeftArrowBitmapDrawMode[BMP_NORMAL]);
	}
	
	//draw right arrow
	m_RightArrowRect.SetRect(clientRect.right - clientRect.Height(),clientRect.top,clientRect.right,clientRect.bottom);
	
	if(m_SizeArea == AreaRight)
	{
		CRTDraw::RTDrawBitmap(&memDC,&m_RightArrowRect,m_ScrollBarHorizontalBitmap[BMP_BACK],m_ScrollBarHorizontalBitmapDrawMode[BMP_BACK]);
		m_RightArrowRect.OffsetRect(0-clientRect.Height()  - 1,0);
	}
	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_RightArrowRect,m_ScrollBarRightArrowBitmap[BMP_DISABLE],m_ScrollBarRightArrowBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bMouseDownArrowForwad)
			CRTDraw::RTDrawBitmap(&memDC,&m_RightArrowRect,m_ScrollBarRightArrowBitmap[BMP_DOWN],m_ScrollBarRightArrowBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_RightArrowRect,m_ScrollBarRightArrowBitmap[BMP_NORMAL],m_ScrollBarRightArrowBitmapDrawMode[BMP_NORMAL]);
	}
	
	//draw back
	m_AreaRect.SetRect(m_LeftArrowRect.right + 1,clientRect.top,m_RightArrowRect.left - 1,clientRect.bottom);
	CRTDraw::RTDrawBitmap(&memDC,&m_AreaRect,m_ScrollBarHorizontalBitmap[BMP_BACK],m_ScrollBarHorizontalBitmapDrawMode[BMP_BACK]);

	SCROLLINFO sif;
	GetScrollInfo(&sif);
	if(m_bDragging)sif.nPos = m_nThumbTrackPos;

	int ThumbWidth = 30;
	if(ThumbWidth > m_AreaRect.Width())ThumbWidth = m_AreaRect.Width() / 2;

	double width = m_AreaRect.Width() - ThumbWidth;
	double range = sif.nMax - sif.nMin;
	double per = width / range;
	int pos = (int)(per * (sif.nPos - sif.nMin));

	if(range <= 0)return;

	m_ThumbRect.left =  m_AreaRect.left + pos;
	m_ThumbRect.top = clientRect.top;
	m_ThumbRect.right = m_ThumbRect.left + ThumbWidth;
	m_ThumbRect.bottom = clientRect.bottom;

	if(m_ThumbRect.right == m_ThumbRect.left)m_ThumbRect.right = m_ThumbRect.left + 3;

	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarHorizontalBitmap[BMP_DISABLE],m_ScrollBarHorizontalBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bDragging)
			CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarHorizontalBitmap[BMP_DOWN],m_ScrollBarHorizontalBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarHorizontalBitmap[BMP_NORMAL],m_ScrollBarHorizontalBitmapDrawMode[BMP_NORMAL]);
	}
}

void CRTScrollBar::DrawVertical(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CMemDC memDC(pDC, &clientRect);
	
	BOOL IsWindowEnable = IsWindowEnabled();
	//draw up arrow of scrollbar
	m_TopArrowRect.SetRect(clientRect.left,clientRect.top,clientRect.right,clientRect.top + clientRect.Width());
	if(m_SizeArea == AreaTop)
	{
		CRTDraw::RTDrawBitmap(&memDC,&m_TopArrowRect,m_ScrollBarVerticalBitmap[BMP_BACK],m_ScrollBarVerticalBitmapDrawMode[BMP_BACK]);
		m_TopArrowRect.OffsetRect(0,clientRect.Width() + 1);
	}
	
	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_TopArrowRect,m_ScrollBarUpArrowBitmap[BMP_DISABLE],m_ScrollBarUpArrowBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bMouseDownArrowForback)
			CRTDraw::RTDrawBitmap(&memDC,&m_TopArrowRect,m_ScrollBarUpArrowBitmap[BMP_DOWN],m_ScrollBarUpArrowBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_TopArrowRect,m_ScrollBarUpArrowBitmap[BMP_NORMAL],m_ScrollBarUpArrowBitmapDrawMode[BMP_NORMAL]);
	}
	
	//draw right arrow
	m_BottomArrowRect.SetRect(clientRect.left,clientRect.bottom - clientRect.Width(),clientRect.right,clientRect.bottom);
	
	if(m_SizeArea == AreaBottom)
	{
		CRTDraw::RTDrawBitmap(&memDC,&m_BottomArrowRect,m_ScrollBarVerticalBitmap[BMP_BACK],m_ScrollBarVerticalBitmapDrawMode[BMP_BACK]);
		m_BottomArrowRect.OffsetRect(0,0-clientRect.Width() + 1);
	}
	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_BottomArrowRect,m_ScrollBarDownArrowBitmap[BMP_DISABLE],m_ScrollBarDownArrowBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bMouseDownArrowForwad)
			CRTDraw::RTDrawBitmap(&memDC,&m_BottomArrowRect,m_ScrollBarDownArrowBitmap[BMP_DOWN],m_ScrollBarDownArrowBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_BottomArrowRect,m_ScrollBarDownArrowBitmap[BMP_NORMAL],m_ScrollBarDownArrowBitmapDrawMode[BMP_NORMAL]);
	}
	
	//draw back
	m_AreaRect.SetRect(clientRect.left,m_TopArrowRect.bottom + 1,clientRect.right,m_BottomArrowRect.top - 1);
	CRTDraw::RTDrawBitmap(&memDC,&m_AreaRect,m_ScrollBarVerticalBitmap[BMP_BACK],m_ScrollBarVerticalBitmapDrawMode[BMP_BACK]);

	SCROLLINFO sif;
	GetScrollInfo(&sif);
	if(m_bDragging)sif.nPos = m_nThumbTrackPos;

	int ThumbWidth = 30;
	if(ThumbWidth > m_AreaRect.Height())ThumbWidth = m_AreaRect.Height()/2;
	double height = m_AreaRect.Height() - ThumbWidth;
	double range = sif.nMax - sif.nMin;
	double per = height / range;
	int pos = (int)(per * (sif.nPos - sif.nMin));
	
	if( range <= 0)return;
	m_ThumbRect.left = clientRect.left;
	m_ThumbRect.top = m_AreaRect.top + pos;
	m_ThumbRect.right = clientRect.right;
	m_ThumbRect.bottom = m_ThumbRect.top + ThumbWidth;
	
	if(!IsWindowEnable)
		CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarVerticalBitmap[BMP_DISABLE],m_ScrollBarVerticalBitmapDrawMode[BMP_DISABLE]);
	else
	{
		if(m_bDragging)
			CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarVerticalBitmap[BMP_DOWN],m_ScrollBarVerticalBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&m_ThumbRect,m_ScrollBarVerticalBitmap[BMP_NORMAL],m_ScrollBarVerticalBitmapDrawMode[BMP_NORMAL]);
	}
}

void CRTScrollBar::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}

void CRTScrollBar::SetHorizontalBitmap(CBitmap* pHorizontal[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarHorizontalBitmap[i] = pHorizontal[i];
		m_ScrollBarHorizontalBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTScrollBar::SetVerticalBitmap(CBitmap* pVertical[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarVerticalBitmap[i] = pVertical[i];
		m_ScrollBarVerticalBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTScrollBar::SetVerticalUpArrowBitmap(CBitmap* pVertical[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarUpArrowBitmap[i] = pVertical[i];
		m_ScrollBarUpArrowBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTScrollBar::SetVerticalDownArrowBitmap(CBitmap* pVertical[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarDownArrowBitmap[i] = pVertical[i];
		m_ScrollBarDownArrowBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTScrollBar::SetHorizontalLeftArrowBitmap(CBitmap* pHorizontal[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarLeftArrowBitmap[i] = pHorizontal[i];
		m_ScrollBarLeftArrowBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTScrollBar::SetHorizontalRightArrowBitmap(CBitmap* pHorizontal[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ScrollBarRightArrowBitmap[i] = pHorizontal[i];
		m_ScrollBarRightArrowBitmapDrawMode[i] = DrawMode[i];
	}
}
BOOL CRTScrollBar::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)
		return CScrollBar::OnEraseBkgnd(pDC);
	else
		return FALSE;
}

void CRTScrollBar::SetSizeArea(ScrollBarSizeArea Area)
{
	m_SizeArea = Area;
	if(IsWindow(m_hWnd))Invalidate();
}

void CRTScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_IsEnableSkin)return CScrollBar::OnLButtonDblClk(nFlags, point);

	OnLButtonDown(nFlags,point);
	OnLButtonUp(nFlags,point);
}


void CRTScrollBar::SetRange(int nmin, int nmax,BOOL bRedraw)
{
	int nMax,nMin;
	GetScrollRange(&nMin,&nMax);
	if(nmin != nMax || nmax != nMax)
	{
		if(m_IsEnableSkin)
		{
			SetScrollRange(nmin,nmax,FALSE);
			RedrawWindow();
		}
		else
		{
			SetScrollRange(nmin,nmax,bRedraw);
		}
	}
}

void CRTScrollBar::SetPos(int pos, BOOL redraw)
{
	int nPos = GetScrollPos();
	if(pos == nPos)return;

	if(!m_IsEnableSkin)
	{
		SetScrollPos(pos,redraw);
	}
	else
	{
		SetScrollPos(pos,FALSE);
		RedrawWindow();
	}
}
