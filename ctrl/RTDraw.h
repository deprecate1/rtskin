///////////////////////////////////////////////////////////////////////////////
//
// Draw.h : header file
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
typedef DWORD HLSCOLOR;
#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

#define BMP_BACK     0
#define BMP_NORMAL   1
#define BMP_FOCUS    2
#define BMP_DOWN     3
#define BMP_DISABLE  4
#define BMP_ACTIVE   1
#define BMP_INACTIVE 2
#define BMP_SLIDER   3
#define BMP_CHECK    1
#define BMP_UNCHECK  2

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb);
COLORREF HLS2RGB (HLSCOLOR hls);

///////////////////////////////////////////////////////////////////////////////
// Performs some modifications on the specified color : luminance and saturation
COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S);

///////////////////////////////////////////////////////////////////////////////
HBITMAP WINAPI GetScreenBitmap (LPCRECT pRect);

class CMemDC : public CDC 
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.right, rcBounds.bottom);
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;
	}
	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) 
			delete m_bitmap;
	}
	CMemDC* operator->() 
	{
		return this;
	}
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBufferDC : public CDC
{
    HDC     m_hDestDC;
    CBitmap m_bitmap;     // Bitmap in Memory DC
    CRect   m_rect;
    HGDIOBJ m_hOldBitmap; // Previous Bitmap

public:
    CBufferDC (HDC hDestDC, const CRect& rcPaint = CRect(0,0,0,0));
   ~CBufferDC ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CPenDC
{
protected:
    CPen m_pen;
    HDC m_hDC;
    HPEN m_hOldPen;

public:
    CPenDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CPenDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBrushDC
{
protected:
    CBrush m_brush;
    HDC m_hDC;
    HBRUSH m_hOldBrush;

public:
    CBrushDC (HDC hDC, COLORREF crColor = CLR_NONE);
   ~CBrushDC ();

    void Color (COLORREF crColor);
    COLORREF Color () const;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CFontDC
{
protected:
    HFONT m_hFont;
    HDC m_hDC;
    HFONT m_hDefFont;
    COLORREF m_crTextOld;

public:
    CFontDC (HDC hDC, LPCTSTR sFaceName, COLORREF crText = CLR_DEFAULT);
    CFontDC (HDC hDC, BYTE nStockFont, COLORREF crText = CLR_DEFAULT);
    CFontDC (HDC hDC, HFONT hFont, COLORREF crText = CLR_DEFAULT);
   ~CFontDC ();

    const CFontDC& operator = (LPCTSTR sFaceName);
    const CFontDC& operator = (BYTE nStockFont);
    const CFontDC& operator = (HFONT hFont);
    const CFontDC& operator = (COLORREF crText);
    operator LPCTSTR ();
    operator COLORREF ();
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CBoldDC
{
protected:
    CFont m_fontBold;
    HDC m_hDC;
    HFONT m_hDefFont;

public:
    CBoldDC (HDC hDC, bool bBold);
   ~CBoldDC ();
};

class CRTDraw
{
public:
	typedef enum _tagDrawMode
	{
		DrawModeFill = 0,
		DrawModeWidthStretch = 1,
		DrawModeHeightStretch = 2,
		DrawModeAllStretch = 3,
		DrawMode3D = 4,
		DrawModeState = 5,
		DrawModeHeightCenter3D = 6,
		DrawModeHeightStretch3D = 7
	}DrawMode;

	static BOOL RTDrawBitmap(CDC* pDC, LPCRECT lpRect, CBitmap* pBitmap,UINT Mode = DrawModeHeightStretch);
	static BOOL RTDrawVerticalText(CDC* pDC,LPCRECT lpRect,LPCTSTR lpText,int length,UINT format,LOGFONT *lf);
	static HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);
};

