// RTFrameWnd.cpp : CMainFrame 类的实现


#include "stdafx.h"
#include "RTFrameWnd.h"
#include "RTDraw.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRTFrameWnd

IMPLEMENT_DYNCREATE(CRTFrameWnd, CFrameWnd)
IMPLEMENT_MENUXP(CRTFrameWnd,CFrameWnd)

BEGIN_MESSAGE_MAP(CRTFrameWnd, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CRTFrameWnd 构造/销毁

CRTFrameWnd::CRTFrameWnd()
{
	m_bActive = FALSE;

	m_ExitButtonState = StateNormal;
	m_MinimizeButtonState = StateNormal;
	m_MaximizeButtonState = StateNormal;
	m_NcMouseState = MouseStateNormal;
	m_SelTitleButton = NoneButton;
}

CRTFrameWnd::~CRTFrameWnd()
{
	
}

COLORREF CRTFrameWnd::m_BorderColor[4] = {0xAD9E9C,0xCEBEB5,0xCEBEB5,0xDED3CE};
BOOL     CRTFrameWnd::m_bEnableSkin = FALSE;
BOOL     CRTFrameWnd::m_bEnableBackAndBorder = FALSE;
BOOL     CRTFrameWnd::m_bEnableTitleBar = FALSE;
CBitmap* CRTFrameWnd::m_TitleBarBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTFrameWnd::m_MenuBarBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTFrameWnd::m_MaximizeButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTFrameWnd::m_MinimizeButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTFrameWnd::m_RestoreButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTFrameWnd::m_ExitButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};

UINT     CRTFrameWnd::m_TitleBarBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTFrameWnd::m_MenuBarBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTFrameWnd::m_MaximizeButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTFrameWnd::m_MinimizeButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTFrameWnd::m_ExitButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTFrameWnd::m_RestoreButtonBitmapDrawMode[5] = {0,0,0,0,0};


int CRTFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CRTFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


// CRTFrameWnd 诊断

#ifdef _DEBUG
void CRTFrameWnd::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CRTFrameWnd::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CRTFrameWnd 消息处理程序
BOOL CRTFrameWnd::RTDrawFrameBorder(CDC *pDC,HRGN hRgn)
{
	//画框架
	CRect rtWnd;
	GetWindowRect(&rtWnd);

	int Frame_BorderWidth = GetSystemMetrics(SM_CXFRAME);
	int Frame_BorderHeight =GetSystemMetrics(SM_CYFRAME);

	if(!m_bEnableBackAndBorder)
	{
		HRGN rgn = NULL;

		//left border
		rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.left + Frame_BorderWidth,rtWnd.bottom);
		if(hRgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hRgn,RGN_AND))
			{
				CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);
		
		//top border
		rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.top + Frame_BorderHeight);
		if(hRgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hRgn,RGN_AND))
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		//right border
		rgn = CreateRectRgn(rtWnd.right - Frame_BorderWidth,rtWnd.top,rtWnd.right,rtWnd.bottom);
		if(hRgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hRgn,RGN_AND))
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		//bottom
		rgn = CreateRectRgn(rtWnd.left,rtWnd.bottom - Frame_BorderHeight,rtWnd.right,rtWnd.bottom);
		if(hRgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hRgn,RGN_AND))
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);
		return TRUE;
	}

	CRect rtBorder;
	rtBorder.SetRect(0,0,rtWnd.Width(),rtWnd.Height());
	//画立体边框

	for(int i = 0; i < Frame_BorderWidth ; i ++)
	{
		if(i < 4)
		{
			pDC->Draw3dRect(i,i,rtWnd.Width() - 2*i,rtWnd.Height() - 2*i,m_BorderColor[i],m_BorderColor[i]);
		}
		else
		{
			pDC->Draw3dRect(i,i,rtWnd.Width() - 2*i,rtWnd.Height() - 2*i,m_BorderColor[3],m_BorderColor[3]);
		}
	}

	return TRUE;
}


