//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasItem.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/09
// RCS-ID:      $Id: SVGCanvasItem.cpp,v 1.19 2008-02-03 17:41:37 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "SVGCanvasItem.h"
#include "SVGCanvas.h"
#include <math.h>
#include <wx/log.h>

#ifdef USE_FFMPEG
#include <wxSVG/mediadec_ffmpeg.h>
#endif

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
  
  MoveTo(element.GetX1().GetAnimVal(), element.GetY1().GetAnimVal());
  LineTo(element.GetX2().GetAnimVal(), element.GetY2().GetAnimVal());
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
  double x = element.GetX().GetAnimVal();
  double y = element.GetY().GetAnimVal();
  double width = element.GetWidth().GetAnimVal();
  double height = element.GetHeight().GetAnimVal();
  double rx = element.GetRx().GetAnimVal();
  double ry = element.GetRy().GetAnimVal();
  
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
  double cx = element.GetCx().GetAnimVal();
  double cy = element.GetCy().GetAnimVal();
  double r = element.GetR().GetAnimVal();
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
  double cx = element.GetCx().GetAnimVal();
  double cy = element.GetCy().GetAnimVal();
  double rx = element.GetRx().GetAnimVal();
  double ry = element.GetRy().GetAnimVal();
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

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxSVGCanvasTextCharList);
WX_DEFINE_OBJARRAY(wxSVGCanvasTextChunkList);

wxSVGCanvasText::wxSVGCanvasText(wxSVGCanvas* canvas):
  wxSVGCanvasItem(wxSVG_CANVAS_ITEM_TEXT), m_canvas(canvas)
{
  m_char = NULL;
  m_tx = m_ty = 0;
  m_textAnchor = wxCSS_VALUE_START;
  m_dominantBaseline = wxCSS_VALUE_AUTO;
}

wxSVGCanvasText::~wxSVGCanvasText()
{
  for (unsigned int i=0; i<m_chunks.Count(); i++)
  	for (unsigned int j=0; j<m_chunks[i].chars.Count(); j++)
		delete m_chunks[i].chars[j].path;
}

void wxSVGCanvasText::Init(wxSVGTextElement& element,
  const wxCSSStyleDeclaration& style)
{
  m_tx = element.GetX().GetAnimVal().Count() ? element.GetX().GetAnimVal()[0] : wxSVGLength(0);
  m_ty = element.GetY().GetAnimVal().Count() ? element.GetY().GetAnimVal()[0] : wxSVGLength(0);
  InitChildren(element, style);
  EndTextAnchor();
}

void wxSVGCanvasText::Init(wxSVGTSpanElement& element,
  const wxCSSStyleDeclaration& style)
{
  if (element.GetX().GetAnimVal().Count())
	EndTextAnchor();
  
  if (element.GetX().GetAnimVal().Count())
	m_tx = element.GetX().GetAnimVal()[0];
  if (element.GetY().GetAnimVal().Count())
	m_ty = element.GetY().GetAnimVal()[0];
  InitChildren(element, style);
  
  if (element.GetX().GetAnimVal().Count())
	EndTextAnchor();
}

