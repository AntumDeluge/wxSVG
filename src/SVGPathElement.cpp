//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPathElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGPathElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGPathElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGPathElement::GetBBox()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  wxSVGRect bbox = m_canvasItem->GetBBox();
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return bbox;
}

float wxSVGPathElement::GetTotalLength()
{
  float res = 0;
  return res;
}

wxSVGPoint wxSVGPathElement::GetPointAtLength(float distance)
{
  wxSVGPoint res;
  return res;
}

unsigned long wxSVGPathElement::GetPathSegAtLength(float distance)
{
  unsigned long res = 0;
  return res;
}

wxSVGPathSegClosePath wxSVGPathElement::CreateSVGPathSegClosePath()
{
  wxSVGPathSegClosePath res;
  return res;
}

wxSVGPathSegMovetoAbs wxSVGPathElement::CreateSVGPathSegMovetoAbs(float x, float y)
{
  wxSVGPathSegMovetoAbs res;
  return res;
}

wxSVGPathSegMovetoRel wxSVGPathElement::CreateSVGPathSegMovetoRel(float x, float y)
{
  wxSVGPathSegMovetoRel res;
  return res;
}

wxSVGPathSegLinetoAbs wxSVGPathElement::CreateSVGPathSegLinetoAbs(float x, float y)
{
  wxSVGPathSegLinetoAbs res;
  return res;
}

wxSVGPathSegLinetoRel wxSVGPathElement::CreateSVGPathSegLinetoRel(float x, float y)
{
  wxSVGPathSegLinetoRel res;
  return res;
}

wxSVGPathSegCurvetoCubicAbs wxSVGPathElement::CreateSVGPathSegCurvetoCubicAbs(float x, float y, float x1, float y1, float x2, float y2)
{
  wxSVGPathSegCurvetoCubicAbs res;
  return res;
}

wxSVGPathSegCurvetoCubicRel wxSVGPathElement::CreateSVGPathSegCurvetoCubicRel(float x, float y, float x1, float y1, float x2, float y2)
{
  wxSVGPathSegCurvetoCubicRel res;
  return res;
}

wxSVGPathSegCurvetoQuadraticAbs wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticAbs(float x, float y, float x1, float y1)
{
  wxSVGPathSegCurvetoQuadraticAbs res;
  return res;
}

wxSVGPathSegCurvetoQuadraticRel wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticRel(float x, float y, float x1, float y1)
{
  wxSVGPathSegCurvetoQuadraticRel res;
  return res;
}

wxSVGPathSegArcAbs wxSVGPathElement::CreateSVGPathSegArcAbs(float x, float y, float r1, float r2, float angle, bool largeArcFlag, bool sweepFlag)
{
  wxSVGPathSegArcAbs res;
  return res;
}

wxSVGPathSegArcRel wxSVGPathElement::CreateSVGPathSegArcRel(float x, float y, float r1, float r2, float angle, bool largeArcFlag, bool sweepFlag)
{
  wxSVGPathSegArcRel res;
  return res;
}

wxSVGPathSegLinetoHorizontalAbs wxSVGPathElement::CreateSVGPathSegLinetoHorizontalAbs(float x)
{
  wxSVGPathSegLinetoHorizontalAbs res;
  return res;
}

wxSVGPathSegLinetoHorizontalRel wxSVGPathElement::CreateSVGPathSegLinetoHorizontalRel(float x)
{
  wxSVGPathSegLinetoHorizontalRel res;
  return res;
}

wxSVGPathSegLinetoVerticalAbs wxSVGPathElement::CreateSVGPathSegLinetoVerticalAbs(float y)
{
  wxSVGPathSegLinetoVerticalAbs res;
  return res;
}

wxSVGPathSegLinetoVerticalRel wxSVGPathElement::CreateSVGPathSegLinetoVerticalRel(float y)
{
  wxSVGPathSegLinetoVerticalRel res;
  return res;
}

wxSVGPathSegCurvetoCubicSmoothAbs wxSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothAbs(float x, float y, float x2, float y2)
{
  wxSVGPathSegCurvetoCubicSmoothAbs res;
  return res;
}

wxSVGPathSegCurvetoCubicSmoothRel wxSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothRel(float x, float y, float x2, float y2)
{
  wxSVGPathSegCurvetoCubicSmoothRel res;
  return res;
}

wxSVGPathSegCurvetoQuadraticSmoothAbs wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothAbs(float x, float y)
{
  wxSVGPathSegCurvetoQuadraticSmoothAbs res;
  return res;
}

wxSVGPathSegCurvetoQuadraticSmoothRel wxSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothRel(float x, float y)
{
  wxSVGPathSegCurvetoQuadraticSmoothRel res;
  return res;
}

