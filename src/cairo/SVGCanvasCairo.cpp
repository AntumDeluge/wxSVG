//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasCairo.cpp
// Purpose:     Cairo render
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasCairo.cpp,v 1.7 2011-07-07 19:25:58 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvasCairo.h"
#include "SVGCanvasPathCairo.h"
#include "SVGCanvasTextCairo.h"
#include "SVGCanvasImageCairo.h"
#include <wx/log.h>
#include <wx/file.h>

wxSVGCanvasCairo::~wxSVGCanvasCairo() {
	if (m_cr)
		cairo_destroy(m_cr);
	if (m_surface)
		cairo_surface_destroy(m_surface);
}

void wxSVGCanvasCairo::Init(int width, int height, bool alpha) {
	m_alpha = alpha;
	m_surface = cairo_image_surface_create(alpha ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24, width, height);
	m_cr = cairo_create(m_surface);
}

int wxSVGCanvasCairo::GetWidth() {
	return cairo_image_surface_get_width(m_surface);
}

int wxSVGCanvasCairo::GetHeight() {
	return cairo_image_surface_get_height(m_surface);
}

wxImage wxSVGCanvasCairo::GetImage() {
	wxImage img;
	int width = GetWidth();
	int height = GetHeight();
	img.Create(width, height);
	if (m_alpha)
		img.InitAlpha();
	unsigned char* src = cairo_image_surface_get_data(m_surface);
	unsigned char* dst = img.GetData();
	unsigned char* dstAlpha = img.GetAlpha();
	for (int i = 0; i < width * height; i++) {
		dst[0] = src[2];
		dst[1] = src[1];
		dst[2] = src[0];
		if (dstAlpha != NULL) {
			*dstAlpha = src[4];
			dstAlpha++;
		}
		src += 4;
		dst += 3;
	}
	return img;
}

void wxSVGCanvasCairo::Clear(wxRGBColor color) {
	if (!m_cr || !m_surface || color == *wxBLACK)
		return;
	cairo_set_source_rgb(m_cr, color.Red() / 255.0, color.Green() / 255.0, color.Blue() / 255.0);
	cairo_rectangle(m_cr, 0, 0, GetWidth(), GetHeight());
	cairo_fill(m_cr);
}

wxSVGCanvasPath* wxSVGCanvasCairo::CreateCanvasPath() {
	return new wxSVGCanvasPathCairo();
}

wxSVGCanvasItem* wxSVGCanvasCairo::CreateItem(wxSVGTextElement* element, const wxCSSStyleDeclaration* style) {
	wxSVGCanvasTextCairo* canvasText = new wxSVGCanvasTextCairo(this);
	canvasText->Init(*element, style != NULL ? *style : (wxCSSStyleDeclaration&) element->GetStyle());
	return canvasText;
}

wxSVGCanvasItem* wxSVGCanvasCairo::CreateItem(wxSVGImageElement* element, const wxCSSStyleDeclaration* style) {
	wxSVGCanvasImageCairo* canvasImage = new wxSVGCanvasImageCairo();
	canvasImage->Init(*element, style != NULL ? *style : (wxCSSStyleDeclaration&) element->GetStyle());
	return canvasImage;
}

wxSVGCanvasItem* wxSVGCanvasCairo::CreateItem(wxSVGVideoElement* element, const wxCSSStyleDeclaration* style) {
	wxSVGCanvasVideoCairo* canvasVideo = new wxSVGCanvasVideoCairo();
	canvasVideo->Init(*element, style != NULL ? *style : (wxCSSStyleDeclaration&) element->GetStyle());
	return canvasVideo;
}

void wxSVGCanvasCairo::DrawItem(wxSVGCanvasItem& item, wxSVGMatrix& matrix,
		const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	switch (item.GetType()) {
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_PATH:
		DrawCanvasPath((wxSVGCanvasPathCairo&) item, matrix, style, svgElem);
		break;
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_TEXT: {
		DrawCanvasText((wxSVGCanvasText&) item, matrix, style, svgElem);
		break;
	}
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_IMAGE:
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_VIDEO:
		DrawCanvasImage((wxSVGCanvasImageCairo&) item, matrix, style, svgElem);
		break;
	}
}

