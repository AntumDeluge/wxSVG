//////////////////////////////////////////////////////////////////////////////
// Name:        SVGEllipseElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGEllipseElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGEllipseElement.h"

wxSVGRect wxSVGEllipseElement::GetBBox()
{
  return wxSVGRect(GetCx() - GetRx(), GetCy() - GetRy(),
	GetCx() + GetRx(), GetCy() + GetRy());
}
