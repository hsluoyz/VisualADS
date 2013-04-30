//////////////////////////////////////////////////////////////////////
/*****************************************************************/
// This file is a part of the E-XD++ MFC Extention Class.
// 2001-200? ucancode.net Software, All Rights Reserved.
// This is only define for UCanCode Software E-XD++ Library.
//
// UCanCode SOFTWARE GRANTS TO YOU (ONE SOFTWARE PROGRAMMER) THE
// LIMITED RIGHT TO USE THIS SOFTWARE ON A SINGLE COMPUTER.  
//  THESE SOURCE FILE ARE CONSIDERED CONFIDENTIONAL AND ARE 
// THE PROPERTY OF UCanCode SOFTWARE AND ARE NOT TO BE RE-DISTRIBUTED
// BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN CONSENT OF 
// UCanCode SOFTWARE.
//
// You can contact us.
// Support@ucancode.net
// http://www.ucancode.net
/********************************************************************/
// FOPGDIPlusProxy.cpp : Defines the class behaviors for the application.
//
 
#include "stdafx.h"
#include "FOPGDIPlusProxy.h"
#include "FOGlobals.h"
#include "FODrawHelper.h"
#include "FOPGDIPlusHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define FOP_CONST_ALPHA 255

IMPLEMENT_DYNCREATE(CFOPGDIPlusProxy, CFOPVisualProxy)

CFOPGDIPlusProxy::CFOPGDIPlusProxy()
{
	m_nTransparent = FOP_CONST_ALPHA;
	m_bWithGDIPlus = FALSE;
}

CFOPGDIPlusProxy::~CFOPGDIPlusProxy()
{

}

/////////////////////////////////////////////////////////////////////////////
// CFOPGDIPlusProxy diagnostics

#ifdef _DEBUG
void CFOPGDIPlusProxy::AssertValid() const
{
	CObject::AssertValid();
}

void CFOPGDIPlusProxy::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CFOPGDIPlusProxy::PrepareTransparent(const BYTE &nTrans)
{
	m_nTransparent = (BYTE)nTrans;
}

void CFOPGDIPlusProxy::Create(const BOOL &bWithGDIPlus)
{
	m_bWithGDIPlus = bWithGDIPlus;
}

void CFOPGDIPlusProxy::ReleaseC()
{
	m_bWithGDIPlus = FALSE;
}

BOOL CFOPGDIPlusProxy::GetGDIPlus()
{
	return m_bWithGDIPlus;
}

void CFOPGDIPlusProxy::FillShadowCloseBezier(CDC *pDC,LPPOINT points,int nCount,COLORREF crStart, COLORREF crEnd, UINT nFillType,CPoint ptOffset,CBrush *pBrush)
{
	CPoint ptNew;
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		ptNew = CPoint(points[i]);
		ptNew += ptOffset;
		points[i] = CPoint(ptNew);
	}

	FillCloseBezier(pDC,points, nCount,
		crStart,
		crEnd,
		nFillType,pBrush);
	
	for (i = 0; i < nCount; i++)
	{
		ptNew = CPoint(points[i]);
		ptNew -= ptOffset;
		points[i] = CPoint(ptNew);
	}
}


void CFOPGDIPlusProxy::FillCloseBezier(CDC *pDC,LPPOINT points,int nCount,COLORREF crStart,COLORREF crEnd,
									  UINT nFillType,CBrush *pBrush)
{
	//pBrush;
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillCloseBezier(pDC, points, nCount, crStart, crEnd, nFillType, pBrush);
		return;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::FillCloseBezier(pDC, points, nCount, crStart, crEnd,nFillType, pBrush);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(points, nCount);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	GraphicsPath localPath;
	localPath.AddBeziers((const Point *)points, nCount);
	localPath.CloseFigure();

	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		graphics.FillPath(pBrushG, &localPath);
	}
	
	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &localPath);

	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::FillPoly(CDC *pDC,LPPOINT points,int nCount,COLORREF crStart,COLORREF crEnd,UINT nFillType,
							   CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillPoly(pDC, points, nCount, crStart, crEnd, nFillType, pBrush);
		return;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::FillPoly(pDC, points, nCount, crStart, crEnd,nFillType, pBrush);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(points, nCount);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPolygon(pBrSolid, (const Point *)points, nCount);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPolygon(pBrSolid, (const Point *)points, nCount);
		}
		graphics.FillPolygon(pBrushG, (const Point *)points, nCount);
	}

	if (NULL != pPenG)
		graphics.DrawPolygon(pPenG, (const Point *)points, nCount);


	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}

	m_nTransparent = 255;
}

BOOL CFOPGDIPlusProxy::FillPolyPolygon(CDC *pDC,LPPOINT points,LPINT lpPolyCounts,int nCount,const CRect &rcPos,
									  COLORREF crStart,COLORREF crEnd,UINT nFillType,CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillPolyPolygon(pDC, points, lpPolyCounts, nCount, rcPos, crStart, crEnd, nFillType, pBrush);
		return TRUE;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::FillPolyPolygon(pDC, points,lpPolyCounts, nCount, rcPos, crStart, crEnd, nFillType, pBrush);
		return TRUE;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(points, nCount);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	GraphicsPath localPath;

	int i = 0;
	LPPOINT localPt = points;
	for (; i<nCount; i++)
	{
		localPath.AddPolygon((const Point *)localPt, lpPolyCounts[i]);
		localPath.CloseFigure();
		localPt += lpPolyCounts[i];
	}

	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		graphics.FillPath(pBrushG, &localPath);
	}

	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &localPath);

	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}

	m_nTransparent = 255;

	return TRUE;
}

