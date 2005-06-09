//////////////////////////////////////////////////////////////////////////////
// Name:        CSSStyleDeclaration.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSStyleDeclaration.cpp,v 1.5 2005-06-09 16:31:30 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "CSSStyleDeclaration.h"
#include <wx/tokenzr.h>
#include <wx/log.h>

wxCSSPrimitiveValue* wxCSSStyleDeclaration::s_emptyCSSValue = new wxCSSPrimitiveValue;
wxSVGColor* wxCSSStyleDeclaration::s_emptySVGColor = new wxSVGColor;
wxSVGPaint* wxCSSStyleDeclaration::s_emptySVGPaint = new wxSVGPaint;
wxSVGPaint* wxCSSStyleDeclaration::s_blackSVGPaint = new wxSVGPaint(0,0,0);

void wxCSSStyleDeclaration::Add(const wxCSSStyleDeclaration& style)
{
  const_iterator it;
  for (it = style.begin(); it != style.end(); ++it)
	(*this)[it->first] = it->second->Clone();
}

wxString wxCSSStyleDeclaration::GetCSSText()
{
  wxString text;
  iterator it;
  for (it = begin(); it != end(); ++it) 
	text = GetPropertyName(it->first) + wxT(":") + it->second->GetCSSText() + wxT(";");
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

static wxSortedArrayString* s_cssProperties = NULL;
#include "css_properties.cpp"
inline void FillCSSProperties()
{
  if (s_cssProperties == NULL)
  {
	s_cssProperties = new wxSortedArrayString;
	for (unsigned int i=0; i<sizeof(s_cssPropertyStrings)/sizeof(s_cssPropertyStrings[0]); i++)
	  s_cssProperties->Add(s_cssPropertyStrings[i]);
  }
}

wxCSS_PROPERTY wxCSSStyleDeclaration::GetPropertyId(const wxString& propertyName)
{
  FillCSSProperties();
  int id = s_cssProperties->Index(propertyName);
  if (id >= 0)
	return wxCSS_PROPERTY(id+1);
  return wxCSS_PROPERTY_UNKNOWN;
}

wxString wxCSSStyleDeclaration::GetPropertyName(wxCSS_PROPERTY propertyId)
{
  FillCSSProperties();
  if (propertyId == wxCSS_PROPERTY_UNKNOWN)
	return wxT("");
  return (*s_cssProperties)[int(propertyId)-1];
}

void wxCSSStyleDeclaration::SetProperty(wxCSS_PROPERTY propertyId, const wxString& svalue)
{
  if (propertyId == wxCSS_PROPERTY_UNKNOWN)
    return;
  wxString value = svalue.Strip(wxString::both);
  wxCSSValue* cssValue = NULL;
  iterator it = find(propertyId);
  if (it != end())
    cssValue = it->second;
  switch (propertyId)
  {
    case wxCSS_PROPERTY_UNKNOWN:
      break;
    case wxCSS_PROPERTY_CLIP_PATH:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  if (value.Left(3) == wxT("url"))
		((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_URI,
		  value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	  else
		((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_COLOR:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetRGBColorValue(ParseColor(value));
      break;
    case wxCSS_PROPERTY_DISPLAY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FILL:
      if (!cssValue)
        cssValue = new wxSVGPaint;
	  ParseSVGPaint(*(wxSVGPaint*)cssValue, value);
      break;
    case wxCSS_PROPERTY_FILL_OPACITY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_FILL_RULE:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FILTER:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      if (value.Left(3) == wxT("url"))
		((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_URI,
		  value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	  else
		((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FONT_FAMILY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_STRING, value);
      break;
    case wxCSS_PROPERTY_FONT_SIZE:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_FONT_STRETCH:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FONT_STYLE:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FONT_VARIANT:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_FONT_WEIGHT:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_MARKER:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_STRING, value);
      break;
    case wxCSS_PROPERTY_MARKER_END:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      if (value.Left(3) == wxT("url"))
		((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_URI,
		  value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	  else
		((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_MARKER_MID:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      if (value.Left(3) == wxT("url"))
		((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_URI,
		  value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	  else
		((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_MARKER_START:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      if (value.Left(3) == wxT("url"))
		((wxCSSPrimitiveValue*)cssValue)->SetStringValue(wxCSS_URI,
		  value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	  else
		((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_OPACITY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_STOP_COLOR:
      if (!cssValue)
        cssValue = new wxSVGColor;
	  ((wxSVGColor*)cssValue)->SetRGBColor(ParseColor(value));
      break;
    case wxCSS_PROPERTY_STOP_OPACITY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_STROKE:
      if (!cssValue)
        cssValue = new wxSVGPaint;
	  ParseSVGPaint(*(wxSVGPaint*)cssValue, value);
      break;
    case wxCSS_PROPERTY_STROKE_DASHOFFSET:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_STROKE_LINECAP:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_STROKE_LINEJOIN:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_STROKE_MITERLIMIT:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_STROKE_OPACITY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_STROKE_WIDTH:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetFloatValue(wxCSS_NUMBER, ParseNumber(value));
      break;
    case wxCSS_PROPERTY_TEXT_ANCHOR:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
      ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
    case wxCSS_PROPERTY_VISIBILITY:
      if (!cssValue)
        cssValue = new wxCSSPrimitiveValue;
	  ((wxCSSPrimitiveValue*)cssValue)->SetIdentValue(wxCSSValue::GetValueId(value));
      break;
  }
  if (it == end())
    (*this)[propertyId] = cssValue;
}

double wxCSSStyleDeclaration::ParseNumber(const wxString& value)
{
  double val = 0;
  value.ToDouble(&val);
  return val;
}

static wxSortedArrayString* s_cssColors = NULL;
#include "css_colors.cpp"
inline void FillCSSColors()
{
  if (s_cssColors == NULL)
  {
	s_cssColors = new wxSortedArrayString;
	for (unsigned int i=0; i<sizeof(s_cssNamedColors)/sizeof(s_cssNamedColors[0]); i++)
	  s_cssColors->Add(s_cssNamedColors[i].name);
  }
}

wxRGBColor wxCSSStyleDeclaration::ParseColor(const wxString& value)
{
  if (!value.length() || value == wxT("none"))
	return wxRGBColor();
  else if (value.GetChar(0) == wxT('#'))
  {
	long r = 0, g = 0, b = 0;
	value.Mid(1,2).ToLong(&r,16);
	value.Mid(3,2).ToLong(&g,16);
	value.Mid(5,2).ToLong(&b,16);
	return wxRGBColor(r,g,b);
  }
  else if (value.Left(3) == wxT("rgb"))
  {
	wxStringTokenizer tkz(value.Mid(3), wxT(",()"));
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
	FillCSSColors();
	int num = s_cssColors->Index(value);
	if (num>=0)
	  return s_cssNamedColors[num].colour;
  }
  return wxRGBColor();
}

void wxCSSStyleDeclaration::ParseSVGPaint(wxSVGPaint& cssValue, const wxString& value)
{
  wxString val = value;
  if (val.Left(3) == wxT("url"))
  {
	cssValue.SetUri(value.AfterFirst(wxT('(')).BeforeFirst(wxT(')')));
	val = value.AfterFirst(wxT(')')).Strip(wxString::both);
  }
  cssValue.SetRGBColor(ParseColor(val));
}