void wxSVGCanvasText::InitChildren(wxSVGTextPositioningElement& element,
  const wxCSSStyleDeclaration& style)
{
  wxSVGElement* elem = (wxSVGElement*) element.GetChildren();
  while (elem)
  {
	if (elem->GetType() == wxSVGXML_TEXT_NODE)
	{
	  BeginChunk(style);
	  InitText(elem->GetContent(), style);
	}
	if (elem->GetType() == wxSVGXML_ELEMENT_NODE &&
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

void wxSVGCanvasText::BeginChunk(const wxCSSStyleDeclaration& style)
{
  wxSVGCanvasTextChunk* chunk = new wxSVGCanvasTextChunk;
  chunk->style.Add(style);
  m_chunks.Add(chunk);
  
  if (style.HasTextAnchor() && m_textAnchor == wxCSS_VALUE_START)
  {
	m_textAnchor = style.GetTextAnchor();
	m_textAnchorBeginIndex = m_chunks.Count()-1;
	// save x-pos: if text anchor is wxCSS_VALUE_END, we will continue from this point
	m_textAnchorBeginPos = m_tx;
  }
  if (style.HasDominantBaseline() && (m_dominantBaseline == wxCSS_VALUE_AUTO
  		|| m_dominantBaseline == wxCSS_VALUE_ALPHABETIC))
  {
	m_dominantBaseline = style.GetDominantBaseline();
	m_dominantBaselineBeginIndex = m_chunks.Count()-1;
  }
}

void wxSVGCanvasText::BeginChar()
{
	m_char = new wxSVGCanvasTextChar;
	m_char->path = m_canvas->CreateCanvasPath();
	m_chunks[m_chunks.GetCount()-1].chars.Add(m_char);
}

void wxSVGCanvasText::EndChar()
{
	m_char->path->End();
}

void wxSVGCanvasText::EndTextAnchor()
{
  if (m_textAnchor != wxCSS_VALUE_START)
  {
	for (int i=m_textAnchorBeginIndex; i<(int)m_chunks.Count(); i++)
	{
	  wxSVGCanvasTextChunk& chunk = m_chunks[i];
	  if (m_textAnchor == wxCSS_VALUE_END)
		chunk.matrix = chunk.matrix.Translate(m_textAnchorBeginPos-m_tx, 0);
	  else if (m_textAnchor == wxCSS_VALUE_MIDDLE)
		chunk.matrix = chunk.matrix.Translate((m_textAnchorBeginPos-m_tx)/2, 0);
	}
	if (m_textAnchor == wxCSS_VALUE_END)
	  m_tx = m_textAnchorBeginPos;
	else if (m_textAnchor == wxCSS_VALUE_MIDDLE)
	  m_tx -= (m_textAnchorBeginPos-m_tx)/2;
	m_textAnchor = wxCSS_VALUE_START;
  }
  if (m_dominantBaseline != wxCSS_VALUE_AUTO
  		&& m_dominantBaseline != wxCSS_VALUE_ALPHABETIC)
  {
	for (int i=m_dominantBaselineBeginIndex; i<(int)m_chunks.Count(); i++)
	{
		wxSVGCanvasTextChunk& chunk = m_chunks[i];
		wxSVGRect chunkBBox = chunk.GetBBox();
		if (chunkBBox.IsEmpty())
			continue;
		if (m_dominantBaseline == wxCSS_VALUE_MIDDLE
	  			|| m_dominantBaseline == wxCSS_VALUE_CENTRAL)
			chunk.matrix = chunk.matrix.Translate(0,
					m_ty - chunkBBox.GetY() - chunkBBox.GetHeight()/2);
		else if (m_dominantBaseline == wxCSS_VALUE_TEXT_AFTER_EDGE)
			chunk.matrix = chunk.matrix.Translate(0,
					m_ty - chunkBBox.GetY());
		else if (m_dominantBaseline == wxCSS_VALUE_TEXT_BEFORE_EDGE)
			chunk.matrix = chunk.matrix.Translate(0,
					m_ty - chunkBBox.GetY() - chunkBBox.GetHeight());
	}
	m_dominantBaseline = wxCSS_VALUE_AUTO;
  }
}

extern wxSVGRect TransformRect(wxSVGRect rect, wxSVGMatrix& matrix);

wxSVGRect wxSVGCanvasTextChunk::GetBBox(const wxSVGMatrix& matrix)
{
  wxSVGRect bbox;
  for (int i=0; i<(int)chars.Count(); i++)
  {
    wxSVGRect elemBBox = chars[i].path->GetBBox(matrix);
    if (elemBBox.IsEmpty())
      elemBBox = &matrix ? chars[i].bbox.MatrixTransform(matrix) : chars[i].bbox;
	if (i == 0)
	  bbox = elemBBox;
	else
	{
	  if (bbox.GetX() > elemBBox.GetX())
	  {
		bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - elemBBox.GetX());
		bbox.SetX(elemBBox.GetX());
	  }
	  if (bbox.GetY() > elemBBox.GetY())
	  {
		bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - elemBBox.GetY());
		bbox.SetY(elemBBox.GetY());
	  }
	  if (bbox.GetX() + bbox.GetWidth() < elemBBox.GetX() + elemBBox.GetWidth())
		bbox.SetWidth(elemBBox.GetX() + elemBBox.GetWidth() - bbox.GetX());
	  if (bbox.GetY() + bbox.GetHeight() < elemBBox.GetY() + elemBBox.GetHeight())
		bbox.SetHeight(elemBBox.GetY() + elemBBox.GetHeight() - bbox.GetY());
	}
  }
  return bbox;
}

wxSVGRect wxSVGCanvasText::GetBBox(const wxSVGMatrix& matrix)
{
  wxSVGRect bbox;
  for (int i=0; i<(int)m_chunks.Count(); i++)
  {
    wxSVGMatrix tmpMatrix = m_chunks[i].matrix;
    if (&matrix)
      tmpMatrix = ((wxSVGMatrix&) matrix).Multiply(m_chunks[i].matrix);
    wxSVGRect elemBBox = m_chunks[i].GetBBox(tmpMatrix);
	if (i == 0)
	  bbox = elemBBox;
	else
	{
	  if (bbox.GetX() > elemBBox.GetX())
	  {
		bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - elemBBox.GetX());
		bbox.SetX(elemBBox.GetX());
	  }
	  if (bbox.GetY() > elemBBox.GetY())
	  {
		bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - elemBBox.GetY());
		bbox.SetY(elemBBox.GetY());
	  }
	  if (bbox.GetX() + bbox.GetWidth() < elemBBox.GetX() + elemBBox.GetWidth())
		bbox.SetWidth(elemBBox.GetX() + elemBBox.GetWidth() - bbox.GetX());
	  if (bbox.GetY() + bbox.GetHeight() < elemBBox.GetY() + elemBBox.GetHeight())
		bbox.SetHeight(elemBBox.GetY() + elemBBox.GetHeight() - bbox.GetY());
	}
  }
  return bbox;
}

