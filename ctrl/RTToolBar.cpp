// RTToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "RTToolBar.h"
#include "RTTools.h"
#include "RTMenu.h"
#include "RTDraw.h"

// CRTToolBar

IMPLEMENT_DYNAMIC(CRTToolBar, CToolBar)
CRTToolBar::CRTToolBar()
{
}

CRTToolBar::~CRTToolBar()
{
}

BOOL CRTToolBar::EnableSkin(BOOL IsEnable)
{
	BOOL temp = m_IsEnableSkin;
	m_IsEnableSkin = IsEnable;
	return temp;
}

BEGIN_MESSAGE_MAP(CRTToolBar, CToolBar)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CBitmap* CRTToolBar::m_ToolBarBitamp[4] = {NULL,NULL,NULL,NULL};
UINT     CRTToolBar::m_ToolBarBitmapDrawMode[4] = {0,0,0,0};
BOOL     CRTToolBar::m_IsEnableSkin = TRUE;

// CRTToolBar 消息处理程序
BOOL CRTToolBar::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)return CToolBar::OnEraseBkgnd(pDC);

	if(m_ToolBarBitamp[0] != NULL)
	{
		CRect rtWnd;
		GetWindowRect(&rtWnd);
		rtWnd.SetRect(0,0,rtWnd.Width(),rtWnd.Height());
		CRTDraw::RTDrawBitmap(pDC,&rtWnd,m_ToolBarBitamp[0],m_ToolBarBitmapDrawMode[0]);
		return TRUE;
	}
	return CToolBar::OnEraseBkgnd(pDC);
}

BOOL CRTToolBar::SetBitmap(CBitmap* ToolBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 4; i ++)
	{
		m_ToolBarBitamp[i] = ToolBarBitmap[i];
		m_ToolBarBitmapDrawMode[i] = DrawMode[i];
	}
	return TRUE;
}

BOOL CRTToolBar::LoadTrueColorToolBar(int  nBtnWidth, UINT uToolBarNormal, UINT uToolBarDisable)
{
	if (uToolBarNormal <= 0 || uToolBarDisable <= 0)
		return FALSE;

	if (!SetTrueColorToolBar(TB_SETIMAGELIST, uToolBarNormal, nBtnWidth))
		return FALSE;

#if 1
	if (!SetTrueColorToolBar(TB_SETHOTIMAGELIST, uToolBarNormal, nBtnWidth))
		return FALSE;
#endif

	if (!SetTrueColorToolBar(TB_SETDISABLEDIMAGELIST, uToolBarDisable, nBtnWidth))
		return FALSE;

	return TRUE;
}

BOOL CRTToolBar::SetTrueColorToolBar(UINT uToolBarType, UINT uToolBar, int nBtnWidth)
{
	CImageList	cImageList;
	CBitmap		cBitmap;
	BITMAP		bmBitmap;

	if (!cBitmap.Attach(LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(uToolBar), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION))
		|| !cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	CSize		cSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	int		nNbBtn		= cSize.cx/nBtnWidth;
	RGBTRIPLE	*rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF	rgbMask	= RGB(rgb[0].rgbtRed, rgb[0].rgbtGreen, rgb[0].rgbtBlue);

	if (!cImageList.Create(nBtnWidth, cSize.cy, ILC_COLOR24 | ILC_MASK, nNbBtn, 0))
		return FALSE;

	if (-1 == cImageList.Add(&cBitmap, rgbMask))
		return FALSE;

	SendMessage(uToolBarType, 0, (LPARAM)cImageList.m_hImageList);
	cImageList.Detach(); 
	cBitmap.Detach();

	return TRUE;
}