void CFOPGDIPlusProxy::FillBezier(CDC *pDC,LPPOINT points,int nCount,COLORREF crStart,COLORREF crEnd,
								 UINT nFillType,CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillBezier(pDC, points, nCount, crStart, crEnd, nFillType, pBrush);
		return;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::FillBezier(pDC, points,nCount, crStart, crEnd,nFillType, pBrush);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(points, nCount);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	GraphicsPath localPath;
	localPath.AddBeziers((const Point *)points, nCount);
	localPath.CloseFigure();

	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart,RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd,RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		graphics.FillPath(pBrushG, &localPath);
	}

	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &localPath);


	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::FillCanShape(CDC *pDC,LPPOINT pointsOld,int nCount,COLORREF crStart,COLORREF crEnd,
								   UINT nFillType,CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillCanShape(pDC, pointsOld, nCount, crStart, crEnd, nFillType, pBrush);
		return;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::FillCanShape(pDC, pointsOld,nCount, crStart, crEnd,nFillType, pBrush);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(pointsOld, nCount);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	GraphicsPath localPath;

	Point ptLine[5] = {Point(pointsOld[19].x, pointsOld[19].y),
		Point(pointsOld[12].x, pointsOld[12].y), Point(pointsOld[6].x, pointsOld[6].y),
		Point(pointsOld[13].x, pointsOld[13].y), Point(pointsOld[19].x, pointsOld[19].y)};
	localPath.AddLines(ptLine, 5);
	localPath.CloseFigure();

	Point ptBezier[7] = {Point(pointsOld[13].x, pointsOld[13].y),
		Point(pointsOld[14].x, pointsOld[14].y), Point(pointsOld[15].x, pointsOld[15].y),
		Point(pointsOld[16].x, pointsOld[16].y), Point(pointsOld[17].x, pointsOld[17].y),
		Point(pointsOld[18].x, pointsOld[18].y), Point(pointsOld[19].x, pointsOld[19].y)};
	localPath.AddBeziers(ptBezier, 7);
	localPath.CloseFigure();

	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart,RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd,RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &localPath);
		}
		graphics.FillPath(pBrushG, &localPath);
	}
	
	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &localPath);


	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawTabOrder(CDC *pDC,const CRect &rcPos,const COLORREF &crColor,CString strText)
{
	CRect rcText = rcPos;
	pDC->FillSolidRect(&rcText,crColor);	
	pDC->DrawText(strText,&rcText,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

#define Hok_PI 3.141592653586f
void CFOPGDIPlusProxy::DoDrawArcLine(CDC *pDC,const CRect &rect,const CPoint &ptStart,const CPoint &ptEnd)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawArcLine(pDC, rect, ptStart, ptEnd);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	RectF theRect(REAL(rect.left), REAL(rect.top), REAL(rect.Width()), REAL(rect.Height()));
	PointF center(theRect.X + theRect.Width/2, theRect.Y + theRect.Height/2);

	double startAngle = 180 * atan2((double)(ptStart.y - center.Y), (double)(ptStart.x - center.X))/Hok_PI;
	double endAngle = 180 * atan2((double)(ptEnd.y - center.Y), (double)(ptEnd.x - center.X))/Hok_PI;
	startAngle = (startAngle>0)? startAngle : startAngle+360;
	endAngle = (endAngle>0)? endAngle : endAngle+360;

	double dAngle = endAngle - startAngle;

	if (NULL != pPenG)
	{
		if (dAngle > 0) 
			graphics.DrawArc(pPenG, theRect, REAL(startAngle), REAL(dAngle)-360);
		else
			graphics.DrawArc(pPenG, theRect, REAL(startAngle), REAL(dAngle));
	}

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawLine(CDC *pDC,const CPoint &ptStart,const CPoint &ptEnd)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawLine(pDC, ptStart, ptEnd);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	if (NULL != pPenG)
		graphics.DrawLine(pPenG, Point(ptStart.x, ptStart.y), Point(ptEnd.x, ptEnd.y));

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawBezierLine(CDC *pDC,LPPOINT points,int nCount)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawBezierLine(pDC, points, nCount);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	
	if (NULL != pPenG)
		graphics.DrawBeziers(pPenG, (const Point*)points, nCount);

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawCustomTrackPort(CDC *pDC,const CPoint &ptPort,const int &nPortWidth,const int &nPortHeight)
{
	pDC;
	ptPort;
	nPortWidth;
	nPortHeight;
}

void CFOPGDIPlusProxy::DoDrawCustomPort(CDC *pDC,const CPoint &ptPort,const int &nPortWidth,const int &nPortHeight,
	COLORREF crStart, COLORREF crEnd, int nBrushType,int nPortType)
{
	pDC;
	ptPort;
	nPortWidth;
	nPortHeight;
	crStart;
	crEnd;
	nBrushType;
	nPortType;
}

void CFOPGDIPlusProxy::DoDrawPolyLine(CDC *pDC,LPPOINT points,int nCount)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawPolyLine(pDC, points, nCount);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	if (NULL != pPenG)
		graphics.DrawLines(pPenG, (const Point *)points, nCount);

	m_nTransparent = 255;
}

// Draw polygon
void CFOPGDIPlusProxy::DoDrawPolygon(CDC *pDC,LPPOINT points,int nCount)
{
	pDC->Polygon(points, nCount);
}

// Drawing tracking line.
void CFOPGDIPlusProxy::DoDrawPolyPolygon(CDC *pDC,LPPOINT points,LPINT lpPoly,int nCount)
{
	pDC->PolyPolygon(points,lpPoly, nCount);
}

void CFOPGDIPlusProxy::DoDrawTextOut(CDC *pDC,const CPoint &ptText,const CString &strText,const int &nLength)
{
	pDC->TextOut(ptText.x,ptText.y,strText,nLength);
}

//////////////////////////////////////////////////////////////////////////

void FOPPreparePolyDrawG(  BOOL						bCloseFigures,
                          ULONG 					nPoly,
                          const ULONG* 				pPoints,
                          const fopPointExt* const*	pPtAry,
                          const BYTE* const* 		pFlgAry,
                          POINT* 					pWinPointAry,
                          BYTE* 					pWinFlagAry		)
{
    ULONG nCurrPoly;
    for( nCurrPoly=0; nCurrPoly<nPoly; ++nCurrPoly )
    {
        const POINT* pCurrPoint = reinterpret_cast<const POINT*>( *pPtAry++ );
        const BYTE* pCurrFlag = *pFlgAry++;
        const ULONG nCurrPoints = *pPoints++;
        const BOOL bHaveFlagArray( (BOOL)pCurrFlag );
        ULONG nCurrPoint;

        if( nCurrPoints )
        {
            *pWinPointAry++ = *pCurrPoint++;
            *pWinFlagAry++  = PT_MOVETO;
            ++pCurrFlag;

            for( nCurrPoint=1; nCurrPoint<nCurrPoints; )
            {
                if( bHaveFlagArray &&
                    ( nCurrPoint + 2 ) < nCurrPoints )
                {
                    BYTE P4( pCurrFlag[ 2 ] );

                    if( ( FOP_SIM_POLY_CONTROL == pCurrFlag[ 0 ] ) &&
                        ( FOP_SIM_POLY_CONTROL == pCurrFlag[ 1 ] ) &&
                        ( FOP_SIM_POLY_NORMAL == P4 || FOP_SIM_POLY_SMOOTH == P4 || FOP_SIM_POLY_SYMMTR == P4 ) )
                    {
                        *pWinPointAry++ = *pCurrPoint++;
                        *pWinFlagAry++  = PT_BEZIERTO;

                        *pWinPointAry++ = *pCurrPoint++;
                        *pWinFlagAry++  = PT_BEZIERTO;

                        *pWinPointAry++ = *pCurrPoint++;
                        *pWinFlagAry++  = PT_BEZIERTO;

                        nCurrPoint += 3;
                        pCurrFlag += 3;
                        continue;
                    }
                }

                *pWinPointAry++ = *pCurrPoint++;
                *pWinFlagAry++  = PT_LINETO;
                ++pCurrFlag;
                ++nCurrPoint;
            }

            if( bCloseFigures )
			{
                pWinFlagAry[-1] |= PT_CLOSEFIGURE;
			}

        }
    }
}


void CFOPGDIPlusProxy::FOPDoRenderPathG(CDC *pDC, ULONG nPoints, const BYTE &nTrans, const fopPointExt* pPtAry, const BYTE* pFlgAry )
{
	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),nTrans);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel);

	fopPointExt pPtMove;

    if( nPoints )
    {
		pPtMove.nLX = pPtAry[0].nLX;
		pPtMove.nLY = pPtAry[0].nLY;
        ++pFlgAry;

        for(USHORT i=1; i<nPoints; ++i,++pFlgAry )
        {
            if( *pFlgAry != FOP_SIM_POLY_CONTROL )
            {
				if (NULL != pPenG)
					graphics.DrawLine(pPenG, Point(pPtMove.nLX, pPtMove.nLY), Point(pPtAry[i].nLX, pPtAry[i].nLY));

				pPtMove.nLX = pPtAry[i].nLX;
				pPtMove.nLY = pPtAry[i].nLY;		
            }
            else if( nPoints - i > 2 )
            {
				Point points[4];
				points[0] = Point(pPtMove.nLX,pPtMove.nLY);
				points[1] = Point(pPtAry[i].nLX,pPtAry[i].nLY);
				points[2] = Point(pPtAry[i+1].nLX,pPtAry[i+1].nLY);
				points[3] = Point(pPtAry[i+2].nLX,pPtAry[i+2].nLY);

				if (NULL != pPenG)
					graphics.DrawBezier(pPenG, 
					points[0],
					points[1],
					points[2],
					points[3]);

                i += 2; pFlgAry += 2;
				pPtMove.nLX = points[3].X;
				pPtMove.nLY = points[3].Y;		
			}
        }
	}

}

