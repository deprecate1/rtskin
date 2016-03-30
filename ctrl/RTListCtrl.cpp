// RTListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "RTListCtrl.h"
#include "RTDraw.h"
// CRTListCtrl

IMPLEMENT_DYNAMIC(CRTListCtrl, CListCtrl)
CRTListCtrl::CRTListCtrl()
{
	m_bDraging = FALSE;
	m_bPainting = FALSE;
}

CRTListCtrl::~CRTListCtrl()
{
}

CBitmap* CRTListCtrl::m_BackBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTListCtrl::m_BackBitmapDrawMode[5] = {0,0,0,0,0};
BOOL     CRTListCtrl::m_bIsEnableSkin = TRUE;

BEGIN_MESSAGE_MAP(CRTListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CRTListCtrl 消息处理程序

void CRTListCtrl::SetBackBitmap(CBitmap* pBitmap[],UINT DrawMode[])
{	
	for(int i = 0; i < 5; i ++)
	{
		m_BackBitmap[i] = pBitmap[i];
		m_BackBitmapDrawMode[i] = DrawMode[i];
	}
}
void CRTListCtrl::EnableSkin(BOOL IsEnable)
{
	m_bIsEnableSkin = IsEnable;
}

void CRTListCtrl::PreSubclassWindow()
{
	if(GetHeaderCtrl() != NULL)
	{
		m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	}

	m_Container.Create(this);
	CListCtrl::PreSubclassWindow();
}

int CRTListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Container.Create(this);
	return 0;
}

void CRTListCtrl::OnPaint()
{
	int nmax;
	int npos;

	nmax = GetScrollLimit(SBS_VERT);
	m_Container.SetVerRange(0,nmax);
	npos = GetScrollPos(SBS_VERT);
	m_Container.SetVerPos(npos);

	nmax = GetScrollLimit(SBS_HORZ);
	m_Container.SetHorRange(0,nmax);
	npos = GetScrollPos(SBS_HORZ);
	m_Container.SetHorPos(npos);
	if(!m_bIsEnableSkin)return CListCtrl::OnPaint();

	CRect rcArea;
	CRect rcWnd;
	GetClientRect(&rcWnd);
	rcArea = rcWnd;

	DWORD style = GetStyle();
	if((style & LVS_NOCOLUMNHEADER) != LVS_NOCOLUMNHEADER && (style & LVS_REPORT) == LVS_REPORT)
	{
		if(IsWindow(m_HeaderCtrl.m_hWnd))
		{
			CRect rtHeader;
			m_HeaderCtrl.GetWindowRect(&rtHeader);
			rcArea.top += rtHeader.Height();
		}
	}

	CClientDC ptDC(this);

	CDC defDC;
	CMemDC memDC(&ptDC,rcArea);

	defDC.CreateCompatibleDC(&ptDC);

	CBitmap  defBmp;
	CBitmap* defOld;
	defBmp.CreateCompatibleBitmap(&ptDC,rcWnd.Width(),rcWnd.Height());
	defOld = defDC.SelectObject(&defBmp);
	
	DefWindowProc(WM_ERASEBKGND, (WPARAM)defDC.m_hDC , 0);
	DefWindowProc(WM_PAINT, (WPARAM)defDC.m_hDC , 0);

	CRTDraw::RTDrawBitmap(&memDC,rcArea,m_BackBitmap[BMP_BACK],m_BackBitmapDrawMode[BMP_BACK]);
	memDC.TransparentBlt(rcArea.left,rcArea.top,rcArea.Width(),rcArea.Height(),&defDC,rcArea.left,rcArea.top,rcArea.Width(),rcArea.Height(),GetSysColor(COLOR_WINDOW));
	CPaintDC dc(this);
	defDC.SelectObject(defOld);
}

BOOL CRTListCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(!m_bIsEnableSkin)
		return CListCtrl::OnEraseBkgnd(pDC);

	DWORD style = GetStyle();
	if((style & LVS_NOCOLUMNHEADER) != LVS_NOCOLUMNHEADER && (style & LVS_REPORT) == LVS_REPORT)
	{
		if(IsWindow(m_HeaderCtrl.m_hWnd))
		{
			m_HeaderCtrl.RedrawWindow();
		}
	}
	return TRUE;
}

void CRTListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		int pos = GetScrollPos(SBS_VERT);
		int newpos = nPos - pos;
		CRect rtItem;
		GetItemRect(0,&rtItem,LVIR_BOUNDS);
		CSize size(0,newpos * rtItem.Height());
		Scroll(size);
	}
	else
	{
		CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CRTListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if((nSBCode & SB_THUMBTRACK) == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		int pos = GetScrollPos(SBS_HORZ);
		int newpos = nPos - pos;
		CSize size(newpos,0);
		Scroll(size);
	}
	else
	{
		CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

#if 0
void CRTListCtrl::MoveWindow(LPCRECT rect)
{
	m_Container.MoveWindow(rect);
}
#endif


BOOL CRTListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CListCtrl::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}



HBRUSH CRTListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
