//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/05/30
// RCS-ID:      $Id: SVGImageElement.cpp,v 1.2 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGImageElement.h"

wxSVGRect wxSVGImageElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}