/*

void CFOPGDIPlusProxy::FOPPolyDrawG(CDC *pDC, const BYTE &nTrans, CONST LPPOINT lppt, CONST LPBYTE lpbTypes, int cCount,
			   BOOL bFill, COLORREF crStart, COLORREF crEnd, UINT nFillType)
{
//	HDC dc = pDC->m_hDC;
	int nIndex;
	LPPOINT pptLastMoveTo = NULL;
	POINT ptStart;
	ptStart.x = 0;
	ptStart.y = 0;
	GraphicsPath localPath;
	GraphicsPath AllPath;

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),nTrans);

	// for each of the points we have...
	for (nIndex = 0; nIndex < cCount; nIndex++)
	{
		switch(lpbTypes[nIndex])
		{
		case PT_MOVETO:
			{
				localPath.StartFigure();
				if (pptLastMoveTo != NULL && nIndex > 0)
				{
//					localPath.AddLine(Point(ptStart.x,ptStart.y),Point(pptLastMoveTo->x, pptLastMoveTo->y));
					//				LineTo(dc, );
					ptStart.x = pptLastMoveTo->x;
					ptStart.y = pptLastMoveTo->y;
				}
				
				ptStart = lppt[nIndex];
				//			MoveToEx(dc, lppt[nIndex].x, lppt[nIndex].y, NULL);
				pptLastMoveTo = &lppt[nIndex];
			}
			break;
			
		case PT_LINETO | PT_CLOSEFIGURE:
			{
				//			LineTo(dc, lppt[nIndex].x, lppt[nIndex].y);
				localPath.AddLine(Point(ptStart.x,ptStart.y),Point(lppt[nIndex].x, lppt[nIndex].y));
				ptStart = lppt[nIndex];
				if (pptLastMoveTo != NULL)
				{
					//				LineTo(dc, pptLastMoveTo->x, pptLastMoveTo->y);
					localPath.AddLine(Point(ptStart.x,ptStart.y),Point(pptLastMoveTo->x, pptLastMoveTo->y));
					ptStart.x = pptLastMoveTo->x;
					ptStart.y = pptLastMoveTo->y;
				}
				localPath.CloseFigure();
				
				AllPath.AddPath(&localPath,false);

				localPath.Reverse();
				localPath.Reset();
				
				pptLastMoveTo = NULL;

			}
			break;
			
		case PT_LINETO:
			{
				//			LineTo(dc, lppt[nIndex].x, lppt[nIndex].y);
				localPath.AddLine(Point(ptStart.x,ptStart.y),Point(lppt[nIndex].x, lppt[nIndex].y));
				ptStart = lppt[nIndex];
			}
			break;
			
		case PT_BEZIERTO | PT_CLOSEFIGURE:
			{
				Point points[4];
				points[0] = Point(ptStart.x,ptStart.y);
				points[1] = Point(lppt[nIndex].x,lppt[nIndex].y);
				points[2] = Point(lppt[nIndex + 1].x,lppt[nIndex + 1].y);
				points[3] = Point(lppt[nIndex + 2].x,lppt[nIndex + 2].y);
				localPath.AddBezier(points[0],points[1],points[2],points[3]);
				//			PolyBezierTo(dc, &lppt[nIndex], 3);
				nIndex += 2;
				ptStart = lppt[nIndex];
				if (pptLastMoveTo != NULL)
				{
					//				LineTo(dc, pptLastMoveTo->x, pptLastMoveTo->y);
					localPath.AddLine(Point(ptStart.x,ptStart.y),Point(pptLastMoveTo->x, pptLastMoveTo->y));
					ptStart.x = pptLastMoveTo->x;
					ptStart.y = pptLastMoveTo->y;
				}
				
				localPath.CloseFigure();

				AllPath.AddPath(&localPath,false);

				localPath.Reverse();
				localPath.Reset();

				pptLastMoveTo = NULL;
			}
			break;
			
		case PT_BEZIERTO:
			{
				Point points[4];
				points[0] = Point(ptStart.x,ptStart.y);
				points[1] = Point(lppt[nIndex].x,lppt[nIndex].y);
				points[2] = Point(lppt[nIndex + 1].x,lppt[nIndex + 1].y);
				points[3] = Point(lppt[nIndex + 2].x,lppt[nIndex + 2].y);
				localPath.AddBezier(points[0],points[1],points[2],points[3]);
				//			PolyBezierTo(dc, &lppt[nIndex], 3);
				nIndex += 2;
				ptStart = lppt[nIndex];
			}
			break;
		}
	}

	if (pptLastMoveTo != NULL && nIndex > 1)
	{
//		LineTo(dc, pptLastMoveTo->x, pptLastMoveTo->y);
		localPath.AddLine(Point(ptStart.x,ptStart.y),Point(pptLastMoveTo->x, pptLastMoveTo->y));
		localPath.CloseFigure();
		AllPath.AddPath(&localPath,false);
	}
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel);

	if(bFill == TRUE)
	{
		Brush *pBrushG = NULL;
		if ( 41 < nFillType && nFillType <62  )
		{
			CRect rc = CFODrawHelper::GetBoundRect(lppt, cCount);
			pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, nTrans, nTrans);
		}
		else
		{
			pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, nTrans, nTrans);
		}

		if (NULL != pBrushG)
		{
			if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
			{
				Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart,RGB(0,0,0), nTrans, nTrans);
				graphics.FillPath(pBrSolid, &AllPath);
			}
			if (nFillType-42 == GRADIENT_ELLIPSE_IN)
			{
				Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd,RGB(0,0,0), nTrans, nTrans);
				graphics.FillPath(pBrSolid, &AllPath);
			}
			graphics.FillPath(pBrushG, &AllPath);
		}

		if ( 41 < nFillType && nFillType <62  )
			_SafeDeletePtr(pBrushG);
	}

	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &AllPath);

	graphics.Flush();

}

*/

