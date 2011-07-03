//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasTextCairo.h
// Purpose:     Cairo canvas text
// Author:      Alex Thuering
// Created:     2011/06/23
// RCS-ID:      $Id: SVGCanvasTextCairo.h,v 1.3 2011-07-03 20:51:58 ntalex Exp $
// Copyright:   (c) 2011 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_TEXT_CAIRO_H
#define WX_SVG_CANVAS_TEXT_CAIRO_H

#include "SVGCanvasItem.h"
#include <cairo/cairo.h>

class wxSVGCanvasTextCairo: public wxSVGCanvasText {
public:
	wxSVGCanvasTextCairo(wxSVGCanvas* canvas);
	~wxSVGCanvasTextCairo();

public:
	virtual void InitText(const wxString& text, const wxCSSStyleDeclaration& style);
	static void SetFont(cairo_t* cr, const wxCSSStyleDeclaration& style);
};

#endif // WX_SVG_CANVAS_TEXT_CAIRO_H
