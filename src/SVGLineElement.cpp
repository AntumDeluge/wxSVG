//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLineElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLineElement.cpp,v 1.2 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLineElement.h"

wxSVGRect wxSVGLineElement::GetBBox()
{
  double x1 = GetX1().GetBaseVal();
  double y1 = GetY1().GetBaseVal();
  double x2 = GetX2().GetBaseVal();
  double y2 = GetY2().GetBaseVal();
  
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