void CFOPGDIPlusProxy::FOPPolyDrawG(CDC *pDC, const BYTE &nTrans, CONST LPPOINT lppt, CONST LPBYTE lpbTypes, int cCount,
			   BOOL bFill, COLORREF crStart, COLORREF crEnd, UINT nFillType)
{
//	HDC dc = pDC->m_hDC;
	int nIndex;
	LPPOINT pptLastMoveTo = NULL;
	POINT ptStart;
	ptStart.x = 0;
	ptStart.y = 0;
	GraphicsPath localPath;
	GraphicsPath AllPath;

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),nTrans);

	// for each of the points we have...
	for (nIndex = 0; nIndex < cCount; nIndex++)
	{
		switch(lpbTypes[nIndex])
		{
		case PT_MOVETO:
			{
				if (pptLastMoveTo != NULL && nIndex > 0)
				{
					localPath.CloseFigure();
				}
				
				pptLastMoveTo = &lppt[nIndex];
				ptStart = lppt[nIndex];
				
			}
			break;
			
		case PT_LINETO | PT_CLOSEFIGURE:
			{
				localPath.AddLine(Point(ptStart.x,ptStart.y),Point(lppt[nIndex].x, lppt[nIndex].y));
				ptStart = lppt[nIndex];
				if (pptLastMoveTo != NULL)
				{
					localPath.CloseFigure();
					AllPath.AddPath(&localPath,false);
					localPath.Reverse();
					localPath.Reset();
					ptStart.x = pptLastMoveTo->x;
					ptStart.y = pptLastMoveTo->y;
				}
				pptLastMoveTo = NULL;

			}
			break;
			
		case PT_LINETO:
			{
				localPath.AddLine(Point(ptStart.x,ptStart.y),Point(lppt[nIndex].x, lppt[nIndex].y));
				ptStart = lppt[nIndex];
			}
			break;
			
		case PT_BEZIERTO | PT_CLOSEFIGURE:
			{
				Point points[4];
				points[0] = Point(ptStart.x,ptStart.y);
				points[1] = Point(lppt[nIndex].x,lppt[nIndex].y);
				points[2] = Point(lppt[nIndex + 1].x,lppt[nIndex + 1].y);
				points[3] = Point(lppt[nIndex + 2].x,lppt[nIndex + 2].y);
				localPath.AddBezier(points[0],points[1],points[2],points[3]);
				nIndex += 2;
				ptStart = lppt[nIndex];
				if (pptLastMoveTo != NULL)
				{
					localPath.CloseFigure();
					AllPath.AddPath(&localPath,false);
					localPath.Reverse();
					localPath.Reset();
					ptStart.x = pptLastMoveTo->x;
					ptStart.y = pptLastMoveTo->y;
				}

				pptLastMoveTo = NULL;
			}
			break;
			
		case PT_BEZIERTO:
			{
				Point points[4];
				points[0] = Point(ptStart.x,ptStart.y);
				points[1] = Point(lppt[nIndex].x,lppt[nIndex].y);
				points[2] = Point(lppt[nIndex + 1].x,lppt[nIndex + 1].y);
				points[3] = Point(lppt[nIndex + 2].x,lppt[nIndex + 2].y);
				localPath.AddBezier(points[0],points[1],points[2],points[3]);
				nIndex += 2;
				ptStart = lppt[nIndex];
			}
			break;
		}
	}

	if (pptLastMoveTo != NULL && nIndex > 1)
	{
		localPath.AddLine(Point(ptStart.x,ptStart.y),Point(pptLastMoveTo->x, pptLastMoveTo->y));
		localPath.CloseFigure();
		AllPath.AddPath(&localPath,false);
	}
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel);

	if(bFill == TRUE)
	{
		Brush *pBrushG = NULL;
		if ( 41 < nFillType && nFillType <62  )
		{
			CRect rc = CFODrawHelper::GetBoundRect(lppt, cCount);
			pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, nTrans, nTrans);
		}
		else
		{
			pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, nTrans, nTrans);
		}

		if (NULL != pBrushG)
		{
			if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
			{
				Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart,RGB(0,0,0), nTrans, nTrans);
				graphics.FillPath(pBrSolid, &AllPath);
			}
			if (nFillType-42 == GRADIENT_ELLIPSE_IN)
			{
				Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd,RGB(0,0,0), nTrans, nTrans);
				graphics.FillPath(pBrSolid, &AllPath);
			}
			graphics.FillPath(pBrushG, &AllPath);
		}

		if ( 41 < nFillType && nFillType <62  )
			_SafeDeletePtr(pBrushG);
	}

	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &AllPath);

	graphics.Flush();

}

