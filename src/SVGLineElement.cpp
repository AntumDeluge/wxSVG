//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLineElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLineElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLineElement.h"

wxSVGRect wxSVGLineElement::GetBBox()
{
  wxSVGRect bbox(GetX1(), GetY1(), GetX2() - GetX1(), GetY2() - GetY1());
  
  if (GetX1() > GetX2())
  {
	bbox.SetX(GetX2());
	bbox.SetWidth(GetX1() - GetX2());
  }
  
  if (GetY1() > GetY2())
  {
	bbox.SetY(GetY2());
	bbox.SetHeight(GetY1() - GetY2());
  }
  
  return bbox;
}

