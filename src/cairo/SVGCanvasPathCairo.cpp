//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasPathCairo.cpp
// Purpose:     Cairo canvas path
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasPathCairo.cpp,v 1.3 2011-06-23 11:29:22 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvasPathCairo.h"
#include <wx/log.h>

wxSVGCanvasPathCairo::wxSVGCanvasPathCairo() {
	m_surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 1, 1);
	m_cr = cairo_create(m_surface);
	m_curx = m_cury = m_cubicx = m_cubicy = m_quadx = m_quady = 0;
}

wxSVGCanvasPathCairo::~wxSVGCanvasPathCairo() {
	cairo_destroy(m_cr);
	cairo_surface_destroy(m_surface);
}

void wxSVGCanvasPathCairo::End() {
}

cairo_path_t* wxSVGCanvasPathCairo::GetPath() {
	return cairo_copy_path(m_cr);
}

wxSVGRect wxSVGCanvasPathCairo::GetBBox(const wxSVGMatrix& matrix) {
	double x1, y1, x2, y2;
	if (&matrix) {
		cairo_matrix_t affine;
		affine.xx = matrix.GetA();
		affine.yx = matrix.GetB();
		affine.xy = matrix.GetC();
		affine.yy = matrix.GetD();
		affine.x0 = matrix.GetE();
		affine.y0 = matrix.GetF();
		cairo_set_matrix(m_cr, &affine);
	}
	cairo_fill_extents(m_cr, &x1, &y1, &x2, &y2);
	return wxSVGRect(x1, y1, x2 - x1, y2 - y1);
}

void wxSVGCanvasPathCairo::MoveToImpl(double x, double y) {
	cairo_move_to(m_cr, x, y);
}

void wxSVGCanvasPathCairo::LineToImpl(double x, double y) {
	cairo_line_to(m_cr, x, y);
}

void wxSVGCanvasPathCairo::CurveToCubicImpl(double x1, double y1, double x2,
		double y2, double x, double y) {
	cairo_curve_to(m_cr, x1, y1, x2, y2, x, y);
}

bool wxSVGCanvasPathCairo::ClosePathImpl() {
	cairo_close_path(m_cr);
	return true;
}

