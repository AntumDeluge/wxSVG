//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasItem.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/09
// RCS-ID:      $Id: SVGCanvasItem.cpp,v 1.7 2005-09-25 11:44:02 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvasItem.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////// wxSVGCanvasPath //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

wxSVGCanvasPath::wxSVGCanvasPath(): wxSVGCanvasItem(wxSVG_CANVAS_ITEM_PATH)
{
  m_fill = true;
  m_curx = m_cury = m_cubicx = m_cubicy = m_quadx = m_quady = 0;
  m_begx = m_begy = 0;
}

void wxSVGCanvasPath::Init(wxSVGLineElement& element)
{
  SetFill(false);
  
  MoveTo(element.GetX1().GetBaseVal(), element.GetY1().GetBaseVal());
  LineTo(element.GetX2().GetBaseVal(), element.GetY2().GetBaseVal());
  End();
}

void wxSVGCanvasPath::Init(wxSVGPolylineElement& element)
{
  SetFill(false);
  
  const wxSVGPointList& points = element.GetPoints();
  if (points.Count())
	MoveTo(points[0].GetX(), points[0].GetY());
  for (unsigned int i=1; i<points.Count(); i++)
	LineTo(points[i].GetX(), points[i].GetY());
  End();
}

void wxSVGCanvasPath::Init(wxSVGPolygonElement& element)
{
  const wxSVGPointList& points = element.GetPoints();
  if (points.Count())
	MoveTo(points[0].GetX(), points[0].GetY());
  for (unsigned int i=1; i<points.Count(); i++)
	LineTo(points[i].GetX(), points[i].GetY());
  ClosePath();
  End();
}

void wxSVGCanvasPath::Init(wxSVGRectElement& element)
{
  double x = element.GetX().GetBaseVal();
  double y = element.GetY().GetBaseVal();
  double width = element.GetWidth().GetBaseVal();
  double height = element.GetHeight().GetBaseVal();
  double rx = element.GetRx().GetBaseVal();
  double ry = element.GetRy().GetBaseVal();
  
  if (rx == 0 && ry == 0)
  {
	MoveTo(x, y);
	LineTo(width, 0, true);
	LineTo(0, height, true);
	LineTo(-width, 0, true);
	ClosePath();
  }
  else
  {
	if (rx == 0)
	  rx = ry;
	if (ry == 0)
	  ry = rx;
	if (rx > width/2)
	  rx = width/2;
	if (ry > height/2)
	  ry = height/2;
	MoveTo(x + rx, y);
	CurveToCubic(x + rx*0.448, y, x, y + ry*0.448, x, y + ry);
	if (ry < height/2)
	  LineTo(x, y + height - ry);
	CurveToCubic(x, y + height - ry*0.448, x + rx*0.448, y + height, x + rx, y + height);
	if(rx < width/2)
	  LineTo(x + width - rx, y + height);
	CurveToCubic(x + width - rx*0.448, y + height, x + width, y + height - ry*0.448, x + width, y + height - ry);
	if(ry < height/2)
	  LineTo(x + width, y + ry);
	CurveToCubic(x + width, y + ry*0.448, x + width - rx*0.448, y, x + width - rx, y);
	if(rx < width/2)
	  LineTo(x + rx, y);
	ClosePath();
  }
  End();
}

void wxSVGCanvasPath::Init(wxSVGCircleElement& element)
{
  double cx = element.GetCx().GetBaseVal();
  double cy = element.GetCy().GetBaseVal();
  double r = element.GetR().GetBaseVal();
  double len = 0.55228474983079356;
  double cos4[] = {1.0, 0.0, -1.0, 0.0, 1.0};
  double sin4[] = {0.0, 1.0, 0.0, -1.0, 0.0};
  
  MoveTo(cx + r, cy);
  
  for (int i = 1; i<5; i++)
  {
	CurveToCubic(
	  cx + (cos4[i-1] + len*cos4[i])*r,
	  cy + (sin4[i-1] + len*sin4[i])*r,
	  cx + (cos4[i] + len*cos4[i-1])*r,
	  cy + (sin4[i] + len*sin4[i-1])*r,
	  cx + (cos4[i])*r,
	  cy + (sin4[i])*r);
  }
  End();
}

