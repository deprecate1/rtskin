///////////////////////////////////////////////////////////////////////////////
//
// Draw.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTDraw.h"
///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb)
{
    unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
    unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
   
    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 

    if ( maxval != minval )
    { 
        float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
        float gnorm = (maxval - GetGValue(rgb)) / mdiff;
        float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   

        saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

        if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
        if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
        if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
        if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
}

///////////////////////////////////////////////////////////////////////////////
static BYTE _ToRGB (float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;
 
  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
                   
  return (BYTE)(rm1 * 255);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF HLS2RGB (HLSCOLOR hls)
{
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;

    if ( saturation == 0.0f )
    {
        return RGB (HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;
     
    if ( luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);

    return RGB (red, green, blue);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S)
{
    HLSCOLOR hls = RGB2HLS (rgb);
    BYTE h = HLS_H(hls);
    BYTE l = HLS_L(hls);
    BYTE s = HLS_S(hls);

    if ( percent_L > 0 )
    {
        l = BYTE(l + ((255 - l) * percent_L) / 100);
    }
    else if ( percent_L < 0 )
    {
        l = BYTE((l * (100+percent_L)) / 100);
    }
    if ( percent_S > 0 )
    {
        s = BYTE(s + ((255 - s) * percent_S) / 100);
    }
    else if ( percent_S < 0 )
    {
        s = BYTE((s * (100+percent_S)) / 100);
    }
    return HLS2RGB (HLS(h, l, s));
}

/////////////////////////////////////////////////////////////////////////////
HBITMAP WINAPI GetScreenBitmap (LPCRECT pRect)
{
    HDC     hDC;
    HDC     hMemDC;
    HBITMAP hNewBitmap = NULL;

    if ( (hDC = ::GetDC (NULL)) != NULL )
    {
        if ( (hMemDC = ::CreateCompatibleDC (hDC)) != NULL )
        {
            if ( (hNewBitmap = ::CreateCompatibleBitmap (hDC, pRect->right - pRect->left, pRect->bottom - pRect->top)) != NULL )
            {
                HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hNewBitmap);
                ::BitBlt (hMemDC, 0, 0, pRect->right - pRect->left, pRect->bottom - pRect->top,
                          hDC, pRect->left, pRect->top, SRCCOPY);
                ::SelectObject (hMemDC, (HGDIOBJ) hOldBitmap);
            }
            ::DeleteDC (hMemDC);
        }
        ::ReleaseDC (NULL, hDC);
    }
    return hNewBitmap;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CBufferDC::CBufferDC (HDC hDestDC, const CRect& rcPaint) : m_hDestDC (hDestDC)
{
    if ( rcPaint.IsRectEmpty() )
    {
        ::GetClipBox (m_hDestDC, m_rect);
    }
    else
    {
        m_rect = rcPaint;
    }
    VERIFY(Attach (::CreateCompatibleDC (m_hDestDC)));
    m_bitmap.Attach (::CreateCompatibleBitmap (m_hDestDC, m_rect.right, m_rect.bottom));
    m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

    if ( m_rect.top > 0 )
    {
        ExcludeClipRect (0, 0, m_rect.right, m_rect.top);
    }
    if ( m_rect.left > 0 )
    {
        ExcludeClipRect (0, m_rect.top, m_rect.left, m_rect.bottom);
    }
}

///////////////////////////////////////////////////////////////////////////////
CBufferDC::~CBufferDC ()
{
    VERIFY(::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY));
    ::SelectObject (m_hDC, m_hOldBitmap);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CPenDC::CPenDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
{
    VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

///////////////////////////////////////////////////////////////////////////////
CPenDC::~CPenDC ()
{
    ::SelectObject (m_hDC, m_hOldPen);
}

///////////////////////////////////////////////////////////////////////////////
void CPenDC::Color (COLORREF crColor)
{
    ::SelectObject (m_hDC, m_hOldPen);
    VERIFY(m_pen.DeleteObject());
    VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CPenDC::Color () const
{
    LOGPEN logPen;

    ((CPenDC*)this)->m_pen.GetLogPen (&logPen);

    return logPen.lopnColor;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CBrushDC::CBrushDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
{
    if ( crColor == CLR_NONE ) VERIFY(m_brush.CreateStockObject (NULL_BRUSH));
    else                       VERIFY(m_brush.CreateSolidBrush (crColor));
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

///////////////////////////////////////////////////////////////////////////////
CBrushDC::~CBrushDC ()
{
    ::SelectObject (m_hDC, m_hOldBrush);
}

///////////////////////////////////////////////////////////////////////////////
void CBrushDC::Color (COLORREF crColor)
{
    ::SelectObject (m_hDC, m_hOldBrush);
    VERIFY(m_brush.DeleteObject());
    if ( crColor == CLR_NONE ) VERIFY(m_brush.CreateStockObject (NULL_BRUSH));
    else                       VERIFY(m_brush.CreateSolidBrush (crColor));
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CBrushDC::Color () const
{
    LOGBRUSH logBrush;

    ((CBrushDC*)this)->m_brush.GetLogBrush (&logBrush);

    return logBrush.lbColor;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, LPCTSTR sFaceName, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = sFaceName;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, BYTE nStockFont, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = nStockFont;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, HFONT hFont, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = hFont;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::~CFontDC ()
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    if ( m_crTextOld != CLR_NONE )
    {
        ::SetTextColor (m_hDC, m_crTextOld);
    }
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (LPCTSTR sFaceName)
{
    LOGFONT lf;

	::GetObject (::GetCurrentObject (m_hDC, OBJ_FONT), sizeof(LOGFONT), &lf);

    if ( _tcsicmp (sFaceName, lf.lfFaceName) )
    {
        if ( m_hDefFont != NULL )
        {
            ::SelectObject (m_hDC, m_hDefFont);
            DeleteObject (m_hFont);
        }
        _tcscpy (lf.lfFaceName, sFaceName);
        m_hFont = ::CreateFontIndirect (&lf);
        m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);
    }
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (BYTE nStockFont)
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    m_hFont = (HFONT)::GetStockObject (nStockFont);
    m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (HFONT hFont)
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    m_hFont = hFont;
    m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (COLORREF crText)
{
    if ( m_crTextOld == CLR_NONE )
    {
        m_crTextOld = ::GetTextColor (m_hDC);
    }
    ::SetTextColor (m_hDC, crText);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::operator LPCTSTR ()
{
    LOGFONT lf;

	::GetObject (::GetCurrentObject (m_hDC, OBJ_FONT), sizeof(LOGFONT), &lf);

    return lf.lfFaceName;
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::operator COLORREF ()
{
    return ::GetTextColor (m_hDC);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CBoldDC::CBoldDC (HDC hDC, bool bBold) : m_hDC (hDC), m_hDefFont (NULL)
{
    LOGFONT lf;

    CFont::FromHandle((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT))->GetLogFont (&lf);

    if ( ( bBold && lf.lfWeight != FW_BOLD) ||
         (!bBold && lf.lfWeight == FW_BOLD) )
    {
        lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

        m_fontBold.CreateFontIndirect (&lf);
        m_hDefFont = (HFONT)::SelectObject (m_hDC, m_fontBold);
    }
}

/////////////////////////////////////////////////////////////////////////////
CBoldDC::~CBoldDC ()
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
    }
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define DB_GETTYPE(Style)       ((Style)&0x00FF)
#define DB_ISENABLED(Style)     (!((Style)&DB_DISABLED))
#define DB_ISBORDER(Style)      (((Style)&DB_BORDER)==DB_BORDER)
#define DB_ISWINDOWDC(Style)    (((Style)&DB_WINDOWDC)==DB_WINDOWDC)
#define DB_ISFLAT(Style)        (((Style)&DB_FLAT)==DB_FLAT)
#define DB_ISPRESSED(Style)     (((Style)&(DB_PRESSED|DB_PRESSED2))!=0)
#define DB_ISPRESSED1(Style)    (((Style)&DB_PRESSED)==DB_PRESSED)
#define DB_ISPRESSED2(Style)    (((Style)&DB_PRESSED2)==DB_PRESSED2)
#define DB_ISOVER(Style)        (((Style)&DB_OVER)==DB_OVER)
#define DB_ISTRANSPARENT(Style) (((Style)&DB_TRANSPARENT)==DB_TRANSPARENT)

/////////////////////////////////////////////////////////////////////////////
static void _DrawTriangle (CDC* pDC, int x, int y, int nSize, bool bDown)
{
    for ( int i = 0; i < nSize; i++ )
    {
        pDC->MoveTo (x-i, bDown ? y-i : y+i);
        pDC->LineTo (x+i+1, bDown ? y-i : y+i);
    }
}
BOOL CRTDraw::RTDrawVerticalText(CDC* pDC,LPCRECT lpRect,LPCTSTR lpText,int length,UINT format,LOGFONT *lf)
{
	char *ptr = (char *)lpText;
	char *start = (char *)lpText;
	char *end = (char *)(lpText + length - 1);
	char temp = NULL;

	BOOL IsUnicode = FALSE;
	while(ptr <= end)
	{
		if(*ptr & 0x80)
		{
			IsUnicode = TRUE;
			break;
		}
	}
	ptr = (char *)lpText;
	CRect rtText;
	CSize sz;
	int top = lpRect->top;
	if(!IsUnicode)
	{
		lf->lfEscapement = 900;
		lf->lfOrientation = 900;
		CFont font;
		font.CreateFontIndirect(lf);
		pDC->SelectObject(&font);
		sz = pDC->GetTextExtent(start,(int)(end - start));
		rtText.left = lpRect->left + (lpRect->right - lpRect->left - sz.cy)/2;
		rtText.top = top;
		rtText.bottom = rtText.top + sz.cx;
		rtText.right = lpRect->right;
		pDC->TextOut(rtText.left,rtText.bottom,start,(int)(end - start));
		return TRUE;
	}

	lf->lfEscapement = 0;
	lf->lfOrientation = 0;
	CFont font;
	font.CreateFontIndirect(lf);
	pDC->SelectObject(&font);
	while(ptr <= end)
	{
		
		//UNICODE 
		if(*ptr & 0x80)
		{
			if(ptr + 1 > end)break;
			start = ptr;
			ptr += 2;
			sz = pDC->GetTextExtent(start,(int)(ptr - start));
			CRect rtText;
			rtText.SetRect(lpRect->left + (lpRect->right - lpRect->left - sz.cx)/2,
				top,lpRect->right,lpRect->bottom);
			pDC->TextOut(rtText.left,rtText.top, start,(int)(ptr - start));
			top += sz.cy;
		}
		else
		{
			start = ptr;
			ptr += 1;
			sz = pDC->GetTextExtent(start,(int)(ptr - start));
			CRect rtText;
			rtText.SetRect(lpRect->left + (lpRect->right - lpRect->left - sz.cx)/2,
				top,lpRect->right,lpRect->bottom);
			pDC->TextOut(rtText.left,rtText.top, start,(int)(ptr - start));
			top += sz.cy;
		}
	}
	return TRUE;
}

//BITMAP Width and height > 9
BOOL CRTDraw::RTDrawBitmap(CDC* pDC, LPCRECT lpRect, CBitmap* pBitmap,UINT Mode)
{
	if(pDC == NULL)return FALSE;
	if(pBitmap == NULL)return FALSE;

	BITMAP bitmap;
	pBitmap->GetBitmap(&bitmap);
	if(bitmap.bmWidth < 9)return FALSE;
	if(bitmap.bmHeight < 9)return FALSE;

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = srcDC.SelectObject(pBitmap);

	if(Mode == DrawModeState)
	{
		pDC->StretchBlt(lpRect->left,lpRect->top,bitmap.bmWidth,bitmap.bmHeight,
			&srcDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		return TRUE;
	}

	if((DrawMode)Mode == DrawModeAllStretch)
	{
		pDC->StretchBlt(lpRect->left,lpRect->top,lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top,&srcDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
		srcDC.SelectObject(pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}

	int height;
	int width;

	if((DrawMode)Mode == DrawModeFill)
	{
		int left = lpRect->left;
		int top = lpRect->top;
		while(left < lpRect->right)
		{
			if(lpRect->right - left <= bitmap.bmWidth)
			{
				width = lpRect->right - left;
				if(lpRect->bottom - top <= bitmap.bmHeight)
				{
					height = lpRect->bottom - top;
					pDC->BitBlt(left,top,width,height,&srcDC,0,0,SRCCOPY);
					break;
				}
				else
				{
					height = bitmap.bmHeight;
					pDC->BitBlt(left,top,width,height,&srcDC,0,0,SRCCOPY);
					top += height;
					left = lpRect->left;
				}
			}
			else
			{
				width = bitmap.bmWidth;
				if(lpRect->bottom - top <= bitmap.bmHeight)
				{
					height = lpRect->bottom - top;
					pDC->BitBlt(left,top,width,height,&srcDC,0,0,SRCCOPY);
				}
				else
				{
					height = bitmap.bmHeight;
					pDC->BitBlt(left,top,width,height,&srcDC,0,0,SRCCOPY);
				}
				left += width;
			}
		}
		srcDC.SelectObject(pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}

	if(Mode == DrawModeHeightStretch)
	{
		height = lpRect->bottom - lpRect->top;
		int top = lpRect->top;
		int left = lpRect->left;
		int right = lpRect->right;

		while((right - left) >= bitmap.bmWidth)
		{
			pDC->StretchBlt(left,top,bitmap.bmWidth,height,
				&srcDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			left += bitmap.bmWidth;
		}
		width = right - left;
		pDC->StretchBlt(left,top,width,height,
				&srcDC,0,0,width,bitmap.bmHeight,SRCCOPY);

		srcDC.SelectObject(&pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}

	if(Mode == DrawModeWidthStretch)
	{
		height = lpRect->bottom - lpRect->top;
		int top = lpRect->top;
		width = lpRect->right - lpRect->left;
		int left = lpRect->left;
		int bottom = lpRect->bottom;

		while((bottom - top) >= bitmap.bmHeight)
		{
			pDC->StretchBlt(left,top,width,bitmap.bmHeight,
				&srcDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
			top += bitmap.bmHeight;
		}
		height = bottom - top;
		pDC->StretchBlt(left,top,width,height,
			&srcDC,4,0,bitmap.bmWidth,height,SRCCOPY);

		srcDC.SelectObject(&pOldBitmap);
		srcDC.DeleteDC();
	}

	if(Mode == DrawMode3D)
	{
		int width = bitmap.bmWidth/3;
		int height = bitmap.bmHeight/3;

		int x = lpRect->left + width;
		int y = lpRect->top + height;

		//fill center
		while( x + width < lpRect->right)
		{
			while(y + height < lpRect->bottom)
			{
				pDC->BitBlt(x,y,width,height,&srcDC,width,height,SRCCOPY);
				y += height;
			}
			y = lpRect->top + height;
			x += width;
		}
		//draw top border
		x = lpRect->left + width;
		y = lpRect->top;
		pDC->BitBlt(lpRect->left,y,width,height,&srcDC,0,0,SRCCOPY);
		while( x + width < lpRect->right)
		{
			pDC->BitBlt(x,y,width,height,&srcDC,width,0,SRCCOPY);
			x += width;
		}
		pDC->BitBlt(lpRect->right - width,y,width,height,&srcDC,width + width,0,SRCCOPY);
		
		//draw left border
		x = lpRect->left;
		y = lpRect->top + height;
		while( y + height < lpRect->bottom)
		{
			pDC->BitBlt(x,y,width,height,&srcDC,0,height,SRCCOPY);
			y += height;
		}

		//draw right border
		x = lpRect->right - width;
		y = lpRect->top + height;
		while( y + height < lpRect->bottom)
		{
			pDC->BitBlt(x,y,width,height,&srcDC,width + width,height,SRCCOPY);
			y += height;
		}
			
		//draw bottom border
		x = lpRect->left + width;
		y = lpRect->bottom - height;
		pDC->BitBlt(lpRect->left,y,width,height,&srcDC,0,height + height,SRCCOPY);
		while( x + width < lpRect->right)
		{
			pDC->BitBlt(x,y,width,height,&srcDC,width,height + height,SRCCOPY);
			x += width;
		}
		pDC->BitBlt(lpRect->right - width,y,width,height,&srcDC,width + width,height + height,SRCCOPY);
	
		srcDC.SelectObject(&pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}
	if(Mode == DrawModeHeightStretch3D)
	{
		int width = bitmap.bmWidth / 3;
		int height = lpRect->bottom - lpRect->top;
		int x = lpRect->left;
		int y = lpRect->top;
		int right = lpRect->right;

		pDC->StretchBlt(x,y,width,height,&srcDC,0,0,width,bitmap.bmHeight,SRCCOPY);
		while((x + width) < right)
		{
			pDC->StretchBlt(x,y,width,height,&srcDC,width,0,width,bitmap.bmHeight,SRCCOPY);
			x += width;
		}
		pDC->StretchBlt(right - width,y,width,height,&srcDC,width + width,0,width,bitmap.bmHeight,SRCCOPY);

		srcDC.SelectObject(&pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}

	if(Mode == DrawModeHeightCenter3D)
	{
		int width = bitmap.bmWidth / 3;
		int height = lpRect->bottom - lpRect->top;
		int x = lpRect->left;
		int y = lpRect->top;
		int right = lpRect->right;
		while((x + width) < right)
		{
			pDC->StretchBlt(x,y,width,height,&srcDC,width,0,width,bitmap.bmHeight,SRCCOPY);
			x += width;
		}
		pDC->StretchBlt(right - width,y,width,height,&srcDC,width,0,width,bitmap.bmHeight,SRCCOPY);

		srcDC.SelectObject(&pOldBitmap);
		srcDC.DeleteDC();
		return TRUE;
	}

	srcDC.SelectObject(&pOldBitmap);
	srcDC.DeleteDC();
	return TRUE;
}

HRGN CRTDraw::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
    HRGN hRgn = NULL;

    if (hBmp)
    {
        // Create a memory DC inside which we will scan the bitmap content
        HDC hMemDC = CreateCompatibleDC(NULL);
        if (hMemDC)
        {
            // Get bitmap size
            BITMAP bm;
            GetObject(hBmp, sizeof(bm), &bm);

            // Create a 32 bits depth bitmap and select it into the memory DC 
            BITMAPINFOHEADER RGB32BITSBITMAPINFO = {    
                    sizeof(BITMAPINFOHEADER),    // biSize 
                    bm.bmWidth,                    // biWidth; 
                    bm.bmHeight,                // biHeight; 
                    1,                            // biPlanes; 
                    32,                            // biBitCount 
                    BI_RGB,                        // biCompression; 
                    0,                            // biSizeImage; 
                    0,                            // biXPelsPerMeter; 
                    0,                            // biYPelsPerMeter; 
                    0,                            // biClrUsed; 
                    0                            // biClrImportant; 
            };
            VOID * pbits32; 
            HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
            if (hbm32)
            {
                HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

                // Create a DC just to copy the bitmap into the memory DC
                HDC hDC = CreateCompatibleDC(hMemDC);
                if (hDC)
                {
                    // Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
                    BITMAP bm32;
                    GetObject(hbm32, sizeof(bm32), &bm32);
                    while (bm32.bmWidthBytes % 4)
                        bm32.bmWidthBytes++;

                    // Copy the bitmap into the memory DC
                    HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
                    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

                    // For better performances, we will use the ExtCreateRegion() function to create the
                    // region. This function take a RGNDATA structure on entry. We will add rectangles by
                    // amount of ALLOC_UNIT number in this structure.
                    #define ALLOC_UNIT    100
                    DWORD maxRects = ALLOC_UNIT;
                    HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
                    RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
                    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
                    pData->rdh.iType = RDH_RECTANGLES;
                    pData->rdh.nCount = pData->rdh.nRgnSize = 0;
                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

                    // Keep on hand highest and lowest values for the "transparent" pixels
                    BYTE lr = GetRValue(cTransparentColor);
                    BYTE lg = GetGValue(cTransparentColor);
                    BYTE lb = GetBValue(cTransparentColor);
                    BYTE hr = min(0xff, lr + GetRValue(cTolerance));
                    BYTE hg = min(0xff, lg + GetGValue(cTolerance));
                    BYTE hb = min(0xff, lb + GetBValue(cTolerance));

                    // Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
                    BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
                    for (int y = 0; y < bm.bmHeight; y++)
                    {
						TRACE("\n");
                        // Scan each bitmap pixel from left to right
                        for (int x = 0; x < bm.bmWidth; x++)
                        {
                            // Search for a continuous range of "non transparent pixels"
                            int x0 = x;
                            LONG *p = (LONG *)p32 + x;
                            while (x < bm.bmWidth)
                            {
								TRACE("1");
                                BYTE b = GetRValue(*p);
                                if (b >= lr && b <= hr)
                                {
                                    b = GetGValue(*p);
                                    if (b >= lg && b <= hg)
                                    {
                                        b = GetBValue(*p);
                                        if (b >= lb && b <= hb)
										{
											TRACE("0");
                                            // This pixel is "transparent"
											break;
										}
                                    }
                                }
                                p++;
                                x++;
                            }

                            if (x > x0)
                            {
                                // Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
                                if (pData->rdh.nCount >= maxRects)
                                {
                                    GlobalUnlock(hData);
                                    maxRects += ALLOC_UNIT;
                                    hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
                                    pData = (RGNDATA *)GlobalLock(hData);
                                }
                                RECT *pr = (RECT *)&pData->Buffer;
                                SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
                                if (x0 < pData->rdh.rcBound.left)
                                    pData->rdh.rcBound.left = x0;
                                if (y < pData->rdh.rcBound.top)
                                    pData->rdh.rcBound.top = y;
                                if (x > pData->rdh.rcBound.right)
                                    pData->rdh.rcBound.right = x;
                                if (y+1 > pData->rdh.rcBound.bottom)
                                    pData->rdh.rcBound.bottom = y+1;
                                pData->rdh.nCount++;

                                // On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
                                // large (ie: > 4000). Therefore, we have to create the region by multiple steps.
                                if (pData->rdh.nCount == 2000)
                                {
                                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
                                    if (hRgn)
                                    {
                                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                                        DeleteObject(h);
                                    }
                                    else
                                        hRgn = h;
                                    pData->rdh.nCount = 0;
                                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
                                }
                            }
                        }

                        // Go to next row (remember, the bitmap is inverted vertically)
                        p32 -= bm32.bmWidthBytes;
                    }

                    // Create or extend the region with the remaining rectangles
                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
                    if (hRgn)
                    {
                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                        DeleteObject(h);
                    }
                    else
                        hRgn = h;

                    // Clean up
                    GlobalFree(hData);
                    SelectObject(hDC, holdBmp);
                    DeleteDC(hDC);
                }

                DeleteObject(SelectObject(hMemDC, holdBmp));
            }

            DeleteDC(hMemDC);
        }    
    }

    return hRgn;
} 
