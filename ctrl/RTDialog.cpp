// RTDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "RTDialog.h"
#include "RTDraw.h"
#include "RTMenu.h"
// CRTDialog 对话框

IMPLEMENT_DYNAMIC(CRTDialog, CDialog)
IMPLEMENT_MENUXP(CRTDialog,CDialog)
CRTDialog::CRTDialog()
	: CDialog()
{
	m_bActive = FALSE;

	m_ExitButtonState = StateNormal;
	m_MinimizeButtonState = StateNormal;
	m_MaximizeButtonState = StateNormal;
	m_NcMouseState = MouseStateNormal;
	m_SelTitleButton = NoneButton;
}

CRTDialog::CRTDialog(UINT uTemplate,CWnd *pParent)
: CDialog(uTemplate,pParent)
{
	m_bActive = FALSE;

	m_ExitButtonState = StateNormal;
	m_MinimizeButtonState = StateNormal;
	m_MaximizeButtonState = StateNormal;
	m_NcMouseState = MouseStateNormal;
	m_SelTitleButton = NoneButton;
}
CRTDialog::CRTDialog(LPCTSTR lpszTemplateName,CWnd* pParent)
	: CDialog(lpszTemplateName,pParent)
{
	m_bActive = FALSE;

	m_ExitButtonState = StateNormal;
	m_MinimizeButtonState = StateNormal;
	m_MaximizeButtonState = StateNormal;
	m_NcMouseState = MouseStateNormal;
	m_SelTitleButton = NoneButton;
}
CRTDialog::~CRTDialog()
{
}

COLORREF CRTDialog::m_BorderColor[4] = {0xAD9E9C,0xCEBEB5,0xCEBEB5,0xDED3CE};
BOOL     CRTDialog::m_bEnableSkin = FALSE;
BOOL     CRTDialog::m_bEnableBack = FALSE;
BOOL     CRTDialog::m_bEnableBorder = FALSE;
BOOL     CRTDialog::m_bEnableTitleBar = FALSE;
CBitmap* CRTDialog::m_TitleBarBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTDialog::m_MenuBarBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTDialog::m_MaximizeButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTDialog::m_MinimizeButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTDialog::m_RestoreButtonBitmap[5] ={ NULL,NULL,NULL,NULL,NULL};
CBitmap* CRTDialog::m_ExitButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};


UINT     CRTDialog::m_TitleBarBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTDialog::m_MenuBarBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTDialog::m_MaximizeButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTDialog::m_MinimizeButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTDialog::m_ExitButtonBitmapDrawMode[5] = {0,0,0,0,0};
UINT     CRTDialog::m_RestoreButtonBitmapDrawMode[5] = {0,0,0,0,0};

void CRTDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRTDialog, CDialog)
END_MESSAGE_MAP()


