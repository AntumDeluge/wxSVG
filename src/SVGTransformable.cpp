//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTransformable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/05
// RCS-ID:      $Id: SVGTransformable.cpp,v 1.2 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTransformable.h"
#include <wx/log.h>

void wxSVGTransformable::Transform(const wxSVGMatrix& matrix)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  transforms.Add(new wxSVGTransform(matrix));
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::Translate(double tx, double ty)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetTranslate(tx,ty); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::Scale(double s)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetScale(s,s); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::Scale(double sx, double sy)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetScale(sx,sy); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::Rotate(double angle, double cx, double cy)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetRotate(angle, cx,cy); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::SkewX(double angle)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetSkewX(angle); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::SkewY(double angle)
{
  wxSVGTransformList& transforms = m_transform.GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform; t->SetSkewY(angle); transforms.Add(t);
  m_transform.SetAnimVal(transforms);
}

void wxSVGTransformable::UpdateMatrix(wxSVGMatrix& matrix)
{
  const wxSVGTransformList& transforms = GetTransform().GetBaseVal();
  for (int i=0; i<(int)transforms.Count(); i++)
	matrix = matrix.Multiply(transforms[i].GetMatrix());
}
