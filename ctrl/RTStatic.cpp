// RTStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "RTStatic.h"
#include "RTDraw.h"

// CRTStatic

IMPLEMENT_DYNAMIC(CRTStatic, CStatic)
CRTStatic::CRTStatic()
{
}

CRTStatic::~CRTStatic()
{
}

CBitmap* CRTStatic::m_StaticBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTStatic::m_StaticBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTStatic::m_IsEnableSkin = FALSE;
BOOL     CRTStatic::m_IsBackTransparent = FALSE;


BEGIN_MESSAGE_MAP(CRTStatic, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRTStatic 消息处理程序


BOOL CRTStatic::OnEraseBkgnd(CDC* pDC)
{
	if(m_StaticBitmap[0] == NULL || !m_IsEnableSkin)
		return CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}

void CRTStatic::OnPaint()
{
	if(!m_IsEnableSkin)return CStatic::OnPaint();

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
	
	DWORD style = GetStyle();
	if(!m_IsBackTransparent && (style & BS_GROUPBOX) != BS_GROUPBOX)
	{
		CRTDraw::RTDrawBitmap(&ptDC,&rcWnd,m_StaticBitmap[BMP_BACK],m_StaticBitmapDrawMode[BMP_BACK]);
	}
	
	ptDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),TransparentColor);
	
	defDC.SelectObject(defOld);
}

void CRTStatic::SetBitmap(CBitmap* StaticBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_StaticBitmap[i]=StaticBitmap[i];
		m_StaticBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTStatic::EnableSkin(BOOL IsEnable,BOOL BackTransparent)
{
	m_IsEnableSkin = IsEnable;
	m_IsBackTransparent = BackTransparent;
}

