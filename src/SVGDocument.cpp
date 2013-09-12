//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.43 2013-09-12 08:42:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGDocument.h"

#ifdef USE_RENDER_AGG
#include "agg/SVGCanvasAgg.h"
#define WX_SVG_CANVAS wxSVGCanvasAgg
#elif defined USE_RENDER_CAIRO
#include "cairo/SVGCanvasCairo.h"
#define WX_SVG_CANVAS wxSVGCanvasCairo
#else //USE_RENDER_LIBART
#include "libart/SVGCanvasLibart.h"
#define WX_SVG_CANVAS wxSVGCanvasLibart
#endif

#include <wx/log.h>

IMPLEMENT_ABSTRACT_CLASS(wxSVGDocument, wxSvgXmlDocument)

wxSVGDocument::wxSVGDocument(const wxSVGDocument& doc): wxSvgXmlDocument(doc) {
	Init();
}

wxSVGDocument::~wxSVGDocument() {
	delete m_canvas;
}

void wxSVGDocument::Init() {
	m_canvas = new WX_SVG_CANVAS;
	m_scale = 1;
	m_scaleY = -1; // == m_scale
	m_time = 0;
}

wxSvgXmlElement* wxSVGDocument::CreateElement(const wxString& tagName) {
	return CreateElementNS(wxT(""), tagName);
}

#include "SVGDocument_CreateElement.cpp"

wxSVGElement* wxSVGDocument::GetElementById(const wxString& id) {
	return GetRootElement() ? (wxSVGElement*) GetRootElement()->GetElementById(id) : NULL;
}

double wxSVGDocument::GetDuration(wxSVGElement* parent) {
	float result = 0;
	wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
	while (elem) {
		float duration = 0;
		if (elem->GetType() == wxSVGXML_ELEMENT_NODE && elem->GetDtd() == wxSVG_VIDEO_ELEMENT)
			duration = ((wxSVGVideoElement*) elem)->GetBegin() + ((wxSVGVideoElement*) elem)->GetDuration();
		else if (elem->GetChildren())
			duration = GetDuration(elem);
		if (result < duration)
			result = duration;
		elem = (wxSVGElement*) elem->GetNext();
	}
	return result;
}

double wxSVGDocument::GetDuration() {
	return GetDuration(GetRootElement());
}

void ApplyAnimation(wxSVGElement* parent) {
	wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
	while (elem) {
		if (elem->GetType() == wxSVGXML_ELEMENT_NODE) {
			switch (elem->GetDtd()) {
				case wxSVG_ANIMATE_ELEMENT:
					((wxSVGAnimateElement*) elem)->ApplyAnimation();
					break;
				default:
					ApplyAnimation(elem);
					break;
			}
		}
		elem = (wxSVGElement*) elem->GetNext();
	}
}

void wxSVGDocument::SetCurrentTime(double seconds) {
	m_time = seconds;
	// animation
	if (GetRootElement())
		ApplyAnimation(GetRootElement());
}

wxImage wxSVGDocument::Render(int width, int height, const wxSVGRect* rect, bool preserveAspectRatio, bool alpha,
		wxProgressDialog* progressDlg) {
	if (!GetRootElement())
		return wxImage();
	
	m_screenCTM = wxSVGMatrix();
	
	if (GetRootElement()->GetWidth().GetBaseVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
		GetRootElement()->SetWidth(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
	if (GetRootElement()->GetHeight().GetBaseVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
		GetRootElement()->SetHeight(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
	
	if (width == -1 || height == -1) {
		width = (int) GetRootElement()->GetWidth().GetAnimVal();
		height = (int) GetRootElement()->GetHeight().GetAnimVal();
		if (width <= 0 || height <= 0) {
			width = (int) GetRootElement()->GetViewBox().GetAnimVal().GetWidth();
			height = (int) GetRootElement()->GetViewBox().GetAnimVal().GetHeight();
		}
	}
	
	if (GetRootElement()->GetWidth().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_PERCENTAGE) {
		wxSVGAnimatedLength l = GetRootElement()->GetWidth();
		l.GetBaseVal().ToViewportWidth(width);
		if (l.GetBaseVal() != ((const wxSVGAnimatedLength&) l).GetAnimVal())
			l.GetAnimVal().ToViewportWidth(width);
		GetRootElement()->SetWidth(l);
	}
	if (GetRootElement()->GetHeight().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_PERCENTAGE) {
		wxSVGAnimatedLength l = GetRootElement()->GetHeight();
		l.GetBaseVal().ToViewportHeight(height);
		if (l.GetBaseVal() != ((const wxSVGAnimatedLength&) l).GetAnimVal())
			l.GetAnimVal().ToViewportHeight(height);
		GetRootElement()->SetHeight(l);
	}
	
	// scale it to fit in
	m_scale = 1;
	m_scaleY = -1; // == m_scale
	if (GetRootElement()->GetWidth().GetAnimVal() > 0 && GetRootElement()->GetHeight().GetAnimVal() > 0) {
		if (preserveAspectRatio) {
			m_scale = width / GetRootElement()->GetWidth().GetAnimVal();
			if (m_scale > height / GetRootElement()->GetHeight().GetAnimVal())
				m_scale = height / GetRootElement()->GetHeight().GetAnimVal();
			m_screenCTM = m_screenCTM.Scale(m_scale);

			width = (int) (m_scale * GetRootElement()->GetWidth().GetAnimVal());
			height = (int) (m_scale * GetRootElement()->GetHeight().GetAnimVal());
		} else {
			m_scale = width / GetRootElement()->GetWidth().GetAnimVal();
			m_scaleY = height / GetRootElement()->GetHeight().GetAnimVal();
			m_screenCTM = m_screenCTM.ScaleNonUniform(m_scale, m_scaleY);
		}
	}
	
	// render only rect if specified
	if (rect && !rect->IsEmpty()) {
		m_screenCTM = m_screenCTM.Translate(-rect->GetX(), -rect->GetY());
		if (rect->GetWidth() * GetScaleX() < width)
			width = (int) (rect->GetWidth() * GetScaleX());
		if (rect->GetHeight() * GetScaleY() < height)
			height = (int) (rect->GetHeight() * GetScaleY());
	}
	
	// render
	m_canvas->Init(width, height, alpha);
	if (!alpha)
		m_canvas->Clear(*wxWHITE);
	GetCanvas()->RenderElement(GetRootElement(), rect, &m_screenCTM, &GetRootElement()->GetStyle(), NULL, NULL,
			progressDlg);

	return m_canvas->GetImage();
}

wxImage wxSVGDocument::RenderElementById(const wxString& id) {
	wxSVGElement* element = GetElementById(id);
	const wxSVGRect* rect = NULL;
	m_screenCTM = wxSVGMatrix();
	m_screenCTM = m_screenCTM.Translate(-10, -10);
	
	m_canvas->Clear();
	GetCanvas()->RenderElement(element, rect, &m_screenCTM, &GetRootElement()->GetStyle(), NULL, NULL, NULL);
	return m_canvas->GetImage();
}
