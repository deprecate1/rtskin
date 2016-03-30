// RTButton.cpp : 实现文件
//

#include "stdafx.h"
#include "RTButton.h"
#include "RTDraw.h"

// CRTButton

IMPLEMENT_DYNAMIC(CRTButton, CButton)
CRTButton::CRTButton()
{
	m_ButtonState = StateNormal;
	m_MouseState = MouseStateNormal;
}

CRTButton::~CRTButton()
{
}

CBitmap* CRTButton::m_ButtonBitmap[5] = {NULL,NULL,NULL,NULL,NULL};
UINT     CRTButton::m_ButtonDrawMode[5] = {0,0,0,0,0};
BOOL     CRTButton::m_IsEnableSkin = TRUE;

BEGIN_MESSAGE_MAP(CRTButton, CButton)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSEMOVE,OnRTMouseMove)
	ON_MESSAGE(WM_LBUTTONDOWN,OnRTLButtonDown)
	ON_MESSAGE(WM_LBUTTONUP,OnRTLButtonUp)
	ON_MESSAGE(WM_LBUTTONDBLCLK,OnRTLButtonDBClick)
END_MESSAGE_MAP()



// CRTButton 消息处理程序






void CRTButton::EnableSkin(BOOL IsEnable)
{
	m_IsEnableSkin = IsEnable;
}

void CRTButton::OnPaint()
{
	if(!m_IsEnableSkin)CButton::OnPaint();
	DWORD style = GetStyle();
	if((style & BS_PUSHBUTTON) != BS_PUSHBUTTON)
		return CButton::OnPaint();
	
	CPaintDC dc(this);
	RTDrawButton(&dc);
}

LRESULT CRTButton::OnRTLButtonDown(WPARAM wParam,LPARAM lParam)
{
	if(!m_IsEnableSkin)return DefWindowProc(WM_LBUTTONDOWN,wParam,lParam);

	m_ButtonState = StateDown;
	m_MouseState = MouseStateDown;
	CClientDC dc(this);
	RTDrawButton(&dc);
	return 0;
}

LRESULT CRTButton::OnRTLButtonUp(WPARAM wParam,LPARAM lParam)
{
	if(!m_IsEnableSkin)return DefWindowProc(WM_LBUTTONUP,wParam,lParam);
	ReleaseCapture();
	m_ButtonState = StateNormal;
	m_MouseState = MouseStateNormal;
	CClientDC dc(this);
	RTDrawButton(&dc);
	CRect rtButton;
	GetWindowRect(&rtButton);
	rtButton.SetRect(0,0,rtButton.Width(),rtButton.Height());
	CPoint point(lParam);
	if(rtButton.PtInRect(point))
	{
		CWnd *Parent = GetParent();
		if(Parent != NULL)Parent->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);
	}
	return 0;
}
LRESULT CRTButton::OnRTMouseMove(WPARAM wParam,LPARAM lParam)
{
	if(!m_IsEnableSkin)return DefWindowProc(WM_MOUSEMOVE,wParam,lParam);

	CRect rtButton;
	GetWindowRect(&rtButton);
	rtButton.SetRect(0,0,rtButton.Width(),rtButton.Height());
	CPoint point(lParam);

	if(wParam == MK_LBUTTON)
	{
		if(rtButton.PtInRect(point))
		{
			if(m_ButtonState != StateDown)
			{
				m_ButtonState = StateDown;
				CClientDC dc(this);
				RTDrawButton(&dc);
			}
		}
		else 
		{
			if(m_ButtonState == StateDown)
			{
				m_ButtonState = StateNormal;
				CClientDC dc(this);
				RTDrawButton(&dc);
			}
		}
	}
	else
	{
		if(rtButton.PtInRect(point))
		{
			if(m_ButtonState != StateFocus)
			{
				SetCapture();
				m_ButtonState = StateFocus;
				CClientDC dc(this);
				RTDrawButton(&dc);
			}
		}
		else 
		{
			if(m_ButtonState == StateFocus)
			{
				m_ButtonState = StateNormal;
				CClientDC dc(this);
				RTDrawButton(&dc);
				ReleaseCapture();
			}
		}
	}
	return 0;
}
LRESULT CRTButton::OnRTLButtonDBClick(WPARAM wParam,LPARAM lParam)
{
	if(!m_IsEnableSkin)return DefWindowProc(WM_MBUTTONDBLCLK,wParam,lParam);
	return 0;
}

