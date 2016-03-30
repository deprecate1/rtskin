#pragma once


// CRTDialog 对话框
#include "RTMenu.h"

class CRTDialog : public CDialog
{
	DECLARE_DYNAMIC(CRTDialog)

public:
	CRTDialog();   // 标准构造函数
	CRTDialog(UINT uTemplate,CWnd* pParent = NULL);   // 标准构造函数
	CRTDialog(LPCTSTR lpszTemplateName,CWnd *pParent = NULL);   // 标准构造函数
	virtual ~CRTDialog();

public:
	typedef enum _tagDrawState
	{
		StateNormal = 0,
		StateDown = 1,
		StateFocus = 2,
		StateDisable = 3
	}DrawState;

	typedef enum _tagMouseState
	{
		MouseStateNormal = 0,
		MouseStateDown =1,
		MouseStateUp = 2
	}MouseState;

	typedef enum _tagTitleButtons
	{
		ExitButton = 0,
		MaximizeButton = 1,
		MinimizeButton = 2,
		NoneButton = -1
	}TitleButtons;


protected:  
	static COLORREF    m_BorderColor[4];
	static CBitmap*    m_TitleBarBitmap[5];
	static CBitmap*    m_MenuBarBitmap[5];
	static CBitmap*    m_MaximizeButtonBitmap[5];
	static CBitmap*    m_MinimizeButtonBitmap[5];
	static CBitmap*    m_RestoreButtonBitmap[5];
	static CBitmap*    m_ExitButtonBitmap[5];

	static UINT        m_TitleBarBitmapDrawMode[5];
	static UINT        m_MenuBarBitmapDrawMode[5];
	static UINT        m_MaximizeButtonBitmapDrawMode[5];
	static UINT        m_MinimizeButtonBitmapDrawMode[5];
	static UINT        m_RestoreButtonBitmapDrawMode[5];
	static UINT        m_ExitButtonBitmapDrawMode[5];
	static BOOL        m_bEnableSkin;
	static BOOL        m_bEnableBorder;
	static BOOL        m_bEnableBack;
	static BOOL        m_bEnableTitleBar;

	BOOL        m_bActive;
	int         m_ExitButtonState;
	int         m_MaximizeButtonState;
	int         m_MinimizeButtonState;
	MouseState  m_NcMouseState;
	TitleButtons m_SelTitleButton;
public:
	static BOOL EnableSkin(BOOL bEnable = TRUE,BOOL bEnableBorder = FALSE,BOOL bEnableBack=FALSE,BOOL bEnableTitleBar = FALSE);
	BOOL RTDrawFrameBorder(CDC *pDC,HRGN hrgn = NULL);
	BOOL RTDrawMenuBar(CDC *pDC,LPARAM lParam,HRGN hrgn = NULL);
	BOOL RTDrawTitleBar(CDC* pDC,BOOL bActive,HRGN hrgn = NULL);
	BOOL RTDrawTitleBarButtons(CDC* pDC);

	static BOOL SetBorderColor(COLORREF ColorArray[]);
	static BOOL SetMaximizeButtonBitmap(CBitmap *MaximizeButtonBitmap[],UINT DrawMode[]);
	static BOOL SetMinimizeButtonBitmap(CBitmap *MinimizeButtonBitmap[],UINT DrawMode[]);
	static BOOL SetExitButtonBitmap(CBitmap *ExitButtonBitmap[],UINT DrawMode[]);
	static BOOL SetRestoreButtonBitmap(CBitmap* RestoreButtonBitmap[],UINT DrawMode[]);
	static BOOL SetTitleBarBitmap(CBitmap* TitleBarBitmap[],UINT DrawMode[]);
	static BOOL SetMenuBarBitmap(CBitmap* MenuBarBitmap[],UINT DrawMode[]);

	LRESULT OnRTNcPaint(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTNcActive(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTActive(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTNodify(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTActiveApp(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTSetIcon(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTNcMouseMove(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTNcLMouseDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTNcLMouseUp(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTNcHitTest(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTGetText(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTSetText(WPARAM wParam, LPARAM lParam);
	LRESULT OnRTEraseBackGround(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTNcRButtonDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTNcRButtonUp(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTSysCommand(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTSetCursor(WPARAM wParam,LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP()
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
