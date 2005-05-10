//////////////////////////////////////////////////////////////////////////////
// Name:        CSSStyleDeclaration.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/03
// RCS-ID:      $Id: CSSStyleDeclaration.h,v 1.1.1.1 2005-05-10 17:51:11 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef wxSVG_CSS_STYLE_DECLARATION_H
#define wxSVG_CSS_STYLE_DECLARATION_H

#include "CSSValue.h"
#include <wx/hashmap.h>

WX_DECLARE_STRING_HASH_MAP(wxCSSValue, wxHashMapCSSValue);

typedef wxString wxCSSStyler;

class wxCSSStyleDeclaration: public wxHashMapCSSValue
{
  public:
	wxCSSStyleDeclaration() {}
	void Add(wxCSSStyleDeclaration& style);
  
  public:
	wxString GetCSSText();
	void SetCSSText(const wxString& text);
	inline wxString GetPropertyValue(const wxString& propertyName)
	  { return GetPropertyCSSValue(propertyName).GetCSSText(); }
	inline wxCSSValue GetPropertyCSSValue(const wxString& propertyName)
	  { iterator it = find(propertyName); if (it != end()) return it->second; return wxCSSValue(); }
	inline wxString RemoveProperty(const wxString& propertyName)
	  { erase(propertyName); return wxT(""); }
	inline void SetProperty(const wxString& propertyName, const wxString& value)
	  { (*this)[propertyName].SetCSSText(value); }
	inline bool HasProperty(const wxString& propertyName)
	  { return find(propertyName) != end(); }
	
	static bool IsStyleAttribute(const wxString& propertyName);
	
	// get methods (GetFill(),...)
#define ATTRDEF(type,func,name,value,defvalue)\
  inline type Get##name() { return HasProperty(wxT(value)) ? GetPropertyCSSValue(wxT(value)).func() : defvalue; }
#include "CSSStyleDeclaration_styles.h"
#undef ATTRDEF

	// has methods
#define ATTRDEF(type,func,name,value,edfvalue)\
  inline bool Has##name() { return HasProperty(wxT(value)); }
#include "CSSStyleDeclaration_styles.h"
#undef ATTRDEF
};

#endif //wxSVG_CSS_STYLE_DECLARATION_H