BOOL CRTToolBar::LoadToolBar (UINT nIDResource)
{
    if ( !CToolBar::LoadToolBar (nIDResource) )
    {
        return false;
    }
	if(!m_IsEnableSkin)return TRUE;

    SendMessage (TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
    return true;
}

BOOL CRTToolBar::InsertControl (int nIndex, CWnd& Ctrl)
{
    TBBUTTON tbbutton;
    int nCount = (int)DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    CWindowRect rcCtrl (Ctrl);

    tbbutton.iBitmap = rcCtrl.Width();
    tbbutton.idCommand = Ctrl.GetDlgCtrlID();
    tbbutton.fsState = 0;
    tbbutton.fsStyle = TBSTYLE_SEP;
    tbbutton.bReserved[0] = 0;
    tbbutton.bReserved[1] = 0;
    tbbutton.iString = -1;
    tbbutton.dwData = NULL;

    if ( nIndex < 0 || nIndex > nCount )
    {
        nIndex = nCount;
    }
    if ( GetToolBarCtrl().InsertButton (nIndex, &tbbutton) )
    {
        m_bDelayedButtonLayout = true;

        return true;
    }
    return false;
}


LRESULT CRTToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CToolBar::WindowProc(message, wParam, lParam);
	if(!m_IsEnableSkin)return lRes;

    if ( m_bCheckVisibility && message == TB_GETBUTTON )
    {
        TBBUTTON* pButton = (TBBUTTON*)lParam;

        if ( IS_CONTROL(*pButton) )
        {
            if ( m_bCheckVisibility == 1 )
				pButton->fsState |=  TBSTATE_HIDDEN;
            else                           
				pButton->fsState &= ~TBSTATE_HIDDEN;
        }
    }
    return lRes;
}

