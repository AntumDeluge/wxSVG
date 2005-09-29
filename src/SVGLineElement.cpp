//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLineElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLineElement.cpp,v 1.2.2.1 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLineElement.h"

wxSVGRect wxSVGLineElement::GetBBox()
{ 
  wxSVGMatrix matrix = GetCTM();
  wxSVGPoint p1 = wxSVGPoint(GetX1().GetBaseVal(), GetY1().GetBaseVal());
  wxSVGPoint p2 = wxSVGPoint(GetX2().GetBaseVal(), GetY2().GetBaseVal());
  p1 = p1.MatrixTransform(matrix);
  p2 = p2.MatrixTransform(matrix);
  
  double x1 = p1.GetX();
  double y1 = p1.GetY();
  double x2 = p2.GetX();
  double y2 = p2.GetY();

  wxSVGRect bbox(x1, y1, x2 - x1, y2 - y1);
  
  if (x1 > x2)
  {
	bbox.SetX(x2);
	bbox.SetWidth(x1 - x2);
  }
  
  if (y1 > y2)
  {
	bbox.SetY(y2);
	bbox.SetHeight(y1 - y2);
  }
  
  return bbox;
}