long wxSVGCanvasText::GetNumberOfChars()
{
	long res = 0;
	for (int i=0; i<(int)m_chunks.Count(); i++)
		res += m_chunks[i].chars.GetCount();
	return res;
}

wxSVGCanvasTextChunk* wxSVGCanvasText::getChunk(unsigned long& charnum)
{
	for (int i=0; i<(int)m_chunks.Count(); i++)
	{
		if (charnum<m_chunks[i].chars.GetCount())
			return &m_chunks[i];
		charnum -= m_chunks[i].chars.GetCount();
	}
	return NULL;
}

double wxSVGCanvasText::GetComputedTextLength()
{
	if (m_chunks.Count() && m_chunks[0].chars.GetCount())
	{
		wxSVGCanvasTextChunk& firstChunk = m_chunks[0]; 
		wxSVGRect bboxFirst = firstChunk.chars[0].path->GetBBox();
		if (bboxFirst.IsEmpty())
			bboxFirst = firstChunk.chars[0].bbox;
		bboxFirst = bboxFirst.MatrixTransform(firstChunk.matrix);
		wxSVGCanvasTextChunk& lastChunk = m_chunks[m_chunks.Count()-1];
		wxSVGRect bboxLast = lastChunk.chars[lastChunk.chars.Count()-1].path->GetBBox();
		if (bboxLast.IsEmpty())
			bboxLast = lastChunk.chars[lastChunk.chars.Count()-1].bbox;
		bboxLast = bboxLast.MatrixTransform(lastChunk.matrix);
		return (double)(bboxLast.GetX() + bboxLast.GetWidth() - bboxFirst.GetX());
	}
	return 0;
}

double wxSVGCanvasText::GetSubStringLength(unsigned long charnum, unsigned long nchars)
{
	unsigned long lastCharnum = charnum + nchars - 1;
	wxSVGCanvasTextChunk* firstChunk = getChunk(charnum);
	wxSVGCanvasTextChunk* lastChunk = getChunk(lastCharnum);
	if (firstChunk != NULL && lastChunk != NULL)
	{
		wxSVGRect bboxFirst = firstChunk->chars[charnum].path->GetBBox();
		if (bboxFirst.IsEmpty())
			bboxFirst = firstChunk->chars[charnum].bbox;
		bboxFirst = bboxFirst.MatrixTransform(firstChunk->matrix);
		wxSVGRect bboxLast = lastChunk->chars[lastCharnum].path->GetBBox();
		if (bboxLast.IsEmpty())
			bboxLast = lastChunk->chars[lastCharnum].bbox;
		bboxLast = bboxLast.MatrixTransform(lastChunk->matrix);
		return (double)(bboxLast.GetX() + bboxLast.GetWidth() - bboxFirst.GetX());
	}
	return 0;
}

wxSVGPoint wxSVGCanvasText::GetStartPositionOfChar(unsigned long charnum)
{
	wxSVGCanvasTextChunk* chunk = getChunk(charnum);
	if (chunk != NULL)
	{
		wxSVGRect bbox = chunk->chars[charnum].path->GetBBox();
		if (bbox.IsEmpty())
			bbox = chunk->chars[charnum].bbox;
		bbox = bbox.MatrixTransform(chunk->matrix);
		return wxSVGPoint(bbox.GetX(), bbox.GetY());
	}
	return wxSVGPoint(0, 0);
}

wxSVGPoint wxSVGCanvasText::GetEndPositionOfChar(unsigned long charnum)
{
    wxSVGCanvasTextChunk* chunk = getChunk(charnum);
	if (chunk != NULL)
	{
		wxSVGRect bbox = chunk->chars[charnum].path->GetBBox();
		if (bbox.IsEmpty())
			bbox = chunk->chars[charnum].bbox;
		bbox = bbox.MatrixTransform(chunk->matrix);
        return wxSVGPoint(bbox.GetX() + bbox.GetWidth(), bbox.GetY());
    }
    return wxSVGPoint(0, 0);
}

wxSVGRect wxSVGCanvasText::GetExtentOfChar(unsigned long charnum)
{
    wxSVGCanvasTextChunk* chunk = getChunk(charnum);
	if (chunk != NULL)
	{
		wxSVGRect bbox = chunk->chars[charnum].path->GetBBox();
		if (bbox.IsEmpty())
			bbox = chunk->chars[charnum].bbox;
		return bbox.MatrixTransform(chunk->matrix);
    }
    return wxSVGRect(0, 0, 0, 0);
}

