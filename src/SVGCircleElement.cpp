//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCircleElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGCircleElement.cpp,v 1.3.2.1 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCircleElement.h"
#include "SVGAnimatedPoints.h"

#include "math.h"

const double pi = 3.1415926;

wxSVGRect wxSVGCircleElement::GetBBox()
{
  double stroke_width = 0;
  if (GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE)
  	stroke_width = GetStrokeWidth();
  
  wxSVGMatrix matrix = GetCTM();
  
  double angles[4];
  angles[0] = atan(matrix.GetC() / matrix.GetA());
  angles[1] = atan(matrix.GetD() / matrix.GetB());
  angles[2] = angles[0] + pi;
  angles[3] = angles[1] + pi;
  
  wxSVGPointList points = wxSVGPointList();
  int i;
  for (i=0;i<4;i++)
  {
  	points.Add(wxSVGPoint(GetR().GetBaseVal() * cos(angles[i]) + GetCx().GetBaseVal(),
  		GetR().GetBaseVal() * sin(angles[i]) + GetCy().GetBaseVal()));
  }

  wxSVGPoint p0 = points[0].MatrixTransform(matrix);
  wxSVGRect bbox(p0.GetX(), p0.GetY(), 0, 0);
  
  wxSVGPoint pi = wxSVGPoint();
  for (int i = 1; i<(int)points.Count(); i++)
  {
  	pi = points[i].MatrixTransform(matrix);
	if (bbox.GetX() > pi.GetX())
	{
	  bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - pi.GetX());
	  bbox.SetX(pi.GetX());
	}
	if (bbox.GetY() > pi.GetY())
	{
	  bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - pi.GetY());
	  bbox.SetY(pi.GetY());
	}
	
	if (bbox.GetX() + bbox.GetWidth() < pi.GetX())
	  bbox.SetWidth(pi.GetX() - bbox.GetX());
	if (bbox.GetY() + bbox.GetHeight() < pi.GetY())
	  bbox.SetHeight(pi.GetY() - bbox.GetY());
  }
  
  return wxSVGRect(bbox.GetX() - stroke_width / 2, bbox.GetY() - stroke_width / 2,
	bbox.GetWidth() + stroke_width, bbox.GetHeight() + stroke_width);
}