// CRTDialog 消息处理程序
BOOL CRTDialog::RTDrawFrameBorder(CDC *pDC,HRGN hrgn)
{
	//画框架
	CRect rtWnd;
	GetWindowRect(&rtWnd);

	int Frame_BorderWidth = GetSystemMetrics(SM_CXDLGFRAME);
	int Frame_BorderHeight =GetSystemMetrics(SM_CYDLGFRAME);

	if(!m_bEnableBorder)
	{
		HRGN rgn = NULL;

		//left border
		rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.left + Frame_BorderWidth,rtWnd.bottom);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);
		
		//top border
		rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.top + Frame_BorderHeight);
		if(hrgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		//right border
		rgn = CreateRectRgn(rtWnd.right - Frame_BorderWidth,rtWnd.top,rtWnd.right,rtWnd.bottom);
		if(hrgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		//bottom
		rgn = CreateRectRgn(rtWnd.left,rtWnd.bottom - Frame_BorderHeight,rtWnd.right,rtWnd.bottom);
		if(hrgn != NULL && NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
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


BOOL CRTDialog::RTDrawMenuBar(CDC *pDC,LPARAM lParam,HRGN hrgn)
{
	CMenu *pMenu = GetMenu();
	if(pMenu == NULL)return TRUE;

	CRect rtMenu;
	CRect rtWnd;
	DWORD style = GetStyle();
	int BorderWidth = 0;
	int BorderHeight = 0;
	int CaptionHeight = 0;

	BorderWidth = GetSystemMetrics(SM_CXDLGFRAME);
	BorderHeight =GetSystemMetrics(SM_CYDLGFRAME);

	//no support toolswindow
	if((style & WS_CAPTION) == WS_CAPTION)
	{
		CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	}
	
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
				CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}
		DeleteObject(rgn);

		int MenuTop = rtMenuItem.top - rtWnd.top;
		MenuItemHeight = rtMenuItem.Height();
		if(MenuTop != PrevTop)
		{
			if(!CRTMenu::m_bEnableSkin)
			{
				rgn = CreateRectRgn(rtWnd.left + PrevLeft,rtWnd.top + PrevTop,rtWnd.right - BorderWidth,rtWnd.top + PrevTop + MenuItemHeight);
				if(hrgn != NULL)
				{
					if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
					{
						CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
					}
				}
				else
				{
					CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
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
		HRGN rgn = CreateRectRgn(rtWnd.left + PrevLeft,rtWnd.top + PrevTop,rtWnd.right - BorderWidth,rtWnd.top + PrevTop + MenuItemHeight);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
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

BOOL CRTDialog::SetMinimizeButtonBitmap(CBitmap *MinimizeButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MinimizeButtonBitmap[i] = MinimizeButtonBitmap[i];
		m_MinimizeButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTDialog::SetTitleBarBitmap(CBitmap *TitleBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_TitleBarBitmap[i] = TitleBarBitmap[i];
		m_TitleBarBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTDialog::SetMenuBarBitmap(CBitmap* MenuBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MenuBarBitmap[i] = MenuBarBitmap[i];
		m_MenuBarBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTDialog::SetMaximizeButtonBitmap(CBitmap* MaximizeButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MaximizeButtonBitmap[i] =  MaximizeButtonBitmap[i];
		m_MinimizeButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTDialog::SetBorderColor(COLORREF ColorArray[])
{
	for(int i = 0; i < 4 ; i++)
		m_BorderColor[i] = ColorArray[i];
	return TRUE;
}

BOOL CRTDialog::RTDrawTitleBar(CDC* pDC,BOOL bActive,HRGN hrgn)
{
	//取得标题栏的位置
	DWORD style = GetStyle();
	if((style & WS_CAPTION) != WS_CAPTION)return TRUE;

	CRect rtWnd;
	GetWindowRect(&rtWnd);

	if(!m_bEnableTitleBar)
	{
		rtWnd.left = rtWnd.left + GetSystemMetrics(SM_CXDLGFRAME);
		rtWnd.top = rtWnd.top + GetSystemMetrics(SM_CYDLGFRAME);
		rtWnd.right = rtWnd.right - GetSystemMetrics(SM_CYDLGFRAME);
		rtWnd.bottom = rtWnd.top + GetSystemMetrics(SM_CYCAPTION);

		HRGN rgn = CreateRectRgn(rtWnd.left,rtWnd.top,rtWnd.right,rtWnd.bottom);
		if(hrgn != NULL)
		{
			if(NULLREGION != CombineRgn(rgn,rgn,hrgn,RGN_AND))
			{
				CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
			}
		}
		else
		{
			CDialog::WindowProc(WM_NCPAINT,(WPARAM)rgn,0);
		}

		DeleteObject(rgn);
		return TRUE;
	}

	CRect rtTitle;
	rtTitle.left = GetSystemMetrics(SM_CXDLGFRAME);
	rtTitle.top =  GetSystemMetrics(SM_CYDLGFRAME);
	rtTitle.right = rtWnd.Width() - GetSystemMetrics(SM_CYDLGFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYCAPTION);
	
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
	CString titleText;
	GetWindowText(titleText);
	
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

LRESULT CRTDialog::OnRTNcPaint(WPARAM wParam, LPARAM lParam)
{
	//绘制非客户区
	if(m_bEnableSkin == FALSE)
	{
		return CDialog::WindowProc(WM_NCPAINT,wParam,lParam);
	}

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

BOOL CRTDialog::EnableSkin(BOOL bEnable,BOOL bEnableBorder,BOOL bEnableBack,BOOL bEnableTitleBar)
{
	m_bEnableSkin = bEnable;
	m_bEnableBack = bEnableBack;
	m_bEnableTitleBar = bEnableTitleBar;
	m_bEnableBorder = bEnableBorder;
	return bEnable;
}

LRESULT CRTDialog::OnRTNcActive(WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCACTIVATE,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCACTIVATE,wParam,lParam);

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return TRUE;
}

LRESULT CRTDialog::OnRTActive(WPARAM wParam,LPARAM lParam)
{
	LRESULT result = CDialog::WindowProc(WM_ACTIVATE,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

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

LRESULT CRTDialog::OnRTNodify(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CDialog::WindowProc(WM_NOTIFY,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return result;
}

LRESULT CRTDialog::OnRTActiveApp(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_ACTIVATEAPP,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_ACTIVATEAPP,wParam,lParam);

	LRESULT result = CDialog::WindowProc(WM_ACTIVATEAPP,wParam,lParam);
	m_bActive = (BOOL)wParam;
	return 0;
}

LRESULT CRTDialog::OnRTSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CDialog::WindowProc(WM_SETTEXT,wParam,lParam);

	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return result;
}

BOOL CRTDialog::SetRestoreButtonBitmap(CBitmap* RestoreButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_RestoreButtonBitmap[i] = RestoreButtonBitmap[i];
		m_RestoreButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}
BOOL CRTDialog::SetExitButtonBitmap(CBitmap* ExitButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ExitButtonBitmap[i] = ExitButtonBitmap[i];
		m_ExitButtonBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}


BOOL CRTDialog::RTDrawTitleBarButtons(CDC* pDC)
{	
	//取得标题栏的位置
	CRect rtWnd;
	CRect rtTitle;
	GetWindowRect(&rtWnd);
	rtTitle.left = GetSystemMetrics(SM_CXDLGFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYDLGFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXDLGFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYCAPTION);
	
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

LRESULT CRTDialog::OnRTNcRButtonUp(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCRBUTTONUP,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCRBUTTONUP,wParam,lParam);

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	return CDialog::WindowProc(WM_NCRBUTTONUP,wParam,lParam);
}

LRESULT CRTDialog::OnRTSetIcon(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CDialog::WindowProc(WM_SETICON,wParam,lParam);
	if(!m_bEnableSkin)return result;
	if(!m_bEnableTitleBar)return result;

	if(IsWindow(m_hWnd))
	{
		CWindowDC dc(this);
		RTDrawTitleBar(&dc,m_bActive);
	}
	return result;
}

LRESULT CRTDialog::OnRTNcMouseMove(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCMOUSEMOVE,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCMOUSEMOVE,wParam,lParam);

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
		result = 0;//DefWindowProc(WM_NCMOUSEMOVE,wParam,lParam);
		break;
	}
	return result;
}

LRESULT CRTDialog::OnRTNcLMouseDown(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);

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
		result = CDialog::WindowProc(WM_NCLBUTTONDOWN,wParam,lParam);
		break;
	}

	RTDrawTitleBar(&dc,m_bActive);
	return result;
}

LRESULT CRTDialog::OnRTNcLMouseUp(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCLBUTTONUP,wParam,lParam);

	LRESULT result = 0;
	if(m_NcMouseState == MouseStateNormal)
	{
		return CDialog::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
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
			CWindowDC dc(this);
			m_SelTitleButton = NoneButton;
			m_ExitButtonState = StateNormal;
			RTDrawTitleBarButtons(&dc);
			SendMessage(WM_CLOSE);
		}
	}
	else
	{
		m_MinimizeButtonState = StateNormal;
		m_MaximizeButtonState = StateNormal;
		m_ExitButtonState = StateNormal;
		result = CDialog::WindowProc(WM_NCLBUTTONUP,wParam,lParam);
	}
	return result;
}

LRESULT CRTDialog::OnRTMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_MOUSEMOVE,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_MOUSEMOVE,wParam,lParam);

	if(m_NcMouseState != MouseStateNormal)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		CPoint point(lParam);
		ClientToScreen(&point);
		UINT hit = (UINT)CDialog::WindowProc(WM_NCHITTEST,wParam,MAKELPARAM(point.x,point.y));
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
	return CDialog::WindowProc(WM_MOUSEMOVE,wParam,lParam);
}

LRESULT CRTDialog::OnRTLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_LBUTTONUP,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_LBUTTONUP,wParam,lParam);

	if(m_NcMouseState != MouseStateNormal)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		CPoint point(lParam);
		ClientToScreen(&point);
		UINT hit = (UINT)CDialog::WindowProc(WM_NCHITTEST,wParam,MAKELPARAM(point.x,point.y));	
		return OnRTNcLMouseUp(hit,MAKELPARAM(point.x,point.y));
	}
	return CDialog::WindowProc(WM_LBUTTONUP,wParam,lParam);
}

LRESULT CRTDialog::OnRTEraseBackGround(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin || m_TitleBarBitmap[BMP_BACK] == NULL || !m_bEnableBack)
		return CDialog::WindowProc(WM_ERASEBKGND,wParam,lParam);

	CDC *pDC = CDC::FromHandle((HDC)wParam);
	CRect rtClient;
	GetClientRect(&rtClient);
	CRTDraw::RTDrawBitmap(pDC,&rtClient,m_TitleBarBitmap[BMP_BACK],m_TitleBarBitmapDrawMode[BMP_BACK]);
	return TRUE;
}

