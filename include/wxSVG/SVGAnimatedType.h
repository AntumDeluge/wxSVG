//////////////////////////////////////////////////////////////////////////////
// Name:        SVGAnimatedType.h
// Author:      Alex Thuering
// Created:     2014/02/24
// RCS-ID:      $Id: SVGAnimatedType.h,v 1.1 2014-03-18 13:09:22 ntalex Exp $
// Copyright:   (c) 2014 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_ANIMATED_TYPE_H
#define WX_SVG_ANIMATED_TYPE_H

#include "RGBColor.h"
#include "SVGLengthList.h"
#include "SVGRect.h"

enum wxSVG_ANIMATED_PROPERTY_TYPE {
	wxSVG_ANIMATED_COLOR,
	wxSVG_ANIMATED_LENGTH,
	wxSVG_ANIMATED_LENGTH_LIST,
	wxSVG_ANIMATED_STRING,
	wxSVG_ANIMATED_UNKNOWN
};

class wxSVGAnimatedType {
public:
	wxSVGAnimatedType(): m_propertyType(wxSVG_ANIMATED_UNKNOWN) {}
	~wxSVGAnimatedType();
	
	wxSVG_ANIMATED_PROPERTY_TYPE GetPropertyType() const { return m_propertyType; }
	const wxRGBColor& GetColor() const { return m_color; }
	const wxSVGLength& GetLength() const { return m_length; }
	const wxSVGLengthList& GetLengthList() const { return m_lengthList; }
	const wxString& GetString() const { return m_string; }
	
    wxString GetValueAsString() const;
    void SetValueAsString(const wxString& value);
	
private:
	wxSVG_ANIMATED_PROPERTY_TYPE m_propertyType;
	wxString m_string;
	wxRGBColor m_color;
	wxSVGLength m_length;
	wxSVGLengthList m_lengthList;
};

#endif // WX_SVG_ANIMATED_TYPE_H
