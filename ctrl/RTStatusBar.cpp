// RTStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "RTStatusBar.h"
#include "RTDraw.h"

// CRTStatusBar

IMPLEMENT_DYNAMIC(CRTStatusBar, CStatusBar)
CRTStatusBar::CRTStatusBar()
{
}

CRTStatusBar::~CRTStatusBar()
{
}

CBitmap* CRTStatusBar::m_StatusBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTStatusBar::m_StatusBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTStatusBar::m_IsEnableSkin = TRUE;

BEGIN_MESSAGE_MAP(CRTStatusBar, CStatusBar)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRTStatusBar 消息处理程序


BOOL CRTStatusBar::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)return CStatusBar::OnEraseBkgnd(pDC);

	CRect rtClient;
	GetWindowRect(&rtClient);
	rtClient.SetRect(0,0,rtClient.Width(),rtClient.Height());
		
	if(!CRTDraw::RTDrawBitmap(pDC,&rtClient,m_StatusBitmap[BMP_BACK],m_StatusBitmapDrawMode[BMP_BACK]))
        return CStatusBar::OnEraseBkgnd(pDC);
	else
		return TRUE;
}

void CRTStatusBar::SetBitmap(CBitmap*  StatusBitmap[], UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_StatusBitmap[i] = StatusBitmap[i];
		m_StatusBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTStatusBar::OnPaint()
{
	if(!m_IsEnableSkin)return CStatusBar::OnPaint();

	CPaintDC cDC(this);

	

    CRect rcItem;
    for ( int i = 0; i < m_nCount; i++ )
    {
        GetItemRect (i, rcItem);

		if(i == m_nCount - 1)
		{
			CRect rcClient;
			GetClientRect(&rcClient);
			if(rcItem.right < rcClient.right)
			{
				rcItem.SetRect(rcItem.left,rcItem.top,rcClient.right,rcItem.bottom);
				
			}
		}
		
		cDC.Draw3dRect(&rcItem,GetSysColor(COLOR_3DSHADOW),0xFFFFFF);
		rcItem.SetRect(rcItem.left + 1,rcItem.top + 1,rcItem.right - 1,rcItem.bottom - 1);
		CString itemText;
		GetPaneText(i,itemText);

		if(m_StatusBitmap[BMP_BACK] != NULL)
		{
			CRTDraw::RTDrawBitmap(&cDC,&rcItem,m_StatusBitmap[BMP_BACK],m_StatusBitmapDrawMode[BMP_BACK]);
		}

		CFont TitleFont;
		NONCLIENTMETRICS nif;
		nif.cbSize = sizeof(nif);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&nif,0);
		nif.lfCaptionFont.lfWeight = FW_NORMAL;
		TitleFont.CreateFontIndirect(&nif.lfCaptionFont);
		CFont *pOldFont = cDC.SelectObject(&TitleFont);
		int oldBkMode = cDC.SetBkMode(TRANSPARENT);
		TEXTMETRIC Metrics;
		cDC.GetTextMetrics(&Metrics);
		rcItem.top = rcItem.top + (rcItem.Height() - Metrics.tmHeight)/2;
		cDC.DrawText(itemText,&rcItem,DT_SINGLELINE);
		cDC.SelectObject(pOldFont);
    }
}

BOOL CRTStatusBar::SetIndicators (const UINT* lpIDArray, int nIDCount)
{
    if ( !CStatusBar::SetIndicators (lpIDArray, nIDCount) )
    {
        return false;
    }
	if(!m_IsEnableSkin)return TRUE;

    SendMessage (SB_SETMINHEIGHT, 20);

    for ( int i = 0; i < nIDCount; i++ )
    {
        UINT a, b;
        int c;

        GetPaneInfo (i, a, b, c);
        SetPaneInfo (i, a, b|SBPS_NOBORDERS, c);
    }
    return true;
}

void CRTStatusBar::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}