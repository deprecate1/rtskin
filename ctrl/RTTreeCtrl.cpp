// RTTreeCtrl.cpp : ʵ���ļ�
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



// CRTTreeCtrl ��Ϣ�������
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
	//XTRACE("CRTTreeCtrl �ػ������꣺%d, %d����%d���ߣ�%d\n", rcWnd.left, rcWnd.top, 
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
	// TODO: �ڴ����ר�ô����/����û���
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

// ֧�ֵݹ飬�Ӹ�Ŀ¼����
HTREEITEM CRTTreeCtrl::FindItem(enum TREECTRL_FINDTYPE findtype, const void *data)
{
	return FindItem(findtype, GetRootItem(), data);
}

// �����TREECTRL_FINDTYPE_BYTEXT��data����char *���ͣ������TREECTRL_FINDTYPE_BYDATA��data����int *���͡�
HTREEITEM CRTTreeCtrl::FindItem(enum TREECTRL_FINDTYPE findtype, HTREEITEM item, const void *data)
{
	CString strText;
	int value;

	if (NULL == item)
		return NULL;
	item = GetChildItem(item);
	while (NULL != item) {
#if 0
		// ������ӽڵ㣬�͵ݹ�
		if (ItemHasChildren(item)) {
			HTREEITEM temp = FindItem(findtype, GetChildItem(item), data);	// �ݹ�
			if(temp)
				return temp;						// �ҵ���
		}
#endif

		// �������ڵ��ǲ���
		strText = GetItemText(item);
		value = (int)GetItemData(item);
		if (TREECTRL_FINDTYPE_BYTEXT == findtype) {
			if(value > 0 && 0 == strcmp((char *)data, (LPCTSTR)strText))
				return item;						// �ҵ���
		} else if(TREECTRL_FINDTYPE_BYDATA == findtype) {
			if(value > 0 && value == *(int *)data)
				return item;						// �ҵ���
		}

		// ��һλ��
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
		// ������ӽڵ㣬�͵ݹ�
		if (ItemHasChildren(item)) {
			cnt += FindItemCount(GetChildItem(item), pText);	// �ݹ�
		}

		// �������ڵ��ǲ���
		strText = GetItemText(item);
		value = (int)GetItemData(item);
		if(value > 0 && 0 == strcmp(pText, (LPCTSTR)strText))
			cnt++;							// �ҵ���

		// ��һλ��
		item = GetNextItem(item, TVGN_NEXT);
	}

	return cnt;
}
