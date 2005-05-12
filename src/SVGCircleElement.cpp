//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCircleElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGCircleElement.cpp,v 1.2 2005-05-12 03:25:13 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCircleElement.h"

wxSVGRect wxSVGCircleElement::GetBBox()
{
  return wxSVGRect(GetCx() - GetR(), GetCy() - GetR(), 2*GetR(), 2*GetR());
}
