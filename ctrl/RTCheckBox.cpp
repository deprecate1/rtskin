// RTCheckBox.cpp : 实现文件
//

#include "stdafx.h"
#include "RTDraw.h"
#include "RTCheckBox.h"


// CRTCheckBox

IMPLEMENT_DYNAMIC(CRTCheckBox, CButton)
CRTCheckBox::CRTCheckBox()
{
}

CRTCheckBox::~CRTCheckBox()
{
}

CBitmap* CRTCheckBox::m_StaticBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTCheckBox::m_StaticBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTCheckBox::m_IsEnableSkin = FALSE;
BOOL     CRTCheckBox::m_IsBackTransparent = FALSE;

BEGIN_MESSAGE_MAP(CRTCheckBox, CButton)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CRTCheckBox 消息处理程序
void CRTCheckBox::SetBitmap(CBitmap* StaticBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_StaticBitmap[i]=StaticBitmap[i];
		m_StaticBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTCheckBox::EnableSkin(BOOL IsEnable,BOOL BackTransparent)
{
	m_IsEnableSkin = IsEnable;
	m_IsBackTransparent = BackTransparent;
}


void CRTCheckBox::OnPaint()
{
	if(!m_IsEnableSkin)return CButton::OnPaint();

	DWORD style = GetStyle();
	if((style & BS_CHECKBOX) != BS_CHECKBOX && (style & BS_AUTOCHECKBOX) != BS_AUTOCHECKBOX)
		return CButton::OnPaint();
	
	CRect rcWnd;
	GetClientRect(&rcWnd);
	CPaintDC ptDC(this);
	CMemDC memDC(&ptDC,rcWnd);

	CDC defDC;

	defDC.CreateCompatibleDC(&ptDC);
	
	CBitmap  defBmp;
	CBitmap* defOld;
	
	defBmp.CreateCompatibleBitmap(&ptDC,rcWnd.right,rcWnd.bottom);
	defOld = defDC.SelectObject(&defBmp);
	
	COLORREF TransparentColor = GetSysColor(COLOR_3DFACE);
	CBrush br(TransparentColor);
	defDC.FillRect(&rcWnd,&br);
	DefWindowProc(WM_ERASEBKGND, (WPARAM)defDC.m_hDC , 0);
	DefWindowProc(WM_PAINT, (WPARAM)defDC.m_hDC , 0);
	
	if(!m_IsBackTransparent)
	{
		CRTDraw::RTDrawBitmap(&memDC,&rcWnd,m_StaticBitmap[BMP_BACK],m_StaticBitmapDrawMode[BMP_BACK]);
	}

	
	memDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),TransparentColor);
	
	return;
	CRect rtIcon;
	TEXTMETRIC tm;
	ptDC.GetTextMetrics(&tm);
	int height = tm.tmHeight;
	rtIcon.left = rcWnd.left;
	rtIcon.top = rcWnd.top + (rcWnd.Height() - height)/2;
	rtIcon.bottom = rtIcon.top + height;
	rtIcon.right = rtIcon.left + height;
	

	if((style & WS_DISABLED) == WS_DISABLED)
		CRTDraw::RTDrawBitmap(&memDC,&rtIcon,m_StaticBitmap[BMP_DISABLE],m_StaticBitmapDrawMode[BMP_DISABLE]);

	int check = GetCheck();
	if(check == BST_CHECKED)
		CRTDraw::RTDrawBitmap(&memDC,&rtIcon,m_StaticBitmap[BMP_CHECK],m_StaticBitmapDrawMode[BMP_CHECK]);
	else
		CRTDraw::RTDrawBitmap(&memDC,&rtIcon,m_StaticBitmap[BMP_UNCHECK],m_StaticBitmapDrawMode[BMP_UNCHECK]);

	defDC.SelectObject(defOld);
}

BOOL CRTCheckBox::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)return CButton::OnEraseBkgnd(pDC);

	return TRUE;
}

LRESULT CRTCheckBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	LRESULT result = CButton::WindowProc(message, wParam, lParam);

	//if(message == BM_SETCHECK)
	//{
	//	Invalidate();
	//}
	//
	return result;
}
