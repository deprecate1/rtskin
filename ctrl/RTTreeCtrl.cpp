// RTTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "RTTreeCtrl.h"
#include "RTDraw.h"

// CRTTreeCtrl

IMPLEMENT_DYNAMIC(CRTTreeCtrl, CTreeCtrl)
CRTTreeCtrl::CRTTreeCtrl()
{
}

CRTTreeCtrl::~CRTTreeCtrl()
{
}

CBitmap* CRTTreeCtrl::m_BackBitmap[4] = {NULL,NULL,NULL,NULL};
UINT     CRTTreeCtrl::m_BackBitmapDrawMode[4] = {0,0,0,0};
BOOL     CRTTreeCtrl::m_IsEnableSkin = TRUE;


BEGIN_MESSAGE_MAP(CRTTreeCtrl, CTreeCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
END_MESSAGE_MAP()



// CRTTreeCtrl 消息处理程序
void CRTTreeCtrl::SetBackBitmap(CBitmap *pBack[],UINT DrawMode[])
{
	for(int i = 0; i < 4; i++)
	{
		m_BackBitmap[i] = pBack[i];
		m_BackBitmapDrawMode[i] = DrawMode[i];
	}
}

void CRTTreeCtrl::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}

BOOL CRTTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(!m_IsEnableSkin)
		return CTreeCtrl::OnEraseBkgnd(pDC);
	return TRUE;
}

void CRTTreeCtrl::OnPaint()
{
	if(!m_IsEnableSkin)return CTreeCtrl::OnPaint();
  

	CRect rcWnd;
	GetClientRect(&rcWnd);
	//XTRACE("CRTTreeCtrl 重画。坐标：%d, %d；宽：%d，高：%d\n", rcWnd.left, rcWnd.top, 
	//	rcWnd.Width(),rcWnd.Height());
	CClientDC ptDC(this);

	CDC defDC;
	CMemDC memDC(&ptDC,rcWnd);

	defDC.CreateCompatibleDC(&ptDC);

	CBitmap  defBmp;
	CBitmap* defOld;
	

	defBmp.CreateCompatibleBitmap(&ptDC,rcWnd.Width(),rcWnd.Height());
	defOld = defDC.SelectObject(&defBmp);
	//rcWnd.SetRect(0, 0, 100, 100);
	
	DefWindowProc(WM_ERASEBKGND, (WPARAM)defDC.m_hDC , 0);
	DefWindowProc(WM_PAINT, (WPARAM)defDC.m_hDC , 0);

	CRTDraw::RTDrawBitmap(&memDC,&rcWnd,m_BackBitmap[BMP_BACK],m_BackBitmapDrawMode[BMP_BACK]);

	memDC.TransparentBlt(rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),&defDC,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),GetSysColor(COLOR_WINDOW));
	CPaintDC dc(this);
	defDC.SelectObject(defOld);
}


CScrollBar* CRTTreeCtrl::GetScrollBarCtrl(int nBar) const
{
	// TODO: 在此添加专用代码和/或调用基类
	return NULL;
	//return CTreeCtrl::GetScrollBarCtrl(nBar);
}

void CRTTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
}

int CRTTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CRTTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	SetRedraw(FALSE);
	*pResult = 0;
}

void CRTTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	SetRedraw(TRUE);
	*pResult = 0;
}

LRESULT CRTTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

// 支持递归，从根目录查找
HTREEITEM CRTTreeCtrl::FindItem(enum TREECTRL_FINDTYPE findtype, const void *data)
{
	return FindItem(findtype, GetRootItem(), data);
}

// 如果是TREECTRL_FINDTYPE_BYTEXT，data就是char *类型，如果是TREECTRL_FINDTYPE_BYDATA，data就是int *类型。
HTREEITEM CRTTreeCtrl::FindItem(enum TREECTRL_FINDTYPE findtype, HTREEITEM item, const void *data)
{
	CString strText;
	int value;

	if (NULL == item)
		return NULL;
	item = GetChildItem(item);
	while (NULL != item) {
#if 0
		// 如果有子节点，就递归
		if (ItemHasChildren(item)) {
			HTREEITEM temp = FindItem(findtype, GetChildItem(item), data);	// 递归
			if(temp)
				return temp;						// 找到了
		}
#endif

		// 看看本节点是不是
		strText = GetItemText(item);
		value = (int)GetItemData(item);
		if (TREECTRL_FINDTYPE_BYTEXT == findtype) {
			if(value > 0 && 0 == strcmp((char *)data, (LPCTSTR)strText))
				return item;						// 找到了
		} else if(TREECTRL_FINDTYPE_BYDATA == findtype) {
			if(value > 0 && value == *(int *)data)
				return item;						// 找到了
		}

		// 下一位？
		item = GetNextItem(item, TVGN_NEXT);
	}

	return NULL;
}
int CRTTreeCtrl::FindItemCount(const char *pText)
{
	return FindItemCount(GetRootItem(), pText);
}

int CRTTreeCtrl::FindItemCount(HTREEITEM item, const char *pText)
{
	CString strText;
	int value;
	int cnt = 0;

	while (NULL != item) {
		// 如果有子节点，就递归
		if (ItemHasChildren(item)) {
			cnt += FindItemCount(GetChildItem(item), pText);	// 递归
		}

		// 看看本节点是不是
		strText = GetItemText(item);
		value = (int)GetItemData(item);
		if(value > 0 && 0 == strcmp(pText, (LPCTSTR)strText))
			cnt++;							// 找到了

		// 下一位？
		item = GetNextItem(item, TVGN_NEXT);
	}

	return cnt;
}
