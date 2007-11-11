/////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.h
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/02
// RCS-ID:      $Id: SVGCanvas.h,v 1.9 2007-11-11 20:05:45 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_H
#define WX_SVG_CANVAS_H

class wxSVGDocument;

#include "SVGCanvasItem.h"

/** Base class for SVG-Renders (backends) */
class wxSVGCanvas: public wxObject
{
  public:
	wxSVGCanvas():  m_itemsCached(true) {}
	virtual ~wxSVGCanvas() {}
	
	virtual void Init(int width, int height) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual wxImage GetImage() = 0;
	virtual void Clear(wxRGBColor color = wxRGBColor(0xFF,0xFF,0xFF)) = 0;
	
	virtual wxSVGCanvasPath* CreateCanvasPath() = 0;
	wxSVGCanvasItem* CreateItem(wxSVGLineElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPolylineElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPolygonElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGRectElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGCircleElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGEllipseElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPathElement* element);
	virtual wxSVGCanvasItem* CreateItem(wxSVGTextElement* element,
	  const wxCSSStyleDeclaration* style = NULL) = 0;
	wxSVGCanvasItem* CreateItem(wxSVGImageElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGVideoElement* element);
	
	virtual void DrawItem(wxSVGCanvasItem& item, wxSVGMatrix& matrix,
      const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) = 0;
    
	void DrawLine(wxSVGLineElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawPolyline(wxSVGPolylineElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawPolygon(wxSVGPolygonElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawRect(wxSVGRectElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawCircle(wxSVGCircleElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawEllipse(wxSVGEllipseElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawPath(wxSVGPathElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawText(wxSVGTextElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawImage(wxSVGImageElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	void DrawVideo(wxSVGVideoElement* element,
	  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style = NULL);
	
	inline bool IsItemsCached() { return m_itemsCached; }
	inline void EnableItemsCache(bool enable = true) { m_itemsCached = enable; }
	
  protected:
	bool m_itemsCached;
	
	void DrawCanvasText(wxSVGCanvasText& canvasText, wxSVGMatrix& matrix,
      const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem);
	
	unsigned int GetGradientStops(const wxSVGSVGElement& svgElem,
      const wxString& href, float overall_opacity, const wxSVGElement*& refElem);
	virtual void SetStopValue(unsigned int index, float offset, float opacity, const wxRGBColor& RGBColor) = 0;
	virtual void AllocateGradientStops(unsigned int stop_count) = 0;
	void GetLinearGradientVector(wxSVGPoint& p1, wxSVGPoint& p2,
      const wxSVGLinearGradientElement& gradElem, wxSVGCanvasPath& path);
	void GetRadialGradientTransform(wxSVGPoint& Focus, wxSVGMatrix& matrix,
      const wxSVGRadialGradientElement& gradElem, wxSVGCanvasPath& path,
      bool scale = false);
};

#define WX_SVG_CREATE_M_CANVAS_ITEM \
  if (m_canvasItem == NULL)\
	m_canvasItem = ((wxSVGDocument*) GetOwnerDocument())->GetCanvas()->CreateItem(this);

#define WX_SVG_CLEAR_M_CANVAS_ITEM \
  if (!((wxSVGDocument*) GetOwnerDocument())->GetCanvas()->IsItemsCached())\
  {\
	delete m_canvasItem;\
	m_canvasItem = NULL;\
  }

#endif // WX_SVG_CANVAS_H
