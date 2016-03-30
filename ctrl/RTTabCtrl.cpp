// RTTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "RTTabCtrl.h"
#include "RTDraw.h"

// CRTTabCtrl

IMPLEMENT_DYNAMIC(CRTTabCtrl, CTabCtrl)
CRTTabCtrl::CRTTabCtrl()
{
	m_CurSel = -1;//no sel page
	m_IsShowTabItem = TRUE;
}

CRTTabCtrl::~CRTTabCtrl()
{
}

BOOL CRTTabCtrl::m_IsEnableSkin = TRUE;


CBitmap* CRTTabCtrl::m_TabBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTTabCtrl::m_TabBitmapDrawMode[5] = {0,0,0,0,0};
BEGIN_MESSAGE_MAP(CRTTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CRTTabCtrl 消息处理程序


BOOL CRTTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	
	CRect rt;
	CTabCtrl::GetClientRect(&rt);
	
	TCITEM tm;
	tm.mask = TCIF_PARAM;
	if(GetItem(m_CurSel,&tm) && tm.lParam != NULL)
	{
		CRect rtArea;
		GetClientRect(&rtArea);
		pDC->ExcludeClipRect(&rtArea);
	}
	if(m_TabBitmap[BMP_BACK] == NULL || !m_IsEnableSkin)
	{
		if(!CTabCtrl::OnEraseBkgnd(pDC))
			return FALSE;
	}
	else
	{
		CRTDraw::RTDrawBitmap(pDC,&rt,m_TabBitmap[BMP_BACK],m_TabBitmapDrawMode[BMP_BACK]);
	}

	
	int ItemCount = GetItemCount();
	CRect rtClient;
	if(ItemCount <= 0 || !m_IsShowTabItem)
	{
		rtClient.SetRect(rt.left + 2,rt.top + 2,rt.right - 2,rt.bottom - 2);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_RECT);
		return TRUE;
	}
	
	DWORD dwStyle = GetStyle();
	if(dwStyle & TCS_BOTTOM || dwStyle & TCS_RIGHT)
	{
        CRect rcItem;
		GetItemRect(ItemCount - 1,&rcItem);
		if(rcItem.right < rt.right)
		{
			rcItem.left = rcItem.right;
			rcItem.right = rt.right - 2;
			pDC->DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_TOP);
		}
		rtClient.SetRect(rt.left + 2,rt.top + 2,rt.right - 2,rcItem.top);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_TOP);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_RIGHT);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_LEFT);	
	}
	else if((dwStyle & TCS_VERTICAL) && (dwStyle & TCS_MULTILINE))
	{
        CRect rcItem;
		GetItemRect(ItemCount - 1,&rcItem);
		if(rcItem.bottom < rt.bottom)
		{
			rcItem.top = rcItem.bottom;
			rcItem.bottom = rt.bottom - 2;
			pDC->DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_RIGHT);
		}
		rtClient.SetRect(rcItem.right,rt.top + 2,rt.right - 2,rt.bottom - 2);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_TOP);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_RIGHT);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_BOTTOM);	
	}
	else
	{
		CRect rcItem;
		GetItemRect(ItemCount - 1,&rcItem);
		if(rcItem.right < rt.right)
		{
			rcItem.left = rcItem.right;
			rcItem.right = rt.right - 2;
			pDC->DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_BOTTOM);
		}
		rtClient.SetRect(rt.left + 2,rcItem.bottom,rt.right - 2,rt.bottom - 2);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_LEFT);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_RIGHT);
		pDC->DrawEdge(&rtClient,BDR_RAISEDINNER,BF_BOTTOM);
	}
	return TRUE;
}

