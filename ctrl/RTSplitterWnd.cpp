// RTSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "RTSplitterWnd.h"
#include "RTDraw.h"

// CRTSplitterWnd

IMPLEMENT_DYNAMIC(CRTSplitterWnd, CWnd)
CRTSplitterWnd::CRTSplitterWnd()
{
	m_Child[0] = NULL;
	m_Child[1] = NULL;
	m_IsVertical = FALSE;
	m_SplitterRect.SetRect(0,0,0,0);
	m_SplitterWidth = 4;
	m_SplitterSize.cx = 200;
	m_SplitterSize.cy = -1;
	m_IsSelSplitter = FALSE;
	m_ImageList = NULL;
}

CRTSplitterWnd::~CRTSplitterWnd()
{
	if(m_ImageList != NULL) 
	{
		m_ImageList->DeleteImageList();
		delete m_ImageList;
	}
}

CBitmap* CRTSplitterWnd::m_SplitterBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTSplitterWnd::m_SplitterBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTSplitterWnd::m_IsEnableSkin = TRUE;
HCURSOR	 CRTSplitterWnd::m_VerCursor = NULL;
HCURSOR  CRTSplitterWnd::m_HorCursor = NULL;

BEGIN_MESSAGE_MAP(CRTSplitterWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CRTSplitterWnd 消息处理程序


void CRTSplitterWnd::SetVertical(CWnd* pLeft, CWnd* pRight, int LeftWidth, int RightWidth)
{
	ASSERT(pLeft != NULL);
	ASSERT(pRight != NULL);

	m_Child[0] = pLeft;
	m_Child[1] = pRight;
	pLeft->SetParent(this);
	pRight->SetParent(this);
	pLeft->ShowWindow(SW_SHOW);
	pRight->ShowWindow(SW_SHOW);
	m_IsVertical = TRUE;
	m_SplitterSize.cx = LeftWidth;
	m_SplitterSize.cy = RightWidth;
	RTSize();
}

void CRTSplitterWnd::SetHorizoncal(CWnd* pTop, CWnd* pBottom, int TopWidth, int BottomWidth)
{
	ASSERT(pTop != NULL);
	ASSERT(pBottom != NULL);

	m_Child[0] = pTop;
	m_Child[1] = pBottom;
	pTop->SetParent(this);
	pBottom->SetParent(this);
	pTop->ShowWindow(SW_SHOW);
	pBottom->ShowWindow(SW_SHOW);
	m_IsVertical = FALSE;
	m_SplitterSize.cx = TopWidth;
	m_SplitterSize.cy = BottomWidth;
	RTSize();
}

void CRTSplitterWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if(!IsWindow(m_hWnd))return;

	RTSize();
}

void CRTSplitterWnd::RTSize()
{
	CRect rt;
	GetClientRect(&rt);
	if(m_IsVertical)
	{
		int left = 0;
		int right = 0;
		if(m_SplitterSize.cx == -1)
		{
			if(m_SplitterSize.cy > rt.Width() - m_SplitterWidth)
				right = rt.Width() - m_SplitterWidth;
			else
				right = m_SplitterSize.cy;

			left = rt.Width() - right - m_SplitterWidth;
		}
		else
		{
			if(rt.Width() - m_SplitterWidth < m_SplitterSize.cx)
				left = rt.Width() - m_SplitterWidth;
			else
				left = m_SplitterSize.cx;
			
			right = rt.Width() - m_SplitterWidth - left;
		}
		m_SplitterRect.SetRect(rt.left + left,rt.top,rt.left + left + m_SplitterWidth,rt.bottom);

		if(m_Child[0] != NULL)m_Child[0]->MoveWindow(rt.left,rt.top,left,rt.Height());
		if(m_Child[1] != NULL)m_Child[1]->MoveWindow(rt.left + left + m_SplitterWidth,rt.top,right,rt.Height());
	}
	else
	{
		int top ;
		int bottom;

		if(m_SplitterSize.cx == -1)
		{
			if(m_SplitterSize.cy > rt.Height() - m_SplitterWidth)
				bottom = rt.Height() - m_SplitterWidth;
			else
				bottom = m_SplitterSize.cy;

			top = rt.Height() - bottom - m_SplitterWidth;
		}
		else
		{
			if(rt.Height() - m_SplitterWidth < m_SplitterSize.cx)
				top = rt.Height() - m_SplitterWidth;
			else
				top = m_SplitterSize.cx;

			bottom = rt.Height() - top - m_SplitterWidth;
		}
		m_SplitterRect.SetRect(rt.left,rt.top + top,rt.right,rt.top + top + m_SplitterWidth);
		if(m_Child[0] != NULL)m_Child[0]->MoveWindow(rt.left,rt.top,rt.Width(),top);
		if(m_Child[1] != NULL)m_Child[1]->MoveWindow(rt.left,rt.top + top + m_SplitterWidth,rt.Width(),bottom);
	}
	Invalidate();
}

BOOL CRTSplitterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	return TRUE;
}

BOOL CRTSplitterWnd::Create(DWORD dwStyle,const RECT &rect,CWnd* pParentWnd,UINT nID,CCreateContext *pContext)
{
	return CWnd::Create(NULL,NULL,dwStyle,rect,pParentWnd,nID,pContext);
}

BOOL CRTSplitterWnd::OnEraseBkgnd(CDC* pDC)
{
	if(m_Child[0] != NULL && m_Child[1] != NULL)return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CRTSplitterWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_SplitterBitmap[BMP_BACK] != NULL && m_IsEnableSkin)
	{
		CRTDraw::RTDrawBitmap(&dc,&m_SplitterRect,m_SplitterBitmap[BMP_BACK],m_SplitterBitmapDrawMode[BMP_BACK]);
	}
	else
	{
		CBrush br(GetSysColor(COLOR_3DFACE));
		dc.FillRect(&m_SplitterRect,&br);
	}
}

void CRTSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags == MK_LBUTTON)
	{
		if(m_IsSelSplitter)
		{
			CPoint MovePoint(point.x,point.y);
			CRect rt;
			GetClientRect(&rt);
			if(m_IsVertical)
			{
				int SPLeft = point.x - m_MouseDownPoint.x;
				if(SPLeft < rt.left || SPLeft + m_SplitterWidth > rt.right)
					return;
		
				if(m_SplitterSize.cx == -1)
					m_SplitterSize.cy = rt.right - SPLeft - m_SplitterWidth;
				else
					m_SplitterSize.cx = SPLeft;

				MovePoint.y = 0;
			}
			else
			{
				int SPTop = point.y - m_MouseDownPoint.y;
				if(SPTop < rt.top || SPTop + m_SplitterWidth > rt.bottom)
					return;

				if(m_SplitterSize.cx == -1)
					m_SplitterSize.cy = rt.bottom - SPTop - m_SplitterWidth;
				else
					m_SplitterSize.cx = SPTop;

				MovePoint.x = 0;
			}
			m_ImageList->DragMove(MovePoint);
		}
		else
		{
			CWnd::OnMouseMove(nFlags, point);
		}
	}
	else if(nFlags == 0)
	{
		static int debug = 0;
		//XTRACE("detect mouse move on split: %d\n", debug++);
		if(m_SplitterRect.PtInRect(point))
		{
			if(m_IsSelSplitter == FALSE)
			{
				m_IsSelSplitter = TRUE;
			}
		}
		else
		{
			if(m_IsSelSplitter == TRUE)
			{
				m_IsSelSplitter = FALSE;
			}
			CWnd::OnMouseMove(nFlags, point);
		}
	}
	else
	{
		CWnd::OnMouseMove(nFlags, point);
	}
}

void CRTSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_IsSelSplitter)
	{  
		if(m_ImageList == NULL)
		{
			m_ImageList = new CImageList();
			m_ImageList->Create(m_SplitterRect.Width(),m_SplitterRect.Height(),ILC_COLOR ,1,1);
		
			CBitmap bmp;
			int CxBitCount = m_SplitterRect.Width() / 16;
			if((m_SplitterRect.Width() % 8 ) != 0)CxBitCount++;
			int CyBitCount = m_SplitterRect.Height();

			WORD *bits = new WORD[CxBitCount * CyBitCount];
			WORD *ptr = bits;

			for(int y = 0; y < CyBitCount; y++)
			{
				if((y % 2) == 0)
				{
					for(int x = 0; x < CxBitCount; x++)
					{
						*ptr = 0x5555;
						ptr++;
					}
				}
				else
				{
					for(int x = 0; x < CxBitCount; x++)
					{
						*ptr = 0xaaaa;
						ptr++;
					}
				}
			}

			bmp.CreateBitmap(m_SplitterRect.Width(),m_SplitterRect.Height(),1,1,bits);
			m_ImageList->Add(&bmp,&bmp);
			delete[] bits;
			CPoint BeginPoint(point.x,point.y);
			m_MouseDownPoint.SetPoint(point.x - m_SplitterRect.left,point.y - m_SplitterRect.top);
			if(m_IsVertical)
			{			
				BeginPoint.y  = 0;
				m_MouseDownPoint.y = 0;
			}
			else
			{
				BeginPoint.x = 0;
				m_MouseDownPoint.x = 0;
			}
			m_ImageList->BeginDrag(0,m_MouseDownPoint);
			m_ImageList->DragEnter(this,BeginPoint);
			SetCapture();
		}
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

void CRTSplitterWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_IsSelSplitter)
	{
		ReleaseCapture();
		if(m_ImageList != NULL)
		{
			m_ImageList->EndDrag();
			m_ImageList->DeleteImageList();
			delete m_ImageList;
			m_ImageList = NULL;
		}
		RTSize();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CRTSplitterWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_IsSelSplitter)
	{
		if(m_IsVertical)
		{
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}
		else
		{
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		return;
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CRTSplitterWnd::SetBitmap(CBitmap* pBitmaps[], UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_SplitterBitmap[i] = pBitmaps[i];
		m_SplitterBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTSplitterWnd::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}


BOOL CRTSplitterWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	GetParent()->SendMessage(WM_NOTIFY,wParam,lParam);
	return CWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CRTSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	BOOL rt = CWnd::OnSetCursor(pWnd, nHitTest, message);

	if(m_VerCursor == NULL)m_VerCursor = LoadCursor(NULL,IDC_SIZEWE);
	if(m_HorCursor == NULL)m_HorCursor = LoadCursor(NULL,IDC_SIZENS);

	if(m_IsSelSplitter)
	{
		if(m_IsVertical && m_VerCursor != NULL)
		{
			SetCursor(m_VerCursor);
			//XTRACE("设置垂直分割鼠标\n");
		}

		if(!m_IsVertical && m_HorCursor != NULL)
		{
			SetCursor(m_HorCursor);
			//XTRACE("设置水平分割鼠标\n");
		}
	}

	return  rt;
}

int CRTSplitterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CRTSplitterWnd::RTSetCursor(HCURSOR hVerCursor,HCURSOR hHorCursor)
{
	m_VerCursor = hVerCursor;
	m_HorCursor = hHorCursor;
}