#define FOP_PLOY_STACKBUF		32

BOOL CFOPGDIPlusProxy::DrawNewPolyLineBezier(CDC *pDC, ULONG nPoints, const fopPointExt* pPtAry, const BYTE* pFlgAry )
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DrawNewPolyLineBezier(pDC, nPoints, pPtAry, pFlgAry);
		return TRUE;
	}

    FOPDoRenderPathG(pDC, nPoints,m_nTransparent, pPtAry, pFlgAry );
    return TRUE;
}

BOOL CFOPGDIPlusProxy::ExtPolyDraw(CDC *pDC,const CRect &rcPos, 
										   ULONG nPoints, 
										   POINT* pWinPointAry,
										   BYTE* pWinFlagAry,
										COLORREF crStart,
										COLORREF crEnd,UINT nType,
										CBrush *pBrush )
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::ExtPolyDraw(pDC, rcPos, nPoints,pWinPointAry, pWinFlagAry, 
			crStart, crEnd, nType, pBrush);
		return TRUE;
	}

	if(nType >= 92)
	{
		CFOPVisualProxy::ExtPolyDraw(pDC, rcPos, nPoints,pWinPointAry, pWinFlagAry, 
			crStart, crEnd, nType, pBrush);
		return TRUE;
	}

	FOPPolyDrawG(pDC,m_nTransparent, pWinPointAry, pWinFlagAry, nPoints, TRUE, crStart, crEnd, nType);

	m_nTransparent = 255;
	return TRUE;
}


