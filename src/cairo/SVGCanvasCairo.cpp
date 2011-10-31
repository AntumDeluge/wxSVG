//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasCairo.cpp
// Purpose:     Cairo render
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasCairo.cpp,v 1.14 2011-10-31 07:53:48 ntalex Exp $
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
	Destroy();
}

void wxSVGCanvasCairo::Destroy() {
	if (m_pattern) {
		cairo_pattern_destroy(m_pattern);
		m_pattern = NULL;
	}
	if (m_cr)
		cairo_destroy(m_cr);
	if (m_surface)
		cairo_surface_destroy(m_surface);
}

void wxSVGCanvasCairo::Init(int width, int height, bool alpha) {
	if (m_surface != NULL && m_alpha == alpha
			&& cairo_image_surface_get_width(m_surface) == width
			&& cairo_image_surface_get_height(m_surface) == height) {
		if (alpha) {
			cairo_set_source_rgba(m_cr, 0.0, 0.0, 0.0, 0.0);
			cairo_rectangle(m_cr, 0, 0, GetWidth(), GetHeight());
			cairo_fill(m_cr);
		}
		return;
	}
	Destroy();
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
			*dstAlpha = src[3];
			dstAlpha++;
		}
		src += 4;
		dst += 3;
	}
	return img;
}

void wxSVGCanvasCairo::Clear(wxRGBColor color) {
	if (!m_cr || !m_surface)
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
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_TEXT:
		DrawCanvasText((wxSVGCanvasText&) item, matrix, style, svgElem);
		break;
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_IMAGE:
		DrawCanvasImage((wxSVGCanvasImage&) item, ((wxSVGCanvasImageCairo&) item).GetCairoPattern(),
				matrix, style, svgElem);
		break;
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_VIDEO:
		DrawCanvasImage((wxSVGCanvasImage&) item, ((wxSVGCanvasVideoCairo&) item).GetCairoPattern(),
				matrix, style, svgElem);
		break;
	}
}

void wxSVGCanvasCairo::SetPaint(cairo_t* cr, const wxSVGPaint& paint, float opacity, wxSVGCanvasPathCairo& canvasPath,
		wxSVGSVGElement& svgElem) {
	if (paint.GetPaintType() >= wxSVG_PAINTTYPE_URI_NONE && paint.GetPaintType() <= wxSVG_PAINTTYPE_URI) {
		wxSVGGradientElement* gradElem = GetGradientElement(svgElem, paint.GetUri());
		if (gradElem != NULL) {
			if (m_pattern != NULL) {
				cairo_pattern_destroy(m_pattern);
				m_pattern = NULL;
			}
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
				if (nstops) {
					cairo_set_source(cr, m_pattern);
				} else {
					cairo_pattern_destroy(m_pattern);
					m_pattern = NULL;
				}
			}
		}
	} else {
		wxRGBColor color = paint.GetRGBColor();
		cairo_set_source_rgba(cr, color.Red() / 255.0, color.Green() / 255.0, color.Blue() / 255.0, opacity);
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

/**
 * Steve Hanov, 2009
 * Released into the public domain.
 */
void cairo_image_surface_blur(cairo_surface_t* surface, double radius) {
	// get width, height
	int width = cairo_image_surface_get_width(surface);
	int height = cairo_image_surface_get_height(surface);
	unsigned char* dst = (unsigned char*) malloc(width * height * 4);
	unsigned* precalc = (unsigned*) malloc(width * height * sizeof(unsigned));
	unsigned char* src = cairo_image_surface_get_data(surface);
	double mul = 1.f / ((radius * 2) * (radius * 2));
	
	// The number of times to perform the averaging. According to wikipedia,
	// three iterations is good enough to pass for a gaussian.
	const int MAX_ITERATIONS = 3;
	
	memcpy(dst, src, width * height * 4);
	for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
		for (int channel = 0; channel < 4; channel++) {
			// precomputation step.
			unsigned char* pix = src;
			unsigned* pre = precalc;
			
			pix += channel;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int tot = pix[0];
					if (x > 0)
						tot += pre[-1];
					if (y > 0)
						tot += pre[-width];
					if (x > 0 && y > 0)
						tot -= pre[-width - 1];
					*pre++ = tot;
					pix += 4;
				}
			}

			// blur step.
			pix = dst + (int) radius * width * 4 + (int) radius * 4 + channel;
			for (int y = radius; y < height - radius; y++) {
				for (int x = radius; x < width - radius; x++) {
					int l = x < radius ? 0 : x - radius;
					int t = y < radius ? 0 : y - radius;
					int r = x + radius >= width ? width - 1 : x + radius;
					int b = y + radius >= height ? height - 1 : y + radius;
					int tot = precalc[r + b * width] + precalc[l + t * width] - precalc[l + b * width]
							- precalc[r + t* width];
					*pix = (unsigned char) (tot * mul);
					pix += 4;
				}
				pix += (int) radius * 2 * 4;
			}
		}
		memcpy(src, dst, width * height * 4);
	}

	free(dst);
	free(precalc);
}

