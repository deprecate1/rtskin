///////////////////////////////////////////////////////////////////////////////
//
// MenuXP.h : header file
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////
// Menu item image management class
class CImgDesc
{
public:
    HIMAGELIST m_hImgList;
    int        m_nIndex;

    CImgDesc (HIMAGELIST hImgList = NULL, int nIndex = 0)
        : m_hImgList (hImgList), m_nIndex (nIndex)
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
class CRTMenu
{
// Operations
public:
    static void InitializeHook ();
    static void UninitializeHook ();

    static void SetXPLookNFeel (CWnd* pWnd, BOOL bXPLook = TRUE);
    static bool GetXPLookNFeel (const CWnd* pWnd);
    static void UpdateMenuBar (CWnd* pWnd);
    static void SetXPLookNFeel (CWnd* pWnd, HMENU hMenu, BOOL bXPLook = TRUE, BOOL bMenuBar = FALSE);
    static bool IsOwnerDrawn (HMENU hMenu);
    static void SetMRUMenuBarItem (RECT& rc);
    static void SetMenuItemImage (UINT nID, HIMAGELIST hImgList, int nIndex);
    static void OnMeasureItem (MEASUREITEMSTRUCT* pMeasureItemStruct);
    static bool OnDrawItem (DRAWITEMSTRUCT* pDrawItemStruct, HWND hWnd);
    static LRESULT OnMenuChar (HMENU hMenu, UINT nChar, UINT nFlags);
	static void SetMenuBarBitmap(CBitmap* MenuBarBitamp[],UINT DrawMode[]);
	static void SetMenuItemBitmap(CBitmap* MenuItemBitmap[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);

// Attributes
protected:
    static CMap <int, int, CString, CString&> ms_sCaptions;
    static CMap <HMENU, HMENU, CString, CString&> ms_sSubMenuCaptions;
    static CMap <int, int, CImgDesc, CImgDesc&> ms_Images;
    static CMap <HMENU, HMENU, CImgDesc, CImgDesc&> ms_SubMenuImages;

public:
	static CBitmap* m_MenuBarBitmap[5];
	static UINT     m_MenuBarBitmapDrawMode[5];
	static CBitmap* m_MenuItemBitmap[5];
	static UINT     m_MenuItemBitmapDrawMode[5];	
	static BOOL m_bEnableSkin;

friend class CRTMenuItem;
friend class CRTWndMenu;
};

///////////////////////////////////////////////////////////////////////////////
#define DECLARE_MENUXP()                                                             \
    protected:                                                                       \
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);     \
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct); \
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);          \
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);

///////////////////////////////////////////////////////////////////////////////
#define ON_MENUXP_MESSAGES() \
	ON_WM_INITMENUPOPUP()    \
	ON_WM_MEASUREITEM()      \
	ON_WM_DRAWITEM()         \
	ON_WM_MENUCHAR()

///////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_MENUXP(theClass, baseClass)                                      \
    IMPLEMENT_MENUXP_(theClass, baseClass, CRTMenu::GetXPLookNFeel (this))

///////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_MENUXP_(theClass, baseClass, bFlag)                              \
    void theClass::OnInitMenuPopup (CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) \
    {                                                                              \
	    baseClass::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);                 \
        CRTMenu::SetXPLookNFeel (this, pPopupMenu->m_hMenu,                        \
                                 bFlag/* && !bSysMenu*/);                          \
    }                                                                              \
    void theClass::OnMeasureItem (int, LPMEASUREITEMSTRUCT lpMeasureItemStruct)    \
    {                                                                              \
        CRTMenu::OnMeasureItem (lpMeasureItemStruct);                              \
    }                                                                              \
    void theClass::OnDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)      \
    {                                                                              \
        if ( !CRTMenu::OnDrawItem (lpDrawItemStruct, m_hWnd) )                     \
        {                                                                          \
            baseClass::OnDrawItem (nIDCtl, lpDrawItemStruct);                      \
        }                                                                          \
    }                                                                              \
    LRESULT theClass::OnMenuChar (UINT nChar, UINT nFlags, CMenu* pMenu)           \
    {                                                                              \
        if ( CRTMenu::IsOwnerDrawn (pMenu->m_hMenu) )                              \
        {                                                                          \
            return CRTMenu::OnMenuChar (pMenu->m_hMenu, nChar, nFlags);            \
        }                                                                          \
	    return baseClass::OnMenuChar (nChar, nFlags, pMenu);                       \
    }

///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
