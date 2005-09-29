//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPolylineElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGPolylineElement.cpp,v 1.2.2.1 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGPolylineElement.h"

wxSVGRect wxSVGPolylineElement::GetBBox()
{
  const wxSVGPointList& points = GetPoints();
  if (points.Count() == 0)
	return wxSVGRect();
  
  wxSVGMatrix matrix = GetCTM();
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
  
  return bbox;
}

