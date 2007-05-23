//////////////////////////////////////////////////////////////////////////////
// Name:        SVGMPathElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_M_PATH_ELEMENT_H
#define WX_SVG_M_PATH_ELEMENT_H

#include "SVGElement.h"
#include "SVGURIReference.h"
#include "SVGExternalResourcesRequired.h"
#include "String.h"
#include "Element.h"

class wxSVGMPathElement:
  public wxSVGElement,
  public wxSVGURIReference,
  public wxSVGExternalResourcesRequired
{
  public:
    wxSVGMPathElement(wxString tagName = wxT("mpath")):
      wxSVGElement(tagName) {}
    virtual ~wxSVGMPathElement() {}
    wxSvgXmlNode* CloneNode(bool deep = true) { return new wxSVGMPathElement(*this); }
    bool HasAttribute(const wxString& name);
    wxString GetAttribute(const wxString& name);
    bool SetAttribute(const wxString& name, const wxString& value);
    wxSvgXmlAttrHash GetAttributes() const;
    virtual wxSVGDTD GetDtd() const { return wxSVG_MPATH_ELEMENT; }
};

#endif // WX_SVG_M_PATH_ELEMENT_H