BOOL CRTFrameWnd::RTDrawMenuBar(CDC *pDC,LPARAM lParam,HRGN hrgn)
{
	CMenu *pMenu = GetMenu();
	if(pMenu == NULL)return TRUE;

	CRect rtMenu;
	CRect rtWnd;
	
	GetWindowRect(&rtWnd);
	rtMenu.left = GetSystemMetrics(SM_CXFRAME);
	rtMenu.top = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CXFRAME) + 1;
	rtMenu.right = rtWnd.Width() - GetSystemMetrics(SM_CXFRAME);
	rtMenu.bottom = rtMenu.top +  GetSystemMetrics(SM_CYMENU);

	UINT ItemCount = pMenu->GetMenuItemCount();

	int PrevTop = rtMenu.top;
	int PrevLeft = rtMenu.left;
	int MenuItemHeight = GetSystemMetrics(SM_CYMENU);
	CRect rtMenuItem;
	
	//画菜单上部线
	CBrush brMenu(GetSysColor(COLOR_MENU));
	CBrush* pOldBrush = pDC->SelectObject(&brMenu);
	pDC->PatBlt(rtMenu.left,rtMenu.top - 1,rtMenu.Width(),1, PATCOPY);
	
	for(UINT items = 0; items < ItemCount; items++)
	{
		GetMenuItemRect(m_hWnd,pMenu->m_hMenu,items,&rtMenuItem);

		//画系统菜单
		HRGN rgn = CreateRectRgn(rtMenuItem.left,rtMenuItem.top,rtMenuItem.right,rtMenuItem.bottom);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		int MenuTop = rtMenuItem.top - rtWnd.top;
		MenuItemHeight = rtMenuItem.Height();
		if(MenuTop != PrevTop)
		{
			if(!CRTMenu::m_bEnableSkin)
			{
				rgn = CreateRectRgn(rtWnd.left + PrevLeft,rtWnd.top + PrevTop,rtWnd.right - GetSystemMetrics(SM_CXFRAME),rtWnd.top + PrevTop + MenuItemHeight);
				if(hrgn != NULL)
				{
					if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
					{
						CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
					}
				}
				else
				{
					CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
				}
				DeleteObject(rgn);
			}
			else
			{
				CRect rtDraw(PrevLeft,PrevTop,rtMenu.right,PrevTop + MenuItemHeight);
				CRTDraw::RTDrawBitmap(pDC,&rtDraw,CRTMenu::m_MenuBarBitmap[BMP_BACK],CRTMenu::m_MenuBarBitmapDrawMode[BMP_BACK]);
			}
			PrevTop = MenuTop;
			PrevLeft = rtMenu.left + rtMenuItem.Width();
		}
		else
		{
			PrevLeft += rtMenuItem.Width();
		}
	}

	if(!CRTMenu::m_bEnableSkin)
	{
		HRGN rgn = CreateRectRgn(rtWnd.left + PrevLeft,rtWnd.top + PrevTop,rtWnd.right - GetSystemMetrics(SM_CXFRAME),rtWnd.top + PrevTop + MenuItemHeight);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);
	}
	else
	{
		CRect rtDraw(PrevLeft,PrevTop,rtMenu.right,PrevTop + MenuItemHeight);
		CRTDraw::RTDrawBitmap(pDC,&rtDraw,CRTMenu::m_MenuBarBitmap[BMP_BACK],CRTMenu::m_MenuBarBitmapDrawMode[BMP_BACK]);
	}
	//画菜单上部线
	pDC->PatBlt(rtMenu.left,PrevTop + MenuItemHeight,rtMenu.Width(),1, PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

BOOL CRTFrameWnd::SetMinimizeButtonBitmap(CBitmap *MinimizeButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MinimizeButtonBitmap[i] = MinimizeButtonBitmap[i];
		m_MinimizeButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTFrameWnd::SetTitleBarBitmap(CBitmap *TitleBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_TitleBarBitmap[i] = TitleBarBitmap[i];
		m_TitleBarBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTFrameWnd::SetMenuBarBitmap(CBitmap* MenuBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MenuBarBitmap[i] = MenuBarBitmap[i];
		m_MenuBarBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTFrameWnd::SetMaximizeButtonBitmap(CBitmap* MaximizeButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MaximizeButtonBitmap[i] =  MaximizeButtonBitmap[i];
		m_MinimizeButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTFrameWnd::SetBorderColor(COLORREF ColorArray[])
{
	for(int i = 0; i < 5 ; i++)
		m_BorderColor[i] = ColorArray[i];
	return TRUE;
}

BOOL CRTFrameWnd::RTDrawTitleBar(CDC* pDC,BOOL bActive,HRGN hrgn)
{
	//取得标题栏的位置
	DWORD style = GetStyle();
	if((style & WS_CAPTION) != WS_CAPTION)return TRUE;

	CRect rtWnd;
	GetWindowRect(&rtWnd);

	if(!m_bEnableTitleBar)
	{
		rtWnd.left = rtWnd.left + GetSystemMetrics(SM_CXFRAME);
		rtWnd.top = rtWnd.top + GetSystemMetrics(SM_CYFRAME);
		rtWnd.right = rtWnd.right - GetSystemMetrics(SM_CXFRAME);
		rtWnd.bottom = rtWnd.top + GetSystemMetrics(SM_CYSIZE);

		HRGN rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.bottom);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CFrameWnd::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}

		DeleteObject(rgn);
		return TRUE;
	}

	CRect rtTitle;
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.Width() - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
	
	CMemDC memDC(pDC,rtTitle);
	CBitmap*  BackBitmap;
	COLORREF TitleTextColor;
	UINT     DrawMode;

	if(bActive)
	{
		BackBitmap = m_TitleBarBitmap[BMP_ACTIVE];
		DrawMode = m_TitleBarBitmapDrawMode[BMP_ACTIVE];
		TitleTextColor = GetSysColor(COLOR_CAPTIONTEXT);
	}
	else
	{
		BackBitmap = m_TitleBarBitmap[BMP_INACTIVE];
		DrawMode = m_TitleBarBitmapDrawMode[BMP_INACTIVE];
		TitleTextColor = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	}

	if(BackBitmap == NULL)return FALSE;
	
	//用位图画标题栏
	CRTDraw::RTDrawBitmap(&memDC,&rtTitle,BackBitmap,DrawMode);

	//画标题栏图标
	HICON hIcon = GetIcon(FALSE);
	CRect rtTitleText;
	if(hIcon != NULL)
	{
		DrawIconEx(memDC.m_hDC,rtTitle.left+1,rtTitle.top+1,hIcon,rtTitle.Height() - 2,rtTitle.Height() - 2,0,NULL,DI_NORMAL);
		rtTitleText.SetRect(rtTitle.left + rtTitle.Height(),rtTitle.top,rtTitle.right - 60,rtTitle.bottom);
	}
	else
	{
		rtTitleText.SetRect(rtTitle.left + 1,rtTitle.top,rtTitle.right - 60,rtTitle.bottom);
	}

	//画标题栏文字
	CFont TitleFont;
	NONCLIENTMETRICS nif;
	nif.cbSize = sizeof(nif);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&nif,0);
	TitleFont.CreateFontIndirect(&nif.lfCaptionFont);
	CFont *pOldFont = memDC.SelectObject(&TitleFont);
	CString titleText = GetTitle();
	
	int oldBkMode = memDC.SetBkMode(TRANSPARENT);
	int oldTextColor = memDC.SetTextColor(TitleTextColor);

	TEXTMETRIC Metrics;
	pDC->GetTextMetrics(&Metrics);
	rtTitleText.top = rtTitle.top + (rtTitle.Height() - Metrics.tmHeight)/2 + 1;
	
	memDC.DrawText(titleText,&rtTitleText,DT_LEFT | DT_SINGLELINE);
	memDC.SetBkMode(oldBkMode);
	memDC.SetTextColor(oldTextColor);
	memDC.SelectObject(&pOldFont);

	//画标题栏按扭
	RTDrawTitleBarButtons(&memDC);
	return TRUE;
}

LRESULT CRTFrameWnd::OnRTNcPaint(WPARAM wParam, LPARAM lParam)
{
	//绘制非客户区
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_NCPAINT,wParam,lParam);

	
	CWindowDC dc(this);
	HRGN hrgn = NULL;
	CRect rtWnd;
	GetWindowRect(&rtWnd);
	if(wParam != 1)
	{
		hrgn = (HRGN)wParam;
	}

	if(hrgn != NULL)
	{
		::SelectClipRgn(dc.m_hDC,hrgn);
		::OffsetClipRgn(dc.m_hDC,0 - rtWnd.left,0 - rtWnd.top);
	}
	RTDrawFrameBorder(&dc,hrgn);
	RTDrawTitleBar(&dc,m_bActive,hrgn);
	RTDrawMenuBar(&dc,lParam);
	if(hrgn != NULL)
	{
		::SelectClipRgn(dc.m_hDC,NULL);
		DeleteObject(hrgn);
	}
	return FALSE;
}

BOOL CRTFrameWnd::EnableSkin(BOOL bEnable,BOOL bEnablBackAndBorder,BOOL bEnableTitleBar)
{
	m_bEnableSkin = bEnable;
	m_bEnableBackAndBorder = bEnablBackAndBorder;
	m_bEnableTitleBar = bEnableTitleBar;
	return bEnable;
}

LRESULT CRTFrameWnd::OnRTNcActive(WPARAM wParam, LPARAM lParam)
{
	return CFrameWnd::WindowProc(WM_NCACTIVATE,wParam,lParam);
}

LRESULT CRTFrameWnd::OnRTActive(WPARAM wParam,LPARAM lParam)
{
	LRESULT result = CFrameWnd::WindowProc(WM_ACTIVATE,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;
		
	if(!IsWindow(m_hWnd))return result;
	CWindowDC dc(this);
	if(wParam == WA_INACTIVE)
	{
		m_bActive = FALSE;
	}
	else
	{
		m_bActive = TRUE;
	}

	OnRTNcPaint(1,0);
	return result;
}

LRESULT CRTFrameWnd::OnRTNodify(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CFrameWnd::WindowProc(WM_NOTIFY,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return 0;
}

LRESULT CRTFrameWnd::OnRTActiveApp(WPARAM wParam,LPARAM lParam)
{
	LRESULT result = CFrameWnd::WindowProc(WM_ACTIVATEAPP,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	m_bActive = (BOOL)wParam;
	return 0;
}

LRESULT CRTFrameWnd::OnRTSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CFrameWnd::WindowProc(WM_SETTEXT,wParam,lParam);

	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return result;
}

BOOL CRTFrameWnd::SetRestoreButtonBitmap(CBitmap* RestoreButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_RestoreButtonBitmap[i] = RestoreButtonBitmap[i];
		m_RestoreButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}
BOOL CRTFrameWnd::SetExitButtonBitmap(CBitmap* ExitButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ExitButtonBitmap[i] = ExitButtonBitmap[i];
		m_ExitButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}


BOOL CRTFrameWnd::RTDrawTitleBarButtons(CDC* pDC)
{	
	//取得标题栏的位置
	CRect rtWnd;
	CRect rtTitle;
	GetWindowRect(&rtWnd);
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
	
	CRect rtButtons;

	//画标题栏按扭
	DWORD dwStyle = GetStyle();
	
	CBitmap* pCloseBox = NULL;
	CBitmap* pMaximizeBox = NULL;
	CBitmap* pMinimizeBox = NULL;

	if(TRUE)
	{
		if(m_ExitButtonState == StateNormal)
		{
			pCloseBox = m_ExitButtonBitmap[BMP_NORMAL];
		}
		else if(m_ExitButtonState == StateDown)
		{
			pCloseBox = m_ExitButtonBitmap[BMP_DOWN];
		}
		else if(m_ExitButtonState == StateFocus)
		{
			pCloseBox = m_ExitButtonBitmap[BMP_FOCUS];
		}
	}

	if(WS_MAXIMIZEBOX & dwStyle)
	{
		if(IsZoomed())
		{
			if(m_MaximizeButtonState == StateNormal)
			{
				pMaximizeBox = m_RestoreButtonBitmap[BMP_NORMAL];
			}
			else if(m_MaximizeButtonState == StateDown)
			{
				pMaximizeBox = m_RestoreButtonBitmap[BMP_DOWN];
			}
			else if(m_MaximizeButtonState == StateFocus)
			{
				pMaximizeBox = m_RestoreButtonBitmap[BMP_FOCUS];
			}
		}
		else
		{
			if(m_MaximizeButtonState == StateNormal)
			{
				pMaximizeBox = m_MaximizeButtonBitmap[BMP_NORMAL];
			}
			else if(m_MaximizeButtonState == StateDown)
			{
				pMaximizeBox = m_MaximizeButtonBitmap[BMP_DOWN];
			}
			else if(m_MaximizeButtonState == StateFocus)
			{
				pMaximizeBox = m_MaximizeButtonBitmap[BMP_FOCUS];
			}
		}
	}
	else if(WS_MINIMIZEBOX & dwStyle)
	{
		if(IsZoomed())
		{
			pMaximizeBox = m_RestoreButtonBitmap[BMP_DISABLE];
		}
		else
		{
			pMaximizeBox = m_MaximizeButtonBitmap[BMP_DISABLE];
		}
	}

	if(WS_MINIMIZEBOX & dwStyle)
	{
		if(m_MinimizeButtonState == StateNormal)
		{
			pMinimizeBox = m_MinimizeButtonBitmap[BMP_NORMAL];
		}
		else if(m_MinimizeButtonState == StateDown)
		{
			pMinimizeBox = m_MinimizeButtonBitmap[BMP_DOWN];
		}
		else if(m_MinimizeButtonState == StateFocus)
		{
			pMinimizeBox = m_MinimizeButtonBitmap[BMP_FOCUS];
		}
	}
	else if(WS_MAXIMIZEBOX & dwStyle)
	{
		pMinimizeBox = m_MinimizeButtonBitmap[BMP_DISABLE];
	}

//重画关闭button
	if(dwStyle & WS_SYSMENU)
	{
		rtButtons.left = rtTitle.right - rtTitle.Height();
		rtButtons.top = rtTitle.top + 2;
		rtButtons.right = rtTitle.right - 2;
		rtButtons.bottom = rtTitle.bottom - 2;
		CRTDraw::RTDrawBitmap(pDC,&rtButtons,pCloseBox,CRTDraw::DrawModeAllStretch);
	}
	//重画最大化/恢复button
	if(dwStyle & WS_MAXIMIZEBOX || dwStyle & WS_MINIMIZEBOX)
	{
		rtButtons.right = rtButtons.left - 2;
		rtButtons.left = rtButtons.left - rtTitle.Height();
		CRTDraw::RTDrawBitmap(pDC,&rtButtons,pMaximizeBox,CRTDraw::DrawModeAllStretch);
	}
	//重画最小化button
	if(dwStyle & WS_MAXIMIZEBOX || dwStyle & WS_MINIMIZEBOX)
	{
		rtButtons.right = rtButtons.left;
		rtButtons.left = rtButtons.left - rtTitle.Height() + 2;
		CRTDraw::RTDrawBitmap(pDC,&rtButtons,pMinimizeBox,CRTDraw::DrawModeAllStretch);
	}
	return TRUE;
}


LRESULT CRTFrameWnd::OnRTSetIcon(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CFrameWnd::WindowProc(WM_SETICON,wParam,lParam);

	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	if(IsWindow(m_hWnd))
	{
		CWindowDC dc(this);
		RTDrawTitleBar(&dc,m_bActive);
	}
	return result;
}

LRESULT CRTFrameWnd::OnRTNcMouseMove(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_NCMOUSEMOVE,wParam,lParam);
	if(!m_bEnableTitleBar)return CFrameWnd::WindowProc(WM_NCMOUSEMOVE,wParam,lParam);

	LRESULT result = 0;
	CWindowDC dc(this);
	switch(wParam)
	{
	case HTMAXBUTTON:
		if(m_NcMouseState == MouseStateNormal)
		{
			if(m_MaximizeButtonState != StateFocus)
			{
				m_MaximizeButtonState = StateFocus;
				m_MinimizeButtonState = StateNormal;
				m_ExitButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		else if(m_NcMouseState == MouseStateDown)
		{
			if(m_SelTitleButton == MaximizeButton)
			{
				if(m_MaximizeButtonState != StateDown)
				{
					m_MaximizeButtonState = StateDown;
					RTDrawTitleBarButtons(&dc);
				}
			}
			else if(m_SelTitleButton != NoneButton)
			{
				m_MinimizeButtonState = StateNormal;
				m_ExitButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		break;
	case HTMINBUTTON:
		if(m_NcMouseState == MouseStateNormal)
		{
			if(m_MinimizeButtonState != StateFocus)
			{
				m_MinimizeButtonState = StateFocus;
				m_MaximizeButtonState = StateNormal;
				m_ExitButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		else if(m_NcMouseState == MouseStateDown)
		{
			if(m_SelTitleButton == MinimizeButton)
			{
				if(m_MinimizeButtonState != StateDown)
				{
					m_MinimizeButtonState = StateDown;
					RTDrawTitleBarButtons(&dc);
				}
			}
			else if(m_SelTitleButton != NoneButton)
			{
				m_MaximizeButtonState = StateNormal;
				m_ExitButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		break;
	case HTCLOSE:
		if(m_NcMouseState == MouseStateNormal)
		{
			if(m_ExitButtonState != StateFocus)
			{
				m_ExitButtonState = StateFocus;
				m_MaximizeButtonState = StateNormal;
				m_MinimizeButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		else if(m_NcMouseState == MouseStateDown)
		{
			if(m_SelTitleButton == ExitButton)
			{
				if(m_ExitButtonState != StateDown)
				{
					m_ExitButtonState = StateDown;
					RTDrawTitleBarButtons(&dc);
				}
			}
			else if(m_SelTitleButton != NoneButton)
			{
				m_MaximizeButtonState = StateNormal;
				m_MinimizeButtonState = StateNormal;
				RTDrawTitleBarButtons(&dc);
			}
		}
		break;
	default:
		if(m_ExitButtonState != StateNormal || m_MinimizeButtonState != StateNormal || m_MaximizeButtonState != StateNormal)
		{
			m_MaximizeButtonState = StateNormal;
			m_MinimizeButtonState = StateNormal;
			m_ExitButtonState = StateNormal;
			RTDrawTitleBarButtons(&dc);
		}
		result = 0;//CFrameWnd::WindowProc(WM_NCMOUSEMOVE,wParam,lParam);
		break;
	}
	return result;
}

LRESULT CRTFrameWnd::OnRTNcLMouseDown(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);
	if(!m_bEnableTitleBar)return CFrameWnd::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);

	LRESULT result = 0;
	CWindowDC dc(this);
	switch(wParam)
	{
	case HTMAXBUTTON:
		m_NcMouseState = MouseStateDown;
		m_SelTitleButton = MaximizeButton;
		m_MaximizeButtonState = StateDown;
		RTDrawTitleBarButtons(&dc);
		SetCapture();
		break;
	case HTMINBUTTON:
		m_NcMouseState = MouseStateDown;
		m_SelTitleButton = MinimizeButton;
		m_MinimizeButtonState = StateDown;
		RTDrawTitleBarButtons(&dc);
		SetCapture();
		break;
	case HTCLOSE:
		m_NcMouseState = MouseStateDown;
		m_SelTitleButton = ExitButton;
		m_ExitButtonState = StateDown;
		RTDrawTitleBarButtons(&dc);
		SetCapture();
		break;
	default:
		result = CFrameWnd::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);
		break;
	}

	RTDrawTitleBar(&dc,m_bActive);
	return result;
}

LRESULT CRTFrameWnd::OnRTNcLMouseUp(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_NCLBUTTONUP,wParam,lParam);

	LRESULT result = 0;
	if(m_NcMouseState == MouseStateNormal)
	{
		return CFrameWnd::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
	}
	m_NcMouseState = MouseStateNormal;
	ReleaseCapture();

	if(wParam == HTMAXBUTTON)
	{
		if(m_SelTitleButton == MaximizeButton)
		{
			CWindowDC dc(this);
			m_SelTitleButton = NoneButton;
			m_MaximizeButtonState = StateNormal;
			RTDrawTitleBarButtons(&dc);
			if(IsZoomed())
			{
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, lParam);
			}
			else
			{
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE,lParam);
			}
		}
	}
	else if(wParam == HTMINBUTTON)
	{
		if(m_SelTitleButton == MinimizeButton)
		{
			CWindowDC dc(this);
			m_SelTitleButton = NoneButton;
			m_MinimizeButtonState = StateNormal;
			RTDrawTitleBarButtons(&dc);
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE,lParam);
		}
	}
	else if(wParam == HTCLOSE)
	{
		if(m_SelTitleButton == ExitButton)
		{
			//CWindowDC dc(this);
			m_SelTitleButton = NoneButton;
			m_ExitButtonState = StateNormal;
			//RTDrawTitleBarButtons(&dc);
			SendMessage(WM_SYSCOMMAND,SC_CLOSE,lParam);
		}
	}
	else
	{
		m_MinimizeButtonState = StateNormal;
		m_MaximizeButtonState = StateNormal;
		m_ExitButtonState = StateNormal;
		result = CFrameWnd::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
	}

	
	
	return result;
}

LRESULT CRTFrameWnd::OnRTMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_MOUSEMOVE,wParam,lParam);
	if(!m_bEnableTitleBar)return CFrameWnd::WindowProc(WM_MOUSEMOVE,wParam,lParam);

	if(m_NcMouseState != MouseStateNormal)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		CPoint point(lParam);
		ClientToScreen(&point);
		UINT hit = (UINT)CFrameWnd::WindowProc(WM_NCHITTEST,wParam,MAKELPARAM(point.x,point.y));
		return OnRTNcMouseMove(hit,MAKELPARAM(point.x,point.y));
	}
	else
	{
		if(m_ExitButtonState != StateNormal || m_MinimizeButtonState != StateNormal || m_MaximizeButtonState != StateNormal)
		{
			CWindowDC dc(this);
			m_MaximizeButtonState = StateNormal;
			m_MinimizeButtonState = StateNormal;
			m_ExitButtonState = StateNormal;
			RTDrawTitleBarButtons(&dc);
		}
	}
	return CFrameWnd::WindowProc(WM_MOUSEMOVE,wParam,lParam);
}

LRESULT CRTFrameWnd::OnRTLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_LBUTTONUP,wParam,lParam);
	if(!m_bEnableTitleBar)return CFrameWnd::WindowProc(WM_LBUTTONUP,wParam,lParam);

	if(m_NcMouseState != MouseStateNormal)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		CPoint point(lParam);
		ClientToScreen(&point);
		UINT hit = (UINT)CFrameWnd::WindowProc(WM_NCHITTEST,wParam,MAKELPARAM(point.x,point.y));	
		return OnRTNcLMouseUp(hit,MAKELPARAM(point.x,point.y));
	}
	return CFrameWnd::WindowProc(WM_LBUTTONUP,wParam,lParam);
}

