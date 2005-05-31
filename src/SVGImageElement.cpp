//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/05/30
// RCS-ID:      $Id: SVGImageElement.cpp,v 1.1 2005-05-31 16:07:54 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGImageElement.h"

wxSVGRect wxSVGImageElement::GetBBox()
{
  return wxSVGRect(GetX(), GetY(), GetWidth(), GetHeight());
}