void CRTToolBar::OnPaint()
{
	if(!m_IsEnableSkin)return CToolBar::OnPaint();
	if ( m_bDelayedButtonLayout )
    {
        Layout();
    }
    CPaintDC cpDC (this);

	cpDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont =  cpDC.SelectObject (CFont::FromHandle ((HFONT)GetStockObject (DEFAULT_GUI_FONT)));
	CRect rcClip;
	cpDC.GetClipBox(&rcClip);
    CPoint ptCursor;
    ::GetCursorPos (&ptCursor);
    ScreenToClient (&ptCursor);

    CClientRect rcClient (this);
    HIMAGELIST m_hImageList = (HIMAGELIST)DefWindowProc (TB_GETIMAGELIST, 0, 0);
    TBBUTTON tbbutton;
    int nCount = (int)DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    int nHotItem = GetToolBarCtrl().GetHotItem();

    for ( int i = 0; i < nCount; i++ )
    {
        VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

        if ( !IS_VISIBLE(tbbutton) )
        {
            continue;
        }
        CRect rcButton;
        VERIFY(DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcButton));

        if ( !CRect().IntersectRect (rcClip, rcButton) )
        {
            continue;
        }
        bool bOver = nHotItem == i && IS_ENABLED(tbbutton);
        bool bPressed = false;

        if ( IS_INDETERMINATE(tbbutton) )
        {
			CPenDC pen(cpDC.m_hDC, ::GetSysColor (COLOR_3DDKSHADOW));

            cpDC.MoveTo (rcButton.left, rcButton.bottom);
            cpDC.LineTo (rcButton.left, rcButton.top);
            cpDC.LineTo (rcButton.right-1, rcButton.top);
            cpDC.LineTo (rcButton.right-1, rcButton.bottom-1);
            cpDC.LineTo (rcButton.left, rcButton.bottom-1);
            bOver = true;
        }
		//focus
        else if ( bOver || IS_CHECKED(tbbutton) )
        {
            bPressed = KEYDOWN(VK_LBUTTON) && rcButton.PtInRect (ptCursor);

            if ( IS_DROPDOWN(tbbutton) && bPressed )
            {
                bPressed = ptCursor.x < rcButton.right-13;

                if ( bPressed )
                {
                    rcButton.right -= 13;
                }
            }

			
			COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
			CPenDC pen (cpDC, crHighLight);
			CBrushDC brush (cpDC, bPressed||(bOver&&IS_CHECKED(tbbutton)) ? HLS_TRANSFORM (crHighLight, +50, -50) : (bOver ? HLS_TRANSFORM (crHighLight, +70, -57) : HLS_TRANSFORM (crHighLight, +80, -66)));

			if(bPressed && m_ToolBarBitamp[BMP_DOWN]!= NULL)
			{
				CRTDraw::RTDrawBitmap(&cpDC,&rcButton,m_ToolBarBitamp[BMP_DOWN],m_ToolBarBitmapDrawMode[BMP_DOWN]);
			}
			else if(!bPressed && m_ToolBarBitamp[BMP_FOCUS] != NULL)
			{
				CRTDraw::RTDrawBitmap(&cpDC,&rcButton,m_ToolBarBitamp[BMP_FOCUS],m_ToolBarBitmapDrawMode[BMP_FOCUS]);
			}
			else
			{
				cpDC.Rectangle (&rcButton);
			}

			if ( IS_DROPDOWN(tbbutton) )
			{
				if ( bPressed )
				{
					int nLeft = rcButton.left;
					rcButton.left = rcButton.right-1;
					rcButton.right += 13;

					if(m_ToolBarBitamp[BMP_DOWN] != NULL)
					{
						CRTDraw::RTDrawBitmap(&cpDC,rcButton,m_ToolBarBitamp[2],m_ToolBarBitmapDrawMode[BMP_DOWN]);
					}
					else
					{
						cpDC.Rectangle (&rcButton);
						brush.Color (HLS_TRANSFORM (crHighLight, +70, -66));
						cpDC.Rectangle (&rcButton);
					}
					rcButton.left = nLeft;
				}
				else
				{
					if(m_ToolBarBitamp[BMP_FOCUS] != NULL)
					{
						CRTDraw::RTDrawBitmap(&cpDC,rcButton,m_ToolBarBitamp[BMP_FOCUS],m_ToolBarBitmapDrawMode[BMP_FOCUS]);
					}
					else
					{
						cpDC.MoveTo (rcButton.right-14, rcButton.top);
						cpDC.LineTo (rcButton.right-14, rcButton.bottom);
					}
				}
			}
		
        }
		//分隔条
        if ( IS_SEPARATOR(tbbutton) )
        {
            CPenDC pen (cpDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -15, 0));

            if ( IS_WRAP(tbbutton) )
            {
                cpDC.MoveTo (rcClient.left+2, rcButton.bottom-4);
                cpDC.LineTo (rcClient.right-2, rcButton.bottom-4);
            }
            else
            {
                cpDC.MoveTo ((rcButton.right+rcButton.left)/2-1, rcButton.top+2);
                cpDC.LineTo ((rcButton.right+rcButton.left)/2-1, rcButton.bottom-2);
            }
        }
        else if ( !IS_CONTROL(tbbutton) )
        {
            if ( IS_DROPDOWN(tbbutton) )
            {
				CPenDC pen (cpDC.m_hDC, ( bOver && !IS_INDETERMINATE(tbbutton) ) ? RGB(0,0,0) : ::GetSysColor (IS_ENABLED(tbbutton) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

                cpDC.MoveTo (rcButton.right-9, (rcButton.top+rcButton.bottom)/2-1);
                cpDC.LineTo (rcButton.right-4, (rcButton.top+rcButton.bottom)/2-1);
                cpDC.MoveTo (rcButton.right-8, (rcButton.top+rcButton.bottom)/2);
                cpDC.LineTo (rcButton.right-5, (rcButton.top+rcButton.bottom)/2);
                cpDC.SetPixel (rcButton.right-7, (rcButton.top+rcButton.bottom)/2+1, pen.Color());
                rcButton.right -= 14;
            }
            if ( tbbutton.iBitmap >= 0 )
            {
                if ( !IS_ENABLED(tbbutton) || (bOver && !bPressed) )
                {
                    HICON hIcon = ImageList_ExtractIcon (NULL, m_hImageList, tbbutton.iBitmap);
                    cpDC.DrawState (CPoint (rcButton.left + ( bOver ? 4 : 3 ), rcButton.top + ( bOver ? 4 : 3 )), m_sizeImage, hIcon, DSS_MONO, CBrush (bOver ? (IS_INDETERMINATE(tbbutton) ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -20, 0) : HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66)) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
                    DestroyIcon (hIcon);
                }
                if ( IS_ENABLED(tbbutton) )
                {
					
                    ::ImageList_Draw (m_hImageList, tbbutton.iBitmap, cpDC.m_hDC,
                                      rcButton.left + ( (bOver && !bPressed) ? 2 : 3 ), rcButton.top + ( (bOver && !bPressed) ? 2 : 3 ), ILD_TRANSPARENT);
                }
            }
        }
    }
}
CSize CRTToolBar::CalcDynamicLayout (int nLength, DWORD dwMode)
{
	if(!m_IsEnableSkin)return CToolBar::CalcDynamicLayout(nLength,dwMode);

    bool bHideControls = (dwMode & LM_VERTDOCK) == LM_VERTDOCK;
    m_bCheckVisibility = (BYTE)(bHideControls ? 1 : 2);
    CSize size = CToolBar::CalcDynamicLayout (nLength, dwMode);
    m_bCheckVisibility = false;

    if ( dwMode & LM_COMMIT )
    {
        TBBUTTON tbbutton;
        int nCount = (int)DefWindowProc (TB_BUTTONCOUNT, 0, 0);

        for ( int i = 0; i < nCount; i++ )
        {
            VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

            if ( !IS_CONTROL(tbbutton) )
            {
                continue;
            }
            CWnd* pWnd = GetDlgItem (tbbutton.idCommand);

            if ( pWnd == NULL )
            {
                continue;
            }
            if ( bHideControls )
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, true);
                pWnd->ShowWindow (SW_HIDE);
            }
            else
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, false);

                // Update control position
                CRect rcControl;

                VERIFY (DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcControl));
                rcControl.DeflateRect (1, 1);
                
                CWindowRect rcCtrl (pWnd);
                int         nNoSize = 0;

                if ( rcControl.Width() == rcCtrl.Width() )
                {
                    nNoSize = SWP_NOSIZE;
                }
                pWnd->SetWindowPos (NULL, rcControl.left, rcControl.top, rcControl.Width(), rcControl.Height(),
                                    nNoSize|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
            }
        }
    }
    return size;
}

