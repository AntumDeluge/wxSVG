//////////////////////////////////////////////////////////////////////////////
// Name:        SVGStylable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGStylable.cpp,v 1.2 2005-06-07 22:15:35 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGStylable.h"

const wxCSSValue& wxSVGStylable::GetPresentationAttribute(const wxString& name)
{
  return m_style.GetPropertyCSSValue(name);
}

bool wxSVGStylable::SetCustomAttribute(const wxString& name, const wxString& value)
{
  wxCSS_PROPERTY id = wxCSSStyleDeclaration::GetPropertyId(name);
  
  if (id == wxCSS_PROPERTY_UNKNOWN)
    return false;
	
  m_style.SetProperty(name, value);
  return true;
}