void CFOPGDIPlusProxy::S_DrawCloseCurve(CDC *pDC, POINT* pWinPointAry, BYTE* pWinFlagAry, int nPoints,  const CRect &rcPos, 
									   COLORREF crStart,COLORREF crEnd,UINT nFillType,CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::S_DrawCloseCurve(pDC, pWinPointAry, pWinFlagAry, nPoints, rcPos, crStart, crEnd, nFillType, pBrush);
		return;
	}

	if(nFillType >= 92)
	{
		CFOPVisualProxy::S_DrawCloseCurve(pDC, pWinPointAry, pWinFlagAry, nPoints, rcPos, crStart, crEnd, nFillType, pBrush);
		return;
	}
	
	GraphicsPath path((Point* )pWinPointAry, pWinFlagAry, nPoints);

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);
	
	Brush *pBrushG = NULL;
	if ( 41 < nFillType && nFillType <62  )
	{
		CRect rc = CFODrawHelper::GetBoundRect(pWinPointAry, nPoints);
		pBrushG = CFOPHokBrushCache::CreateGradientBrush(nFillType, crStart, crEnd, rc, m_nTransparent, m_nTransparent);
	}
	else
	{
		pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(nFillType, crStart, crEnd, m_nTransparent, m_nTransparent);
	}
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	
	if (NULL != pBrushG)
	{
		if (nFillType-42 == GRADIENT_ELLIPSE_OUT)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crStart, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &path);
		}
		if (nFillType-42 == GRADIENT_ELLIPSE_IN)
		{
			Brush *pBrSolid = CFOPHokBrushCache::GetInstance()->GetBrush(1, crEnd, RGB(0,0,0), m_nTransparent, m_nTransparent);
			graphics.FillPath(pBrSolid, &path);
		}
		graphics.FillPath(pBrushG, &path);
	}
	
	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &path);
	
	
	if ( 41 < nFillType && nFillType <62  )
	{
		_SafeDeletePtr(pBrushG);
	}
	
	m_nTransparent = 255;

}

BOOL CFOPGDIPlusProxy::FillNewPolygonBezier(CDC *pDC,const CRect &rcPos, ULONG nPoints,
		const fopPointExt* pPtAry, const BYTE* pFlgAry, COLORREF crStart,COLORREF crEnd,UINT nType, CBrush *pBrush )
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillNewPolygonBezier(pDC, rcPos, nPoints,pPtAry, pFlgAry, crStart, crEnd, nType, pBrush);
		return TRUE;
	}

	if(nType >= 92)
	{
		CFOPVisualProxy::FillNewPolygonBezier(pDC, rcPos, nPoints,pPtAry, pFlgAry, crStart, crEnd, nType, pBrush);
		return TRUE;
	}

    POINT	aStackAry1[FOP_PLOY_STACKBUF];
    BYTE	aStackAry2[FOP_PLOY_STACKBUF];
    POINT* 	pWinPointAry;
    BYTE* 	pWinFlagAry;
    if( nPoints > FOP_PLOY_STACKBUF )
    {
        pWinPointAry = new POINT[nPoints];
        pWinFlagAry = new BYTE[nPoints];
    }
    else
    {
        pWinPointAry = aStackAry1;
        pWinFlagAry = aStackAry2;
    }

	FOPPreparePolyDrawG(true, 1, &nPoints, &pPtAry, &pFlgAry, pWinPointAry, pWinFlagAry);

	FOPPolyDrawG(pDC,m_nTransparent, pWinPointAry, pWinFlagAry, nPoints, TRUE, crStart, crEnd, nType);

    if( pWinPointAry != aStackAry1 )
    {
		_SafeDeletePtrArr(pWinPointAry);
		_SafeDeletePtrArr(pWinFlagAry);
	}

    return TRUE;
}

BOOL CFOPGDIPlusProxy::DrawPolyPolygonBezier(CDC *pDC,const CRect &rcPos, ULONG nPoly, const ULONG* pPoints,
											const fopPointExt* const* pPtAry, const BYTE* const* pFlgAry,
											COLORREF crStart,COLORREF crEnd,UINT nType, CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DrawPolyPolygonBezier(pDC, rcPos, nPoly,pPoints, pPtAry, pFlgAry, 
			crStart, crEnd, nType, pBrush);
		return TRUE;
	}

	if(nType >= 92)
	{
		CFOPVisualProxy::DrawPolyPolygonBezier(pDC, rcPos, nPoly,pPoints, pPtAry, pFlgAry, 
			crStart, crEnd, nType, pBrush);
		return TRUE;
	}

    ULONG nCurrPoly, nTotalPoints;
    const ULONG* pCurrPoints = pPoints;
    for( nCurrPoly=0, nTotalPoints=0; nCurrPoly<nPoly; ++nCurrPoly )
        nTotalPoints += *pCurrPoints++;

    POINT	aStackAry1[FOP_PLOY_STACKBUF];
    BYTE	aStackAry2[FOP_PLOY_STACKBUF];
    POINT* 	pWinPointAry;
    BYTE* 	pWinFlagAry;
    if( nTotalPoints > FOP_PLOY_STACKBUF )
    {
        pWinPointAry = new POINT[ nTotalPoints ];
        pWinFlagAry = new BYTE[ nTotalPoints ];
    }
    else
    {
        pWinPointAry = aStackAry1;
        pWinFlagAry = aStackAry2;
    }

    FOPPreparePolyDrawG(TRUE, nPoly, pPoints, pPtAry, pFlgAry, pWinPointAry, pWinFlagAry);

	FOPPolyDrawG(pDC,m_nTransparent, pWinPointAry, pWinFlagAry, nTotalPoints, TRUE, crStart, crEnd, nType);

    if( pWinPointAry != aStackAry1 )
    {
		_SafeDeletePtrArr(pWinPointAry);
		_SafeDeletePtrArr(pWinFlagAry);
    }
    return TRUE;
}

