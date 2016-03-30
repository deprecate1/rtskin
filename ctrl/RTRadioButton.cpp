// RTRadioButton.cpp : 实现文件
//

#include "stdafx.h"
#include "RTDraw.h"
#include "RTRadioButton.h"


// CRTRadioButton

IMPLEMENT_DYNAMIC(CRTRadioButton, CButton)
CRTRadioButton::CRTRadioButton()
{
}

CRTRadioButton::~CRTRadioButton()
{
}

CBitmap* CRTRadioButton::m_StaticBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTRadioButton::m_StaticBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTRadioButton::m_IsEnableSkin = FALSE;
BOOL     CRTRadioButton::m_IsBackTransparent = FALSE;

BEGIN_MESSAGE_MAP(CRTRadioButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRTRadioButton 消息处理程序
void CRTRadioButton::SetBitmap(CBitmap* StaticBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_StaticBitmap[i]=StaticBitmap[i];
		m_StaticBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTRadioButton::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}

void CRTRadioButton::BackTransparent(BOOL Transparent)
{
	m_IsBackTransparent = Transparent;
}


BOOL CRTRadioButton::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)	return CButton::OnEraseBkgnd(pDC);

	return TRUE;
}

void CRTRadioButton::OnPaint()
{
	
	if(!m_IsEnableSkin)return CButton::OnPaint();

	DWORD style = GetStyle();
	if((style & BS_RADIOBUTTON) != BS_RADIOBUTTON && (style & BS_AUTORADIOBUTTON) != BS_AUTORADIOBUTTON)
		return CButton::OnPaint();
	
	CRect rcWnd;
	GetClientRect(&rcWnd);
	CPaintDC ptDC(this);

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
		CRTDraw::RTDrawBitmap(&ptDC,&rcWnd,m_StaticBitmap[BMP_BACK],m_StaticBitmapDrawMode[BMP_BACK]);
	}

	CRect rtIcon;
	TEXTMETRIC tm;
	ptDC.GetTextMetrics(&tm);
	int height = tm.tmHeight;
	rtIcon.left = rcWnd.left;
	rtIcon.top = rcWnd.top + (rcWnd.Width() - height)/2;
	rtIcon.bottom = rtIcon.top + height;
	rtIcon.right = rtIcon.left + height;
	
	ptDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),TransparentColor);
	
	//if((style & WS_DISABLED) == WS_DISABLED)
	//	CRTDraw::RTDrawBitmap(&ptDC,&rtIcon,m_StaticBitmap[BMP_DISABLE],m_StaticBitmapDrawMode[BMP_DISABLE]);

	//int check = GetCheck();
	//if(check == BST_CHECKED)
	//	CRTDraw::RTDrawBitmap(&ptDC,&rtIcon,m_StaticBitmap[BMP_CHECK],m_StaticBitmapDrawMode[BMP_CHECK]);
	//else
	//	CRTDraw::RTDrawBitmap(&ptDC,&rtIcon,m_StaticBitmap[BMP_UNCHECK],m_StaticBitmapDrawMode[BMP_UNCHECK]);

	defDC.SelectObject(defOld);
}
