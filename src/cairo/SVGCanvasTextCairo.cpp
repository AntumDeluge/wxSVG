//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasTextCairo.h
// Purpose:     Cairo canvas text
// Author:      Alex Thuering
// Created:     2011/06/23
// RCS-ID:      $Id: SVGCanvasTextCairo.cpp,v 1.4 2011-07-22 21:53:02 ntalex Exp $
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
	
	// create path from text
	cairo_t* cr = ((wxSVGCanvasPathCairo*) m_char->path)->GetCr();
	PangoLayout* layout = pango_cairo_create_layout(cr);
	PangoFontDescription* font = pango_font_description_new();
	pango_font_description_set_family(font, style.GetFontFamily().ToAscii());
	pango_font_description_set_weight(font, style.GetFontWeight() == wxCSS_VALUE_BOLD ? PANGO_WEIGHT_BOLD
			: PANGO_WEIGHT_NORMAL);
	pango_font_description_set_style(font, style.GetFontStyle() == wxCSS_VALUE_ITALIC ? PANGO_STYLE_ITALIC
			: (style.GetFontStyle() == wxCSS_VALUE_OBLIQUE ? PANGO_STYLE_OBLIQUE : PANGO_STYLE_NORMAL));
	pango_font_description_set_absolute_size(font, style.GetFontSize() * PANGO_SCALE);
	PangoContext* ctx = pango_layout_get_context(layout);
	PangoFont* f = pango_context_load_font(ctx, font);
	if (f == NULL)
		pango_font_description_set_style(font, PANGO_STYLE_NORMAL);
	pango_layout_set_font_description(layout, font);
	
	if (style.GetTextAnchor() != wxCSS_VALUE_START)
		pango_layout_set_alignment(layout, style.GetTextAnchor() == wxCSS_VALUE_MIDDLE ? PANGO_ALIGN_CENTER : PANGO_ALIGN_RIGHT);
	pango_layout_set_text(layout, (const char*) text.utf8_str(), -1);
	
	int baseline = pango_layout_get_baseline(layout);
	m_char->path->MoveTo(m_tx, m_ty - ((double)baseline / PANGO_SCALE));
	pango_cairo_layout_path(cr, layout);
	
	// set bbox and increase current position (m_tx)
	int lwidth, lheight;
	pango_layout_get_size(layout, &lwidth, &lheight);
	double width = ((double)lwidth / PANGO_SCALE);
	double height = ((double)lheight / PANGO_SCALE);
	m_char->bbox = wxSVGRect(m_tx, m_ty, width, height);
	wxSVGRect bbox = m_char->path->GetResultBBox(style);
	m_tx += width > bbox.GetWidth() ? width : bbox.GetWidth();
		
	g_object_unref(layout);
	pango_font_description_free(font);
}
