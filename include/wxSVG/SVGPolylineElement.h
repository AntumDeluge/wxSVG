//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPolylineElement.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_POLYLINE_ELEMENT_H
#define WX_SVG_POLYLINE_ELEMENT_H

class wxSVGCanvasItem;

#include "SVGElement.h"
#include "SVGTests.h"
#include "SVGLangSpace.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGStylable.h"
#include "SVGTransformable.h"
#include "EventTarget.h"
#include "SVGAnimatedPoints.h"
#include "String.h"

class wxSVGPolylineElement:
  public wxSVGElement,
  public wxSVGTests,
  public wxSVGLangSpace,
  public wxSVGExternalResourcesRequired,
  public wxSVGStylable,
  public wxSVGTransformable,
  public wxEventTarget,
  public wxSVGAnimatedPoints
{
  public:

  protected:
	wxSVGCanvasItem* m_canvasItem;
  public:
	inline wxSVGCanvasItem* GetCanvasItem() { return m_canvasItem; }
	inline void SetCanvasItem(wxSVGCanvasItem* canvasItem) { m_canvasItem = canvasItem; }

  public:
    wxSVGPolylineElement(wxSVGDocument* doc, wxString tagName = wxT("polyline")):
      wxSVGElement(doc, tagName), m_canvasItem(NULL) {}
    virtual ~wxSVGPolylineElement() {}
    wxXmlNode* CloneNode(bool deep = true) { return new wxSVGPolylineElement(*this); }
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& name, const wxString& value);
    virtual const wxSVGDTD GetDtd() const { return wxSVG_POLYLINE_ELEMENT; }
};

#endif // WX_SVG_POLYLINE_ELEMENT_H
