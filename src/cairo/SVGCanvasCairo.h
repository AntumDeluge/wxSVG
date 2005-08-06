//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasCairo.h
// Purpose:     Cairo render
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasCairo.h,v 1.1 2005-08-06 19:25:51 ntalex Exp $
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
    wxImage GetImage();
	void Clear(wxRGBColor color = wxRGBColor(0xFF,0xFF,0xFF));
	
	wxSVGCanvasPath* CreateCanvasPath();
    wxSVGCanvasItem* CreateItem(wxSVGTextElement* element,
	  wxCSSStyleDeclaration* style = NULL);
    
	void DrawItem(wxSVGCanvasItem& item,
	  wxSVGMatrix& matrix, wxCSSStyleDeclaration& style);
  
  protected:
	cairo_t* m_cr;
	cairo_surface_t* m_surface;
    unsigned char* m_data;
	void DrawCanvasPath(wxSVGCanvasPathCairo& canvasPath,
	  wxSVGMatrix& matrix, wxCSSStyleDeclaration& style);
    void DrawCanvasText(wxSVGCanvasTextFreetype& canvasText,
      wxSVGMatrix& matrix, wxCSSStyleDeclaration& style);
};

#endif // WX_SVG_CANVAS_CAIRO_H
