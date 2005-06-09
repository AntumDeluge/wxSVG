//////////////////////////////////////////////////////////////////////////////
// Name:        SVGFontFaceNameElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_FONT_FACE_NAME_ELEMENT_H
#define WX_SVG_FONT_FACE_NAME_ELEMENT_H

#include "SVGElement.h"
#include "String.h"

class wxSVGFontFaceNameElement:
  public wxSVGElement
{
  public:
    wxSVGFontFaceNameElement(wxSVGDocument* doc, wxString tagName = wxT("font-face-name")):
      wxSVGElement(doc, tagName) {}
    virtual ~wxSVGFontFaceNameElement() {}
    wxXmlNode* CloneNode(bool deep = true) { return new wxSVGFontFaceNameElement(*this); }
    bool SetAttribute(const wxString& name, const wxString& value);
    virtual const wxSVGDTD GetDtd() const { return wxSVG_FONT_FACE_NAME_ELEMENT; }
};

#endif // WX_SVG_FONT_FACE_NAME_ELEMENT_H