void CRTTabCtrl::GetClientRect(LPRECT lpRect)
{
	CRect rtWnd;
	CTabCtrl::GetClientRect(&rtWnd);
	int ItemCount = GetItemCount();
	if(ItemCount <= 0 || !m_IsShowTabItem)
	{
		lpRect->left = rtWnd.left + 5;
		lpRect->right = rtWnd.right - 5;
		lpRect->bottom = rtWnd.bottom - 5;
		lpRect->top = rtWnd.top + 5;
	}
	else
	{
		DWORD dwStyle = GetStyle();
		CRect rtItem;
		GetItemRect(0,&rtItem);
		if(dwStyle & TCS_BOTTOM || dwStyle & TCS_RIGHT)
		{
			lpRect->left = rtWnd.left + 5;
			lpRect->right = rtWnd.right - 5;
			lpRect->bottom = rtWnd.bottom - rtItem.Height() - 4;
			lpRect->top = rtWnd.top + 5;
		}
		else if((dwStyle & TCS_VERTICAL) && (dwStyle & TCS_MULTILINE))
		{
			lpRect->left = rtWnd.left + rtItem.Width() + 4;
			lpRect->right = rtWnd.right - 5;
			lpRect->bottom = rtWnd.bottom - 5;
			lpRect->top = rtWnd.top + 5;
		}
		else
		{
			lpRect->left = rtWnd.left + 5;
			lpRect->right = rtWnd.right - 5;
			lpRect->bottom = rtWnd.bottom - 5;
			lpRect->top = rtWnd.top + rtItem.Height() + 4;
		}
	}
}