void wxSVGCanvasCairo::DrawPath(cairo_t* cr, wxSVGCanvasPathCairo& canvasPath, wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style,
		wxSVGSVGElement& svgElem) {
	cairo_matrix_t mat;
	cairo_matrix_init(&mat, matrix.GetA(), matrix.GetB(), matrix.GetC(), matrix.GetD(), matrix.GetE(), matrix.GetF());
	cairo_set_matrix(cr, &mat);
	
	// Filling
	if (canvasPath.GetFill() && style.GetFill().Ok() && style.GetFill().GetPaintType() != wxSVG_PAINTTYPE_NONE) {
		cairo_path_t* path = canvasPath.GetPath();
		cairo_append_path(cr, path);
		SetPaint(cr, style.GetFill(), style.GetOpacity()*style.GetFillOpacity(), canvasPath, svgElem);
		cairo_fill(cr);
		cairo_path_destroy(path);
	}
	
	// Stroking
	if (style.GetStroke().Ok() && style.GetStrokeWidth() > 0
			&& style.GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE) {
		cairo_path_t* path = canvasPath.GetPath();
		cairo_append_path(cr, path);
		SetPaint(cr, style.GetStroke(), style.GetOpacity()*style.GetStrokeOpacity(), canvasPath, svgElem);
		wxSVGCanvasPathCairo::ApplyStrokeStyle(cr, style);
		cairo_stroke(cr);
		cairo_path_destroy(path);
	}
}

void wxSVGCanvasCairo::DrawCanvasPath(wxSVGCanvasPathCairo& canvasPath, wxSVGMatrix& matrix,
		const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	// check Filter
	if (style.GetFilter().GetCSSPrimitiveType() == wxCSS_URI && style.GetFilter().GetStringValue().length() > 1) {
		wxString filterId = style.GetFilter().GetStringValue().substr(1);
		wxSVGElement* filterElem = (wxSVGSVGElement*) svgElem.GetElementById(filterId);
		// feGaussianBlur
		if (filterElem && filterElem->GetDtd() == wxSVG_FILTER_ELEMENT && filterElem->GetFirstChild() != NULL
				&& ((wxSVGSVGElement*) filterElem->GetFirstChild())->GetDtd() == wxSVG_FEGAUSSIANBLUR_ELEMENT) {
			float radius = ((wxSVGFEGaussianBlurElement*) filterElem->GetFirstChild())->GetStdDeviationX().GetAnimVal();
			wxSVGRect rect = canvasPath.GetResultBBox(style, matrix.Inverse());
			rect.SetX(rect.GetX() - radius*2);
			rect.SetY(rect.GetY() - radius*2);
			rect.SetWidth(rect.GetWidth() + radius*4);
			rect.SetHeight(rect.GetHeight() + radius*4);
			int width = (int) rect.GetWidth();
			int height = (int) rect.GetHeight();
			cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
			cairo_t* cr = cairo_create(surface);
			wxSVGMatrix matrix2 = wxSVGMatrix(1, 0, 0, 1, - rect.GetX(), - rect.GetY()).Multiply(matrix);
			DrawPath(cr, canvasPath, matrix2, style, svgElem);
			cairo_image_surface_blur(surface, radius);
			
			// draw surface
			cairo_save(m_cr);
			cairo_matrix_t mat;
			cairo_matrix_init(&mat, 1, 0, 0, 1, rect.GetX(), rect.GetY());
			cairo_set_matrix(m_cr, &mat);
			
			cairo_pattern_t* pattern = cairo_pattern_create_for_surface(surface);
			cairo_set_source(m_cr, pattern);
			cairo_rectangle(m_cr, 0, 0, width, height);
			cairo_paint(m_cr); // fill the rectangle using the pattern
			cairo_new_path(m_cr);
			cairo_restore(m_cr);
			cairo_destroy(cr);
			cairo_surface_destroy(surface);
			return;
		}
	}
	DrawPath(m_cr, canvasPath, matrix, style, svgElem);
}