LRESULT CRTDialog::OnRTNcRButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_NCRBUTTONDOWN,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_NCRBUTTONDOWN,wParam,lParam);

	return 0;
}

LRESULT CRTDialog::OnRTSysCommand(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEnableSkin)return CDialog::WindowProc(WM_SYSCOMMAND,wParam,lParam);
	if(!m_bEnableTitleBar)return CDialog::WindowProc(WM_SYSCOMMAND,wParam,lParam);

	CWindowDC dc(this);
	RTDrawTitleBar(&dc,m_bActive);
	
	return CDialog::WindowProc(WM_SYSCOMMAND,wParam,lParam);
}

LRESULT CRTDialog::OnRTSetCursor(WPARAM wParam,LPARAM lParam)
{
	LRESULT rt = CDialog::WindowProc(WM_SETCURSOR,wParam,lParam); 
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

LRESULT CRTDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
	case WM_ERASEBKGND:
		return OnRTEraseBackGround(wParam,lParam);
	case WM_NCRBUTTONUP:
		return OnRTNcRButtonUp(wParam,lParam);
	case WM_NCRBUTTONDOWN:
		return OnRTNcRButtonDown(wParam,lParam);
	case WM_SYSCOMMAND:
		return OnRTSysCommand(wParam,lParam);
	case WM_SETCURSOR:
		return OnRTSetCursor(wParam,lParam);
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