void CRTToolBar::OnButtonDropDown (NMHDR* lpnmhdr, LRESULT* plResult)
{
	if(!m_IsEnableSkin)return;
    LPNMTOOLBAR lpnmtb = (LPNMTOOLBAR)lpnmhdr;
    *plResult = TBDDRET_DEFAULT;

    if ( !m_pDockSite->SendMessage (WM_COMMAND, DROPDOWN(lpnmtb->iItem)) )
    {
        *plResult = TBDDRET_TREATPRESSED;
    }
}

void CRTToolBar::TrackPopupMenu (int nID, CMenu* pMenu)
{
    CRect rcTBItem;
    int nTBIndex = CommandToIndex (nID);
    TPMPARAMS tpmp = { sizeof TPMPARAMS };
    
    GetItemRect (nTBIndex, rcTBItem);
    ClientToScreen (rcTBItem);
    CopyRect (&tpmp.rcExclude, rcTBItem);
    rcTBItem.OffsetRect (4, 0);
    CRTMenu::SetMRUMenuBarItem (rcTBItem);
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)|TBBS_INDETERMINATE);
    UpdateWindow();
    ::TrackPopupMenuEx (pMenu->GetSafeHmenu(), TPM_LEFTBUTTON, tpmp.rcExclude.left, tpmp.rcExclude.bottom, GetParentFrame()->GetSafeHwnd(), &tpmp);
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)&~TBBS_INDETERMINATE);
}
