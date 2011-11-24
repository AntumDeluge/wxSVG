//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/11/23
// RCS-ID:      $Id: SVGVideoElement.cpp,v 1.4 2011-11-24 00:02:55 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGVideoElement.h"
#include "SVGCanvas.h"
#include <wx/log.h>

wxSVGRect wxSVGVideoElement::GetBBox(wxSVG_COORDINATES coordinates) {
	wxSVGRect bbox = wxSVGRect(GetX().GetAnimVal(), GetY().GetAnimVal(), GetWidth().GetAnimVal(),
			GetHeight().GetAnimVal());
	if (coordinates != wxSVG_COORDINATES_USER)
		bbox.MatrixTransform(GetMatrix(coordinates));
	return bbox;
}

wxSVGRect wxSVGVideoElement::GetResultBBox(wxSVG_COORDINATES coordinates) {
	return GetBBox(coordinates);
}

void wxSVGVideoElement::SetCanvasItem(wxSVGCanvasItem* canvasItem) {
	if (m_canvasItem)
		delete m_canvasItem;
	m_canvasItem = canvasItem;
}

double wxSVGVideoElement::GetDuration() {
	if (GetDur() > 0)
		return GetDur();
	if (GetClipEnd() > 0)
		return GetClipEnd() > GetClipBegin() ? GetClipEnd() - GetClipBegin() : 0;
	WX_SVG_CREATE_M_CANVAS_ITEM
	double duration = ((wxSVGCanvasVideo*) m_canvasItem)->GetDuration();
	WX_SVG_CLEAR_M_CANVAS_ITEM
	if (GetClipBegin() > 0)
		duration = duration > GetClipBegin() ? duration - GetClipBegin() : 0;
	return duration;
}
