///////////////////////////////////////////////////////////////////////////////
//
// MenuXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTMenu.h"
#include "RTTools.h"
#include "RTDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef DWORD_PTR
    typedef ULONG DWORD_PTR;
    typedef long LONG_PTR;
#endif

#ifndef ODS_HOTLIGHT
    #define ODS_HOTLIGHT 0x0040
#endif
#ifndef ODS_INACTIVE
    #define ODS_INACTIVE 0x0080
#endif

#ifndef ODS_NOACCEL
    #define ODS_NOACCEL 0x0100
#endif

#ifndef DT_HIDEPREFIX
    #define DT_HIDEPREFIX 0x00100000
#endif

#ifndef SPI_GETKEYBOARDCUES
    #define SPI_GETKEYBOARDCUES 0x100A
#endif

// From <winuser.h>
#ifndef OBM_CHECK
    #define OBM_CHECK 32760
#endif

#define IMGWIDTH 16
#define IMGHEIGHT 16
#define IMGPADDING 6
#define TEXTPADDING 8
#define TEXTPADDING_MNUBR 4
#define SM_CXSHADOW 4

const TCHAR _WndPropName_OldProc[] = _T("XPWndProp_OldProc");
const TCHAR _WndPropName_MenuXP[] = _T("XPWndProp_MenuXP");

///////////////////////////////////////////////////////////////////////////////
// Menu item management class
//
class CRTMenuItem
{
protected:
    MENUITEMINFO m_miInfo;
    CString      m_sCaption;
    CImgDesc     m_ImgDesc;
    HIMAGELIST   m_hImgList;
    int          m_nIndex;

public:
    CRTMenuItem ();
    CRTMenuItem (HMENU hMenu, UINT uItem, bool fByPosition = true);
   ~CRTMenuItem ();

// Properties
public:
    int   GetCaption   (CString& sCaption) const;
    int   GetShortCut  (CString& sShortCut) const;
    bool  GetSeparator () const;
    bool  GetChecked   () const;
    bool  GetRadio     () const;
    bool  GetDisabled  () const;
    bool  GetDefault   () const;
    HMENU GetPopup     () const;
    UINT  GetID        () const;

// Methods
public:
    int  GetCaptionWidth  (CDC* pDC) const;
    int  GetShortCutWidth (CDC* pDC) const;
    int  GetHeight        (CDC* pDC) const;
    bool Draw             (CDC* pDC, LPCRECT pRect, bool bSelected, bool bMenuBar = false, bool bHotLight = false, bool bInactive = false, bool bNoAccel = false) const;
public:
    static BYTE ms_nCheck;
    static CRect ms_rcMRUMenuBarItem;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BYTE CRTMenuItem::ms_nCheck = 0;
CRect CRTMenuItem::ms_rcMRUMenuBarItem (0, 0, 0, 0);

///////////////////////////////////////////////////////////////////////////////
CRTMenuItem::CRTMenuItem ()
{
    memset (&m_miInfo, 0, sizeof(MENUITEMINFO));
}

///////////////////////////////////////////////////////////////////////////////
CRTMenuItem::CRTMenuItem (HMENU hMenu, UINT uItem, bool fByPosition)
{
    memset (&m_miInfo, 0, sizeof(MENUITEMINFO));
    m_miInfo.cbSize = sizeof(MENUITEMINFO);
    m_miInfo.fMask = MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE|MIIM_DATA|MIIM_ID;
    VERIFY (::GetMenuItemInfo (hMenu, uItem, fByPosition, &m_miInfo));

    if ( !(m_miInfo.fType & MFT_SEPARATOR) )
    {
        if ( m_miInfo.hSubMenu != NULL )
        {
            CRTMenu::ms_sSubMenuCaptions.Lookup (m_miInfo.hSubMenu, m_sCaption);
            CRTMenu::ms_SubMenuImages.Lookup (m_miInfo.hSubMenu, m_ImgDesc);
        }
        else
        {
            CRTMenu::ms_sCaptions.Lookup (m_miInfo.wID, m_sCaption);
            CRTMenu::ms_Images.Lookup (m_miInfo.wID, m_ImgDesc);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CRTMenuItem::~CRTMenuItem ()
{
}

///////////////////////////////////////////////////////////////////////////////
int CRTMenuItem::GetCaption (CString& sCaption) const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        return sCaption.GetLength();
    }
    sCaption = sCaption.Left (nShortCutPos);

    return nShortCutPos-1;
}
///////////////////////////////////////////////////////////////////////////////
int CRTMenuItem::GetShortCut (CString& sShortCut) const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    CString sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        sShortCut = "";
        return 0;
    }
    int nLength = sCaption.GetLength()-nShortCutPos-1;

    sShortCut = sCaption.Right (nLength);

    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::GetSeparator () const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    return (m_miInfo.fType & MFT_SEPARATOR) == MFT_SEPARATOR;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::GetChecked () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_CHECKED) == MFS_CHECKED;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::GetRadio () const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    return (m_miInfo.fType & MFT_RADIOCHECK) == MFT_RADIOCHECK;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::GetDisabled () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_GRAYED) == MFS_GRAYED;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::GetDefault () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_DEFAULT) == MFS_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
HMENU CRTMenuItem::GetPopup () const
{
    ASSERT(m_miInfo.fMask & MIIM_SUBMENU);
    return m_miInfo.hSubMenu;
}

///////////////////////////////////////////////////////////////////////////////
UINT CRTMenuItem::GetID () const
{
    ASSERT(m_miInfo.fMask & MIIM_ID);
    return m_miInfo.wID;
}