void CRTTabCtrl::OnPaint()
{	
	CPaintDC dc(this); 

	if(m_CurSel != GetCurSel())
	{
		OnTcnSelchanging(m_CurSel);
		OnTcnSelchange();
		m_CurSel = GetCurSel();
	}

	if(!m_IsShowTabItem)return;
	int ItemCount = GetItemCount();
	if(ItemCount <= 0)return;
	CRect rt;
	CTabCtrl::GetClientRect(rt);
	CRgn rgn;
	rgn.CreateRectRgn(2,2,rt.right - 2,rt.bottom - 2);
	dc.SelectClipRgn(&rgn);

	CRect rcItem;
	DWORD dwStyle = GetStyle();
	int OldModule = dc.SetBkMode(TRANSPARENT);
	CFont *pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);

	for(int i = 0; i < ItemCount; i ++)
	{
		char ItemText[1025] = "";
		TCITEM tim;
		tim.mask = TCIF_IMAGE | TCIF_STATE | TCIF_TEXT;
		tim.pszText = ItemText;
		tim.cchTextMax = 1024;
		if(!GetItem(i,&tim))continue;
		GetItemRect(i,&rcItem);

		DWORD ItemState = 0;
		if(i == GetCurSel())ItemState = TCIS_BUTTONPRESSED;
		
		if(dwStyle & TCS_BOTTOM || dwStyle & TCS_RIGHT)
		{	
			if(ItemState & TCIS_BUTTONPRESSED)
			{
				
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_BOTTOM);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_LEFT);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_RIGHT);
				
			}
			else
			{
				dc.DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_TOP);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_BOTTOM);
				CRect rtTemp;
				rtTemp.SetRect(rcItem.left,rcItem.top + 1,rcItem.right,rcItem.bottom);
				if(i != GetCurSel() - 1)
				{
					dc.DrawEdge(&rtTemp,BDR_RAISEDINNER,BF_RIGHT);
				}
				dc.DrawEdge(&rtTemp,BDR_RAISEDINNER,BF_LEFT);
				
			}
			CRect rcText;
			TEXTMETRIC Metrics;
			dc.GetTextMetrics(&Metrics);
			CImageList *ImgList = GetImageList();
			if(ImgList != NULL)
			{
				CRect rtIcon;
				rtIcon.top = rcItem.top + 1;
				rtIcon.bottom = rcItem.bottom - 1;
				rtIcon.left = rcItem.left + 1;
				rtIcon.right = rtIcon.left + rtIcon.Height() - 2;
				HICON hIcon = ImgList->ExtractIcon(tim.iImage);
				if(hIcon != NULL)
				{
					DrawIconEx(dc.m_hDC,rtIcon.left,rtIcon.top,hIcon, rtIcon.Width(),rtIcon.Height(),0,NULL,DI_NORMAL);
					DeleteObject(hIcon);
				}
				rcText.SetRect(rcItem.left + rcItem.Height(),rcItem.top + (rcItem.Height() - Metrics.tmHeight)/2,rcItem.right,rcItem.bottom);
				dc.DrawText(ItemText,(int)strlen(ItemText),&rcText,DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			else
			{
				rcText.SetRect(rcItem.left,rcItem.top + (rcItem.Height() - Metrics.tmHeight)/2,rcItem.right,rcItem.bottom);
				dc.DrawText(ItemText,(int)strlen(ItemText),&rcText,DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
		}
		else if((dwStyle & TCS_VERTICAL) && (dwStyle & TCS_MULTILINE))
		{
			if(ItemState & TCIS_BUTTONPRESSED)
			{
				
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_TOP);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_LEFT);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_BOTTOM);
				
			}
			else
			{
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_LEFT);
				dc.DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_RIGHT);
				CRect rtTemp;
				rtTemp.SetRect(rcItem.left + 1,rcItem.top,rcItem.right - 1,rcItem.bottom);
				dc.DrawEdge(&rtTemp,BDR_RAISEDINNER,BF_TOP);
				if(i != GetCurSel() - 1)
				{
					dc.DrawEdge(&rtTemp,BDR_RAISEDOUTER,BF_BOTTOM);
				}

			}
			CRect rcText;
			CImageList *ImgList = GetImageList();
			if(ImgList != NULL)
			{
				CRect rtIcon;
				rtIcon.left = rcItem.left + 1;
				rtIcon.right = rcItem.right - 1;
				rtIcon.top = rcItem.top + 1;
				rtIcon.bottom = rtIcon.top + rcItem.Width() - 2;
				
				HICON hIcon = ImgList->ExtractIcon(tim.iImage);
				if(hIcon != NULL)
				{
					DrawIconEx(dc.m_hDC,rtIcon.left,rtIcon.top,hIcon, rtIcon.Width(),rtIcon.Height(),0,NULL,DI_NORMAL);
					DeleteObject(hIcon);
				}
				rcText.SetRect(rcItem.left,rcItem.top + rcItem.Width(),rcItem.right,rcItem.bottom);
				LOGFONT lf ;
				pFont->GetLogFont(&lf);
				CRTDraw::RTDrawVerticalText(&dc,&rcText,ItemText,(int)strlen(ItemText), DT_LEFT | DT_SINGLELINE,&lf);
			}
			else
			{
				rcText.SetRect(rcItem.left,rcItem.top + 4,rcItem.right,rcItem.bottom);
				LOGFONT lf ;
				pFont->GetLogFont(&lf);
				CRTDraw::RTDrawVerticalText(&dc,&rcText,ItemText,(int)strlen(ItemText), DT_LEFT | DT_SINGLELINE,&lf);
			}
		}
		else
		{
			if(ItemState & TCIS_BUTTONPRESSED)
			{
				
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_TOP);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_LEFT);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_RIGHT);
				
			}
			else
			{
				dc.DrawEdge(&rcItem,BDR_SUNKENOUTER,BF_BOTTOM);
				dc.DrawEdge(&rcItem,BDR_RAISEDINNER,BF_TOP);
				CRect rtTemp;
				rtTemp.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom - 1);
				if(i != GetCurSel() - 1)
				{
					dc.DrawEdge(&rtTemp,BDR_RAISEDINNER,BF_RIGHT);
					
				}
				dc.DrawEdge(&rtTemp,BDR_RAISEDINNER,BF_LEFT);
			}
			CRect rcText;
			TEXTMETRIC Metrics;
			dc.GetTextMetrics(&Metrics);
			CImageList *ImgList = GetImageList();
			if(ImgList != NULL)
			{
				CRect rtIcon;
				rtIcon.top = rcItem.top + 1;
				rtIcon.bottom = rcItem.bottom - 1;
				rtIcon.left = rcItem.left + 1;
				rtIcon.right = rtIcon.left + rtIcon.Height() - 2;
				HICON hIcon = ImgList->ExtractIcon(tim.iImage);
				if(hIcon != NULL)
				{
					DrawIconEx(dc.m_hDC,rtIcon.left,rtIcon.top,hIcon, rtIcon.Width(),rtIcon.Height(),0,NULL,DI_NORMAL);
					DeleteObject(hIcon);
				}
				rcText.SetRect(rcItem.left + rcItem.Height(),rcItem.top + (rcItem.Height() - Metrics.tmHeight)/2,rcItem.right,rcItem.bottom);
				dc.DrawText(ItemText,(int)strlen(ItemText),&rcText,DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			else
			{
				rcText.SetRect(rcItem.left,rcItem.top + (rcItem.Height() - Metrics.tmHeight)/2,rcItem.right,rcItem.bottom);
				dc.DrawText(ItemText,(int)strlen(ItemText),&rcText,DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
		}
	}
	dc.SetBkMode(OldModule);
	dc.SelectObject(pOldFont);

}

void CRTTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);
	int ItemCount = GetItemCount();
	CRect rtClient;
	GetClientRect(&rtClient);
	for(int i = 0; i < ItemCount; i++)
	{
		TCITEM tm;
		ZeroMemory(&tm,sizeof(tm));
		tm.mask = TCIF_PARAM;
		if(GetItem(i,&tm))
		{
			CWnd* pWnd = (CWnd *)tm.lParam;
			if(pWnd != NULL && IsWindow(pWnd->m_hWnd))
			{
				pWnd->MoveWindow(&rtClient);
			}
		}
	}
}


