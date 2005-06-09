//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDefsElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_DEFS_ELEMENT_H
#define WX_SVG_DEFS_ELEMENT_H

#include "SVGElement.h"
#include "SVGTests.h"
#include "SVGLangSpace.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGStylable.h"
#include "SVGTransformable.h"
#include "EventTarget.h"
#include "String.h"

class wxSVGDefsElement:
  public wxSVGElement,
  public wxSVGTests,
  public wxSVGLangSpace,
  public wxSVGExternalResourcesRequired,
  public wxSVGStylable,
  public wxSVGTransformable,
  public wxEventTarget
{
  public:
    wxSVGDefsElement(wxSVGDocument* doc, wxString tagName = wxT("defs")):
      wxSVGElement(doc, tagName) {}
    virtual ~wxSVGDefsElement() {}
    wxXmlNode* CloneNode(bool deep = true) { return new wxSVGDefsElement(*this); }
    virtual wxSVGRect GetBBox() { return wxSVGLocatable::GetBBox(this); }
    bool SetAttribute(const wxString& name, const wxString& value);
    virtual const wxSVGDTD GetDtd() const { return wxSVG_DEFS_ELEMENT; }
};

#endif // WX_SVG_DEFS_ELEMENT_H
