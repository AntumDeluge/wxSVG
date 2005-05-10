//////////////////////////////////////////////////////////////////////////////
// Name:        CSSValue.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSValue.h,v 1.1.1.1 2005-05-10 17:51:11 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef wxSVG_CSS_VALUE_H
#define wxSVG_CSS_VALUE_H

#include <wx/string.h>
#include <wx/geometry.h>
#include "RGBColor.h"

class wxCSSValueBase
{
  public:
	enum wxCSS_VALUE_TYPE
	{
	  wxCSS_INHERIT = 0,
	  wxCSS_PRIMITIVE_VALUE = 1,
	  wxCSS_VALUE_LIST = 2,
	  wxCSS_CUSTOM = 3
	};
  
  protected:
	wxString m_cssText;
	wxCSS_VALUE_TYPE m_cssValueType;
  
  public:
	wxCSSValueBase(): m_cssValueType(wxCSS_INHERIT) {}
	wxCSSValueBase(wxCSS_VALUE_TYPE cssValueType): m_cssValueType(cssValueType) {}
	inline wxString GetCSSText() { return m_cssText; }
	inline void SetCSSText(const wxString& cssText) { m_cssText = cssText.Strip(wxString::both); }
	inline wxCSS_VALUE_TYPE GetCSSValueType() { return m_cssValueType; }
	inline void SetCSSValueType(wxCSS_VALUE_TYPE cssValueType) { m_cssValueType = cssValueType; }
};

class wxCSSPrimitiveValue: public wxCSSValueBase
{
  public:
	enum wxCSS_PRIMITIVE_TYPE
	{
		wxCSS_UNKNOWN    = 0,
		wxCSS_NUMBER     = 1,
		wxCSS_PERCENTAGE = 2,
		wxCSS_EMS        = 3,
		wxCSS_EXS        = 4,
		wxCSS_PX         = 5,
		wxCSS_CM         = 6,
		wxCSS_MM         = 7,
		wxCSS_IN         = 8,
		wxCSS_PT         = 9,
		wxCSS_PC         = 10,
		wxCSS_DEG        = 11,
		wxCSS_RAD        = 12,
		wxCSS_GRAD       = 13,
		wxCSS_MS         = 14,
		wxCSS_S          = 15,
		wxCSS_HZ         = 16,
		wxCSS_KHZ        = 17,
		wxCSS_DIMENSION  = 18,
		wxCSS_STRING     = 19,
		wxCSS_URI        = 20,
		wxCSS_IDENT      = 21,
		wxCSS_ATTR       = 22,
		wxCSS_COUNTER    = 23,
		wxCSS_RECT       = 24,
		wxCSS_RGBCOLOR   = 25
	};
  
  protected:
	wxCSS_PRIMITIVE_TYPE m_primitiveType;
  
  public:
	wxCSSPrimitiveValue(): wxCSSValueBase(wxCSS_PRIMITIVE_VALUE) {}
	
	inline wxCSS_PRIMITIVE_TYPE GetCSSPrimitiveType() { return m_primitiveType; }
	inline void SetCSSPrimitiveType(wxCSS_PRIMITIVE_TYPE primitiveType) { m_primitiveType = primitiveType; }
	
	void     SetFloatValue(wxCSS_PRIMITIVE_TYPE unitType, float floatValue);
	float    GetFloatValue(wxCSS_PRIMITIVE_TYPE unitType = wxCSS_NUMBER);
	
	void     SetStringValue(wxCSS_PRIMITIVE_TYPE stringType, const wxString& stringValue);
	wxString GetStringValue() { return GetCSSText(); }
	
	bool GetBooleanValue();
	wxRect     GetRectValue();
	wxRGBColor GetRGBColorValue();
};

typedef wxCSSPrimitiveValue wxCSSValue;

#endif //wxSVG_CSS_VALUE_H

