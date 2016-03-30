#include "StdAfx.h"
#include "RTSkin.h"
#include "RTXmlFile.h"
#include "RTDraw.h"
#include "RTDialog.h"
#include "RTButton.h"
#include "RTStatic.h"
#include "RTTreeCtrl.h"
#include "RTTabCtrl.h"
#include "RTStatusBar.h"
#include "RTSplitterWnd.h"
#include "RTListCtrl.h"
#include "RTScrollBar.h"
#include "RTEdit.h"
#include "RTRichEdit.h"
#include "RTFrameWnd.h"
#include "RTToolBar.h"
#include "RTMenu.h"
#include "RTCheckBox.h"

CRTSkin::CRTSkin(void)
{
	for(int i = 0; i < 5; i++)
	{
		m_SplitterWndBitmap[i] = NULL;
		m_TitleBarBitmap[i] = NULL;
		m_MenuBarBitmap[i] = NULL;
		m_MaximizeButtonBitmap[i] = NULL;
		m_MinimizeButtonBitmap[i] = NULL;
		m_RestoreButtonBitmap[i] = NULL;
		m_ExitButtonBitmap[i] = NULL;
		m_ToolBarBitmap[i] = NULL;
		m_ListCtrlBitmap[i] = NULL;
		m_TreeCtrlBitmap[i] = NULL;
		m_ButtonBitmap[i] = NULL;
		m_StatusBarBitmap[i] = NULL;
		m_HeaderCtrlBitmap[i] = NULL;
		m_ListCtrlBitmap[i] = NULL;
		
		m_ScrollBarVerticalBitmap[i] = NULL;
		m_ScrollBarHorizontalBitmap[i] = NULL;
		m_ScrollBarUpArrowBitmap[i] = NULL;
		m_ScrollBarLeftArrowBitmap[i] = NULL;
		m_ScrollBarDownArrowBitmap[i] = NULL;
		m_ScrollBarRightArrowBitmap[i] = NULL;
		m_MenuItemBitmap[i] = NULL;

		m_TitleBarBitmapDrawMode[i] = 0;
		m_MenuBarBitmapDrawMode[i] =  0;
		m_MaximizeButtonBitmapDrawMode[i] = 0;
		m_MinimizeButtonBitmapDrawMode[i] = 0;
		m_ExitButtonBitmapDrawMode[i] = 0;
		m_RestoreButtonBitmapDrawMode[i] = 0;
		m_ToolBarBitmapDrawMode[i] = 0;
		m_ButtonBitmapDrawMode[i] = 0;
		m_TreeCtrlBitmapDrawMode[i] = 0;
		m_StatusBarBitmapDrawMode[i] = 0;
		m_BorderColor[i] = 0;
		m_ListCtrlBitmapDrawMode[i] = 0;
	}

}

CRTSkin::~CRTSkin(void)
{
	Clear();
}

