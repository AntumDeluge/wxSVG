//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTransform.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGTransform.cpp,v 1.5 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTransform.h"
#include <math.h>

void wxSVGTransform::SetTranslate(double tx, double ty)
{
  m_type = wxSVG_TRANSFORM_TRANSLATE;
  m_angle = 0;
  m_matrix = wxSVGMatrix(1, 0, 0, 1, tx, ty);
}

void wxSVGTransform::SetScale(double sx, double sy)
{
  m_type = wxSVG_TRANSFORM_SCALE;
  m_angle = 0;
  m_matrix = wxSVGMatrix(sx, 0, 0, sy, 0, 0);
}

void wxSVGTransform::SetRotate(double angle, double cx, double cy)
{
  m_type = wxSVG_TRANSFORM_ROTATE;
  m_angle = angle;
  if (cx == 0 && cy == 0)
  {
	angle = angle*M_PI/180;
	m_matrix = wxSVGMatrix(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0);
  }
  else
  {
	m_matrix = wxSVGMatrix(1, 0, 0, 1, cx, cy);
	m_matrix = m_matrix.Rotate(angle);
	m_matrix = m_matrix.Translate(-cx, -cy);
  }
}

void wxSVGTransform::SetSkewX(double angle)
{
  m_type = wxSVG_TRANSFORM_SKEWX;
  m_angle = angle;
  m_matrix = wxSVGMatrix(1, 0, tan(angle*M_PI/180), 1, 0, 0);
}

void wxSVGTransform::SetSkewY(double angle)
{
  m_type = wxSVG_TRANSFORM_SKEWY;
  m_angle = angle;
  m_matrix = wxSVGMatrix(1, tan(angle*M_PI/180), 0, 1, 0, 0);
}
