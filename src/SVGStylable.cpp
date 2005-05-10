//////////////////////////////////////////////////////////////////////////////
// Name:        SVGStylable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGStylable.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGStylable.h"

wxCSSValue wxSVGStylable::GetPresentationAttribute(const wxString& name)
{
  wxCSSValue res;
  return res;
}

bool wxSVGStylable::SetCustomAttribute(const wxString& name, const wxString& value)
{
  if(!wxCSSStyleDeclaration::IsStyleAttribute(name))
    return false;
	
  m_style.SetProperty(name, value);
  return true;
}