void CRTSkin::Clear()
{
	
	for(int i = 0; i < 5; i ++)
	{
		if(m_MenuItemBitmap[i] != NULL)
		{
			m_MenuItemBitmap[i]->DeleteObject();
			delete m_MenuItemBitmap[i];
			m_MenuItemBitmap[i] = NULL;
		}
		if(m_SplitterWndBitmap[i] != NULL)
		{
			m_SplitterWndBitmap[i]->DeleteObject();
			delete m_SplitterWndBitmap[i];
			m_SplitterWndBitmap[i] = NULL;
		}
		if(m_TitleBarBitmap[i] != NULL)
		{
			m_TitleBarBitmap[i]->DeleteObject();
			delete m_TitleBarBitmap[i];
			m_TitleBarBitmap[i] = NULL;
		}
		if(m_MenuBarBitmap[i] != NULL)
		{
			m_MenuBarBitmap[i]->DeleteObject();
			delete m_MenuBarBitmap[i];
			m_MenuBarBitmap[i] = NULL;
		}
		if(m_MaximizeButtonBitmap[i] != NULL)
		{
			m_MaximizeButtonBitmap[i]->DeleteObject();
			delete m_MaximizeButtonBitmap[i];
			m_MaximizeButtonBitmap[i] = NULL;
		}
		if(m_MinimizeButtonBitmap[i] != NULL)
		{
			m_MinimizeButtonBitmap[i]->DeleteObject();
			delete m_MinimizeButtonBitmap[i];
			m_MinimizeButtonBitmap[i] = NULL;
		}
		if(m_RestoreButtonBitmap[i] != NULL)
		{
			m_RestoreButtonBitmap[i]->DeleteObject();
			delete m_RestoreButtonBitmap[i];
			m_RestoreButtonBitmap[i] = NULL;
		}
		if(m_ExitButtonBitmap[i] != NULL)
		{
			m_ExitButtonBitmap[i]->DeleteObject();
			delete m_ExitButtonBitmap[i];
			m_ExitButtonBitmap[i] = NULL;
		}
		if(m_ToolBarBitmap[i] != NULL)
		{
			m_ToolBarBitmap[i]->DeleteObject();
			delete m_ToolBarBitmap[i];
			m_ToolBarBitmap[i] = NULL;
		}if(m_ButtonBitmap[i] != NULL)
		{
			m_ButtonBitmap[i]->DeleteObject();
			delete m_ButtonBitmap[i];
			m_ButtonBitmap[i] = NULL;
		}
		if(m_TreeCtrlBitmap[i] != NULL)
		{
			m_TreeCtrlBitmap[i]->DeleteObject();
			delete m_TreeCtrlBitmap[i];
			m_TreeCtrlBitmap[i] = NULL;
		}
		if(m_StatusBarBitmap[i] != NULL)
		{
			m_StatusBarBitmap[i]->DeleteObject();
			delete m_StatusBarBitmap[i];
			m_StatusBarBitmap[i] = NULL;
		}

		if(m_HeaderCtrlBitmap[i] != NULL)
		{
			m_HeaderCtrlBitmap[i]->DeleteObject();
			delete m_HeaderCtrlBitmap[i];
			m_HeaderCtrlBitmap[i] = NULL;
		}
//
		if(m_ScrollBarVerticalBitmap[i] != NULL)
		{
			m_ScrollBarVerticalBitmap[i]->DeleteObject();
			delete m_ScrollBarVerticalBitmap[i];
			m_ScrollBarVerticalBitmap[i] = NULL;
		}
		if(m_ScrollBarHorizontalBitmap[i] != NULL)
		{
			m_ScrollBarHorizontalBitmap[i]->DeleteObject();
			delete m_ScrollBarHorizontalBitmap[i];
			m_ScrollBarHorizontalBitmap[i] = NULL;
		}
		if(m_ScrollBarUpArrowBitmap[i] != NULL)
		{
			m_ScrollBarUpArrowBitmap[i]->DeleteObject();
			delete m_ScrollBarUpArrowBitmap[i];
			m_ScrollBarUpArrowBitmap[i] = NULL;
		}
		if(m_ScrollBarLeftArrowBitmap[i] != NULL)
		{
			m_ScrollBarLeftArrowBitmap[i]->DeleteObject();
			delete m_ScrollBarLeftArrowBitmap[i];
			m_ScrollBarLeftArrowBitmap[i] = NULL;
		}
		if(m_ScrollBarDownArrowBitmap[i] != NULL)
		{
			m_ScrollBarDownArrowBitmap[i]->DeleteObject();
			delete m_ScrollBarDownArrowBitmap[i];
			m_ScrollBarDownArrowBitmap[i] = NULL;
		}
		if(m_ScrollBarRightArrowBitmap[i] != NULL)
		{
			m_ScrollBarRightArrowBitmap[i]->DeleteObject();
			delete m_ScrollBarRightArrowBitmap[i];
			m_ScrollBarRightArrowBitmap[i] = NULL;
		}
		if(m_ListCtrlBitmap[i] != NULL)
		{
			m_ListCtrlBitmap[i]->DeleteObject();
			delete m_ListCtrlBitmap[i];
			m_ListCtrlBitmap[i] = NULL;
		}
	}
}

CBitmap* CRTSkin::LoadBitmap(LPCTSTR lpRelativeFileName)
{
	if(lpRelativeFileName == NULL)return NULL;
	char StyleFileName[MAX_PATH];
	ZeroMemory(StyleFileName,MAX_PATH);
	strcpy(StyleFileName,m_BasePath);
	strcat(StyleFileName,lpRelativeFileName);
	HANDLE hHandle = LoadImage(NULL,StyleFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hHandle == NULL)return NULL;
	CBitmap* pBitmap = new CBitmap();
	if(!pBitmap->Attach(hHandle))
	{
		DeleteObject(hHandle);
		return NULL;
	}
	return pBitmap;
}


