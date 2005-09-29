//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPathElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGPathElement.cpp,v 1.2.2.1 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGPathElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGPathElement::GetBBox()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  wxSVGMatrix matrix = GetCTM();
  wxSVGRect bbox = m_canvasItem->GetBBox(matrix);
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return bbox;
}

double wxSVGPathElement::GetTotalLength()
{
  double res = 0;
  return res;
}

wxSVGPoint wxSVGPathElement::GetPointAtLength(double distance)
{
  wxSVGPoint res;
  return res;
}

unsigned long wxSVGPathElement::GetPathSegAtLength(double distance)
{
  unsigned long res = 0;
  return res;
}

wxSVGPathSegClosePath wxSVGPathElement::CreateSVGPathSegClosePath()
{
  wxSVGPathSegClosePath res;
  return res;
}

wxSVGPathSegMovetoAbs wxSVGPathElement::CreateSVGPathSegMovetoAbs(double x, double y)
{
  wxSVGPathSegMovetoAbs res;
  return res;
}

wxSVGPathSegMovetoRel wxSVGPathElement::CreateSVGPathSegMovetoRel(double x, double y)
{
  wxSVGPathSegMovetoRel res;
  return res;
}

wxSVGPathSegLinetoAbs wxSVGPathElement::CreateSVGPathSegLinetoAbs(double x, double y)
{
  wxSVGPathSegLinetoAbs res;
  return res;
}

wxSVGPathSegLinetoRel wxSVGPathElement::CreateSVGPathSegLinetoRel(double x, double y)
{
  wxSVGPathSegLinetoRel res;
  return res;
}

wxSVGPathSegCurvetoCubicAbs wxSVGPathElement::CreateSVGPathSegCurvetoCubicAbs(double x, double y, double x1, double y1, double x2, double y2)
{
  wxSVGPathSegCurvetoCubicAbs res;
  return res;
}

wxSVGPathSegCurvetoCubicRel wxSVGPathElement::CreateSVGPathSegCurvetoCubicRel(double x, double y, double x1, double y1, double x2, double y2)
{
  wxSVGPathSegCurvetoCubicRel res;
  return res;
}

wxSVGPathSegCurvetoQuadraticAbs wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticAbs(double x, double y, double x1, double y1)
{
  wxSVGPathSegCurvetoQuadraticAbs res;
  return res;
}

wxSVGPathSegCurvetoQuadraticRel wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticRel(double x, double y, double x1, double y1)
{
  wxSVGPathSegCurvetoQuadraticRel res;
  return res;
}

wxSVGPathSegArcAbs wxSVGPathElement::CreateSVGPathSegArcAbs(double x, double y, double r1, double r2, double angle, bool largeArcFlag, bool sweepFlag)
{
  wxSVGPathSegArcAbs res;
  return res;
}

wxSVGPathSegArcRel wxSVGPathElement::CreateSVGPathSegArcRel(double x, double y, double r1, double r2, double angle, bool largeArcFlag, bool sweepFlag)
{
  wxSVGPathSegArcRel res;
  return res;
}

wxSVGPathSegLinetoHorizontalAbs wxSVGPathElement::CreateSVGPathSegLinetoHorizontalAbs(double x)
{
  wxSVGPathSegLinetoHorizontalAbs res;
  return res;
}

wxSVGPathSegLinetoHorizontalRel wxSVGPathElement::CreateSVGPathSegLinetoHorizontalRel(double x)
{
  wxSVGPathSegLinetoHorizontalRel res;
  return res;
}

wxSVGPathSegLinetoVerticalAbs wxSVGPathElement::CreateSVGPathSegLinetoVerticalAbs(double y)
{
  wxSVGPathSegLinetoVerticalAbs res;
  return res;
}

wxSVGPathSegLinetoVerticalRel wxSVGPathElement::CreateSVGPathSegLinetoVerticalRel(double y)
{
  wxSVGPathSegLinetoVerticalRel res;
  return res;
}

wxSVGPathSegCurvetoCubicSmoothAbs wxSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothAbs(double x, double y, double x2, double y2)
{
  wxSVGPathSegCurvetoCubicSmoothAbs res;
  return res;
}

wxSVGPathSegCurvetoCubicSmoothRel wxSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothRel(double x, double y, double x2, double y2)
{
  wxSVGPathSegCurvetoCubicSmoothRel res;
  return res;
}

wxSVGPathSegCurvetoQuadraticSmoothAbs wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothAbs(double x, double y)
{
  wxSVGPathSegCurvetoQuadraticSmoothAbs res;
  return res;
}

wxSVGPathSegCurvetoQuadraticSmoothRel wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothRel(double x, double y)
{
  wxSVGPathSegCurvetoQuadraticSmoothRel res;
  return res;
}

