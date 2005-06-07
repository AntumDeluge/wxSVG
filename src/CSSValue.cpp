//////////////////////////////////////////////////////////////////////////////
// Name:        CSSValue.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSValue.cpp,v 1.4 2005-06-07 22:30:30 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "CSSValue.h"
#include <wx/tokenzr.h>

wxCSSPrimitiveValue::wxCSSPrimitiveValue(const wxCSSPrimitiveValue& src)
{
  m_primitiveType = src.m_primitiveType;
  if (m_primitiveType == wxCSS_UNKNOWN);
  else if (int(m_primitiveType) >= int(wxCSS_NUMBER) &&
		   int(m_primitiveType) <= int(wxCSS_DIMENSION))
	m_number = src.m_number;
  else if (m_primitiveType == wxCSS_IDENT)
	m_ident = src.m_ident;
  else if (m_primitiveType == wxCSS_STRING ||
		   m_primitiveType == wxCSS_URI ||
  		   m_primitiveType == wxCSS_ATTR)
	m_string = new wxString(*src.m_string);
  else if (m_primitiveType == wxCSS_RECT)
	m_rect = new wxRect(*src.m_rect);
  else if (m_primitiveType == wxCSS_RGBCOLOR)
	m_color = new wxRGBColor(*src.m_color);
}

wxString wxCSSPrimitiveValue::GetCSSText() const
{
  return GetStringValue();
}

void wxCSSPrimitiveValue::SetStringValue(wxCSS_PRIMITIVE_TYPE stringType, const wxString& stringValue)
{
  if (m_primitiveType != wxCSS_STRING &&
	  m_primitiveType != wxCSS_URI &&
	  m_primitiveType != wxCSS_ATTR)
  {
	CleanUp();
	m_string = new wxString;
  }
  m_primitiveType = stringValue == wxCSS_STRING ||
					stringValue == wxCSS_URI ||
					stringValue == wxCSS_ATTR ? stringType : wxCSS_STRING;
  *m_string = stringValue;
}

wxString wxCSSPrimitiveValue::GetStringValue() const
{
  switch (m_primitiveType)
  {
	case wxCSS_STRING:
	case wxCSS_URI:
	case wxCSS_ATTR:
	  return *m_string;
    case wxCSS_IDENT:
	  break;
	case wxCSS_COUNTER:
	  break;
	case wxCSS_RECT:
	  break;
	case wxCSS_RGBCOLOR:
	  if (m_color->Ok())
		return wxString::Format(_T("#%02x%02x%02x"),
		  m_color->Red(), m_color->Green(), m_color->Blue());
	  break;
	case wxCSS_UNKNOWN:
	  break;
	default:
	  return wxString::Format(wxT("%f"), m_number);
  }
  return wxT("");
}

void wxCSSPrimitiveValue::SetFloatValue(wxCSS_PRIMITIVE_TYPE unitType, double floatValue)
{
  CleanUp();
  m_primitiveType =
	int(unitType) >= int(wxCSS_NUMBER) &&
	int(unitType) <= int(wxCSS_DIMENSION) ? unitType : wxCSS_NUMBER;
  m_number = floatValue;
}

double wxCSSPrimitiveValue::GetFloatValue(wxCSS_PRIMITIVE_TYPE unitType) const
{
  if (int(m_primitiveType) >= int(wxCSS_NUMBER) &&
	  int(m_primitiveType) <= int(wxCSS_DIMENSION))
	return m_number;
  return 0;
}

void wxCSSPrimitiveValue::SetRectValue(wxRect rect)
{
  if (m_primitiveType != wxCSS_RECT)
  {
	CleanUp();
	m_rect = new wxRect;
  }
  m_primitiveType = wxCSS_RECT;
  *m_rect = rect;
}

wxRect wxCSSPrimitiveValue::GetRectValue() const
{
  if (m_primitiveType == wxCSS_RECT)
	return *m_rect;
  return wxRect();
}

void wxCSSPrimitiveValue::SetRGBColorValue(wxRGBColor color)
{
  if (m_primitiveType != wxCSS_RGBCOLOR)
  {
	CleanUp();
	m_color = new wxRGBColor;
  }
  m_primitiveType = wxCSS_RGBCOLOR;
  *m_color = color;
}

wxRGBColor wxCSSPrimitiveValue::GetRGBColorValue() const
{
  if (m_primitiveType == wxCSS_RGBCOLOR)
	return *m_color;
  return wxRGBColor();
}

void wxCSSPrimitiveValue::SetIdent(int ident)
{
  if (m_primitiveType != wxCSS_IDENT)
	CleanUp();
  m_primitiveType = wxCSS_IDENT;
  m_ident = ident;
}

int wxCSSPrimitiveValue::GetIdent() const
{
  if (m_primitiveType == wxCSS_IDENT)
	return m_ident;
  return 0;
}

void wxCSSPrimitiveValue::CleanUp()
{
  if (m_primitiveType == wxCSS_STRING ||
	  m_primitiveType == wxCSS_URI ||
	  m_primitiveType == wxCSS_ATTR)
	delete m_string;
  else if (m_primitiveType == wxCSS_RECT)
	delete m_rect;
  else if (m_primitiveType == wxCSS_RGBCOLOR)
	delete m_color;
}
