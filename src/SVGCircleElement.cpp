//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCircleElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGCircleElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCircleElement.h"

wxSVGRect wxSVGCircleElement::GetBBox()
{
  return wxSVGRect(GetCx() - GetR(), GetCy() - GetR(),
	GetCx() + GetR(), GetCy() + GetR());
}