BOOL CRTSkin::Load(LPCTSTR lpStyleFileName)
{
	CRTXmlFile cf;

	if(!cf.Open(lpStyleFileName)) {
		XTRACE(XTRACE_COLOR_RED, "读取皮肤配置文件 %s 失败。\n", lpStyleFileName);
		exit(2);
	}

	char StyleFileName[MAX_PATH];
	strcpy(StyleFileName,lpStyleFileName);
	int nlen = (int)strlen(lpStyleFileName);
	while(nlen > 0)
	{
		if(StyleFileName[nlen - 1] == '\\')
		{
			StyleFileName[nlen] = NULL;
			break;
		}
		nlen --;
	}
	if(nlen == 0)return FALSE;

	strcpy(m_BasePath,StyleFileName);
	Clear();
	XTRACE("解析皮肤文件完成\n");

	CRTXmlItem *pItem = NULL;
	pItem = cf.GetItem("ScrollBarHorizontal");
	if(pItem != NULL)
	{
		m_ScrollBarHorizontalBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarHorizontalBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarHorizontalBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarHorizontalBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarHorizontalBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarHorizontalBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarHorizontalBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarHorizontalBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarHorizontalBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarHorizontalBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("MenuItem");
	if(pItem != NULL)
	{
		m_MenuItemBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_MenuItemBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_MenuItemBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_MenuItemBitmap[BMP_SLIDER] = LoadBitmap(pItem->GetKeyValue("Slider"));
		m_MenuItemBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_MenuItemBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_MenuItemBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_MenuItemBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_MenuItemBitmapDrawMode[BMP_SLIDER] = StrToUINT(pItem->GetKeyValue("SliderDrawMode"));
		m_MenuItemBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ScrollBarVertical");
	if(pItem != NULL)
	{
		m_ScrollBarVerticalBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarVerticalBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarVerticalBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarVerticalBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarVerticalBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarVerticalBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarVerticalBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarVerticalBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarVerticalBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarVerticalBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ScrollBarLeftArrow");
	if(pItem != NULL)
	{
		m_ScrollBarLeftArrowBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarLeftArrowBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarLeftArrowBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarLeftArrowBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarLeftArrowBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarLeftArrowBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarLeftArrowBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarLeftArrowBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarLeftArrowBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarLeftArrowBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ScrollBarRightArrow");
	if(pItem != NULL)
	{
		m_ScrollBarRightArrowBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarRightArrowBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarRightArrowBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarRightArrowBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarRightArrowBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarRightArrowBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarRightArrowBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarRightArrowBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarRightArrowBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarRightArrowBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ScrollBarUpArrow");
	if(pItem != NULL)
	{
		m_ScrollBarUpArrowBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarUpArrowBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarUpArrowBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarUpArrowBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarUpArrowBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarUpArrowBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarUpArrowBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarUpArrowBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarUpArrowBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarUpArrowBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ScrollBarDownArrow");
	if(pItem != NULL)
	{
		m_ScrollBarDownArrowBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ScrollBarDownArrowBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ScrollBarDownArrowBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ScrollBarDownArrowBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ScrollBarDownArrowBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_ScrollBarDownArrowBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ScrollBarDownArrowBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ScrollBarDownArrowBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ScrollBarDownArrowBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ScrollBarDownArrowBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("TitleBar");
	if(pItem != NULL)
	{
		m_TitleBarBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_TitleBarBitmap[BMP_ACTIVE] = LoadBitmap(pItem->GetKeyValue("Active"));
		m_TitleBarBitmap[BMP_INACTIVE] = LoadBitmap(pItem->GetKeyValue("InActive"));
		m_TitleBarBitmapDrawMode[BMP_ACTIVE] = StrToUINT(pItem->GetKeyValue("ActiveDrawMode"));
		m_TitleBarBitmapDrawMode[BMP_INACTIVE] = StrToUINT(pItem->GetKeyValue("InActiveDrawMode"));
		m_TitleBarBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
	}
	pItem = cf.GetItem("HeaderCtrl");
	if(pItem != NULL)
	{
		m_HeaderCtrlBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_HeaderCtrlBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_HeaderCtrlBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_HeaderCtrlBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_HeaderCtrlBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));

		m_HeaderCtrlBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_HeaderCtrlBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_HeaderCtrlBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_HeaderCtrlBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_HeaderCtrlBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("SplitterWnd");
	if(pItem != NULL)
	{
		m_SplitterWndBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_SplitterWndBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_SplitterWndBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_SplitterWndBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_SplitterWndBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_SplitterWndBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_SplitterWndBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_SplitterWndBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_SplitterWndBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_SplitterWndBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}
	pItem = cf.GetItem("MenuBar");
	if(pItem != NULL)
	{
		m_MenuBarBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_MenuBarBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_MenuBarBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_MenuBarBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_MenuBarBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_MenuBarBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_MenuBarBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_MenuBarBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_MenuBarBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_MenuBarBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("BorderColor");
	if(pItem != NULL)
	{
		m_BorderColor[0] = StrToUINT(pItem->GetKeyValue("Color0"));
		m_BorderColor[1] = StrToUINT(pItem->GetKeyValue("Color1"));
		m_BorderColor[2] = StrToUINT(pItem->GetKeyValue("Color2"));
		m_BorderColor[3] = StrToUINT(pItem->GetKeyValue("Color3"));
	}

	pItem = cf.GetItem("ToolBar");
	if(pItem != NULL)
	{
		m_ToolBarBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ToolBarBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ToolBarBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ToolBarBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ToolBarBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_ToolBarBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
		m_ToolBarBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ToolBarBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ToolBarBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ToolBarBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ExitButton");
	if(pItem != NULL)
	{
		m_ExitButtonBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ExitButtonBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ExitButtonBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ExitButtonBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_ExitButtonBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ExitButtonBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ExitButtonBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ExitButtonBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("RestoreButton");
	if(pItem != NULL)
	{
		m_RestoreButtonBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_RestoreButtonBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_RestoreButtonBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_RestoreButtonBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_RestoreButtonBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_RestoreButtonBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_RestoreButtonBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_RestoreButtonBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("MaximizeButton");
	if(pItem != NULL)
	{
		m_MaximizeButtonBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_MaximizeButtonBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_MaximizeButtonBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_MaximizeButtonBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_MaximizeButtonBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_MaximizeButtonBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_MaximizeButtonBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_MaximizeButtonBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("MinimizeButton");
	if(pItem != NULL)
	{
		m_MinimizeButtonBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_MinimizeButtonBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_MinimizeButtonBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_MinimizeButtonBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_MinimizeButtonBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_MinimizeButtonBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_MinimizeButtonBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_MinimizeButtonBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}
	
	pItem = cf.GetItem("Button");
	if(pItem != NULL)
	{
		m_ButtonBitmap[BMP_NORMAL] = LoadBitmap(pItem->GetKeyValue("Normal"));
		m_ButtonBitmap[BMP_FOCUS] = LoadBitmap(pItem->GetKeyValue("Focus"));
		m_ButtonBitmap[BMP_DOWN] = LoadBitmap(pItem->GetKeyValue("Down"));
		m_ButtonBitmap[BMP_DISABLE] = LoadBitmap(pItem->GetKeyValue("Disable"));
		m_ButtonBitmapDrawMode[BMP_NORMAL] = StrToUINT(pItem->GetKeyValue("NormalDrawMode"));
		m_ButtonBitmapDrawMode[BMP_FOCUS] = StrToUINT(pItem->GetKeyValue("FocusDrawMode"));
		m_ButtonBitmapDrawMode[BMP_DOWN] = StrToUINT(pItem->GetKeyValue("DownDrawMode"));
		m_ButtonBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("DisableDrawMode"));
	}

	pItem = cf.GetItem("ListCtrl");
	if(pItem != NULL)
	{
		m_ListCtrlBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_ListCtrlBitmapDrawMode[BMP_BACK]= StrToUINT(pItem->GetKeyValue("BackDrawMode"));
	}

	pItem = cf.GetItem("TreeCtrl");
	if(pItem != NULL)
	{
		m_TreeCtrlBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_TreeCtrlBitmapDrawMode[BMP_BACK] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
	}

	pItem = cf.GetItem("StatusBar");
	if(pItem != NULL)
	{
		m_StatusBarBitmap[BMP_BACK] = LoadBitmap(pItem->GetKeyValue("Back"));
		m_StatusBarBitmapDrawMode[BMP_DISABLE] = StrToUINT(pItem->GetKeyValue("BackDrawMode"));
	}
	XTRACE("加载皮肤图像完成\n");

	CRTFrameWnd::SetExitButtonBitmap(m_ExitButtonBitmap,m_ExitButtonBitmapDrawMode);
	CRTFrameWnd::SetRestoreButtonBitmap(m_RestoreButtonBitmap,m_RestoreButtonBitmapDrawMode);
	CRTFrameWnd::SetMaximizeButtonBitmap(m_MaximizeButtonBitmap,m_MaximizeButtonBitmapDrawMode);
	CRTFrameWnd::SetMinimizeButtonBitmap(m_MinimizeButtonBitmap,m_MinimizeButtonBitmapDrawMode);
	CRTFrameWnd::SetTitleBarBitmap(m_TitleBarBitmap,m_TitleBarBitmapDrawMode);
	CRTFrameWnd::SetMenuBarBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTFrameWnd::SetBorderColor(m_BorderColor);
	
	CRTDialog::SetExitButtonBitmap(m_ExitButtonBitmap,m_ExitButtonBitmapDrawMode);
	CRTDialog::SetRestoreButtonBitmap(m_RestoreButtonBitmap,m_RestoreButtonBitmapDrawMode);
	CRTDialog::SetMaximizeButtonBitmap(m_MaximizeButtonBitmap,m_MaximizeButtonBitmapDrawMode);
	CRTDialog::SetMinimizeButtonBitmap(m_MinimizeButtonBitmap,m_MinimizeButtonBitmapDrawMode);
	CRTDialog::SetTitleBarBitmap(m_TitleBarBitmap,m_TitleBarBitmapDrawMode);
	CRTDialog::SetMenuBarBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTDialog::SetBorderColor(m_BorderColor);

	CRTSplitterWnd::SetBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTButton::SetButtonBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTStatusBar::SetBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTToolBar::SetBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTTabCtrl::SetTabBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTStatic::SetBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTHeaderCtrl::SetHeaderBitmap(m_HeaderCtrlBitmap,m_HeaderCtrlBitmapDrawMode);
	CRTTreeCtrl::SetBackBitmap(m_ListCtrlBitmap,m_ListCtrlBitmapDrawMode);
	CRTListCtrl::SetBackBitmap(m_ListCtrlBitmap,m_ListCtrlBitmapDrawMode);
	CRTScrollBar::SetVerticalBitmap(m_ScrollBarVerticalBitmap,m_ScrollBarVerticalBitmapDrawMode);
	CRTScrollBar::SetHorizontalBitmap(m_ScrollBarHorizontalBitmap,m_ScrollBarHorizontalBitmapDrawMode);
	CRTScrollBar::SetHorizontalLeftArrowBitmap(m_ScrollBarLeftArrowBitmap,m_ScrollBarLeftArrowBitmapDrawMode);
	CRTScrollBar::SetHorizontalRightArrowBitmap(m_ScrollBarRightArrowBitmap,m_ScrollBarRightArrowBitmapDrawMode);
	CRTScrollBar::SetVerticalDownArrowBitmap(m_ScrollBarDownArrowBitmap,m_ScrollBarDownArrowBitmapDrawMode);
	CRTScrollBar::SetVerticalUpArrowBitmap(m_ScrollBarUpArrowBitmap,m_ScrollBarUpArrowBitmapDrawMode);
	CRTEdit::SetBackBitmap(m_ListCtrlBitmap,m_MenuBarBitmapDrawMode);

	CRTCheckBox::SetBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);

	CRTMenu::SetMenuBarBitmap(m_MenuBarBitmap,m_MenuBarBitmapDrawMode);
	CRTMenu::SetMenuItemBitmap(m_MenuItemBitmap,m_MenuItemBitmapDrawMode);

	XTRACE("关联皮肤图像完成\n");
	return TRUE;
}

UINT CRTSkin::StrToUINT(LPCTSTR lpValue)
{
	if(lpValue == NULL)return 0;
	return (UINT)atol(lpValue);
}

void CRTSkin::GetFullPathName(LPCTSTR lpRelativeFile, char* Buf)
{
	if(lpRelativeFile == NULL)return;

	ZeroMemory(Buf,MAX_PATH);
	strcpy(Buf,m_BasePath);
	strcat(Buf,lpRelativeFile);
}



void CRTSkin::UseSkin(BOOL IsEnable)
{
	CRTScrollBar::EnableSkin(IsEnable);
	//CRTFrameWnd::EnableSkin(IsEnable);
	//CRTDialog::EnableSkin(IsEnable);
	CRTEdit::EnableSkin(IsEnable);
	CRTToolBar::EnableSkin(IsEnable);
	CRTStatic::EnableSkin(IsEnable);
	CRTTreeCtrl::EnableSkin(IsEnable);
	CRTListCtrl::EnableSkin(IsEnable);
	CRTStatic::EnableSkin(IsEnable);
	CRTTabCtrl::EnableSkin(IsEnable);
	CRTStatusBar::EnableSkin(IsEnable);
	CRTSplitterWnd::EnableSkin(IsEnable);
	CRTHeaderCtrl::EnableSkin(IsEnable);
	CRTButton::EnableSkin(IsEnable);
	CRTCheckBox::EnableSkin(IsEnable);
	//CRTMenu::EnableSkin(IsEnable);
}
