//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasTextCairo.h
// Purpose:     Cairo canvas text
// Author:      Alex Thuering
// Created:     2011/06/23
// RCS-ID:      $Id: SVGCanvasTextCairo.h,v 1.4 2011-07-22 21:53:02 ntalex Exp $
// Copyright:   (c) 2011 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_TEXT_CAIRO_H
#define WX_SVG_CANVAS_TEXT_CAIRO_H

#include "SVGCanvasItem.h"
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

class wxSVGCanvasTextCairo: public wxSVGCanvasText {
public:
	wxSVGCanvasTextCairo(wxSVGCanvas* canvas);
	~wxSVGCanvasTextCairo();

public:
	virtual void InitText(const wxString& text, const wxCSSStyleDeclaration& style);
	static PangoFontDescription* GetFontDescription(const wxCSSStyleDeclaration& style);
};

#endif // WX_SVG_CANVAS_TEXT_CAIRO_H
