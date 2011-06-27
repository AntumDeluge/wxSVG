//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasTextCairo.h
// Purpose:     Cairo canvas text
// Author:      Alex Thuering
// Created:     2011/06/23
// RCS-ID:      $Id: SVGCanvasTextCairo.cpp,v 1.2 2011-06-27 21:14:58 ntalex Exp $
// Copyright:   (c) 2011 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvasTextCairo.h"
#include "SVGCanvasPathCairo.h"
#include <wx/log.h>

wxSVGCanvasTextCairo::wxSVGCanvasTextCairo(wxSVGCanvas* canvas): wxSVGCanvasText(canvas) {
}

wxSVGCanvasTextCairo::~wxSVGCanvasTextCairo() {
}

void wxSVGCanvasTextCairo::InitText(const wxString& text, const wxCSSStyleDeclaration& style) {
	BeginChar();
	m_char->path->MoveTo(m_tx, m_ty);
	cairo_t* cr = ((wxSVGCanvasPathCairo*) m_char->path)->GetCr();
	// set font
	cairo_select_font_face(cr, style.GetFontFamily().ToAscii(),
			style.GetFontStyle() == wxCSS_VALUE_ITALIC ? CAIRO_FONT_SLANT_ITALIC
				: (style.GetFontStyle() == wxCSS_VALUE_OBLIQUE ? CAIRO_FONT_SLANT_OBLIQUE : CAIRO_FONT_SLANT_NORMAL),
			style.GetFontWeight() == wxCSS_VALUE_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, style.GetFontSize());
	cairo_text_path(cr, (const char*) text.utf8_str());
	
	cairo_text_extents_t extents;
	cairo_text_extents(cr, (const char*) text.utf8_str(), &extents);
	m_char->bbox = wxSVGRect(m_tx + extents.x_bearing, m_ty + extents.y_bearing, extents.width, extents.height);
	m_tx += extents.width;
}
