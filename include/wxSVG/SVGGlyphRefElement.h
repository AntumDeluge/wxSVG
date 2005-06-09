//////////////////////////////////////////////////////////////////////////////
// Name:        SVGGlyphRefElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_GLYPH_REF_ELEMENT_H
#define WX_SVG_GLYPH_REF_ELEMENT_H

#include "SVGElement.h"
#include "SVGURIReference.h"
#include "SVGStylable.h"
#include "String.h"

class wxSVGGlyphRefElement:
  public wxSVGElement,
  public wxSVGURIReference,
  public wxSVGStylable
{
  protected:
    wxString m_glyphRef;
    wxString m_format;
    float m_x;
    float m_y;
    float m_dx;
    float m_dy;

  public:
    inline const wxString& GetGlyphRef() { return m_glyphRef; }
    inline void SetGlyphRef(const wxString& n) { m_glyphRef = n; }

    inline const wxString& GetFormat() { return m_format; }
    inline void SetFormat(const wxString& n) { m_format = n; }

    inline float GetX() const { return m_x; }
    inline void SetX(const float n) { m_x = n; }

    inline float GetY() const { return m_y; }
    inline void SetY(const float n) { m_y = n; }

    inline float GetDx() const { return m_dx; }
    inline void SetDx(const float n) { m_dx = n; }

    inline float GetDy() const { return m_dy; }
    inline void SetDy(const float n) { m_dy = n; }

  public:
    wxSVGGlyphRefElement(wxSVGDocument* doc, wxString tagName = wxT("glyphRef")):
      wxSVGElement(doc, tagName), m_x(0), m_y(0), m_dx(0), m_dy(0) {}
    virtual ~wxSVGGlyphRefElement() {}
    wxXmlNode* CloneNode(bool deep = true) { return new wxSVGGlyphRefElement(*this); }
    bool SetAttribute(const wxString& name, const wxString& value);
    virtual const wxSVGDTD GetDtd() const { return wxSVG_GLYPHREF_ELEMENT; }
};

#endif // WX_SVG_GLYPH_REF_ELEMENT_H
