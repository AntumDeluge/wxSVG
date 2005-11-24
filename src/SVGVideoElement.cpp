//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/11/23
// RCS-ID:      $Id: SVGVideoElement.cpp,v 1.1 2005-11-24 11:39:58 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGVideoElement.h"

wxSVGRect wxSVGVideoElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}