#define FOP_POLYPOLY_STACKBUF		32

BOOL CFOPGDIPlusProxy::FillNewPolyPolygon(CDC *pDC,const CRect &rcPos, USHORT nPoly, 
										 const FOPSimpleCompositePolygon& rPolyPoly,
										 COLORREF crStart,COLORREF crEnd,UINT nFillType,
										 CBrush *pBrush)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::FillNewPolyPolygon(pDC, rcPos, nPoly,rPolyPoly,crStart, crEnd, nFillType, pBrush);
		return TRUE;
	}

	BOOL bReturn		= TRUE;
    ULONG				aStackAry1[FOP_POLYPOLY_STACKBUF];
    FOPCONSTPOINT		aStackAry2[FOP_POLYPOLY_STACKBUF];
    BYTE*				aStackAry3[FOP_POLYPOLY_STACKBUF];
    ULONG*				pPointAry;
    FOPCONSTPOINT* 		pPointAryAry;
    const BYTE**	 	pFlagAryAry;
    USHORT				i = 0, j = 0, last = 0;
    BOOL				bHaveBezier = FALSE;

    if ( nPoly > FOP_POLYPOLY_STACKBUF )
    {
        pPointAry		= new ULONG[nPoly];
        pPointAryAry	= new FOPCONSTPOINT[nPoly];
        pFlagAryAry		= new const BYTE*[nPoly];
    }
    else
    {
        pPointAry		= aStackAry1;
        pPointAryAry	= aStackAry2;
        pFlagAryAry		= (const BYTE**)aStackAry3;
    }

    do
    {
        const FOPSimplePolygon&	rPoly = rPolyPoly[ i ];
        int nSize = rPoly.GetPointCount();
        if ( nSize )
        {
            pPointAry[j]	= nSize;
            pPointAryAry[j] = (FOPCONSTPOINT )rPoly.GetConstPointAry();
            pFlagAryAry[j]  = rPoly.GetConstFlagAry();
            last 			= i;

            if( pFlagAryAry[j] )
			{
                bHaveBezier = TRUE;
			}

            ++j;
        }

        ++i;
    }
    while ( i < nPoly );

    if ( j == 1 )
    {
        if( bHaveBezier )
		{
			FillNewPolygonBezier(pDC,rcPos, *pPointAry, *pPointAryAry, *pFlagAryAry,crStart,crEnd,nFillType,pBrush);
        }
        else
        {
			FillPoly(pDC,(POINT *)*pPointAryAry,*pPointAry,crStart,crEnd,nFillType,pBrush);
        }
    }
    else
    {
        if( bHaveBezier )
        {
            DrawPolyPolygonBezier(pDC,rcPos,j,pPointAry, pPointAryAry, pFlagAryAry,crStart,crEnd,nFillType,pBrush);
        }
        else
        {
			bReturn = FALSE;
        }
    }

    if ( pPointAry != aStackAry1 )
    {
        _SafeDeletePtrArr(pPointAry);
        _SafeDeletePtrArr(pPointAryAry);
        _SafeDeletePtrArr(pFlagAryAry);
    }

	m_nTransparent = 255;

	return bReturn;
}

void CFOPGDIPlusProxy::DoDrawArrowLine(CDC *pDC,const CPoint &ptStart,const CPoint &ptEnd)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawArrowLine(pDC, ptStart, ptEnd);
		return;
	}

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	if (NULL != pPenG)
		graphics.DrawLine(pPenG, Point(ptStart.x, ptStart.y), Point(ptEnd.x, ptEnd.y));

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawArrowAngleArcLine(CDC *pDC,const CPoint &ptStart,const CPoint &ptEnd,
		const int &nWidth,const float &startAngle,const float &endAngle,const COLORREF &crFill,const BOOL &bFill)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawArrowAngleArcLine(pDC, ptStart, ptEnd, nWidth, startAngle, endAngle, crFill, bFill);
		return;
	}
	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	if (NULL != pPenG)
		graphics.DrawArc(pPenG, ptEnd.x-nWidth, ptEnd.y-nWidth, 2*nWidth, 2*nWidth,startAngle,endAngle);

	if (bFill)
	{
		Brush* pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(1, crFill,RGB(0,0,0), m_nTransparent, m_nTransparent);
		if (NULL != pBrushG)
			graphics.FillEllipse(pBrushG, ptEnd.x - nWidth, ptEnd.y - nWidth, 2*nWidth, 2*nWidth);
	}

	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DrawText(CDC *pDC,const CString &strText, CRect rcRect, UINT uFormat)
{

	CFOPVisualProxy::DrawText(pDC, strText, rcRect, uFormat);
}

