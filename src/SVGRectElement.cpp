//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRectElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGRectElement.cpp,v 1.2.2.2 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRectElement.h"
#include "SVGAnimatedPoints.h"

#include "math.h"

const double pi = 3.1415926;

wxSVGRect wxSVGRectElement::GetBBox()
{
  double stroke_width = 0;
  if (GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE)
  	stroke_width = GetStrokeWidth();

  wxSVGMatrix matrix = GetCTM();
  wxSVGPointList points = wxSVGPointList();
  
  if (GetRx().GetBaseVal() == 0 || GetRy().GetBaseVal() == 0)
  {
  	double x1 = GetX().GetBaseVal() - stroke_width / 2;
  	double y1 = GetY().GetBaseVal() - stroke_width / 2;
  	double x2 = GetX().GetBaseVal() + GetWidth().GetBaseVal() + stroke_width / 2;
  	double y2 = GetY().GetBaseVal() + GetHeight().GetBaseVal() + stroke_width / 2;

  	points.Add(wxSVGPoint(x1, y1));
  	points.Add(wxSVGPoint(x1, y2));
  	points.Add(wxSVGPoint(x2, y2));
  	points.Add(wxSVGPoint(x2, y1));
  }
  else
  {
  	double angles[4];
  	angles[0] = atan((GetRy().GetBaseVal() * matrix.GetC()) / (GetRx().GetBaseVal() * matrix.GetA()));
  	angles[1] = atan((GetRy().GetBaseVal() * matrix.GetD()) / (GetRx().GetBaseVal() * matrix.GetB()));
  	angles[2] = angles[0] + pi;
  	angles[3] = angles[1] + pi;
  
  	int i;
  	for (i=0;i<4;i++)
  	{
  		if (angles[i] < 0 || angles[i] >= 3 * pi / 2)
			points.Add(wxSVGPoint(GetX().GetBaseVal() + GetWidth().GetBaseVal() - GetRx().GetBaseVal() * (1 - cos(angles[i])), 
				GetY().GetBaseVal() + GetHeight().GetBaseVal() - GetRy().GetBaseVal() * (1 + sin(angles[i]))));
		else if (angles[i] < pi / 2)
			points.Add(wxSVGPoint(GetX().GetBaseVal() + GetWidth().GetBaseVal() - GetRx().GetBaseVal() * (1 - cos(angles[i])), 
				GetY().GetBaseVal() + GetRy().GetBaseVal() * (1 - sin(angles[i]))));
		else if (angles[i] < pi)
			points.Add(wxSVGPoint(GetX().GetBaseVal() + GetRx().GetBaseVal() * (1 + cos(angles[i])), 
				GetY().GetBaseVal() + GetRy().GetBaseVal() * (1 - sin(angles[i]))));
		else if (angles[i] < 3 * pi / 2)
			points.Add(wxSVGPoint(GetX().GetBaseVal() + GetRx().GetBaseVal() * (1 + cos(angles[i])), 
				GetY().GetBaseVal() + GetHeight().GetBaseVal() - GetRy().GetBaseVal() * (1 + sin(angles[i]))));
  	}
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
  
  if (GetRx().GetBaseVal() == 0 || GetRy().GetBaseVal() == 0)
  {
  	return bbox;
  }
  else
  {
  	return wxSVGRect(bbox.GetX() - stroke_width / 2, bbox.GetY() - stroke_width / 2,
	  bbox.GetWidth() + stroke_width, bbox.GetHeight() + stroke_width);
  }
}
