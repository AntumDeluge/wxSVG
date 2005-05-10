//////////////////////////////////////////////////////////////////////////////
// Name:        CSSStyleDeclaration.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSStyleDeclaration.cpp,v 1.1.1.1 2005-05-10 17:51:30 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "CSSStyleDeclaration.h"
#include <wx/tokenzr.h>
#include <wx/log.h>

void wxCSSStyleDeclaration::Add(wxCSSStyleDeclaration& style)
{
  iterator it;
  for (it = style.begin(); it != style.end(); ++it)
	SetProperty(it->first, it->second.GetCSSText());
}

wxString wxCSSStyleDeclaration::GetCSSText()
{
  wxString text;
  iterator it;
  for (it = begin(); it != end(); ++it) 
	text = it->first + wxT(":") + it->second.GetCSSText() + wxT(";");
  return text;
}

void wxCSSStyleDeclaration::SetCSSText(const wxString& text)
{
  wxStringTokenizer tkz(text, wxT(";"));
  while (tkz.HasMoreTokens()) 
  { 
	wxString token = tkz.GetNextToken().Strip(wxString::both);
	int pos = token.find(wxT(':'));
	if (pos<=0)
	  continue;
	SetProperty(token.substr(0,pos), token.substr(pos+1));
  }
}

bool wxCSSStyleDeclaration::IsStyleAttribute(const wxString& propertyName)
{
#define ATTRDEF(type,func,name,value,defvalue) if (propertyName == wxT(value)) return true;
#include "CSSStyleDeclaration_styles.h"
#undef ATTRDEF
  return false;
}
