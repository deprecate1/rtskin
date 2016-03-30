// RTEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "RTEdit.h"
#include "RTDraw.h"

// CRTEdit

IMPLEMENT_DYNAMIC(CRTEdit, CEdit)
CRTEdit::CRTEdit()
{
}

CRTEdit::~CRTEdit()
{
}


BEGIN_MESSAGE_MAP(CRTEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


CBitmap * CRTEdit::m_BackBitmap[5] = {NULL,NULL,NULL,NULL};
UINT      CRTEdit::m_BackBitmapDrawMode[5] = {0,0,0,0,0};
BOOL      CRTEdit::m_bEnableSkin = TRUE;
// CRTEdit 消息处理程序

BOOL CRTEdit::OnEraseBkgnd(CDC* pDC)
{
	if(!m_bEnableSkin) return CEdit::OnEraseBkgnd(pDC);
	
	return TRUE;
}

void CRTEdit::SetBackBitmap(CBitmap* pBack[],UINT DrawMode[])
{
	for(int i = 0;i < 5; i++)
	{
		m_BackBitmap[i] = pBack[i];
		m_BackBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTEdit::EnableSkin(BOOL IsEnable)
{
	m_bEnableSkin = IsEnable;
}

void CRTEdit::OnPaint()
{
	if(!m_bEnableSkin)return CEdit::OnPaint();

	if(m_BackBitmap[BMP_BACK] == NULL)
		return CEdit::OnPaint();
    
	DrawText();
	CPaintDC dc(this);
}


void CRTEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown(nFlags, point);
	if(m_bEnableSkin)DrawText();
}

void CRTEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags == MK_LBUTTON)
		DrawText();
	CEdit::OnMouseMove(nFlags, point);
}

void CRTEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	if(m_bEnableSkin)DrawText();
}

void CRTEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonUp(nFlags, point);
}

void CRTEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	
	if(m_bEnableSkin)DrawText();
}

LRESULT CRTEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_bEnableSkin && ( message == WM_CUT || message == WM_PASTE || WM_CHAR))
	{
        DrawText();
	}
	return CEdit::WindowProc(message, wParam, lParam);
}

void CRTEdit::DrawText(void)
{
	if(m_BackBitmap[BMP_BACK] == NULL)
		return;
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

	DefWindowProc(WM_ERASEBKGND, (WPARAM)defDC.m_hDC , 0);
	DefWindowProc(WM_PAINT, (WPARAM)defDC.m_hDC , 0);

	CRTDraw::RTDrawBitmap(&memDC,&rcWnd,m_BackBitmap[BMP_BACK],m_BackBitmapDrawMode[BMP_BACK]);

	memDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),GetSysColor(COLOR_WINDOW));
	defDC.SelectObject(defOld);
}
