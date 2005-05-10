/////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.h
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/02
// RCS-ID:      $Id: SVGCanvas.h,v 1.1.1.1 2005-05-10 17:51:13 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_H
#define WX_SVG_CANVAS_H

#include "SVGCanvasItem.h"

class wxSVGCanvas: public wxObject
{
  public:
	wxSVGCanvas() { m_itemsCached = true; }
	virtual ~wxSVGCanvas() {}
	
	virtual void SetImage(wxImage* image) = 0;
	virtual void Clear(wxRGBColor color = wxRGBColor(0xFF,0xFF,0xFF)) = 0;
	
	virtual wxSVGCanvasPath* CreateCanvasPath() = 0;
	wxSVGCanvasItem* CreateItem(wxSVGLineElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPolylineElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPolygonElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGRectElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGCircleElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGEllipseElement* element);
	wxSVGCanvasItem* CreateItem(wxSVGPathElement* element);
	
	virtual void DrawItem(wxSVGCanvasItem& item,
	  wxSVGMatrix& matrix, wxCSSStyleDeclaration& style) = 0;
    
	void DrawLine(wxSVGLineElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawPolyline(wxSVGPolylineElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawPolygon(wxSVGPolygonElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawRect(wxSVGRectElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawCircle(wxSVGCircleElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawEllipse(wxSVGEllipseElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	void DrawPath(wxSVGPathElement* element,
	  wxSVGMatrix* matrix = NULL, wxCSSStyleDeclaration* style = NULL);
	
	inline bool IsItemsCached() { return m_itemsCached; }
	inline void EnableItemsCache(bool enable = true) { m_itemsCached = enable; }
	
  protected:
	bool m_itemsCached;
};

#endif // WX_SVG_CANVAS_H