///////////////////////////////////////////////////////////////////////////////
int CRTMenuItem::GetCaptionWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sCaption;
    int nLength = 0;

    if ( GetCaption (sCaption) > 0 )
    {
        int nPos = sCaption.Find ('&');
        CBoldDC bold (*pDC, GetDefault());

        if ( nPos >= 0 )
        {
            sCaption = sCaption.Left (nPos) + sCaption.Right (sCaption.GetLength()-nPos-1);
        }
        nLength = pDC->GetTextExtent (sCaption).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
int CRTMenuItem::GetShortCutWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sShortCut;
    int nLength = 0;

    if ( GetShortCut (sShortCut) > 0 )
    {
        CBoldDC bold (*pDC, GetDefault());

        nLength = pDC->GetTextExtent (sShortCut).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
int CRTMenuItem::GetHeight (CDC* pDC) const
{
    TEXTMETRIC tm;

    pDC->GetTextMetrics (&tm);
                     
    return GetSeparator() ? tm.tmHeight/2+2 : tm.tmHeight+4;
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenuItem::Draw (CDC* pDC, LPCRECT pRect, bool bSelected,bool bMenuBar, bool bHotLight, bool bInactive, bool bNoAccel) const
{
    COLORREF crBackImg = CLR_NONE;
    bool bMenuBarItemSelected = false;

    if ( bMenuBar && bSelected )
    {
		if(CRTMenu::m_MenuBarBitmap[BMP_DOWN] != NULL && CRTMenu::m_bEnableSkin)
		{
			CRTDraw::RTDrawBitmap(pDC,pRect,CRTMenu::m_MenuBarBitmap[BMP_DOWN],CRTMenu::m_MenuBarBitmapDrawMode[BMP_DOWN]);
		}
		else
		{
			CRect rc (pRect);
			CPenDC pen (*pDC, ::GetSysColor (COLOR_3DDKSHADOW));
			CBrushDC brush (*pDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));

			rc.right -= TEXTPADDING_MNUBR;
			ms_rcMRUMenuBarItem = rc;
			bMenuBarItemSelected = true;
			pDC->Rectangle (rc);
			rc.left = rc.right;
			rc.right += TEXTPADDING_MNUBR;
			pDC->FillSolidRect (rc, ::GetSysColor (COLOR_3DFACE));

			for ( int x = 0; x < SM_CXSHADOW; x++ )
			{
				for ( int y = ( x < 2 ) ? 2-x : 0; y < rc.Height()-x-((x>0)?1:2); y++ )
				{
					int nMakeSpec = 78+(3-(y==0?0:(y==1?(x<2?0:1):(y==2?(x<2?x:2):x))))*5;
					COLORREF cr = pDC->GetPixel (rc.right-x-1, rc.top+y+SM_CXSHADOW);
					COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
									((nMakeSpec * int(GetGValue(cr))) / 100),
									((nMakeSpec * int(GetBValue(cr))) / 100));
					pDC->SetPixel (rc.right-x-1, rc.top+y+SM_CXSHADOW, cr2);
				}
			}
		}
    }
    else if ( bSelected || (bHotLight && !bInactive) )
    {
        COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
        CPenDC pen (*pDC, crHighLight);
        CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) : HLS_TRANSFORM (crHighLight, +70, -57));

        if ( bMenuBar )
        {
			if(CRTMenu::m_MenuBarBitmap[BMP_FOCUS] != NULL && CRTMenu::m_bEnableSkin)
			{
				CRTDraw::RTDrawBitmap(pDC,pRect,CRTMenu::m_MenuBarBitmap[BMP_FOCUS],CRTMenu::m_MenuBarBitmapDrawMode[BMP_FOCUS]);
			}
			else
			{
				CRect rc (pRect);
				rc.right -= TEXTPADDING_MNUBR;
				pDC->Rectangle (rc);
				rc.left = rc.right;
				rc.right += TEXTPADDING_MNUBR;
				pDC->FillSolidRect (rc, ::GetSysColor (COLOR_3DFACE));
			}
        }
        else
        {
			if(CRTMenu::m_MenuItemBitmap[BMP_FOCUS] != NULL && CRTMenu::m_bEnableSkin)
			{
				CRect rc (pRect);
				rc.right = IMGWIDTH+IMGPADDING;
				CRTDraw::RTDrawBitmap(pDC,&rc,CRTMenu::m_MenuItemBitmap[BMP_SLIDER],CRTMenu::m_MenuItemBitmapDrawMode[BMP_SLIDER]);
				rc.left = rc.right;
				rc.right = pRect->right;
				CRTDraw::RTDrawBitmap(pDC,&rc,CRTMenu::m_MenuItemBitmap[BMP_FOCUS],CRTMenu::m_MenuItemBitmapDrawMode[BMP_FOCUS]);
			}
			else
			{
			  pDC->Rectangle (pRect);
			}
        }
    }
    else if ( !bMenuBar )
    {
        CRect rc (pRect);
		rc.right = IMGWIDTH+IMGPADDING;
		if(CRTMenu::m_MenuItemBitmap[BMP_SLIDER] != NULL && CRTMenu::m_bEnableSkin)
		{
			CRTDraw::RTDrawBitmap(pDC,&rc,CRTMenu::m_MenuItemBitmap[BMP_SLIDER],CRTMenu::m_MenuItemBitmapDrawMode[BMP_SLIDER]);
		}
		else
		{
			pDC->FillSolidRect (rc, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));
		}
		rc.left = rc.right;
		rc.right = pRect->right;
		if(CRTMenu::m_MenuItemBitmap[BMP_NORMAL] != NULL && CRTMenu::m_bEnableSkin)
		{
			CRTDraw::RTDrawBitmap(pDC,&rc,CRTMenu::m_MenuItemBitmap[BMP_NORMAL],CRTMenu::m_MenuItemBitmapDrawMode[BMP_NORMAL]);
		}
		else
		{
			pDC->FillSolidRect (rc, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +75, 0));
		}
    }
    else//MenuBar
    {
		if(CRTMenu::m_MenuBarBitmap[BMP_NORMAL] != NULL && CRTMenu::m_bEnableSkin)
		{
			CRTDraw::RTDrawBitmap(pDC,pRect,CRTMenu::m_MenuBarBitmap[BMP_NORMAL],CRTMenu::m_MenuBarBitmapDrawMode[BMP_NORMAL]);
		}
		else
		{
			pDC->FillSolidRect (pRect, ::GetSysColor (COLOR_3DFACE));
		}
    }
    if ( GetSeparator() )
    {
        CPenDC pen (*pDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0));

        pDC->MoveTo (pRect->left+IMGWIDTH+IMGPADDING+TEXTPADDING,  (pRect->top+pRect->bottom)/2);
        pDC->LineTo (pRect->right-1, (pRect->top+pRect->bottom)/2);
    }
    else
    {
        CRect rc (pRect);
        CString sCaption;

        if ( GetCaption (sCaption) > 0 )
        {
            pDC->SetTextColor (bInactive ? ::GetSysColor (COLOR_3DSHADOW) : (GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0) : ::GetSysColor (COLOR_MENUTEXT)));
            pDC->SetBkMode (TRANSPARENT);

            BOOL bKeyboardCues = true;
            ::SystemParametersInfo (SPI_GETKEYBOARDCUES, 0, &bKeyboardCues, 0);
            DWORD dwHidePrefix = ( bNoAccel && !bKeyboardCues ) ? DT_HIDEPREFIX : 0;

            if ( bMenuBar )
            {
                rc.right -= TEXTPADDING_MNUBR;
                pDC->DrawText (sCaption, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|dwHidePrefix);
            }
            else
            {
                CBoldDC bold (*pDC, GetDefault());

                rc.left = IMGWIDTH+IMGPADDING+TEXTPADDING;
                pDC->DrawText (sCaption, rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|dwHidePrefix);

                CString sShortCut;

                if ( GetShortCut (sShortCut) > 0 )
                {
                    rc.right -= TEXTPADDING+4;
                    pDC->DrawText (sShortCut, rc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
                }
                if ( GetChecked() )
                {
                    COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
                    CPenDC pen (*pDC, crHighLight);
                    CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) :
                                                                      (bSelected ? HLS_TRANSFORM (crHighLight, +50, -50) : HLS_TRANSFORM (crHighLight, +70, -57)));

                    pDC->Rectangle (CRect (pRect->left+1, pRect->top+1, pRect->left+IMGWIDTH+4, pRect->bottom-1));
                }
                if ( m_ImgDesc.m_hImgList != NULL && m_ImgDesc.m_nIndex != -1 )
		{
#if 0
                    bool bOver = !GetDisabled() && bSelected;

                    if ( GetDisabled() || (bSelected && !GetChecked()) )
                    {
                        HICON hIcon = ImageList_ExtractIcon (NULL, m_ImgDesc.m_hImgList, m_ImgDesc.m_nIndex);
                        pDC->DrawState (CPoint (pRect->left + ( bOver ? 4 : 3 ), rc.top + ( bOver ? 4 : 3 )),
                                        CSize (IMGWIDTH, IMGHEIGHT), hIcon, DSS_MONO,
                                        CBrush (bOver ? HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
                        DestroyIcon (hIcon);
                    }
                    if ( !GetDisabled() )
                    {
                        ::ImageList_Draw (m_ImgDesc.m_hImgList, m_ImgDesc.m_nIndex, pDC->m_hDC,
                                          pRect->left+( (bSelected && !GetChecked()) ? 2 : 3 ), rc.top+( (bSelected && !GetChecked()) ? 2 : 3 ), ILD_TRANSPARENT);
		    }
#endif
                }
                else if ( GetChecked() )
                {
                    // Draw the check mark
                    rc.left  = pRect->left+IMGWIDTH/4+1;
                    rc.right = rc.left + IMGWIDTH-1;

                    if ( GetRadio() )
                    {
                        CPoint ptCenter = rc.CenterPoint();
                        COLORREF crBullet = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0) : ::GetSysColor (COLOR_MENUTEXT);
                        CPenDC pen (*pDC, crBullet);
                        CBrushDC brush (*pDC, crBullet);
                        
                        pDC->Ellipse (CRect (ptCenter.x-4, ptCenter.y-3, ptCenter.x+3, ptCenter.y+4));
                        pDC->SetPixel (ptCenter.x+1, ptCenter.y+2, crBackImg);
                    }
                    else
                    {
                        pDC->SetBkColor (crBackImg);
                        HBITMAP hBmp = LoadBitmap (NULL, MAKEINTRESOURCE(OBM_CHECK));
                        pDC->DrawState (CPoint (rc.left,rc.top+3), CSize(rc.Size()), hBmp, DSS_NORMAL, (HBRUSH)NULL);
                        DeleteObject (hBmp);
                    }
                }
            }
        }
    }
    return bMenuBarItemSelected;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CMap <int, int, CString, CString&> CRTMenu::ms_sCaptions;
