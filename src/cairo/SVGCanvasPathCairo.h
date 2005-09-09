//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasPathCairo.h
// Purpose:     Cairo canvas path
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasPathCairo.h,v 1.1.2.2 2005-09-09 09:36:46 etisserant Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_PATH_CAIRO_H
#define WX_SVG_CANVAS_PATH_CAIRO_H

#include "SVGCanvas.h"
#include <cairo/cairo.h>

class wxSVGCanvasPathCairo: public wxSVGCanvasPath
{
  protected:
	cairo_t* m_cr;
	cairo_surface_t* m_surface;
	cairo_path_t* m_cairo_path;
	double m_curx, m_cury, m_cubicx, m_cubicy, m_quadx, m_quady;
	
  public:
	wxSVGCanvasPathCairo();
	virtual ~wxSVGCanvasPathCairo();
    
	void End();
	wxSVGRect GetBBox();
	
	cairo_path_t* GetPath();
	
	void MoveToImpl(double x, double y);
	void LineToImpl(double x, double y);
	void CurveToCubicImpl(double x1, double y1, double x2, double y2, double x, double y);
	bool ClosePathImpl();
};

#endif // WX_SVG_CANVAS_PATH_CAIRO_H
