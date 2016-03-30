// RTHeaderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "RTHeaderCtrl.h"
#include "RTDraw.h"

#define HDF_SORTUP              0x0400
#define HDF_SORTDOWN            0x0200
// CRTHeaderCtrl

IMPLEMENT_DYNAMIC(CRTHeaderCtrl, CHeaderCtrl)
CRTHeaderCtrl::CRTHeaderCtrl()
{
}

CRTHeaderCtrl::~CRTHeaderCtrl()
{
}

BOOL CRTHeaderCtrl::m_IsEanbleRTStyle = TRUE;
CBitmap* CRTHeaderCtrl::m_HeaderBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTHeaderCtrl::m_HeaderBitmapDrawMode[5] = {0,0,0,0,0};

BEGIN_MESSAGE_MAP(CRTHeaderCtrl, CHeaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRTHeaderCtrl 消息处理程序
void CRTHeaderCtrl::EnableSkin(BOOL IsEnable)
{
	m_IsEanbleRTStyle = IsEnable;
}

void CRTHeaderCtrl::SetHeaderBitmap(CBitmap* pHeader[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i++)
	{
		m_HeaderBitmap[i] = pHeader[i];
		m_HeaderBitmapDrawMode[i] = DrawMode[i];
	}
}

BOOL CRTHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEanbleRTStyle)
		return CHeaderCtrl::OnEraseBkgnd(pDC);
	else
		return FALSE;
}

void CRTHeaderCtrl::OnPaint()
{
	if(!m_IsEanbleRTStyle)return CHeaderCtrl::OnPaint();

	CPaintDC dc(this); // device context for painting
	
	CRect rect, rectItem, clientRect;
	GetClientRect(&rect);
	GetClientRect(&clientRect);
	CMemDC memDC(&dc, rect);
	
	CRTDraw::RTDrawBitmap(&memDC,&rect,m_HeaderBitmap[BMP_BACK],m_HeaderBitmapDrawMode[BMP_BACK]);
	
	int nItems = GetItemCount();

	memDC.SetBkMode(TRANSPARENT);
	CFont* font = GetFont();
	CFont* def_font = memDC.SelectObject(font);
	for(int i = 0; i <nItems; i++)
	{
		
		TCHAR buf[256];
		HD_ITEM hditem;
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( i, &hditem );
		GetItemRect(i, &rect);
		
		
		if(hditem.fmt & HDF_SORTUP)
			CRTDraw::RTDrawBitmap(&memDC,&rect,m_HeaderBitmap[BMP_FOCUS],m_HeaderBitmapDrawMode[BMP_FOCUS]);
		else if(hditem.fmt & HDF_SORTDOWN)
			CRTDraw::RTDrawBitmap(&memDC,&rect,m_HeaderBitmap[BMP_DOWN],m_HeaderBitmapDrawMode[BMP_DOWN]);
		else
			CRTDraw::RTDrawBitmap(&memDC,&rect,m_HeaderBitmap[BMP_NORMAL],m_HeaderBitmapDrawMode[BMP_NORMAL]);

		
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS ;
		
		CRect rtText(&rect);
		if((hditem.fmt & HDF_RIGHT) == HDF_RIGHT)
		{
			rtText.right -= 6;
			uFormat |= DT_RIGHT;
		}
		else if((hditem.fmt & HDF_CENTER) == HDF_CENTER)
			uFormat |= DT_CENTER;
		else
		{
			rtText.left += 6;
			uFormat |= DT_LEFT;
		}
		memDC.DrawText(buf, &rtText, uFormat);	
	}
	memDC.SelectObject(def_font);
}
