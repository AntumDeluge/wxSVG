//////////////////////////////////////////////////////////////////////////////
// Name:        CSSValue.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSValue.cpp,v 1.2 2005-05-11 03:12:36 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "CSSValue.h"
#include <wx/tokenzr.h>

void wxCSSPrimitiveValue::SetFloatValue(wxCSS_PRIMITIVE_TYPE unitType, float floatValue)
{
  m_primitiveType = unitType;
  m_cssText = wxString::Format(wxT("%f"), floatValue);
}

float wxCSSPrimitiveValue::GetFloatValue(wxCSS_PRIMITIVE_TYPE unitType)
{
  double value = 0;
  m_cssText.ToDouble(&value);
  return value;
}

void wxCSSPrimitiveValue::SetStringValue(wxCSS_PRIMITIVE_TYPE stringType, const wxString& stringValue)
{
  m_primitiveType = stringType;
  m_cssText = stringValue;
}

bool wxCSSPrimitiveValue::GetBooleanValue()
{
  return m_cssText.Upper() == wxT("true") || m_cssText == wxT("1");
}

wxRect wxCSSPrimitiveValue::GetRectValue()
{
  return wxRect();
}

static wxSortedArrayString s_colours;
#include "CSSValue_colours.h"

wxRGBColor wxCSSPrimitiveValue::GetRGBColorValue()
{
  if (!m_cssText.length() || m_cssText == wxT("none"))
	return wxRGBColor();
  else if (m_cssText.GetChar(0) == wxT('#'))
  {
	long r = 0, g = 0, b = 0;
	m_cssText.Mid(1,2).ToLong(&r,16);
	m_cssText.Mid(3,2).ToLong(&g,16);
	m_cssText.Mid(5,2).ToLong(&b,16);
	return wxRGBColor(r,g,b);
  }
  else if (m_cssText.Left(3) == wxT("rgb"))
  {
	wxStringTokenizer tkz(m_cssText.Mid(3), wxT(",()"));
	long rgb[3] = { 0, 0, 0 };
	for (int i=0; tkz.HasMoreTokens() && i<3;)
	{
	  wxString token = tkz.GetNextToken().Strip(wxString::both);
	  if (token.length())
		token.ToLong(&rgb[i++]);
	}
	return wxRGBColor(rgb[0], rgb[1], rgb[2]);
  }
  else
  {
	if (s_colours.Count() == 0)
	  for (unsigned int i=0; i<sizeof(s_namedColours)/sizeof(s_namedColours[0]); i++)
		s_colours.Add(s_namedColours[i].name);
	int num = s_colours.Index(m_cssText);
	if (num>=0)
	  return s_namedColours[num].colour;
  }
  return wxRGBColor();
}