CMap <HMENU, HMENU, CString, CString&> CRTMenu::ms_sSubMenuCaptions;
CMap <int, int, CImgDesc, CImgDesc&> CRTMenu::ms_Images;
CMap <HMENU, HMENU, CImgDesc, CImgDesc&> CRTMenu::ms_SubMenuImages;
CBitmap* CRTMenu::m_MenuBarBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTMenu::m_MenuBarBitmapDrawMode[5] = {0,0,0,0,0};
CBitmap* CRTMenu::m_MenuItemBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTMenu::m_MenuItemBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTMenu::m_bEnableSkin = FALSE;

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::EnableSkin(BOOL IsEnable)
{
	m_bEnableSkin = IsEnable;
}

void CRTMenu::SetXPLookNFeel (CWnd* pWnd, BOOL bXPLook)
{
    if ( bXPLook )
    {
        ::SetProp (pWnd->GetSafeHwnd(), _WndPropName_MenuXP, (HANDLE)TRUE);
    }
    else
    {
        ::RemoveProp (pWnd->GetSafeHwnd(), _WndPropName_MenuXP);
    }
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenu::GetXPLookNFeel (const CWnd* pWnd)
{
    return ::GetProp (pWnd->GetSafeHwnd(), _WndPropName_MenuXP) != NULL;
}

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::UpdateMenuBar (CWnd* pWnd)
{
    if ( GetXPLookNFeel (pWnd) )
    {
        HMENU hMenu = pWnd->GetMenu()->GetSafeHmenu();

        if ( hMenu != NULL )
        {
            SetXPLookNFeel (pWnd, hMenu, true, true);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::SetXPLookNFeel (CWnd* pWnd, HMENU hMenu, BOOL bXPLook, BOOL bMenuBar)
{
    if ( !bXPLook )
    {
        // TODO: Remove the ownerdraw style ?
        return;
    }
//    TRACE(_T("Referenced captions : %i\n"), ms_sCaptions.GetCount()+ms_sSubMenuCaptions.GetCount());
    // Clean up old references...
    // ... for captions
    POSITION pos = ms_sSubMenuCaptions.GetStartPosition();

    while ( pos != NULL )
    {
        HMENU hSubMenu;
        CString sBuff;

        ms_sSubMenuCaptions.GetNextAssoc (pos, hSubMenu, sBuff);

        if ( !::IsMenu (hSubMenu) )
        {
            ms_sSubMenuCaptions.RemoveKey (hSubMenu);
        }
    }
    // ... for images
    pos = ms_SubMenuImages.GetStartPosition();

    while ( pos != NULL )
    {
        HMENU hSubMenu;
        CImgDesc ImgDesc;

        ms_SubMenuImages.GetNextAssoc (pos, hSubMenu, ImgDesc);

        if ( !::IsMenu (hSubMenu) )
        {
            ms_SubMenuImages.RemoveKey (hSubMenu);
        }
    }
    ASSERT(hMenu != NULL);
    int nItemCount = ::GetMenuItemCount (hMenu);
    MENUITEMINFO mii = { sizeof MENUITEMINFO, MIIM_ID|MIIM_TYPE|MIIM_SUBMENU };
    CClientDC cDC (AfxGetMainWnd());
    TEXTMETRIC tm;
    CFontDC font (cDC, DEFAULT_GUI_FONT);

    cDC.GetTextMetrics (&tm);

    int nHeight = max(tm.tmHeight+2,IMGHEIGHT)+4;
    int nSepHeight = tm.tmHeight/2+2;
    int nCaptionLength = 0;
    int nShortCutLength = 0;
    CPtrList* pListControlBars = NULL;

    if ( pWnd != NULL && !bMenuBar )
    {
        if ( pWnd->IsKindOf (RUNTIME_CLASS(CMDIFrameWnd)) )
        {
            CMDIChildWnd* pActiveChild = ((CMDIFrameWnd*)pWnd)->MDIGetActive();

            if ( pActiveChild != NULL && pActiveChild->GetSystemMenu (false)->GetSafeHmenu() == hMenu )
            {
                CRTMenuItem::ms_rcMRUMenuBarItem.SetRectEmpty();
                return;
            }
        }
        if ( pWnd->IsKindOf (RUNTIME_CLASS(CFrameWnd)) && !((CFrameWnd*)pWnd)->m_listControlBars.IsEmpty() )
        {
            pListControlBars = &((CFrameWnd*)pWnd)->m_listControlBars;
        }
        else
        {
            CFrameWnd* pFrame = pWnd->GetParentFrame();

            if ( pFrame != NULL && pFrame->IsKindOf (RUNTIME_CLASS(CMDIChildWnd)) )
            {
                pFrame = pFrame->GetParentFrame();
            }
            if ( pFrame != NULL )
            {
                pListControlBars = &pFrame->m_listControlBars;
            }
        }
    }
    for ( int i = 0; i < nItemCount; i++ )
    {
        TCHAR sCaption[256] = _T("");
        mii.dwTypeData = sCaption;
        mii.cch = 255;
        mii.fMask &= ~MIIM_DATA;
        ::GetMenuItemInfo (hMenu, i, true, &mii);

        if ( (mii.fType & MFT_OWNERDRAW) == 0 && (!bMenuBar || (mii.fType & MFT_BITMAP) == 0) )
        {
            mii.fType |= MFT_OWNERDRAW;

            if ( bMenuBar )
            {
                CRect rcText (0, 0, 1000, 0);

                cDC.DrawText (sCaption, (int)_tcslen (sCaption), rcText, DT_SINGLELINE|DT_LEFT|DT_CALCRECT);
                mii.dwItemData = MAKELONG(MAKEWORD(0, 0), rcText.Width());
                mii.fMask |= MIIM_DATA;
            }
            ::SetMenuItemInfo (hMenu, i, true, &mii);

            if ( (mii.fType & MFT_SEPARATOR) == 0 )
            {
                CString sBuff(sCaption);

                if ( mii.hSubMenu != NULL )
                {
                    ms_sSubMenuCaptions.SetAt (mii.hSubMenu, sBuff);
                }
                else
                {
                    ms_sCaptions.SetAt (mii.wID, sBuff);
                }
                if ( pListControlBars != NULL )
                {
                    POSITION pos = pListControlBars->GetHeadPosition();

                    while ( pos != NULL )
                    {
                        CControlBar* pBar = (CControlBar*)pListControlBars->GetNext (pos);
                        ASSERT(pBar != NULL);
                        TCHAR sClassName[256];

                        ::GetClassName (pBar->m_hWnd, sClassName, lengthof (sClassName));

                        if ( !_tcsicmp (sClassName, _T("ToolbarWindow32")) )
                        {
                            TBBUTTONINFO tbbi = { sizeof(TBBUTTONINFO), TBIF_COMMAND|TBIF_IMAGE };

                            if ( pBar->SendMessage (TB_GETBUTTONINFO, mii.wID, (LPARAM)&tbbi) != -1 &&
                                 (UINT)tbbi.idCommand == mii.wID && tbbi.iImage != -1 )
                            {
                                CImgDesc imgDesc ((HIMAGELIST)pBar->SendMessage (TB_GETIMAGELIST, 0, 0), tbbi.iImage);

                                if ( mii.hSubMenu != NULL )
                                {
                                    ms_SubMenuImages.SetAt (mii.hSubMenu, imgDesc);
                                }
                                else
                                {
                                    ms_Images.SetAt (mii.wID, imgDesc);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        if ( !bMenuBar )
        {
            CRTMenuItem mnuItem (hMenu, i);
            int       nWidth = mnuItem.GetCaptionWidth (&cDC);

            if ( nWidth > nCaptionLength )
            {
                nCaptionLength = nWidth;
            }
            nWidth = mnuItem.GetShortCutWidth (&cDC);

            if ( nWidth > nShortCutLength )
            {
                nShortCutLength = nWidth;
            }
        }
    }
    if ( !bMenuBar )
    {
        for ( int j = 0; j < nItemCount; j++ )
        {
            mii.fMask = MIIM_TYPE;
            ::GetMenuItemInfo (hMenu, j, true, &mii);

            if ( (mii.fType & MFT_SEPARATOR) == 0 )
            {
                mii.dwItemData = MAKELONG(MAKEWORD(nHeight, nShortCutLength), nCaptionLength);
            }
            else
            {
                mii.dwItemData = nSepHeight;
            }
            mii.fMask = MIIM_DATA;
            ::SetMenuItemInfo (hMenu, j, true, &mii);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenu::IsOwnerDrawn (HMENU hMenu)
{
    MENUITEMINFO mii = { sizeof MENUITEMINFO, MIIM_TYPE };

    ::GetMenuItemInfo (hMenu, 0, true, &mii);

    return (mii.fType & MFT_OWNERDRAW) != 0;
}

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::SetMRUMenuBarItem (RECT& rc)
{
    CRTMenuItem::ms_rcMRUMenuBarItem = rc;
}

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::SetMenuItemImage (UINT nID, HIMAGELIST hImgList, int nIndex)
{
    CImgDesc imgDesc (hImgList, nIndex);
    ms_Images.SetAt (nID, imgDesc);
}

void CRTMenu::SetMenuBarBitmap(CBitmap* MenuBarBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MenuBarBitmap[i] = MenuBarBitmap[i];
		m_MenuBarBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTMenu::SetMenuItemBitmap(CBitmap* MenuItemBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_MenuItemBitmap[i] = MenuItemBitmap[i];
		m_MenuItemBitmapDrawMode[i] = DrawMode[i];
	}
}
///////////////////////////////////////////////////////////////////////////////
void CRTMenu::OnMeasureItem (MEASUREITEMSTRUCT* pMeasureItemStruct)
{
    if ( pMeasureItemStruct->CtlType == ODT_MENU )
    {
        pMeasureItemStruct->itemHeight = LOBYTE(LOWORD(pMeasureItemStruct->itemData));

        if ( pMeasureItemStruct->itemHeight == 0 )
        {
            // This is a menubar item
            pMeasureItemStruct->itemWidth = HIWORD(pMeasureItemStruct->itemData) + TEXTPADDING_MNUBR;
        }
        else
        {
            pMeasureItemStruct->itemWidth = IMGWIDTH + IMGPADDING + HIWORD(pMeasureItemStruct->itemData) + TEXTPADDING + HIBYTE(LOWORD(pMeasureItemStruct->itemData)) + TEXTPADDING + 4;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
bool CRTMenu::OnDrawItem (DRAWITEMSTRUCT* pDrawItemStruct, HWND hWnd)
{
    if ( pDrawItemStruct->CtlType != ODT_MENU )
    {
        return false;
    }
    ASSERT (pDrawItemStruct->CtlType == ODT_MENU);
    CBufferDC cDC (pDrawItemStruct->hDC, pDrawItemStruct->rcItem);
    CRTMenuItem item ((HMENU)pDrawItemStruct->hwndItem, pDrawItemStruct->itemID, false);
    CFontDC font (cDC, DEFAULT_GUI_FONT);

	if ( item.Draw (&cDC, &pDrawItemStruct->rcItem, (pDrawItemStruct->itemState&ODS_SELECTED)!=0,LOBYTE(LOWORD(pDrawItemStruct->itemData))==0, (pDrawItemStruct->itemState&ODS_HOTLIGHT)!=0, (pDrawItemStruct->itemState&ODS_INACTIVE)!=0, (pDrawItemStruct->itemState&ODS_NOACCEL)!=0) )
    {
        CRect rc;

        ::GetMenuItemRect (hWnd, (HMENU)pDrawItemStruct->hwndItem, 0, rc);
        ::ClientToScreen (hWnd, CRTMenuItem::ms_rcMRUMenuBarItem);
        CRTMenuItem::ms_rcMRUMenuBarItem.top = rc.top;
        CRTMenuItem::ms_rcMRUMenuBarItem.bottom = rc.bottom;
        
        if ( CWnd::FromHandle (hWnd)->IsKindOf (RUNTIME_CLASS(CDialog)) )
        {
            CRTMenuItem::ms_rcMRUMenuBarItem.OffsetRect (1, 0);
        }
    }
    CRTMenuItem::ms_nCheck++;
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CRTMenu::OnMenuChar (HMENU hMenu, UINT nChar, UINT nFlags)
{
    if ( (nFlags & (MF_POPUP|MF_SYSMENU)) == MF_POPUP || nFlags == 0 )
    {
        int nItemCount = ::GetMenuItemCount (hMenu);

        nChar = toupper (nChar);

        for ( int i = 0; i < nItemCount; i++ )
        {
            CRTMenuItem mnuItem (hMenu, i);
            CString sCaption;

            mnuItem.GetCaption (sCaption);
            sCaption.MakeUpper();

            for ( int nPos = sCaption.GetLength()-2; nPos >= 0; nPos-- )
            {
                if ( sCaption[nPos] == '&' && (UINT)toupper (sCaption[nPos+1]) == nChar &&
                     (nPos == 0 || sCaption[nPos-1] != '&') )
                {
                    return MAKELRESULT(i,2);
                }
            }
        }
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CRTWndMenu class : management of the window used by system to display popup menus
//
class CRTWndMenu
{
public:
    CRTWndMenu (HWND hWnd);
   ~CRTWndMenu ();

public:
    static CRTWndMenu* FromHandle (HWND hWnd, bool bPermanent = true);

protected:
    void OnWindowPosChanging (WINDOWPOS* pWP);
    void OnEraseBkgnd ();
    void OnPrint (CDC* pDC, bool bOwnerDrawnItems);
    void OnNcPaint ();
    void OnShowWindow (bool bShow);
    void OnNcDestroy ();

private:
    static LRESULT CALLBACK WindowsHook (int code, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK SubClassMenuProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;
    CRect m_rcMenu;
    CPoint m_ptMenu;
    CBitmap m_bmpBkGnd;

    static CMap <HWND, HWND, CRTWndMenu*, CRTWndMenu*> ms_WndMenuMap;
    static DWORD ms_dwRefCount;
    static HHOOK ms_hHookOldMenuCbtFilter;

friend class CRTMenu;
};

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::InitializeHook ()
{
    CRTWndMenu::ms_dwRefCount++;

    if ( CRTWndMenu::ms_hHookOldMenuCbtFilter == NULL )
    {
        CRTWndMenu::ms_hHookOldMenuCbtFilter = ::SetWindowsHookEx (WH_CALLWNDPROC, CRTWndMenu::WindowsHook, AfxGetApp()->m_hInstance, ::GetCurrentThreadId());
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTMenu::UninitializeHook ()
{
    if ( CRTWndMenu::ms_dwRefCount == 0 )
    {
        return;
    }
    if ( --CRTWndMenu::ms_dwRefCount == 0 )
    {
        POSITION pos = CRTWndMenu::ms_WndMenuMap.GetStartPosition();

        while ( pos != NULL )
        {
            HWND hKey;
            CRTWndMenu* pVal;

            CRTWndMenu::ms_WndMenuMap.GetNextAssoc (pos, hKey, pVal);
            delete pVal;
        }
        CRTWndMenu::ms_WndMenuMap.RemoveAll();

        if ( CRTWndMenu::ms_hHookOldMenuCbtFilter != NULL )
        {
            ::UnhookWindowsHookEx (CRTWndMenu::ms_hHookOldMenuCbtFilter);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CMap <HWND, HWND, CRTWndMenu*, CRTWndMenu*> CRTWndMenu::ms_WndMenuMap;
DWORD CRTWndMenu::ms_dwRefCount = 0;
HHOOK CRTWndMenu::ms_hHookOldMenuCbtFilter = NULL;

///////////////////////////////////////////////////////////////////////////////
CRTWndMenu::CRTWndMenu (HWND hWnd)
    : m_hWnd (hWnd), m_rcMenu (0, 0, 0, 0), m_ptMenu (-0xFFFF, -0xFFFF)
{
}

///////////////////////////////////////////////////////////////////////////////
CRTWndMenu::~CRTWndMenu ()
{
    WNDPROC oldWndProc = (WNDPROC)::GetProp (m_hWnd, _WndPropName_OldProc);

    if ( oldWndProc != NULL )
    {
        ::SetWindowLong (m_hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
        ::RemoveProp (m_hWnd, _WndPropName_OldProc);
    }
    ms_WndMenuMap.RemoveKey (m_hWnd);
}

///////////////////////////////////////////////////////////////////////////////
CRTWndMenu* CRTWndMenu::FromHandle (HWND hWnd, bool bPermanent)
{
    CRTWndMenu* pWnd = NULL;

    if ( ms_WndMenuMap.Lookup (hWnd, pWnd) )
    {
        return pWnd;
    }
    if ( bPermanent )
    {
        return NULL;
    }
    pWnd = new CRTWndMenu (hWnd);
    ms_WndMenuMap.SetAt (hWnd, pWnd);

    return pWnd;
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnWindowPosChanging (WINDOWPOS* pWP)
{
    if ( GetWinVersion() < wvWinXP )
    {
        pWP->cx += SM_CXSHADOW;
        pWP->cy += SM_CXSHADOW;
    }
    pWP->y--;
    m_ptMenu.x = pWP->x;
    m_ptMenu.y = pWP->y;
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnEraseBkgnd ()
{
    if ( !IsWindowVisible (m_hWnd) )
    {
        CClientRect rc (m_hWnd);

        if ( m_bmpBkGnd.m_hObject != NULL )
        {
            m_bmpBkGnd.DeleteObject();
        }
        m_bmpBkGnd.Attach (GetScreenBitmap (CRect (m_ptMenu.x, m_ptMenu.y, rc.right+m_ptMenu.x+10,
                                                   rc.bottom+m_ptMenu.y+10)));
    }
}

///////////////////////////////////////////////////////////////////////////////
void DrawShadow (HDC hDCIn, HDC hDCOut, RECT& rc)
{
    for ( int x = 0; x < rc.right-1; x++ )
    {
        int nEnd = ( x > rc.right-SM_CXSHADOW*2 ) ? rc.right-SM_CXSHADOW-x : SM_CXSHADOW;

        for ( int y = ( x < 2 ) ? 2-x : x > rc.right-SM_CXSHADOW-3 ? x-rc.right+SM_CXSHADOW+3 : 0; y < nEnd; y++ )
        {
            int nMakeSpec = 78+(3-(x==0?0:(x==1?(y<2?0:1):(x==2?(y<2?y:2):y))))*5;
            COLORREF cr = GetPixel (hDCIn, x+SM_CXSHADOW, rc.bottom-y-1);
            COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
			                   ((nMakeSpec * int(GetGValue(cr))) / 100),
			                   ((nMakeSpec * int(GetBValue(cr))) / 100));
			SetPixel (hDCOut, x+SM_CXSHADOW, rc.bottom-y-1, cr2);
        }
    }
    for ( int x = 0; x < SM_CXSHADOW; x++ )
    {
        for ( int y = ( x < 2 ) ? 2-x : 0; y < rc.bottom-x-SM_CXSHADOW-((x>0)?1:2); y++ )
        {
            int nMakeSpec = 78+(3-(y==0?0:(y==1?(x<2?0:1):(y==2?(x<2?x:2):x))))*5;
            COLORREF cr = GetPixel (hDCIn, rc.right-x-1, y+SM_CXSHADOW);
            COLORREF cr2 = RGB(((nMakeSpec * int(GetRValue(cr))) / 100),
			                   ((nMakeSpec * int(GetGValue(cr))) / 100),
			                   ((nMakeSpec * int(GetBValue(cr))) / 100));
			SetPixel (hDCOut, rc.right-x-1, y+SM_CXSHADOW, cr2);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnPrint (CDC* pDC, bool bOwnerDrawnItems)
{
    CWindowRect rc (m_hWnd);
    CBrushDC br (pDC->m_hDC);
    CPenDC pen (pDC->m_hDC, ::GetSysColor (COLOR_3DDKSHADOW));

    rc.OffsetRect (-rc.TopLeft());
    m_rcMenu = rc;

    if ( GetWinVersion() < wvWinXP )
    {
        rc.right -= SM_CXSHADOW;
        rc.bottom -= SM_CXSHADOW;
    }
    pDC->Rectangle (rc);
    pen.Color (HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +75, 0));
    rc.DeflateRect (1, 1);
    pDC->Rectangle (rc);
    rc.DeflateRect (1, 1);
    pDC->Rectangle (rc);

    if ( bOwnerDrawnItems && !CRTMenuItem::ms_rcMRUMenuBarItem.IsRectEmpty() &&
         CRTMenuItem::ms_rcMRUMenuBarItem.bottom == m_ptMenu.y+1 )
    {
        pen.Color (HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));
        pDC->MoveTo (CRTMenuItem::ms_rcMRUMenuBarItem.left-m_ptMenu.x-3, 0);
        pDC->LineTo (CRTMenuItem::ms_rcMRUMenuBarItem.left-m_ptMenu.x+CRTMenuItem::ms_rcMRUMenuBarItem.Width()-5, 0);
    }
    if ( GetWinVersion() < wvWinXP )
    {
        rc.right += SM_CXSHADOW+2;
        rc.bottom += SM_CXSHADOW+2;

        CDC cMemDC;
        cMemDC.CreateCompatibleDC (pDC);
        HGDIOBJ hOldBitmap = ::SelectObject (cMemDC.m_hDC, m_bmpBkGnd);
        pDC->BitBlt (0, rc.bottom-SM_CXSHADOW, SM_CXSHADOW*2, SM_CXSHADOW, &cMemDC, 0, rc.bottom-SM_CXSHADOW, SRCCOPY);
        pDC->BitBlt (rc.right-SM_CXSHADOW, rc.bottom-SM_CXSHADOW, SM_CXSHADOW, SM_CXSHADOW, &cMemDC, rc.right-SM_CXSHADOW, rc.bottom-SM_CXSHADOW, SRCCOPY);
        pDC->BitBlt (rc.right-SM_CXSHADOW, 0, SM_CXSHADOW, SM_CXSHADOW*2, &cMemDC, rc.right-SM_CXSHADOW, 0, SRCCOPY);
        DrawShadow (cMemDC.m_hDC, pDC->m_hDC, rc);
        ::SelectObject (cMemDC.m_hDC, hOldBitmap);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnNcPaint ()
{
    CWindowDC cDC (CWnd::FromHandle (m_hWnd));
    CDC* pDC = &cDC;
    CWindowRect rc (m_hWnd);

    m_ptMenu.x = rc.left;
    m_ptMenu.y = rc.top;
    rc.OffsetRect (-rc.TopLeft());

    if ( rc != m_rcMenu )
    {
        m_rcMenu = rc;

        CBrushDC br (pDC->m_hDC);
        CPenDC pen (pDC->m_hDC, ::GetSysColor (COLOR_3DDKSHADOW));

        if ( GetWinVersion() < wvWinXP )
        {
            rc.right -= SM_CXSHADOW;
            rc.bottom -= SM_CXSHADOW;
        }

        pDC->Rectangle (rc);
        pen.Color (HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +75, 0));
        rc.DeflateRect (1, 1);
		pDC->Rectangle (rc);
        rc.DeflateRect (1, 1);
		pDC->Rectangle (rc);
		
        if ( !CRTMenuItem::ms_rcMRUMenuBarItem.IsRectEmpty() &&
             CRTMenuItem::ms_rcMRUMenuBarItem.bottom == m_ptMenu.y+1 )
        {
            pen.Color (HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));
            pDC->MoveTo (CRTMenuItem::ms_rcMRUMenuBarItem.left-m_ptMenu.x-3, 0);
            pDC->LineTo (CRTMenuItem::ms_rcMRUMenuBarItem.left-m_ptMenu.x+CRTMenuItem::ms_rcMRUMenuBarItem.Width()-5, 0);
        }
        if ( GetWinVersion() < wvWinXP )
        {
            rc.right += SM_CXSHADOW+2;
            rc.bottom += SM_CXSHADOW+2;

            DrawShadow (pDC->m_hDC, pDC->m_hDC, rc);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnShowWindow (bool bShow)
{
    if ( !bShow )
    {
        delete this;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CRTWndMenu::OnNcDestroy ()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CRTWndMenu::WindowsHook (int code, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;

    // Not a real loop (just for 'break' branchment)
    while ( code == HC_ACTION )
    {
        HWND hWnd = pStruct->hwnd;

        // Normal and special handling for menu 0x10012
        if ( pStruct->message != WM_CREATE && pStruct->message != 0x01E2 )
        {
            break;
        }
        CWnd* pFrame = CWnd::GetForegroundWindow();

		if ( pFrame == NULL || !CRTMenu::GetXPLookNFeel (pFrame) || !CRTMenu::m_bEnableSkin )
        {
            break;
        }
        TCHAR sClassName[10];
        int Count = ::GetClassName (hWnd, sClassName, lengthof(sClassName));

        // Check for the menu-class
        if ( Count != 6 || _tcscmp (sClassName, _T("#32768")) != 0 )
        {
            break;
        }
        VERIFY(CRTWndMenu::FromHandle (pStruct->hwnd, false) != NULL);

        if ( ::GetProp (pStruct->hwnd, _WndPropName_OldProc) != NULL )
        {
            // Already subclassed
            break;
        }
        // Subclass the window
        WNDPROC oldWndProc = (WNDPROC)(LONG_PTR)::GetWindowLong (pStruct->hwnd, GWL_WNDPROC);

        if ( oldWndProc == NULL )
        {
            break;
        }
        ASSERT(oldWndProc != SubClassMenuProc);

        if ( !SetProp (pStruct->hwnd, _WndPropName_OldProc, oldWndProc) )
        {
            break;
        }
        if ( !SetWindowLong (pStruct->hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)SubClassMenuProc) )
        {
            ::RemoveProp (pStruct->hwnd, _WndPropName_OldProc);
            break;
        }
        // Success !
        break;
    }
    return CallNextHookEx (CRTWndMenu::ms_hHookOldMenuCbtFilter, code, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CRTWndMenu::SubClassMenuProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC oldWndProc = (WNDPROC)::GetProp (hWnd, _WndPropName_OldProc);
    CRTWndMenu* pWnd = NULL;

    switch ( uMsg )
    {
    case WM_NCCALCSIZE:
        {
            LRESULT lResult = CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);

            if ( GetWinVersion() < wvWinXP )
            {
                NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;

                lpncsp->rgrc[0].right -= SM_CXSHADOW;
                lpncsp->rgrc[0].bottom -= SM_CXSHADOW;
            }
            return lResult;
        }

    case WM_WINDOWPOSCHANGING:
        if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
        {
            pWnd->OnWindowPosChanging ((LPWINDOWPOS)lParam);
        }
        break;

    case WM_ERASEBKGND:
        if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
        {
            pWnd->OnEraseBkgnd();
        }
        break;

    case WM_PRINT:
        {
            BYTE nCheck = CRTMenuItem::ms_nCheck;
            LRESULT lResult = CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
            bool bOwnerDrawnItems = nCheck != CRTMenuItem::ms_nCheck;

            if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
            {
                pWnd->OnPrint (CDC::FromHandle ((HDC)wParam), bOwnerDrawnItems);
            }
            return lResult;
        }

    case WM_NCPAINT:
        if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
        {
            pWnd->OnNcPaint();
            return 0;
        }
        break;

    case WM_SHOWWINDOW:
        if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
        {
            pWnd->OnShowWindow (wParam != 0);
        }
        break;

    case WM_NCDESTROY:
        if ( (pWnd=CRTWndMenu::FromHandle (hWnd)) != NULL )
        {
            pWnd->OnNcDestroy();
        }
        break;
    }
    return CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
}
