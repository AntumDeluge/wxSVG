//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPathElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGPathElement.cpp,v 1.5 2007-07-20 08:27:39 gusstdie Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGPathElement.h"
#include "SVGCanvas.h"
#include <wx/log.h>

wxSVGRect wxSVGPathElement::GetBBox(wxSVG_COORDINATES coordinates)
{
	wxSVGMatrix matrix = wxSVGLocatable::GetCTM(this);
  WX_SVG_CREATE_M_CANVAS_ITEM
  wxSVGRect bbox = coordinates == wxSVG_COORDINATES_USER ? 
    m_canvasItem->GetBBox() : m_canvasItem->GetBBox(GetMatrix(coordinates));
  WX_SVG_CLEAR_M_CANVAS_ITEM
  return bbox.MatrixTransform(matrix);
}

wxSVGRect wxSVGPathElement::GetResultBBox(wxSVG_COORDINATES coordinates)
{
  wxCSSStyleDeclaration style = GetResultStyle(*this);
  if (style.GetStroke().GetPaintType() == wxSVG_PAINTTYPE_NONE)
    return GetBBox(coordinates);
  WX_SVG_CREATE_M_CANVAS_ITEM
  wxSVGRect bbox = coordinates == wxSVG_COORDINATES_USER ?
    m_canvasItem->GetResultBBox(style) :
    m_canvasItem->GetResultBBox(style, GetMatrix(coordinates));
  WX_SVG_CLEAR_M_CANVAS_ITEM
  return bbox;
}

void wxSVGPathElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
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

