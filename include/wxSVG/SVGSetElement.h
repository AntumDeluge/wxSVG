//////////////////////////////////////////////////////////////////////////////
// Name:        SVGSetElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_SET_ELEMENT_H
#define WX_SVG_SET_ELEMENT_H

#include "SVGAnimationElement.h"
#include "String.h"

class wxSVGSetElement:
  public wxSVGAnimationElement
{
  public:
    wxSVGSetElement(wxSVGDocument* doc, wxString tagName = wxT("set")):
      wxSVGAnimationElement(doc, tagName) {}
    virtual ~wxSVGSetElement() {}
    wxXmlNode* CloneNode(bool deep = true) { return new wxSVGSetElement(*this); }
    bool SetAttribute(const wxString& name, const wxString& value);
    virtual const wxSVGDTD GetDtd() const { return wxSVG_SET_ELEMENT; }
};

#endif // WX_SVG_SET_ELEMENT_H
