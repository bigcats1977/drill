#include "stdafx.h"
#include "CeXDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CCeXDib::CCeXDib()
{
    m_dwLineWidth = 0;
    m_wColors = 0;

    m_hDib = NULL;
    m_hMemDC = NULL;
    m_hBitmap = NULL;
    m_lpBits = NULL;

    FreeResources();
}

CCeXDib::~CCeXDib()
{
    FreeResources();
}

void CCeXDib::FreeResources()
{
    if (m_hMemDC)
        ::DeleteDC(m_hMemDC);
    DELETE_OBJECT(m_hBitmap);
    DELETE_POINT(m_hDib);

    m_hDib = NULL;
    m_hMemDC = NULL;
    m_hBitmap = NULL;
    m_lpBits = NULL;
    memset(&m_bi, 0, sizeof(m_bi));
} // End of FreeResources

HDIB CCeXDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
    LPBITMAPINFOHEADER  lpbi = NULL;   // Pointer to BITMAPINFOHEADER
    DWORD               dwLen = 0;      // Size of memory block

    FreeResources();

    // Following <switch> is taken from
    // CDIBSectionLite class by Chris Maunder
    switch (wBitCount)
    {
    case 1:
        m_wColors = 2;
        break;
#ifdef _WIN32_WCE
    case 2:   // winCE only
        m_wColors = 4;
        break;
#endif
    case 4:
        m_wColors = 16;
        break;
    case 8:
        m_wColors = 256;
        break;
        /*
                case 16:
                case 24:
                case 32:    // 16,24 or 32 bpp have no color table
                    m_wColors = 0;
                    break;
        */
    default:
        m_wColors = 0;
    } // switch

    m_dwLineWidth = WIDTHBYTES(wBitCount * dwWidth);

    // Initialize BITMAPINFOHEADER
    m_bi.biSize = sizeof(BITMAPINFOHEADER);
    m_bi.biWidth = dwWidth;      // fill in width from parameter
    m_bi.biHeight = dwHeight;     // fill in height from parameter
    m_bi.biPlanes = 1;            // must be 1
    m_bi.biBitCount = wBitCount;    // from parameter
    m_bi.biCompression = BI_RGB;
    m_bi.biSizeImage = m_dwLineWidth * dwHeight;
    m_bi.biXPelsPerMeter = 0;
    m_bi.biYPelsPerMeter = 0;
    m_bi.biClrUsed = 0;
    m_bi.biClrImportant = 0;

    // Calculate size of memory block required to store the DIB.  This
    // block should be big enough to hold the BITMAPINFOHEADER, the color
    // table, and the bits.
    dwLen = GetSize();

    m_hDib = new HDIB[dwLen]; // Allocate memory block to store our bitmap
    ASSERT_NULL_R(m_hDib, NULL);

    // Use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
    lpbi = (LPBITMAPINFOHEADER)(m_hDib);
    *lpbi = m_bi;

    return m_hDib; // Return handle to the DIB
} // End of Create

DWORD CCeXDib::GetSize()
{
    return m_bi.biSize + m_bi.biSizeImage + GetPaletteSize();
} // End of GetSize

DWORD CCeXDib::GetPaletteSize()
{
    return (m_wColors * sizeof(RGBQUAD));
} // End of GetPaletteSize

LPBYTE CCeXDib::GetBits()
{
    ASSERT_NULL_R(m_hDib, NULL);

    return ((LPBYTE)m_hDib + *(LPDWORD)m_hDib + GetPaletteSize());
} // End of GetBits

DWORD CCeXDib::GetWidth()
{
    return m_bi.biWidth;
} // End of GetWidth

DWORD CCeXDib::GetHeight()
{
    return m_bi.biHeight;
} // End of GetHeight

DWORD CCeXDib::GetLineWidth()
{
    return m_dwLineWidth;
} // End of GetLineWidth