void wxSVGCanvasPath::Init(wxSVGEllipseElement& element)
{
  double cx = element.GetCx().GetBaseVal();
  double cy = element.GetCy().GetBaseVal();
  double rx = element.GetRx().GetBaseVal();
  double ry = element.GetRy().GetBaseVal();
  double len = 0.55228474983079356;
  double cos4[] = {1.0, 0.0, -1.0, 0.0, 1.0};
  double sin4[] = {0.0, 1.0, 0.0, -1.0, 0.0};
  
  MoveTo(cx + rx, cy);
  
  for (int i = 1; i<5; i++)
  {
	CurveToCubic(
	  cx + (cos4[i-1] + len*cos4[i])*rx,
	  cy + (sin4[i-1] + len*sin4[i])*ry,
	  cx + (cos4[i] + len*cos4[i-1])*rx,
	  cy + (sin4[i] + len*sin4[i-1])*ry,
	  cx + (cos4[i])*rx,
	  cy + (sin4[i])*ry);
  }
  End();
}

void wxSVGCanvasPath::Init(wxSVGPathElement& element)
{
  const wxSVGPathSegList& segList = element.GetPathSegList();
  for (int i = 0; i < (int)segList.Count(); i++)
  {
	switch (segList[i].GetPathSegType())
	{
	  case wxPATHSEG_MOVETO_ABS:
	  {
		wxSVGPathSegMovetoAbs& seg = (wxSVGPathSegMovetoAbs&)(segList[i]);
		MoveTo(seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_MOVETO_REL:
	  {
		wxSVGPathSegMovetoRel& seg = (wxSVGPathSegMovetoRel&)(segList[i]);
		MoveTo(seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_LINETO_ABS:
	  {
		wxSVGPathSegLinetoAbs& seg = (wxSVGPathSegLinetoAbs&)segList[i];
		LineTo(seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_LINETO_REL:
	  {
		wxSVGPathSegLinetoRel& seg = (wxSVGPathSegLinetoRel&)segList[i];
		LineTo(seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_LINETO_HORIZONTAL_ABS:
	  {
		wxSVGPathSegLinetoHorizontalAbs& seg = (wxSVGPathSegLinetoHorizontalAbs&)segList[i];
		LineToHorizontal(seg.GetX());
		break;
	  }
	  case wxPATHSEG_LINETO_HORIZONTAL_REL:
	  {
		wxSVGPathSegLinetoHorizontalRel& seg = (wxSVGPathSegLinetoHorizontalRel&)segList[i];
		LineToHorizontal(seg.GetX(), true);
		break;
	  }
	  case wxPATHSEG_LINETO_VERTICAL_ABS:
	  {
		wxSVGPathSegLinetoVerticalAbs& seg = (wxSVGPathSegLinetoVerticalAbs&)segList[i];
		LineToVertical(seg.GetY());
		break;
	  }
	  case wxPATHSEG_LINETO_VERTICAL_REL:
	  {
		wxSVGPathSegLinetoVerticalRel& seg = (wxSVGPathSegLinetoVerticalRel&)segList[i];
		LineToVertical(seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_CURVETO_CUBIC_ABS:
	  {
		wxSVGPathSegCurvetoCubicAbs& seg = (wxSVGPathSegCurvetoCubicAbs&)segList[i];
		CurveToCubic(seg.GetX1(), seg.GetY1(), seg.GetX2(), seg.GetY2(), seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_CURVETO_CUBIC_REL:
	  {
		wxSVGPathSegCurvetoCubicRel& seg = (wxSVGPathSegCurvetoCubicRel&)segList[i];
		CurveToCubic(seg.GetX1(), seg.GetY1(), seg.GetX2(), seg.GetY2(), seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_CURVETO_CUBIC_SMOOTH_ABS:
	  {
		wxSVGPathSegCurvetoCubicSmoothAbs& seg = (wxSVGPathSegCurvetoCubicSmoothAbs&)segList[i];
		CurveToCubicSmooth(seg.GetX2(), seg.GetY2(), seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_CURVETO_CUBIC_SMOOTH_REL:
	  {
		wxSVGPathSegCurvetoCubicSmoothRel& seg = (wxSVGPathSegCurvetoCubicSmoothRel&)segList[i];
		CurveToCubicSmooth(seg.GetX2(), seg.GetY2(), seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_CURVETO_QUADRATIC_ABS:
	  {
		wxSVGPathSegCurvetoQuadraticAbs& seg = (wxSVGPathSegCurvetoQuadraticAbs&)segList[i];
		CurveToQuadratic(seg.GetX1(), seg.GetY1(), seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_CURVETO_QUADRATIC_REL:
	  {
		wxSVGPathSegCurvetoQuadraticRel& seg = (wxSVGPathSegCurvetoQuadraticRel&)segList[i];
		CurveToQuadratic(seg.GetX1(), seg.GetY1(), seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS:
	  {
		wxSVGPathSegCurvetoQuadraticSmoothAbs& seg = (wxSVGPathSegCurvetoQuadraticSmoothAbs&)segList[i];
		CurveToQuadraticSmooth(seg.GetX(), seg.GetY());
		break;
	  }
	  case wxPATHSEG_CURVETO_QUADRATIC_SMOOTH_REL:
	  {
		wxSVGPathSegCurvetoQuadraticSmoothRel& seg = (wxSVGPathSegCurvetoQuadraticSmoothRel&)segList[i];
		CurveToQuadraticSmooth(seg.GetX(), seg.GetY(), true);
		break;
	  }
	  case wxPATHSEG_ARC_ABS:
	  {
		wxSVGPathSegArcAbs& seg = (wxSVGPathSegArcAbs&)segList[i];
		Arc(seg.GetX(), seg.GetY(), seg.GetR1(), seg.GetR2(), seg.GetAngle(),
			seg.GetLargeArcFlag(), seg.GetSweepFlag());
		break;
	  }
	  case wxPATHSEG_ARC_REL:
	  {
		wxSVGPathSegArcRel& seg = (wxSVGPathSegArcRel&)segList[i];
		Arc(seg.GetX(), seg.GetY(), seg.GetR1(), seg.GetR2(), seg.GetAngle(),
			seg.GetLargeArcFlag(), seg.GetSweepFlag(), true);
		break;
	  }
	  case wxPATHSEG_CLOSEPATH:
		ClosePath();
		break;
	  case wxPATHSEG_UNKNOWN:
		break;
	}
  }
  End();
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////// Path functions /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void wxSVGCanvasPath::MoveTo(double x, double y, bool relative)
{
  if (relative)
  {
	x += m_curx;
	y += m_cury;
  }
  MoveToImpl(x, y);
  m_begx = m_curx = x;
  m_begy = m_cury = y;
}

void wxSVGCanvasPath::LineTo(double x, double y, bool relative)
{
  if (relative)
  {
	x += m_curx;
	y += m_cury;
  }
  LineToImpl(x, y);
  m_curx = x;
  m_cury = y;
}

void wxSVGCanvasPath::LineToHorizontal(double x, bool relative)
{
  if (relative)
	x += m_curx;
  LineToImpl(x, m_cury);
  m_curx = x;
}

void wxSVGCanvasPath::LineToVertical(double y, bool relative)
{
  if (relative)
	y += m_cury;
  LineToImpl(m_curx, y);
  m_cury = y;
}

void wxSVGCanvasPath::CurveToCubic(double x1, double y1, double x2, double y2, double x, double y, bool relative)
{
  if (relative)
  {
	x1 += m_curx;
	y1 += m_cury;
	x2 += m_curx;
	y2 += m_cury;
	x += m_curx;
	y += m_cury;
  }
  CurveToCubicImpl(x1, y1, x2, y2, x, y);
  m_curx = x;
  m_cury = y;
  m_cubicx = 2*m_curx - x2;
  m_cubicy = 2*m_cury - y2;
}

void wxSVGCanvasPath::CurveToCubicSmooth(double x2, double y2, double x, double y, bool relative)
{
  if (relative)
  {
	x2 += m_curx;
	y2 += m_cury;
	x += m_curx;
	y += m_cury;
  }
  CurveToCubicImpl(m_cubicx, m_cubicy, x2, y2, x, y);
  m_curx = x;
  m_cury = y;
  m_cubicx = 2*m_curx - x2;
  m_cubicy = 2*m_cury - y2;
}

void wxSVGCanvasPath::CurveToQuadratic(double x1, double y1, double x, double y, bool relative)
{
  if (relative)
  {
	x1 += m_curx;
	y1 += m_cury;
	x += m_curx;
	y += m_cury;
  }
  m_quadx = 2*x - x1;
  m_quady = 2*y - y1;
  double x2 = (x + 2*x1)/3;
  double y2 = (y + 2*y1)/3;
  x1 = (m_curx + 2*x1)/3;
  y1 = (m_cury + 2*y1)/3;
  CurveToCubicImpl(x1, y1, x2, y2, x, y);
  m_curx = x;
  m_cury = y;
}

void wxSVGCanvasPath::CurveToQuadraticSmooth(double x, double y, bool relative)
{
  if (relative)
  {
	x += m_curx;
	y += m_cury;
  }
  double x1 = (m_curx + 2*m_quadx)/3;
  double y1 = (m_cury + 2*m_quady)/3;
  double x2 = (x + 2*m_quadx)/3;
  double y2 = (y + 2*m_quady)/3;
  CurveToCubicImpl(x1, y1, x2, y2, x, y);
  m_curx = x;
  m_cury = y;
  m_quadx = 2*x - m_quadx;
  m_quady = 2*y - m_quady;
}

// This works by converting the SVG arc to "simple" beziers.
// For each bezier found a svgToCurve call is done.
// Adapted from Niko's code in kdelibs/kdecore/svgicons.
void wxSVGCanvasPath::Arc(double x, double y, double r1, double r2,
  double angle, bool largeArcFlag, bool sweepFlag,
  bool relative)
{
  if (relative)
  {
	x += m_curx;
	y += m_cury;
  }
  
  double sin_th = sin(angle*(M_PI/180.0));
  double cos_th = cos(angle*(M_PI/180.0));

  double dx = (m_curx - x)/2.0;
  double dy = (m_cury - y)/2.0;
	  
  double _x1 =  cos_th*dx + sin_th*dy;
  double _y1 = -sin_th*dx + cos_th*dy;
  double Pr1 = r1*r1;
  double Pr2 = r2*r2;
  double Px = _x1*_x1;
  double Py = _y1*_y1;

  // Spec : check if radii are large enough
  double check = Px/Pr1 + Py/Pr2;
  if (check > 1)
  {
	r1 = r1*sqrt(check);
	r2 = r2*sqrt(check);
  }

  double a00 = cos_th/r1;
  double a01 = sin_th/r1;
  double a10 = -sin_th/r2;
  double a11 = cos_th/r2;

  double x0 = a00*m_curx + a01*m_cury;
  double y0 = a10*m_curx + a11*m_cury;

  double x1 = a00*x + a01*y;
  double y1 = a10*x + a11*y;

  /* (x0, y0) is current point in transformed coordinate space.
	 (x1, y1) is new point in transformed coordinate space.
	 The arc fits a unit-radius circle in this space. */

  double d = (x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0);

  double sfactor_sq = 1.0/d - 0.25;

  if (sfactor_sq < 0)
	sfactor_sq = 0;

  double sfactor = sqrt(sfactor_sq);

  if (sweepFlag == largeArcFlag)
	sfactor = -sfactor;

  double xc = 0.5*(x0 + x1) - sfactor*(y1 - y0);
  double yc = 0.5*(y0 + y1) + sfactor*(x1 - x0);

  /* (xc, yc) is center of the circle. */
  double th0 = atan2(y0 - yc, x0 - xc);
  double th1 = atan2(y1 - yc, x1 - xc);

  double th_arc = th1 - th0;
  if (th_arc < 0 && sweepFlag)
	th_arc += 2*M_PI;
  else if (th_arc > 0 && !sweepFlag)
	th_arc -= 2*M_PI;

  int n_segs = (int) (int) ceil(fabs(th_arc/(M_PI*0.5 + 0.001)));

  for (int i = 0; i < n_segs; i++)
  {
	double sin_th, cos_th;
	double a00, a01, a10, a11;
	double x1, y1, x2, y2, x3, y3;
	double t;
	double th_half;

	double _th0 = th0 + i*th_arc/n_segs;
	double _th1 = th0 + (i + 1)*th_arc/n_segs;

	sin_th = sin(angle*(M_PI/180.0));
	cos_th = cos(angle*(M_PI/180.0));

	/* inverse transform compared with rsvg_path_arc */
	a00 = cos_th*r1;
	a01 = -sin_th*r2;
	a10 = sin_th*r1;
	a11 = cos_th*r2;

	th_half = 0.5*(_th1 - _th0);
	t = (8.0/3.0)*sin(th_half*0.5)*sin(th_half*0.5)/sin(th_half);
	x1 = xc + cos(_th0) - t*sin(_th0);
	y1 = yc + sin(_th0) + t*cos(_th0);
	x3 = xc + cos(_th1);
	y3 = yc + sin(_th1);
	x2 = x3 + t*sin(_th1);
	y2 = y3 - t*cos(_th1);

	CurveToCubicImpl(a00*x1 + a01*y1, a10*x1 + a11*y1, a00*x2 + a01*y2,
	  a10*x2 + a11*y2, a00*x3 + a01*y3, a10*x3 + a11*y3);
  }
  m_curx = x;
  m_cury = y;
}

bool wxSVGCanvasPath::ClosePath()
{
  bool isClosed = ClosePathImpl();
  m_curx = m_begx;
  m_cury = m_begy;
  return isClosed;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////// wxSVGCanvasText //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

wxSVGCanvasText::wxSVGCanvasText(): wxSVGCanvasItem(wxSVG_CANVAS_ITEM_TEXT)
{
  m_tx = m_ty = 0;
}

void wxSVGCanvasText::Init(wxSVGTextElement& element,
  wxCSSStyleDeclaration& style)
{
  m_tx = element.GetX().GetBaseVal().Count() ? element.GetX().GetBaseVal()[0] : wxSVGLength(0);
  m_ty = element.GetY().GetBaseVal().Count() ? element.GetY().GetBaseVal()[0] : wxSVGLength(0);
  InitChildren(element, style);
  EndChunk();
}

void wxSVGCanvasText::Init(wxSVGTSpanElement& element,
  wxCSSStyleDeclaration& style)
{
  if (element.GetX().GetBaseVal().Count())
	EndChunk();
  
  if (element.GetX().GetBaseVal().Count())
	m_tx = element.GetX().GetBaseVal()[0];
  if (element.GetY().GetBaseVal().Count())
	m_ty = element.GetY().GetBaseVal()[0];
  InitChildren(element, style);
  
  if (element.GetX().GetBaseVal().Count())
	EndChunk();
}

void wxSVGCanvasText::InitChildren(wxSVGTextPositioningElement& element,
  wxCSSStyleDeclaration& style)
{
  wxSVGElement* elem = (wxSVGElement*) element.GetChildren();
  while (elem)
  {
	if (elem->GetType() == wxXML_TEXT_NODE)
	{
	  BeginChunk(style);
	  wxString text = elem->GetContent();
	  InitText(text);
	}
	if (elem->GetType() == wxXML_ELEMENT_NODE &&
		elem->GetDtd() == wxSVG_TSPAN_ELEMENT)
	{
	  wxSVGTSpanElement& tElem = (wxSVGTSpanElement&)*elem;
	  wxCSSStyleDeclaration tStyle(style);
	  tStyle.Add(tElem.GetStyle());
	  Init(tElem, tStyle);
	}
	elem = (wxSVGElement*) elem->GetNext();
  }
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// wxSVGCanvasImage //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void wxSVGCanvasImage::Init(wxSVGImageElement& element)
{
  m_x = element.GetX().GetAnimVal();
  m_y = element.GetY().GetAnimVal();
  m_width = element.GetWidth().GetBaseVal();
  m_height = element.GetHeight().GetBaseVal();
  m_href = element.GetHref();
  wxSVGCanvasImage* prevItem = (wxSVGCanvasImage*) element.GetCanvasItem();
  if (prevItem != NULL && prevItem->m_href == m_href)
    m_image = prevItem->m_image;
  else
    m_image.LoadFile(m_href);
}
