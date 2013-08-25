//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasImageCairo.h
// Purpose:     Cairo canvas image
// Author:      Alex Thuering
// Created:     2011/06/23
// RCS-ID:      $Id: SVGCanvasImageCairo.h,v 1.3 2013-08-25 12:53:34 ntalex Exp $
// Copyright:   (c) 2011 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_IMAGE_CAIRO_H
#define WX_SVG_CANVAS_IMAGE_CAIRO_H

#include "SVGCanvasItem.h"
#include <cairo/cairo.h>

class wxSVGCanvasImageCairoData {
public:
	wxSVGCanvasImageCairoData(wxImage image);
	~wxSVGCanvasImageCairoData();
	
	void IncRef() { m_count++; }
	int DecRef() { return (--m_count); }
	
	cairo_pattern_t* GetPattern() { return m_pattern; }
	
private:
    int m_count;
	unsigned char* m_buffer;
	cairo_surface_t* m_surface;
	cairo_pattern_t* m_pattern;
};

class wxSVGCanvasImageCairo: public wxSVGCanvasImage {
public:
	wxSVGCanvasImageCairo();
	wxSVGCanvasImageCairo(wxSVGCanvasItemType type);
	virtual ~wxSVGCanvasImageCairo();
	virtual void Init(wxSVGImageElement& element, const wxCSSStyleDeclaration& style, wxProgressDialog* progressDlg);
	virtual cairo_pattern_t* GetCairoPattern() { return m_data != NULL ? m_data->GetPattern() : NULL; }
	
private:
	wxSVGCanvasImageCairoData* m_data;
};

class wxSVGCanvasVideoCairo: public wxSVGCanvasVideo {
public:
	wxSVGCanvasVideoCairo();
	virtual ~wxSVGCanvasVideoCairo();
	virtual void Init(wxSVGVideoElement& element, const wxCSSStyleDeclaration& style, wxProgressDialog* progressDlg);
	virtual cairo_pattern_t* GetCairoPattern() { return m_data != NULL ? m_data->GetPattern() : NULL; }
	
private:
	wxSVGCanvasImageCairoData* m_data;
};

#endif // WX_SVG_CANVAS_IMAGE_CAIRO_H
