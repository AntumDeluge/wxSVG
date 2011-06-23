//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasCairo.h
// Purpose:     Cairo render
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasCairo.h,v 1.3 2011-06-23 11:29:22 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_CAIRO_H
#define WX_SVG_CANVAS_CAIRO_H

#include "SVGCanvas.h"
#include <cairo/cairo.h>

class wxSVGCanvasPathCairo;
class wxSVGCanvasTextFreetype;

class wxSVGCanvasCairo: public wxSVGCanvas
{
  public:
	wxSVGCanvasCairo() { m_cr = NULL; m_surface = NULL; m_data = NULL; }
	virtual ~wxSVGCanvasCairo();
	
	void Init(int width, int height);
    int GetWidth();
    int GetHeight();
    wxImage GetImage();
	void Clear(wxRGBColor color = wxRGBColor(0xFF,0xFF,0xFF));
	
	wxSVGCanvasPath* CreateCanvasPath();
    wxSVGCanvasItem* CreateItem(wxSVGTextElement* element, const wxCSSStyleDeclaration* style = NULL);
    
	void DrawItem(wxSVGCanvasItem& item, wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style,
			wxSVGSVGElement& svgElem);
  
  protected:
	cairo_t* m_cr;
	cairo_surface_t* m_surface;
    unsigned char* m_data;
	void DrawCanvasPath(wxSVGCanvasPathCairo& canvasPath,
	  wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style);
    void SetStopValue(unsigned int index, float offset, float opacity,
      const wxRGBColor& rgbColor);
	void AllocateGradientStops(unsigned int stop_count);
};

#endif // WX_SVG_CANVAS_CAIRO_H