void CCeXDib::BlendPalette(COLORREF crColor, DWORD dwPerc)
{
    long    i = 0;
    long    r = 0;
    long    g = 0;
    long    b = 0;
    LPBYTE  pbDst = NULL;
    RGBQUAD* pPal = NULL;

    ASSERT_NULL(m_hDib);
    ASSERT_ZERO(m_wColors);

    pbDst = (LPBYTE)(m_hDib)+sizeof(BITMAPINFOHEADER);
    pPal = (RGBQUAD*)pbDst;

    r = GetRValue(crColor);
    g = GetGValue(crColor);
    b = GetBValue(crColor);

    CHECK_VALUE_UP(dwPerc, 100);

    for (i = 0; i < m_wColors; i++)
    {
        pPal[i].rgbBlue = (BYTE)((pPal[i].rgbBlue * (100 - dwPerc) + b * dwPerc) / 100);
        pPal[i].rgbGreen = (BYTE)((pPal[i].rgbGreen * (100 - dwPerc) + g * dwPerc) / 100);
        pPal[i].rgbRed = (BYTE)((pPal[i].rgbRed * (100 - dwPerc) + r * dwPerc) / 100);
    } // for
} // End of BlendPalette

void CCeXDib::Clear(BYTE ucVal)
{
    ASSERT_NULL(m_hDib);

    memset(GetBits(), ucVal, m_bi.biSizeImage);
} // End of Clear

void CCeXDib::SetPixelIndex(DWORD dwX, DWORD dwY, BYTE ucIndex)
{
    LPBYTE  lpDst = NULL;

    ASSERT_NULL(m_hDib);
    ASSERT_ZERO(m_wColors);
    COMP_BL(dwX, 0);
    COMP_BL(dwY, 0);
    COMP_BGE(dwX, (DWORD)m_bi.biWidth);
    COMP_BGE(dwY, (DWORD)m_bi.biHeight);

    lpDst = GetBits();
    lpDst[(m_bi.biHeight - dwY - 1) * m_dwLineWidth + dwX] = ucIndex;
} // End of SetPixelIndex

void CCeXDib::Clone(CCeXDib* pcdSrc)
{
    ASSERT_NULL(pcdSrc);

    Create(pcdSrc->GetWidth(), pcdSrc->GetHeight(), pcdSrc->GetBitCount());

    ASSERT_NULL(m_hDib);
    memcpy(m_hDib, pcdSrc->m_hDib, GetSize());
} // End of Clone

WORD CCeXDib::GetBitCount()
{
    return m_bi.biBitCount;
} // End of GetBitCount

void CCeXDib::SetPaletteIndex(BYTE ucIdx, BYTE ucR, BYTE ucG, BYTE ucB)
{
    LPBYTE  lpDst = NULL;
    long    lIdx = 0;

    ASSERT_NULL(m_hDib);
    ASSERT_ZERO(m_wColors);

    lpDst = (LPBYTE)(m_hDib)+sizeof(BITMAPINFOHEADER);
    if (ucIdx < m_wColors)
    {
        lIdx = ucIdx * sizeof(RGBQUAD);
        lpDst[lIdx++] = ucB;
        lpDst[lIdx++] = ucG;
        lpDst[lIdx++] = ucR;
        lpDst[lIdx] = 0;
    } // if
} // End of SetPaletteIndex

void CCeXDib::Draw(HDC hDC, DWORD dwX, DWORD dwY)
{
    HBITMAP hOldBitmap = NULL;

    ASSERT_NULL(hDC);

    if (m_hBitmap == NULL)
    {
        m_hBitmap = CreateDIBSection(hDC, (BITMAPINFO*)m_hDib, DIB_RGB_COLORS, &m_lpBits, NULL, 0);
        ASSERT_NULL(m_hBitmap);
        if (m_lpBits == NULL)
        {
            ::DeleteObject(m_hBitmap);
            m_hBitmap = NULL;
            return;
        } // if
    } // if

    if (NULL != m_lpBits)
        memcpy(m_lpBits, GetBits(), m_bi.biSizeImage);

    if (m_hMemDC == NULL)
    {
        m_hMemDC = CreateCompatibleDC(hDC);
        ASSERT_NULL(m_hMemDC);
    } // if

    hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

    BitBlt(hDC, dwX, dwY, m_bi.biWidth, m_bi.biHeight, m_hMemDC, 0, 0, SRCCOPY);

    SelectObject(m_hMemDC, hOldBitmap);
} // End of Draw