BOOL CRTFrameWnd::LoadFrame (UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
    if ( !CFrameWnd::LoadFrame (nIDResource, dwDefaultStyle, pParentWnd, pContext) )
    {
        return false;
    }
 
    return true;
}

LRESULT CRTFrameWnd::OnRTSysCommand(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CFrameWnd::WindowProc(WM_SYSCOMMAND,wParam,lParam);
	if(!m_bEnableTitleBar)return CFrameWnd::WindowProc(WM_SYSCOMMAND,wParam,lParam);

	if(wParam != SC_CLOSE)
	{
		CWindowDC dc(this);
		RTDrawTitleBar(&dc,m_bActive);
	}
	return CFrameWnd::WindowProc(WM_SYSCOMMAND,wParam,lParam);
}

LRESULT CRTFrameWnd::OnRTSetCursor(WPARAM wParam,LPARAM lParam)
{
	LRESULT rt = CFrameWnd::WindowProc(WM_SETCURSOR,wParam,lParam); 
	if(!m_bEnableSkin)return rt;
	if(!m_bEnableTitleBar)return rt;

	UINT nHitTest = LOWORD(lParam);
	if(m_bEnableSkin)
	{
		CWindowDC dc(this);
		switch(nHitTest)
		{
		case HTBORDER:
		case HTLEFT:
		case HTRIGHT:
		case HTTOP:
		case HTBOTTOM:
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
		case HTTOPLEFT:
		case HTTOPRIGHT:
			RTDrawTitleBar(&dc,m_bActive);
			break;
		default:
			break;
		}
		
	}

	return rt;
}



