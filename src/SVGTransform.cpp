//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTransform.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGTransform.cpp,v 1.2 2005-05-30 23:23:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTransform.h"

void wxSVGTransform::SetTranslate(float tx, float ty)
{
  m_type = wxSVG_TRANSFORM_TRANSLATE;
  m_angle = 0;
  m_matrix = wxSVGMatrix(1, 0, 0, 1, tx, ty);
}

void wxSVGTransform::SetScale(float sx, float sy)
{
  m_type = wxSVG_TRANSFORM_SCALE;
  m_angle = 0;
  m_matrix = wxSVGMatrix(sx, 0, 0, sy, 0, 0);
}

void wxSVGTransform::SetRotate(float angle, float cx, float cy)
{
  m_type = wxSVG_TRANSFORM_ROTATE;
  m_angle = angle;
  m_matrix = wxSVGMatrix(1, 0, 0, 1, cx, cy);
  m_matrix = m_matrix.Rotate(angle);
  m_matrix.Translate(-cx, -cy);
}

void wxSVGTransform::SetSkewX(float angle)
{
  m_type = wxSVG_TRANSFORM_SKEWX;
  m_angle = angle;
  m_matrix = wxSVGMatrix(angle, 0, 0, 1, 0, 0);
}

void wxSVGTransform::SetSkewY(float angle)
{
  m_type = wxSVG_TRANSFORM_SKEWY;
  m_angle = angle;
  m_matrix = wxSVGMatrix(1, 0, 0, angle, 0, 0);
}
