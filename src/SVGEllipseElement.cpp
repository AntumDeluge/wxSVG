//////////////////////////////////////////////////////////////////////////////
// Name:        SVGEllipseElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGEllipseElement.cpp,v 1.2 2005-05-12 03:25:13 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGEllipseElement.h"

wxSVGRect wxSVGEllipseElement::GetBBox()
{
  return wxSVGRect(GetCx() - GetRx(), GetCy() - GetRy(), 2*GetRx(), 2*GetRy());
}
