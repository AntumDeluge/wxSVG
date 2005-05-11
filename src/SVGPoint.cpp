//////////////////////////////////////////////////////////////////////////////
// Name:        SVGPoint.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGPoint.cpp,v 1.2 2005-05-11 20:05:40 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGPoint.h"

wxSVGPoint wxSVGPoint::MatrixTransform(const wxSVGMatrix& matrix)
{
  wxSVGPoint res(m_x*matrix.GetA() + m_y*matrix.GetC() + matrix.GetE(),
	m_x*matrix.GetB() + m_y*matrix.GetD() + matrix.GetF());
  return res;
}