void CRTButton::RTDrawButton(CDC *pDC)
{
	CRect rtText;
	CRect rtWnd;
	GetClientRect(&rtText);
	
	CString ItemText;
	GetWindowText(ItemText);

	int OldBack = pDC->SetBkMode(TRANSPARENT);
	COLORREF OldText = pDC->SetTextColor(0xFFFF);
	CFont* pFont = GetFont();
	CFont* pOldFont = pDC->SelectObject(pFont);

	DWORD style = GetStyle();//GetButtonStyle();

	UINT uFormat = 0;

	if((style & BS_MULTILINE) != BS_MULTILINE)
		uFormat |= DT_SINGLELINE;

	if((style & BS_TOP) == BS_TOP)
		uFormat |= DT_TOP;
	else if((style & BS_BOTTOM) == BS_BOTTOM)
		uFormat |= DT_BOTTOM;
	else
		uFormat |= DT_VCENTER | DT_SINGLELINE;

	if((style & BS_LEFT) == BS_LEFT)
		uFormat |= DT_LEFT;
	else if((style & BS_RIGHT) == BS_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_CENTER;

	uFormat |= DT_END_ELLIPSIS;

	if(style & WS_DISABLED)
	{
		m_ButtonState = StateDisable;
		
		if(!CRTDraw::RTDrawBitmap(pDC,&rtText,m_ButtonBitmap[BMP_DISABLE],m_ButtonDrawMode[BMP_DISABLE]))
		{
			
			pDC->DrawEdge(&rtText,BDR_RAISEDINNER,BF_RECT);
		}
		COLORREF ColorWhite = GetSysColor(COLOR_BTNTEXT);
		COLORREF ColorGray = GetSysColor(COLOR_BTNSHADOW);
		pDC->SetTextColor(ColorWhite);
		pDC->DrawText(ItemText,&rtText,uFormat);
		rtText.top += 1;
		rtText.left += 1;
		pDC->SetTextColor(ColorGray);
		pDC->DrawText(ItemText,&rtText,uFormat);
		return;
	}
	
	if(m_ButtonState == StateNormal)
	{
		if(!CRTDraw::RTDrawBitmap(pDC,&rtText,m_ButtonBitmap[BMP_NORMAL],m_ButtonDrawMode[BMP_NORMAL]))
		{
			pDC->DrawEdge(&rtText,BDR_RAISEDINNER,BF_RECT);
		}
	}
	else if(m_ButtonState == StateFocus)
	{
		if(!CRTDraw::RTDrawBitmap(pDC,&rtText,m_ButtonBitmap[BMP_FOCUS],m_ButtonDrawMode[BMP_FOCUS]))
		{
			pDC->DrawEdge(&rtText,BDR_RAISEDINNER,BF_RECT);
		}
	}
	else
	{
		if(!CRTDraw::RTDrawBitmap(pDC,&rtText,m_ButtonBitmap[BMP_DOWN],m_ButtonDrawMode[BMP_DOWN]))
		{
			pDC->DrawEdge(&rtText,BDR_SUNKENOUTER,BF_RECT);
		}
	}
	COLORREF ColorWhite = GetSysColor(COLOR_BTNTEXT);
	pDC->SetTextColor(ColorWhite);
	pDC->DrawText(ItemText,&rtText,uFormat);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBack);
	pDC->SetTextColor(OldText);
}

void CRTButton::SetButtonBitmap(CBitmap* ButtonBitmap[],UINT DrawMode[])
{
	for(int i = 0; i < 5; i ++)
	{
		m_ButtonBitmap[i] = ButtonBitmap[i];
		m_ButtonDrawMode[i] = DrawMode[i];
	}
}

LRESULT CRTButton::OnRTMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if(!m_IsEnableSkin)return DefWindowProc(WM_MOUSELEAVE,wParam,lParam);
	return 0;
}
LRESULT CRTButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CButton::WindowProc(message, wParam, lParam);
}
