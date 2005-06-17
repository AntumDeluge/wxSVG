//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRectElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGRectElement.cpp,v 1.2 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRectElement.h"

wxSVGRect wxSVGRectElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}