long wxSVGCanvasText::GetCharNumAtPosition(const wxSVGPoint& point)
{
	double X = point.GetX();
	double Y = point.GetY();
	for (int n = 0; n < (int)m_chunks.Count(); n++)
	{
		wxSVGCanvasTextChunk& chunk = m_chunks[n];
		wxSVGRect bbox;
		for (int i = 0; i < (int)chunk.chars.Count();i++)
		{
			bbox = chunk.chars[i].path->GetBBox().MatrixTransform(chunk.matrix);
			double Xmin = bbox.GetX();
			double Xmax = Xmin + bbox.GetWidth(); 
			double Ymin = bbox.GetY();
			double Ymax = Ymin + bbox.GetHeight(); 
			if (X >= Xmin && X <= Xmax && Y >= Ymin && Y <= Ymax)
				return i;
		}
	}
    return -1;
}

double wxSVGCanvasText::GetRotationOfChar(unsigned long charnum)
{
    return 0;
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////// wxSVGCanvasImage //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void wxSVGCanvasImage::Init(wxSVGImageElement& element)
{
  m_x = element.GetX().GetAnimVal();
  m_y = element.GetY().GetAnimVal();
  m_width = element.GetWidth().GetAnimVal();
  m_height = element.GetHeight().GetAnimVal();
  m_href = element.GetHref();
  wxSVGCanvasImage* prevItem = (wxSVGCanvasImage*) element.GetCanvasItem();
  if (prevItem != NULL && prevItem->m_href == m_href)
    m_image = prevItem->m_image;
  else if (m_href.length())
  {
    long pos = 0;
    wxString filename = m_href;
    if (m_href.Find(wxT('#')) != wxNOT_FOUND)
    {
      filename = m_href.Before(wxT('#'));
      m_href.After(wxT('#')).ToLong(&pos);
    }
#ifdef USE_FFMPEG
    bool log = wxLog::EnableLogging(false);
    m_image.LoadFile(filename);
    wxLog::EnableLogging(log);
    if (!m_image.Ok())
    {
      wxFfmpegMediaDecoder decoder;
      if (decoder.Load(filename))
      {
        double duration = decoder.GetDuration();
        if (pos > 0 || duration > 0) {
          decoder.SetPosition(pos > 0 ? pos/1000 : duration * 0.05);
        }
        m_image = decoder.GetNextFrame();
        decoder.Close();
      }
    }
#else
    m_image.LoadFile(filename);
#endif
  }
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// wxSVGCanvasVideo //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

wxSVGCanvasVideo::wxSVGCanvasVideo(): wxSVGCanvasImage(wxSVG_CANVAS_ITEM_VIDEO)
{
#ifdef USE_FFMPEG
  m_mediaDecoder = NULL;
#endif
}

wxSVGCanvasVideo::~wxSVGCanvasVideo()
{
#ifdef USE_FFMPEG
  if (m_mediaDecoder)
    delete m_mediaDecoder;
#endif
}

void wxSVGCanvasVideo::Init(wxSVGVideoElement& element)
{
  m_x = element.GetX().GetAnimVal();
  m_y = element.GetY().GetAnimVal();
  m_width = element.GetWidth().GetAnimVal();
  m_height = element.GetHeight().GetAnimVal();
  m_href = element.GetHref();
  m_time = ((wxSVGDocument*)element.GetOwnerDocument())->GetCurrentTime();
  wxSVGCanvasVideo* prevItem = (wxSVGCanvasVideo*) element.GetCanvasItem();
  if (prevItem != NULL && prevItem->m_href == m_href)
  {
#ifdef USE_FFMPEG
    m_mediaDecoder = prevItem->m_mediaDecoder;
    prevItem->m_mediaDecoder = NULL;
#endif
    m_duration = prevItem->m_duration;
#ifdef USE_FFMPEG
    if (prevItem->m_time != m_time)
    {
      if (m_time > 0)
        m_mediaDecoder->SetPosition(m_time);
      m_image = m_mediaDecoder->GetNextFrame();
    } else
#endif
      m_image = prevItem->m_image;
  }
  else if (m_href.length())
  {
#ifdef USE_FFMPEG
    m_mediaDecoder = new wxFfmpegMediaDecoder();
    if (m_mediaDecoder->Load(m_href)) {
      m_duration = m_mediaDecoder->GetDuration();
      if (m_time > 0)
        m_mediaDecoder->SetPosition(m_time);
      m_image = m_mediaDecoder->GetNextFrame();
    }
    else
    {
      delete m_mediaDecoder;
      m_mediaDecoder = NULL;
    }
#endif
  }
}
