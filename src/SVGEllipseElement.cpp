//////////////////////////////////////////////////////////////////////////////
// Name:        SVGEllipseElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGEllipseElement.cpp,v 1.3 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGEllipseElement.h"

wxSVGRect wxSVGEllipseElement::GetBBox()
{
  return wxSVGRect(GetCx().GetBaseVal() - GetRx().GetBaseVal(),
	GetCy().GetBaseVal() - GetRy().GetBaseVal(),
	2*GetRx().GetBaseVal(), 2*GetRy().GetBaseVal());
}
