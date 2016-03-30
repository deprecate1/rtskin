#pragma once

class CRTSkin
{
public:
	CRTSkin(void);
	~CRTSkin(void);
	CBitmap* LoadBitmap(LPCTSTR lpRelativeFileName);
	void Clear();
public:
	UINT        StrToUINT(LPCTSTR lpValue);
	CBitmap*    m_TitleBarBitmap[5];
	CBitmap*    m_MenuBarBitmap[5];
	CBitmap*    m_MaximizeButtonBitmap[5];
	CBitmap*    m_MinimizeButtonBitmap[5];
	CBitmap*    m_RestoreButtonBitmap[5];
	CBitmap*    m_ExitButtonBitmap[5];
	CBitmap*    m_ToolBarBitmap[5];
	CBitmap*    m_StatusBarBitmap[5];
	CBitmap*    m_ButtonBitmap[5];
	CBitmap*    m_ListCtrlBitmap[5];
	CBitmap*    m_TreeCtrlBitmap[5];
	CBitmap*    m_SplitterWndBitmap[5];
	CBitmap*    m_HeaderCtrlBitmap[5];
	CBitmap*    m_MenuItemBitmap[5];

    UINT        m_MenuItemBitmapDrawMode[5];
    UINT        m_HeaderCtrlBitmapDrawMode[5];
	UINT        m_TitleBarBitmapDrawMode[5];
	UINT        m_MenuBarBitmapDrawMode[5];
	UINT        m_MaximizeButtonBitmapDrawMode[5];
	UINT        m_MinimizeButtonBitmapDrawMode[5];
	UINT        m_RestoreButtonBitmapDrawMode[5];
	UINT        m_ExitButtonBitmapDrawMode[5];
	UINT        m_ToolBarBitmapDrawMode[5];
	UINT        m_ButtonBitmapDrawMode[5];
	UINT        m_TreeCtrlBitmapDrawMode[5];
	UINT        m_StatusBarBitmapDrawMode[5];
	UINT        m_SplitterWndBitmapDrawMode[5];
	UINT        m_ListCtrlBitmapDrawMode[5];
	COLORREF    m_BorderColor[4];

	CBitmap* m_ScrollBarVerticalBitmap[5];
	CBitmap* m_ScrollBarHorizontalBitmap[5];
	UINT     m_ScrollBarVerticalBitmapDrawMode[5];
	UINT     m_ScrollBarHorizontalBitmapDrawMode[5];

	CBitmap* m_ScrollBarUpArrowBitmap[5];
	CBitmap* m_ScrollBarLeftArrowBitmap[5];
	CBitmap* m_ScrollBarDownArrowBitmap[5];
	CBitmap* m_ScrollBarRightArrowBitmap[5];

	UINT     m_ScrollBarUpArrowBitmapDrawMode[5];
	UINT     m_ScrollBarLeftArrowBitmapDrawMode[5];
	UINT     m_ScrollBarDownArrowBitmapDrawMode[5];
	UINT     m_ScrollBarRightArrowBitmapDrawMode[5];


protected:
	char m_AppPath[MAX_PATH];
	char m_BasePath[MAX_PATH];
public:
	BOOL Load(LPCTSTR lpStyleFileName);
	void GetFullPathName(LPCTSTR lpRelativeFile, char* Buf);
	void UseSkin(BOOL IsEnable);
};