void CCeXDib::SetGrayPalette()
{
    int         i = 0;
    RGBQUAD     tPal[256];
    LPBYTE      lpDst = NULL;

    ASSERT_NULL(m_hDib);
    ASSERT_ZERO(m_wColors);

    lpDst = (LPBYTE)(m_hDib)+sizeof(BITMAPINFOHEADER);
    for (i = 0; i < m_wColors; i++)
    {
        tPal[i] = RGB2RGBQUAD(RGB(i, i, i));
    } // for

    if (m_wColors > 0)
        tPal[m_wColors - 1] = RGB2RGBQUAD(RGB(255, 255, 255));

    memcpy(lpDst, &tPal[0], GetPaletteSize());
} // End of SetGrayPalette

RGBQUAD CCeXDib::RGB2RGBQUAD(COLORREF clr)
{
    RGBQUAD tRGBQuad;

    tRGBQuad.rgbRed = GetRValue(clr);   /* get R, G, and B out of DWORD */
    tRGBQuad.rgbGreen = GetGValue(clr);
    tRGBQuad.rgbBlue = GetBValue(clr);
    tRGBQuad.rgbReserved = 0;

    return tRGBQuad;
} // End of RGB2RGBQUAD

WORD CCeXDib::GetNumColors()
{
    return m_wColors;
} // End of GetNumColors

BOOL CCeXDib::WriteBMP(LPCTSTR bmpFileName)
{
    HANDLE           hFile = NULL;
    DWORD            dwByte = 0;
    BITMAPFILEHEADER tBPH;

    ASSERT_NULL_R(bmpFileName, FALSE);
    ASSERT_NULL_R(m_hDib, FALSE);
    COMP_BE_R(*bmpFileName, _T('\0'), FALSE);

    // open if exist ini file
    hFile = CreateFile(bmpFileName,            // pointer to name of the file
        GENERIC_WRITE,          // access mode
        0,                      // share mode
        NULL,                   // pointer to security descriptor
        CREATE_ALWAYS,          // how to create
        FILE_ATTRIBUTE_NORMAL,  // file attributes
        NULL                    // handle to file with attributes to copy
    );
    COMP_BE_R(hFile, INVALID_HANDLE_VALUE, FALSE);

    // Fill in the fields of the file header
    tBPH.bfType = BFT_BITMAP;
    tBPH.bfSize = GetSize() + sizeof(BITMAPFILEHEADER);
    tBPH.bfReserved1 = 0;
    tBPH.bfReserved2 = 0;
    tBPH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + m_bi.biSize + GetPaletteSize();

    // Write the file header
    // write ini (sync mode <-> no overlapped)
    WriteFile(hFile,                      // handle of file to write
        (LPSTR)&tBPH,               // address of buffer that contains data
        sizeof(BITMAPFILEHEADER),   // number of bytes to write
        &dwByte,                    // address of number of bytes written
        NULL                        // address of structure for data
    );

    // Write the DIB header and the bits
    // write ini (sync mode <-> no overlapped)
    WriteFile(hFile,                      // handle of file to write
        (LPSTR)m_hDib,             // address of buffer that contains data
        GetSize(),                  // number of bytes to write
        &dwByte,                    // address of number of bytes written
        NULL                        // address of structure for data
    );

    CloseHandle(hFile);             // free file handle

    return TRUE;
} // End of WriteBMP