void CFOPGDIPlusProxy::S_FillPolygon(CDC *pDC, POINT* points, int nCount,const COLORREF &crFill)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::S_FillPolygon(pDC, points, nCount, crFill);
		return;
	}
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	
	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);
	Brush* pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(1, crFill,RGB(0,0,0), m_nTransparent, m_nTransparent);
	
	if (NULL != pPenG)
		graphics.DrawPolygon(pPenG, (const Point *)points, nCount);
	if (NULL != pBrushG)
		graphics.FillPolygon(pBrushG, (const Point *)points, nCount);
	
	m_nTransparent = 255;
}


void CFOPGDIPlusProxy::S_Ellipse(CDC *pDC, LPCRECT lpRect,const COLORREF &crFill)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::S_Ellipse(pDC, lpRect, crFill);
		return;
	}
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	
	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);
	Brush* pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(1, crFill,RGB(0,0,0), m_nTransparent, m_nTransparent);
	
	if (NULL != pPenG)
		graphics.DrawEllipse(pPenG, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
	if (NULL != pBrushG)
		graphics.FillEllipse(pBrushG, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
	
	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::S_DrawCurve(CDC *pDC, POINT* lppt, BYTE* pTypes, int nCount, const BOOL &bClose)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::S_DrawCurve(pDC, lppt, pTypes, nCount, bClose);
		return;
	}
	
	GraphicsPath path((Point* )lppt, pTypes, nCount);

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	
	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);
	
	if (NULL != pPenG)
		graphics.DrawPath(pPenG, &path);
	
	m_nTransparent = 255;
}

void CFOPGDIPlusProxy::DoDrawArrowPolygon(CDC *pDC,LPPOINT points,int nCount,const COLORREF &crFill)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawArrowPolygon(pDC, points, nCount, crFill);
		return;
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(HokGdiPlusG::m_smoothingMode);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing

	Pen* pPenG = CFOPHokPenCache::GetInstance()->GetPen(pDC->GetCurrentPen(),m_nTransparent);
	Brush* pBrushG = CFOPHokBrushCache::GetInstance()->GetBrush(1, crFill,RGB(0,0,0), m_nTransparent, m_nTransparent);

	if (NULL != pPenG)
		graphics.DrawPolygon(pPenG, (const Point *)points, nCount);
	if (NULL != pBrushG)
		graphics.FillPolygon(pBrushG, (const Point *)points, nCount);

	m_nTransparent = 255;
}

BOOL CFOPGDIPlusProxy::IsGDIPlus()
{
	return TRUE;
}

void CFOPGDIPlusProxy::DoDrawBitmap(CDC *pDC, CFOBitmap *pBitmap, CRect rcPos, 
									const int &nImageX, const int &nImageY, const int &nImageWidth, const int &nImageHeight,
									const BOOL &m_bImageTransparent, const COLORREF &m_crImageTransparent)
{
	if(!m_bWithGDIPlus || !gfxData.m_bDefWithGDIPlus)
	{
		CFOPVisualProxy::DoDrawBitmap(pDC, pBitmap, rcPos, nImageX, nImageY, nImageWidth, nImageHeight, m_bImageTransparent, m_crImageTransparent);
		return;
	}

	Bitmap *pBitmapPlus = new Bitmap(pBitmap->m_lpBMI, pBitmap->m_lpBuf);
	
	if (NULL == pBitmapPlus)
		return;
	
	int m_nColorOffset = 1;
	ImageAttributes m_imAtt;
	if (TRUE == m_bImageTransparent)
	{
		m_imAtt.Reset();
		m_imAtt.ClearColorKey();
		
		int nR = GetRValue(m_crImageTransparent)-m_nColorOffset<0? 0:GetRValue(m_crImageTransparent)-m_nColorOffset;
		int nG = GetGValue(m_crImageTransparent)-m_nColorOffset<0? 0:GetGValue(m_crImageTransparent)-m_nColorOffset;
		int nB = GetBValue(m_crImageTransparent)-m_nColorOffset<0? 0:GetBValue(m_crImageTransparent)-m_nColorOffset;
		Color crLow((BYTE)nR, (BYTE)nG, (BYTE)nB);
		
		nR = GetRValue(m_crImageTransparent)+m_nColorOffset>255? 255:GetRValue(m_crImageTransparent)+m_nColorOffset;
		nG = GetGValue(m_crImageTransparent)+m_nColorOffset>255? 255:GetGValue(m_crImageTransparent)+m_nColorOffset;
		nB = GetBValue(m_crImageTransparent)+m_nColorOffset>255? 255:GetBValue(m_crImageTransparent)+m_nColorOffset;
		Color crHigh((BYTE)nR, (BYTE)nG, (BYTE)nB);
		
		m_imAtt.SetColorKey(crLow, crHigh);
	}
	
	ColorMatrix cm = {0,};
	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = cm.m[4][4] = 1;
	//	cm.m[3][3] = 0.5;
	cm.m[3][3] = float(m_nTransparent)/255.0f;
	m_imAtt.SetColorMatrix(&cm);
	
	RectF rcDest;
	rcDest.X = (float)rcPos.left;
	rcDest.Y = (float)rcPos.top;
	rcDest.Width = (float)rcPos.Width();
	rcDest.Height = (float)rcPos.Height();
	
	Graphics graphics(pDC->m_hDC);
	graphics.SetPageUnit(UnitPixel); // Must be set for printing
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetCompositingQuality(CompositingQualityHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	graphics.DrawImage(pBitmapPlus, rcDest, (float)nImageX, (float)nImageY, (float)nImageWidth, (float)nImageHeight, UnitPixel, &m_imAtt);
	delete pBitmapPlus;

	m_nTransparent = 255;
}
