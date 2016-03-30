// RTScrollControlContainer.cpp : 实现文件
//

#include "stdafx.h"
#include "RTScrollControlContainer.h"


// CRTScrollControlContainer

IMPLEMENT_DYNAMIC(CRTScrollControlContainer, CWnd)
CRTScrollControlContainer::CRTScrollControlContainer()
{
	m_Control = NULL;
}

CRTScrollControlContainer::~CRTScrollControlContainer()
{
}


BEGIN_MESSAGE_MAP(CRTScrollControlContainer, CWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CRTScrollControlContainer 消息处理程序


void CRTScrollControlContainer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Control->SendMessage(WM_HSCROLL,MAKEWPARAM(nSBCode,nPos),NULL);
}

void CRTScrollControlContainer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Control->SendMessage(WM_VSCROLL,MAKEWPARAM(nSBCode,nPos),NULL);
}

BOOL CRTScrollControlContainer::Create(CWnd* pControl)
{
	if(pControl == NULL)return FALSE;

	m_Control = pControl;

	CRect rtWnd;
	pControl->GetWindowRect(&rtWnd);
	pControl->GetParent()->ScreenToClient(&rtWnd);
	DWORD style = WS_CHILD|WS_VISIBLE;
	DWORD ExStyle = 0;
    DWORD MoveStyle = 0;
	DWORD MoveExStyle = 0;
	DWORD ListStyle = pControl->GetStyle();
	DWORD ListExStyle = pControl->GetExStyle();

	if((ListStyle & WS_BORDER) == WS_BORDER)
	{
		style |= WS_BORDER;
		MoveStyle = WS_BORDER;
	}
	if((ListStyle & WS_DLGFRAME) == WS_DLGFRAME)
	{
		style |= WS_DLGFRAME;
		MoveStyle |= WS_DLGFRAME;
	}
	if((ListExStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
	{
		ExStyle |= WS_EX_CLIENTEDGE;
		MoveExStyle |= WS_EX_CLIENTEDGE;
	}
	if((ListExStyle & WS_EX_STATICEDGE) == WS_EX_STATICEDGE)
	{
		ExStyle |= WS_EX_STATICEDGE;
		MoveExStyle |= WS_EX_STATICEDGE;
	}
	if((ListExStyle & WS_EX_WINDOWEDGE ) == WS_EX_WINDOWEDGE)
	{
		ExStyle |= WS_EX_WINDOWEDGE;
		MoveExStyle |= WS_EX_WINDOWEDGE;
	}

	if(!CreateEx(ExStyle,NULL,NULL,style,rtWnd,pControl->GetParent(),65535))return FALSE;

	pControl->SetParent(this);
	GetClientRect(&rtWnd);

	pControl->ModifyStyle(MoveStyle,0);
	pControl->ModifyStyleEx(MoveExStyle,0);
	
	m_VerScrollBar.Create(WS_CHILD|SBS_VERT,CRect(0,0,0,0),pControl->GetParent(),65535);
	m_HorScrollBar.Create(WS_CHILD|SBS_HORZ,CRect(0,0,0,0),pControl->GetParent(),65535);
	m_VerScrollBar.SetSizeArea(CRTScrollBar::AreaBottom);
	InitializeFlatSB(pControl->m_hWnd);
	FlatSB_EnableScrollBar(pControl->m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);
	FlatSB_ShowScrollBar(pControl->m_hWnd,SB_BOTH,FALSE);
	ReSize();
	return 0;
}

void CRTScrollControlContainer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(!IsWindow(m_HorScrollBar.m_hWnd))return;
	if(!IsWindow(m_VerScrollBar.m_hWnd))return;
	if(!IsWindow(m_Control->m_hWnd))return;
	ReSize();
}


void CRTScrollControlContainer::ReSize(void)
{
	if(m_Control == NULL)return;
	CRect rtWnd;
	GetClientRect(&rtWnd);

	CRect rtList;
	CRect rtVer;
	CRect rtHor;

	DWORD ListExStyle = m_Control->GetExStyle();
	int ScrollWidth = 0;
	int ScrollHeight = 0;

	if(m_VerScrollBar.IsWindowVisible())
	{
		ScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	}
	
	if(m_HorScrollBar.IsWindowVisible())
	{
		ScrollHeight = GetSystemMetrics(SM_CYHSCROLL);
	}

	if((ListExStyle & WS_EX_RIGHTSCROLLBAR) == WS_EX_RIGHTSCROLLBAR)
	{
		rtList.SetRect(rtWnd.left,rtWnd.top,rtWnd.right - ScrollWidth,rtWnd.bottom - ScrollHeight);
		rtVer.SetRect(rtList.right,rtWnd.top,rtWnd.right,rtWnd.bottom);
		rtHor.SetRect(rtWnd.left,rtList.bottom,rtList.right,rtWnd.bottom);
	}
	else
	{
		rtList.SetRect(rtWnd.left + ScrollWidth,rtWnd.top,rtWnd.right,rtWnd.bottom - ScrollHeight);
		rtVer.SetRect(rtWnd.left,rtWnd.top,rtList.left,rtWnd.bottom);
		rtHor.SetRect(rtList.left,rtList.bottom,rtWnd.right,rtWnd.bottom);
	}
	if(IsWindow(m_Control->m_hWnd))m_Control->MoveWindow(&rtList);
	if(IsWindow(m_VerScrollBar.m_hWnd))m_VerScrollBar.MoveWindow(&rtVer);
	if(IsWindow(m_HorScrollBar.m_hWnd))m_HorScrollBar.MoveWindow(&rtHor);
}

void CRTScrollControlContainer::SetHorPos(int pos, BOOL redraw)
{
	m_HorScrollBar.SetPos(pos,redraw);
}

void CRTScrollControlContainer::SetVerRange(int nmin, int nmax,BOOL redraw)
{
	DWORD style = m_Control->GetStyle();

	if((style & WS_VSCROLL) == WS_VSCROLL)
	{
		if(!m_VerScrollBar.IsWindowVisible())
		{
			m_VerScrollBar.ShowWindow(SW_SHOW);
			if(m_HorScrollBar.IsWindowVisible())
			{
				m_VerScrollBar.SetSizeArea(CRTScrollBar::AreaBottom);
			}
			else
			{
				m_VerScrollBar.SetSizeArea(CRTScrollBar::AreaNone);
			}
			ReSize();
		}
	}
	else
	{
		if(m_VerScrollBar.IsWindowVisible())
		{
			m_VerScrollBar.ShowWindow(SW_HIDE);
			ReSize();
		}
	}
	m_VerScrollBar.SetRange(nmin,nmax,redraw);
}


void CRTScrollControlContainer::SetVerPos(int pos, BOOL redraw)
{
	m_VerScrollBar.SetPos(pos,redraw);
}

void CRTScrollControlContainer::SetHorRange(int nmin, int nmax,BOOL redraw)
{
	DWORD style = m_Control->GetStyle();

	if((style & WS_HSCROLL) == WS_HSCROLL)
	{
		if(!m_HorScrollBar.IsWindowVisible())
		{
			m_HorScrollBar.ShowWindow(SW_SHOW);
			m_VerScrollBar.SetSizeArea(CRTScrollBar::AreaBottom);
			ReSize();
		}
	}
	else
	{
		if(m_HorScrollBar.IsWindowVisible())
		{
			m_HorScrollBar.ShowWindow(SW_HIDE);
			m_VerScrollBar.SetSizeArea(CRTScrollBar::AreaNone);
			ReSize();
		}
	}
	
	m_HorScrollBar.SetRange(nmin,nmax,redraw);
}

BOOL CRTScrollControlContainer::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CRTScrollControlContainer::PassMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	return FALSE;
}

LRESULT CRTScrollControlContainer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message > WM_USER)
	{
		CWnd* pParent = GetParent();
		if(pParent != NULL)pParent->SendMessage(message,wParam,lParam);
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
