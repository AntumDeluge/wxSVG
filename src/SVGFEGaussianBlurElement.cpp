//////////////////////////////////////////////////////////////////////////////
// Name:        SVGFEGaussianBlurElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGFEGaussianBlurElement.cpp,v 1.4 2011-06-27 21:14:14 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGFEGaussianBlurElement.h"

void wxSVGFEGaussianBlurElement::SetStdDeviation(double stdDeviationX, double stdDeviationY)
{
  SetStdDeviationX(stdDeviationX);
  SetStdDeviationY(stdDeviationY);
}

bool wxSVGFEGaussianBlurElement::HasCustomAttribute(const wxString& name) {
	return name == wxT("stdDeviation");
}

wxString wxSVGFEGaussianBlurElement::GetCustomAttribute(const wxString& name) {
	return name == wxT("stdDeviation") ? wxString::Format(wxT("%g"), m_stdDeviationX.GetBaseVal()) : wxT("");
}

bool wxSVGFEGaussianBlurElement::SetCustomAttribute(const wxString& name, const wxString& value) {
	double dvalue;
	if (name == wxT("stdDeviation") && value.ToDouble(&dvalue)) {
		m_stdDeviationX.SetBaseVal(dvalue);
		m_stdDeviationY.SetBaseVal(dvalue);
		return true;
	}
	return false;
}
