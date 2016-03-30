#pragma once


// CRTButton

class CRTButton : public CButton
{
	DECLARE_DYNAMIC(CRTButton)
public:
	CRTButton();
	virtual ~CRTButton();

	typedef enum _tagDrawState
	{
		StateNormal = 0,
		StateDown = 1,
		StateFocus = 2,
		StateDisable = 3
	}DrawState;

	typedef enum _tagMouseState
	{
		MouseStateNormal = 0,
		MouseStateDown =1,
		MouseStateUp = 2
	}MouseState;

protected:
	static CBitmap* m_ButtonBitmap[5];
	static UINT     m_ButtonDrawMode[5];
	static BOOL     m_IsEnableSkin;
	DrawState       m_ButtonState;
	MouseState      m_MouseState;
public:
	static void SetButtonBitmap(CBitmap* ButtonBitmap[],UINT DrawMode[]);
	static void EnableSkin(BOOL IsEnable = TRUE);
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnRTLButtonDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTLButtonUp(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTMouseMove(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTLButtonDBClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnRTMouseLeave(WPARAM wParam,LPARAM lParam);
	void    RTDrawButton(CDC *pDC);
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


