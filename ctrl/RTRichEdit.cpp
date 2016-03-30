// RTRichEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "RTRichEdit.h"
#include "RTDraw.h"

// CRTRichEdit

IMPLEMENT_DYNAMIC(CRTRichEdit, CRichEditCtrl)
CRTRichEdit::CRTRichEdit()
{
}

CRTRichEdit::~CRTRichEdit()
{
}

CBitmap* CRTRichEdit::m_BackBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTRichEdit::m_BackBitmapDrawMode[5]={0,0,0,0,0};
BOOL     CRTRichEdit::m_bEnableSkin = TRUE;

BEGIN_MESSAGE_MAP(CRTRichEdit, CRichEditCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CRTRichEdit 消息处理程序


void CRTRichEdit::OnPaint()
{
	if(!m_bEnableSkin)return CRichEditCtrl::OnPaint();

	CRect rcWnd;
	GetClientRect(&rcWnd);

	CClientDC ptDC(this);

	CDC defDC;
	CMemDC memDC(&ptDC,rcWnd);

	defDC.CreateCompatibleDC(&ptDC);

	CBitmap  defBmp;
	CBitmap* defOld;
	
	defBmp.CreateCompatibleBitmap(&ptDC,rcWnd.Width(),rcWnd.Height());
	defOld = defDC.SelectObject(&defBmp);

	//DefWindowProc(WM_ERASEBKGND, (WPARAM)defDC.m_hDC , 0);
	DefWindowProc(WM_PAINT, (WPARAM)defDC.m_hDC , 0);

	CRTDraw::RTDrawBitmap(&memDC,&rcWnd,m_BackBitmap[BMP_BACK],m_BackBitmapDrawMode[BMP_BACK]);
	memDC.BitBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,SRCCOPY);
	//memDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),GetSysColor(COLOR_WINDOW));
	CPaintDC dc(this);

	defDC.SelectObject(defOld);
}

BOOL CRTRichEdit::OnEraseBkgnd(CDC* pDC)
{
    if(!m_bEnableSkin)return CRichEditCtrl::OnEraseBkgnd(pDC);

	return TRUE;
}

void CRTRichEdit::PreSubclassWindow()
{
	m_Container.Create(this);
	CRichEditCtrl::PreSubclassWindow();
}

void CRTRichEdit::SetBackBitmap(CBitmap* pBack[],UINT DrawMode[])
{
	for(int i = 0; i < 5 ; i ++)
	{
		m_BackBitmap[i] = pBack[i];
		m_BackBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTRichEdit::EnableSkin(BOOL IsEnable)
{
	m_bEnableSkin = IsEnable;
}