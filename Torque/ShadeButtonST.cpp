#include "stdafx.h"
#include "ShadeButtonST.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/* SetShade处理函数组 */
void (CShadeButtonST::*g_SetShadeProc[])(BYTE, long, long, BYTE*) = 
{
    /*  0 */    &CShadeButtonST::SetNoise,
    /*  1 */    &CShadeButtonST::SetDiagShade,
    /*  2 */    &CShadeButtonST::SetHShade,
    /*  3 */    &CShadeButtonST::SetVShade,
    /*  4 */    &CShadeButtonST::SetHBump,
    /*  5 */    &CShadeButtonST::SetVBump,
    /*  6 */    &CShadeButtonST::SetSoftBump,
    /*  7 */    &CShadeButtonST::SetHardBump,
    /*  8 */    &CShadeButtonST::SetMetal
};

#define CAL_SHADE_PARA()  {                             \
        iLineWidth = m_xdNormal.GetLineWidth();         \
        iGrainX2   = RAND_MAX/(max(1,2*ucGranularity)); \
        iMaxDX     = 255-ucGranularity;                 \
        iMinDX     = ucGranularity;                     \
}

CShadeButtonST::CShadeButtonST()
{
}

CShadeButtonST::~CShadeButtonST()
{
}

//build the shaded palette
void CShadeButtonST::BuildPalette(COLORREF clrBase,BYTE ucColoring, COLORREF clrShade)
{
    int         i = 0;
    int         r,g,b;
    COLORREF    clrMid = GetSysColor(COLOR_BTNFACE);
    COLORREF    clrLow = GetSysColor(COLOR_BTNSHADOW);

    //build the shaded palette
    for(i=0;i<129;i++)
    {
        r=((128-i)*GetRValue(clrLow)+i*GetRValue(clrMid))/128;
        g=((128-i)*GetGValue(clrLow)+i*GetGValue(clrMid))/128;
        b=((128-i)*GetBValue(clrLow)+i*GetBValue(clrMid))/128;

        m_xdNormal.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
        m_xdHori.SetPaletteIndex  ((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
        m_xdVert.SetPaletteIndex  ((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
    }
    for(i=1;i<129;i++)
    {
        r=((128-i)*GetRValue(clrMid)+i*GetRValue(clrBase))/128;
        g=((128-i)*GetGValue(clrMid)+i*GetGValue(clrBase))/128;
        b=((128-i)*GetBValue(clrMid)+i*GetBValue(clrBase))/128;

        m_xdNormal.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
        m_xdHori.SetPaletteIndex  ((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
        m_xdVert.SetPaletteIndex  ((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
    }

    m_xdNormal.BlendPalette(clrShade,ucColoring); //color the palette

    return;
}

void CShadeButtonST::BuildDotBM()
{
    BYTE    *pucDst = NULL;
    int     i       = 0;
    int     j       = 0;

    pucDst = m_xdHori.GetBits();        //build the horiz. dotted focus bitmap
    ASSERT_NULL(pucDst);
    j = m_xdHori.GetWidth();
    for(i=0;i<j;i++)
    {
//      pucDst[i]=64+127*(i%2);   //soft
        pucDst[i]=255*(i%2);      //hard
    }

    pucDst = m_xdVert.GetBits();        //build the vert. dotted focus bitmap
    ASSERT_NULL(pucDst);
    j = m_xdVert.GetHeight();
    for(i=0;i<j;i++)
    {
//      *pucDst=64+127*(i%2);     //soft
        *pucDst=255*(i%2);        //hard
        pucDst+=4;
    }

    return ;
}

/*  METAL SHADE*/
void CShadeButtonST::SetMetal(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    int     i,j,k;
    int     a,x,y,xs,d;
    int     iLineWidth  = 0;
    int     iGrainX2    = 0;
    int     iMaxDX      = 0;
    int     iMinDX      = 0;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();

    m_xdNormal.Clear();

    // create the strokes(描线) 
    k=40;   //stroke ucGranularity
    for(i=0;i<200;i++)
    {
        x  = rand()/(RAND_MAX/liXSize); //stroke postion
        y  = rand()/(RAND_MAX/liYSize); //stroke position
        xs = rand()/(RAND_MAX/min(liXSize,liYSize))/2; //stroke lenght
        d  = rand()/(RAND_MAX/k);  //stroke color
        for(j=0;j<xs;j++)
        {
            if (((x-j)>0)&&((y+j)<liYSize))
                m_xdNormal.SetPixelIndex(x-j,y+j,(BYTE)d);
            if (((x+j)<liXSize)&&((y-j)>0))
                m_xdNormal.SetPixelIndex(liXSize-x+j,y-j,(BYTE)d);
        }
    }

    //blend strokes with SHS_DIAGONAL
    a = (iMaxDX-iMinDX-k)/2;
    for(i = 0; i < liYSize; i++) 
    {
        for(j = 0; j < liXSize; j++)
        {
            d          = posDst[j]+((a*i)/liYSize+(a*(liXSize-j))/liXSize);
            posDst[j]  = (BYTE)d;
            posDst[j] += (BYTE)(rand()/iGrainX2);
        }
        posDst += iLineWidth;
    }

    return;
}

/*  HARDBUMP SHADE*/
void CShadeButtonST::SetHardBump(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k;
    long    a,x,y,d,xs;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();

    //set horizontal bump
    for(i = 0; i < liYSize; i++) 
    {
        k = (255*i/liYSize)-127;
        k = (k*(k*k)/128)/128;
        k = (k*(128-ucGranularity*2))/128+128;
        for(j = 0; j < liXSize; j++) 
        {
            posDst[j]  = (BYTE)k;
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst+=iLineWidth;
    }
    
    //set vertical bump
    d = min(16,liXSize/6); //max edge=16
    a = liYSize*liYSize/4;
    if(a == 0)
        a = 1;
    if(d == 0)
        d = 1;
    posDst = pucDst;
    for(i = 0; i < liYSize; i++) 
    {
        y = i-liYSize/2;
        for(j = 0; j < liXSize; j++) 
        {
            x  = j-liXSize/2;
            xs = liXSize/2-d+(y*y*d)/a;
            if (x>xs) 
                posDst[j] = (BYTE)(iMinDX+(((liXSize-j)*128)/d));
            if ((x+xs)<0) 
                posDst[j] = (BYTE)(iMaxDX-((j*128)/d));
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}

/*  SOFTBUMP SHADE*/
void CShadeButtonST::SetSoftBump(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k,h;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();
    
    for(i = 0; i < liYSize; i++) 
    {
        h = (255*i/liYSize)-127;
        for(j = 0; j < liXSize; j++) 
        {
            k = (255*(liXSize-j)/liXSize)-127;
            k = (h*(h*h)/128)/128+(k*(k*k)/128)/128;
            k = k*(128-ucGranularity)/128+128;
            if (k<iMinDX) 
                k = iMinDX;
            if (k>iMaxDX) 
                k = iMaxDX;
            posDst[j]  = (BYTE)k;
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}


/*  VBUMP SHADE*/
void CShadeButtonST::SetVBump(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();

    for(j = 0; j < liXSize; j++) 
    {
        k = (255*(liXSize-j)/liXSize)-127;
        k = (k*(k*k)/128)/128;
        k = (k*(128-ucGranularity))/128+128;
        for(i = 0; i < liYSize; i++) 
        {
            posDst[j+i*iLineWidth]  = (BYTE)k;
            posDst[j+i*iLineWidth] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
    }

    return;
}

/*  HBUMP SHADE*/
void CShadeButtonST::SetHBump(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();
    
    for(i = 0; i < liYSize; i++) 
    {
        k = (255*i/liYSize)-127;
        k = (k*(k*k)/128)/128;
        k = (k*(128-ucGranularity))/128+128;
        for(j = 0; j < liXSize; j++) {
            posDst[j]  = (BYTE)k;
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}

/*  DIAG SHADE*/
void CShadeButtonST::SetDiagShade(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,a;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();
    
    a=(iMaxDX-iMinDX)/2;
    for(i = 0; i < liYSize; i++) 
    {
        for(j = 0; j < liXSize; j++) 
        {
            posDst[j]  = (BYTE)(iMinDX+a*i/liYSize+a*(liXSize-j)/liXSize);
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}

/*  H SHADE*/
void CShadeButtonST::SetHShade(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k,a;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();

    a = iMaxDX-iMinDX;
    for(i = 0; i < liYSize; i++) 
    {
        k = a*i/liYSize+iMinDX;
        for(j = 0; j < liXSize; j++) 
        {
            posDst[j]  = (BYTE)k;
            posDst[j] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}

/*  V SHADE*/
void CShadeButtonST::SetVShade(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j,k,a;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();

    a = iMaxDX-iMinDX;
    for(j = 0; j < liXSize; j++) 
    {
        k = a*(liXSize-j)/liXSize+iMinDX;
        for(i = 0; i < liYSize; i++) 
        {
            posDst[j+i*iLineWidth]  = (BYTE)k;
            posDst[j+i*iLineWidth] += (BYTE)(rand()/iGrainX2-ucGranularity);
        }
    }

    return;
}

/*  NOISE SHADE*/
void CShadeButtonST::SetNoise(BYTE ucGranularity, long liXSize, long liYSize, BYTE *pucDst)
{
    long    i,j;
    long    iLineWidth,iGrainX2,iMaxDX,iMinDX;
    BYTE    *posDst = pucDst;

    ASSERT_NULL(pucDst);
    /* 计算参数 */
    CAL_SHADE_PARA();
    
    for(i = 0; i < liYSize; i++) 
    {
        for(j = 0; j < liXSize; j++) 
        {
            posDst[j] = (BYTE)(128+rand()/iGrainX2-ucGranularity);
        }
        posDst += iLineWidth;
    }

    return;
}

/* 设置Shade按钮的Shade */
void CShadeButtonST::SetShade(UINT nShadeID, BYTE ucGranularity, BYTE ucHighLight, BYTE ucColoring, COLORREF clrShade)
{
    long     lXSize,lYSize;
    BYTE     *pucDst = NULL;
    RECT     rect;
    COLORREF clrBase;
    
    GetWindowRect(&rect);
    lYSize=rect.bottom-rect.top;
    lXSize=rect.right-rect.left ;

    m_xdHori.Create(max(1,lXSize /*-2*m_FocusRectMargin-1*/ ),1,8); //create the horizontal focus bitmap
    m_xdVert.Create(1,max(1,lYSize /*-2*m_FocusRectMargin*/),8);    //create the vertical focus bitmap

    //create the default bitmap
    m_xdNormal.Create(lXSize,lYSize,8);

    //get the button base colors
    clrBase = GetSysColor(COLOR_BTNHIGHLIGHT);

    //build the shaded palette
    BuildPalette(clrBase, ucColoring, clrShade);

    //build the horiz. dotted focus bitmap
    BuildDotBM();
    
    pucDst = m_xdNormal.GetBits();
    ASSERT_NULL(pucDst);

    (this->* g_SetShadeProc[nShadeID])(ucGranularity, lXSize, lYSize, pucDst);

    //build the other bitmaps
    m_xdDisabled.Clone(&m_xdNormal);  
    m_xdOver.Clone(&m_xdNormal);
    m_xdOver.BlendPalette(clrBase,ucHighLight);
    m_xdDown.Clone(&m_xdOver);
}

DWORD CShadeButtonST::OnDrawBackground(CDC *pDC, LPCRECT pRect)
{
    if (m_bMouseOnButton)
    {
        if (m_bIsPressed)
            m_xdOver.Draw(pDC->GetSafeHdc(),1,1);
        else
            m_xdOver.Draw(pDC->GetSafeHdc(),0,0);

        return BTNST_OK;
    }

    m_xdNormal.Draw(pDC->GetSafeHdc(),0,0);

    return BTNST_OK;
} // End of OnDrawBackground

DWORD CShadeButtonST::OnDrawBorder(CDC* pDC, LPCRECT pRect)
{
    BASE_BTNST_CLASS::OnDrawBorder(pDC, pRect);

    return BTNST_OK;
} // End of OnDrawBorder


