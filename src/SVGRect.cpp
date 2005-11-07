//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRect.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/09/27
// RCS-ID:      $Id: SVGRect.cpp,v 1.2 2005-11-07 17:36:57 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRect.h"
#include <wx/tokenzr.h>

wxString wxSVGRect::GetValueAsString() const
{
  return wxString::Format(wxT("%f %f %f %f"),
    GetX(), GetY(), GetWidth(), GetHeight());
}

void wxSVGRect::SetValueAsString(const wxString& value)
{
  double val;
  wxStringTokenizer tkz(value, wxT(", \\t"));
  int pi = 0;
  while (tkz.HasMoreTokens() && pi<4) 
  { 
    wxString token = tkz.GetNextToken(); 
    token.ToDouble(&val);
    switch (pi)
    {
      case 0: SetX(val); break;
      case 1: SetY(val); break;
      case 2: SetWidth(val); break;
      case 3: SetHeight(val); break;
    }
    pi++;
  }
}