void CRTTabCtrl::SetTabBitmap(CBitmap* TabBitmap[], UINT DrawMode[])
{
	for(int i = 0; i < 5; i++)
	{
		m_TabBitmap[i] = TabBitmap[i];
		m_TabBitmapDrawMode[i] = DrawMode[i];
	}
}

BOOL CRTTabCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CTabCtrl::PreCreateWindow(cs))
		return FALSE;
	cs.style &= ~TCS_MULTISELECT;
	return TRUE;

}

void CRTTabCtrl::PreSubclassWindow()
{
	CTabCtrl::PreSubclassWindow();
}

void CRTTabCtrl::ShowTabItem(BOOL IsVisable)
{
	m_IsShowTabItem = IsVisable;
}
LONG CRTTabCtrl::Add(int Position,LPCTSTR lpszItemText,int Image,CWnd* pItemWnd)
{
	TCITEM tm;
	ZeroMemory(&tm,sizeof(tm));
	tm.lParam = (LPARAM)pItemWnd;
	tm.pszText = (LPSTR)lpszItemText;
	tm.cchTextMax = (int)strlen(lpszItemText);
	tm.iImage = Image;
	tm.mask = TCIF_IMAGE|TCIF_PARAM|TCIF_TEXT;
	int pos = CTabCtrl::InsertItem(Position,&tm);
	if(pos == -1)return -1;

	if(pItemWnd != NULL && IsWindow(pItemWnd->m_hWnd))
	{
		pItemWnd->SetParent(this);
		if(pos == GetCurSel())
		{
			pItemWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pItemWnd->ShowWindow(SW_HIDE);
		}
	}
	return pos;
}
void CRTTabCtrl::OnTcnSelchange()
{
	int i = GetCurSel();
	if(i >= 0)
	{
		TCITEM tm;
		ZeroMemory(&tm,sizeof(tm));
		tm.mask = TCIF_PARAM;
		if(GetItem(i,&tm))
		{
			CWnd* pWnd = (CWnd *)tm.lParam;
			if(pWnd != NULL && IsWindow(pWnd->m_hWnd))
			{
				pWnd->ShowWindow(SW_SHOW);
			}
		}
	}
}

void CRTTabCtrl::OnTcnSelchanging(int oldSel)
{
	int i = oldSel;
	if(i >= 0)
	{
		TCITEM tm;
		ZeroMemory(&tm,sizeof(tm));
		tm.mask = TCIF_PARAM;
		if(GetItem(i,&tm))
		{
			CWnd* pWnd = (CWnd *)tm.lParam;
			if(pWnd != NULL && IsWindow(pWnd->m_hWnd))
			{
				pWnd->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CRTTabCtrl::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}
BOOL CRTTabCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	GetParent()->SendMessage(WM_NOTIFY,wParam,lParam);
	return CTabCtrl::OnNotify(wParam, lParam, pResult);
}