LRESULT CRTFrameWnd::OnRTNcHitTest(WPARAM wParam, LPARAM lParam)
{
	LRESULT lrst = CFrameWnd::WindowProc(WM_NCHITTEST,wParam,lParam);
	return lrst;
}

LRESULT CRTFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITMENUPOPUP:
		if(m_bEnableSkin && m_bEnableTitleBar)
		{
			CWindowDC dc(this);
			RTDrawTitleBar(&dc,m_bActive);
		}
		break;
	case WM_NCPAINT:
		return OnRTNcPaint(wParam,lParam);
	case WM_NCACTIVATE:
		return OnRTNcActive(wParam,lParam);
	case WM_ACTIVATE:
		return OnRTActive(wParam,lParam);
	case WM_ACTIVATEAPP:
		return OnRTActiveApp(wParam,lParam);
	case WM_NOTIFY:
		return OnRTNodify(wParam,lParam);
	case WM_SETTEXT:
		return OnRTSetText(wParam,lParam);
	case WM_SETICON:
		return OnRTSetIcon(wParam,lParam);
	case WM_NCMOUSEMOVE:
		return OnRTNcMouseMove(wParam,lParam);
	case WM_NCLBUTTONDOWN:
		return OnRTNcLMouseDown(wParam,lParam);
	case WM_NCLBUTTONUP:
		OnRTNcLMouseUp(wParam,lParam);
	case WM_LBUTTONUP:
		return OnRTLButtonUp(wParam,lParam);
	case WM_MOUSEMOVE:
		return OnRTMouseMove(wParam,lParam);
	case WM_SYSCOMMAND:
		return OnRTSysCommand(wParam,lParam);
	case WM_SETCURSOR:
		return OnRTSetCursor(wParam,lParam);
	default:
		break;
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}
