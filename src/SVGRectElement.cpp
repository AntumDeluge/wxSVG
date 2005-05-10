//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRectElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGRectElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRectElement.h"

wxSVGRect wxSVGRectElement::GetBBox()
{
  return wxSVGRect(GetX(), GetY(), GetWidth(), GetHeight());
}
