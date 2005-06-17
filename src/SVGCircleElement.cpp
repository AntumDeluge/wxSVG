//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCircleElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGCircleElement.cpp,v 1.3 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCircleElement.h"

wxSVGRect wxSVGCircleElement::GetBBox()
{
  return wxSVGRect(GetCx().GetBaseVal() - GetR().GetBaseVal(),
	GetCy().GetBaseVal() - GetR().GetBaseVal(),
	2*GetR().GetBaseVal(), 2*GetR().GetBaseVal());
}