void wxSVGCanvasCairo::SetPaint(const wxSVGPaint& paint, float opacity, wxSVGCanvasPathCairo& canvasPath,
		wxSVGSVGElement& svgElem) {
	if (paint.GetPaintType() >= wxSVG_PAINTTYPE_URI_NONE && paint.GetPaintType() <= wxSVG_PAINTTYPE_URI) {
		wxSVGGradientElement* gradElem = GetGradientElement(svgElem, paint.GetUri());
		if (gradElem != NULL) {
			m_pattern = NULL;
			switch (gradElem->GetDtd()) {
			case wxSVG_LINEARGRADIENT_ELEMENT: {
				wxSVGPoint p1, p2;
				GetLinearGradientVector(p1, p2, (wxSVGLinearGradientElement&) *gradElem, canvasPath);
				m_pattern = cairo_pattern_create_linear(p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY());
				break;
			}
			case wxSVG_RADIALGRADIENT_ELEMENT: {
				wxSVGRadialGradientElement* radialGradElem = (wxSVGRadialGradientElement*) gradElem;
				double r = radialGradElem->GetR().GetAnimVal();
				double cx = radialGradElem->GetCx().GetAnimVal();
				double cy = radialGradElem->GetCy().GetAnimVal();
				double fx = radialGradElem->GetFx().GetAnimVal();
				double fy = radialGradElem->GetFy().GetAnimVal();
				
				if (gradElem->GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_UNKNOWN
						|| gradElem->GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_OBJECTBOUNDINGBOX) {
					wxSVGRect bbox = canvasPath.GetBBox();
					r = r * sqrt(bbox.GetWidth() * bbox.GetWidth() + bbox.GetHeight() * bbox.GetHeight());
					cx = bbox.GetX() + cx * bbox.GetWidth();
					cy = bbox.GetY() + cy * bbox.GetHeight();
					fx = bbox.GetX() + fx * bbox.GetWidth();
					fy = bbox.GetY() + fy * bbox.GetHeight();
				}
				m_pattern = cairo_pattern_create_radial(fx, fy, 0.0, cx, cy, r); 
				break;
			}
			default:
				break;
			}
			if (m_pattern != NULL) {
				int nstops = GetGradientStops(svgElem, gradElem, opacity);
				if (nstops)
					cairo_set_source(m_cr, m_pattern);
				else
					cairo_pattern_destroy(m_pattern);
				m_pattern = NULL;
			}
		}
	} else {
		wxRGBColor color = paint.GetRGBColor();
		cairo_set_source_rgba(m_cr, color.Red() / 255.0, color.Green() / 255.0, color.Blue() / 255.0, opacity);
	}
}


void wxSVGCanvasCairo::SetStopValue(unsigned int index, float offset,
		float opacity, const wxRGBColor& rgbColor) {
	cairo_pattern_add_color_stop_rgba(m_pattern, offset, rgbColor.Red() / 255.0, rgbColor.Green() / 255.0,
			rgbColor.Blue() / 255.0, opacity);
}

void wxSVGCanvasCairo::AllocateGradientStops(unsigned int stop_count) {
	// nothing to do
}

