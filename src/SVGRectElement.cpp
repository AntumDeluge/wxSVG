//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRectElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGRectElement.cpp,v 1.2.2.1 2005-09-01 13:00:53 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRectElement.h"

wxSVGRect wxSVGRectElement::GetBBox()
{
  double stroke_width = 0;
  if (GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE)
  	stroke_width = GetStrokeWidth();
  return wxSVGRect(GetX().GetBaseVal() - stroke_width / 2,
    GetY().GetBaseVal() - stroke_width / 2,
	GetWidth().GetBaseVal() + stroke_width,
	GetHeight().GetBaseVal() + stroke_width);
}