void wxSVGCanvasCairo::DrawCanvasImage(wxSVGCanvasImage& canvasImage, cairo_pattern_t* cairoPattern,
		wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem) {
	if (cairoPattern == NULL)
		return;
	
	cairo_save(m_cr);
	
	cairo_matrix_t mat;
	cairo_matrix_init(&mat, matrix.GetA(), matrix.GetB(), matrix.GetC(), matrix.GetD(), matrix.GetE(), matrix.GetF());
	cairo_set_matrix(m_cr, &mat);
	
	// scale context
	double x = canvasImage.m_x;
	double y = canvasImage.m_y;
	double scaleX = canvasImage.m_width / canvasImage.m_image.GetWidth();
	double scaleY = canvasImage.m_height / canvasImage.m_image.GetHeight();
	wxSVG_PRESERVEASPECTRATIO align = canvasImage.GetPreserveAspectRatio().GetAlign();
	bool alignX = false;
	if (align > wxSVG_PRESERVEASPECTRATIO_NONE) {
		if (canvasImage.GetPreserveAspectRatio().GetMeetOrSlice() != wxSVG_MEETORSLICE_SLICE) {
			alignX = scaleX > scaleY;
		} else {
			cairo_rectangle(m_cr, x, y, canvasImage.m_width, canvasImage.m_height);
			cairo_clip(m_cr);
			alignX = scaleX < scaleY;
		}
		if (alignX) {
			scaleX = scaleY;
			if (align == wxSVG_PRESERVEASPECTRATIO_XMIDYMIN
					|| align == wxSVG_PRESERVEASPECTRATIO_XMIDYMID
					|| align == wxSVG_PRESERVEASPECTRATIO_XMIDYMAX)
				x += (canvasImage.m_width - canvasImage.m_image.GetWidth() * scaleX) / 2;
			else if (align == wxSVG_PRESERVEASPECTRATIO_XMAXYMIN
					|| align == wxSVG_PRESERVEASPECTRATIO_XMAXYMID
					|| align == wxSVG_PRESERVEASPECTRATIO_XMAXYMAX)
				x += canvasImage.m_width - canvasImage.m_image.GetWidth() * scaleX;
		} else {
			scaleY = scaleX;
			if (align == wxSVG_PRESERVEASPECTRATIO_XMINYMID
					|| align == wxSVG_PRESERVEASPECTRATIO_XMIDYMID
					|| align == wxSVG_PRESERVEASPECTRATIO_XMAXYMID)
				y += (canvasImage.m_height - canvasImage.m_image.GetHeight() * scaleY) / 2;
			else if (align == wxSVG_PRESERVEASPECTRATIO_XMINYMAX
					|| align == wxSVG_PRESERVEASPECTRATIO_XMIDYMAX
					|| align == wxSVG_PRESERVEASPECTRATIO_XMAXYMAX)
				y += canvasImage.m_height - canvasImage.m_image.GetHeight() * scaleY;
		}
	}
	cairo_translate(m_cr, x, y);
	cairo_scale(m_cr, scaleX, scaleY);
	
	// prepare to draw the image
	cairo_set_source(m_cr, cairoPattern);
	// use the original size here since the context is scaled already...
	cairo_rectangle(m_cr, 0, 0, canvasImage.m_image.GetWidth(), canvasImage.m_image.GetHeight());
	// fill the rectangle using the pattern
	cairo_paint_with_alpha(m_cr, style.GetOpacity());
	cairo_new_path(m_cr);
	
	// clean up
	cairo_restore(m_cr);
}
