//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTSpanElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_T_SPAN_ELEMENT_H
#define WX_SVG_T_SPAN_ELEMENT_H

#include "SVGTextPositioningElement.h"
#include "String.h"
#include "Element.h"

class wxSVGTSpanElement:
  public wxSVGTextPositioningElement
{
  public:
    wxSVGTSpanElement(wxString tagName = wxT("tspan")):
      wxSVGTextPositioningElement(tagName) {}
    virtual ~wxSVGTSpanElement() {}
    wxSvgXmlNode* CloneNode(bool deep = true) { return new wxSVGTSpanElement(*this); }
    bool HasAttribute(const wxString& name);
    wxString GetAttribute(const wxString& name);
    bool SetAttribute(const wxString& name, const wxString& value);
    wxSvgXmlAttrHash GetAttributes() const;
    virtual wxSVGDTD GetDtd() const { return wxSVG_TSPAN_ELEMENT; }
};

#endif // WX_SVG_T_SPAN_ELEMENT_H