void wxSVGCanvasCairo::DrawCanvasPath(wxSVGCanvasPathCairo& canvasPath,
		wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	cairo_matrix_t mat;
	cairo_matrix_init(&mat, matrix.GetA(), matrix.GetB(), matrix.GetC(), matrix.GetD(), matrix.GetE(), matrix.GetF());
	cairo_set_matrix(m_cr, &mat);
	
	// Filling
	if (canvasPath.GetFill() && style.GetFill().Ok() && style.GetFill().GetPaintType() != wxSVG_PAINTTYPE_NONE) {
		cairo_path_t* path = canvasPath.GetPath();
		cairo_append_path(m_cr, path);
		SetPaint(style.GetFill(), style.GetOpacity()*style.GetFillOpacity(), canvasPath, svgElem);
		cairo_fill(m_cr);
		cairo_path_destroy(path);
	}
	
	// Stroking
	if (style.GetStroke().Ok() && style.GetStrokeWidth() > 0
			&& style.GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE) {
		cairo_path_t* path = canvasPath.GetPath();
		cairo_append_path(m_cr, path);
		SetPaint(style.GetStroke(), style.GetOpacity()*style.GetStrokeOpacity(), canvasPath, svgElem);
		cairo_set_line_width(m_cr, style.GetStrokeWidth());
		cairo_stroke(m_cr);
		cairo_path_destroy(path);
	}
}

void wxSVGCanvasCairo::DrawCanvasText(wxSVGCanvasText& canvasText, wxSVGMatrix& matrix,
		const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	for (int i = 0; i < (int) canvasText.m_chunks.Count(); i++) {
		wxSVGCanvasTextChunk& chunk = canvasText.m_chunks[i];
		chunk.style.SetFillRule(wxCSS_VALUE_EVENODD);
		wxSVGMatrix pathMatrix = matrix.Multiply(chunk.matrix);
		if (chunk.style.GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE) {
			for (unsigned int j=0; j<chunk.chars.Count(); j++)
				DrawCanvasPath((wxSVGCanvasPathCairo&) *chunk.chars[j].path, pathMatrix, chunk.style, svgElem);
		} else if (chunk.style.GetFill().GetPaintType() != wxSVG_PAINTTYPE_NONE && chunk.style.GetFillOpacity() > 0) {
			cairo_matrix_t mat;
			cairo_matrix_init(&mat, pathMatrix.GetA(), pathMatrix.GetB(), pathMatrix.GetC(),
					pathMatrix.GetD(), pathMatrix.GetE(), pathMatrix.GetF());
			cairo_set_matrix(m_cr, &mat);
			wxSVGCanvasTextCairo::SetFont(m_cr, chunk.style);
			SetPaint(chunk.style.GetFill(), chunk.style.GetFillOpacity(), (wxSVGCanvasPathCairo&) *chunk.chars[0].path,
					svgElem);
			cairo_move_to(m_cr, chunk.x, chunk.y);
			cairo_show_text(m_cr, (const char*) chunk.text.utf8_str());
		}
	}
}

void wxSVGCanvasCairo::DrawCanvasImage(wxSVGCanvasImageCairo& canvasImage, wxSVGMatrix& matrix,
		const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	if (canvasImage.GetCairoPattern() == NULL)
		return;
	
	cairo_save(m_cr);
	
	cairo_matrix_t mat;
	cairo_matrix_init(&mat, matrix.GetA(), matrix.GetB(), matrix.GetC(), matrix.GetD(), matrix.GetE(), matrix.GetF());
	cairo_set_matrix(m_cr, &mat);
	
	// In case we're scaling the image by using a width and height different
	// than the bitmap's size create a pattern transformation on the surface and
	// draw the transformed pattern.
	wxDouble scaleX = canvasImage.m_width / canvasImage.m_image.GetWidth();
	wxDouble scaleY = canvasImage.m_height / canvasImage.m_image.GetHeight();
	cairo_scale(m_cr, scaleX, scaleY);
	
	// prepare to draw the image
	cairo_set_source(m_cr, canvasImage.GetCairoPattern());
	// use the original size here since the context is scaled already...
	cairo_rectangle(m_cr, canvasImage.m_x, canvasImage.m_y,
			canvasImage.m_image.GetWidth(), canvasImage.m_image.GetHeight());
	// fill the rectangle using the pattern
	cairo_paint_with_alpha(m_cr, style.GetOpacity());
	cairo_new_path(m_cr);
	
	// clean up
	cairo_restore(m_cr);
}
