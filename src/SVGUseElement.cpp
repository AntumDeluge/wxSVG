//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUseElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/09/21
// RCS-ID:      $Id: SVGUseElement.cpp,v 1.1 2005-09-25 11:33:42 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUseElement.h"

wxSVGRect wxSVGUseElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}
