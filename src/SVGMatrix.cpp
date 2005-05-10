//////////////////////////////////////////////////////////////////////////////
// Name:        SVGMatrix.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/05
// RCS-ID:      $Id: SVGMatrix.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGMatrix.h"
#include <math.h>

wxSVGMatrix wxSVGMatrix::Multiply(const wxSVGMatrix& secondMatrix)
{
  wxSVGMatrix res; 
  res.SetA(GetA()*secondMatrix.GetA() + GetC()*secondMatrix.GetB());
  res.SetB(GetB()*secondMatrix.GetA() + GetD()*secondMatrix.GetB());
  res.SetC(GetA()*secondMatrix.GetC() + GetC()*secondMatrix.GetD());
  res.SetD(GetB()*secondMatrix.GetC() + GetD()*secondMatrix.GetD());
  res.SetE(GetA()*secondMatrix.GetE() + GetC()*secondMatrix.GetF() + GetE());
  res.SetF(GetB()*secondMatrix.GetE() + GetD()*secondMatrix.GetF() + GetF());
  return res;
}

wxSVGMatrix wxSVGMatrix::Inverse()
{
  wxSVGMatrix res;
  return res;
}

wxSVGMatrix wxSVGMatrix::Translate(float x, float y)
{
  return Multiply(wxSVGMatrix(1, 0, 0, 1, x, y));
}

wxSVGMatrix wxSVGMatrix::Scale(float scaleFactor)
{
  return Multiply(wxSVGMatrix(scaleFactor, 0, 0, scaleFactor, 0, 0));
}

wxSVGMatrix wxSVGMatrix::ScaleNonUniform(float scaleFactorX, float scaleFactorY)
{
  return Multiply(wxSVGMatrix(scaleFactorX, 0, 0, scaleFactorY, 0, 0));
}

wxSVGMatrix wxSVGMatrix::Rotate(float angle)
{
  return Multiply(wxSVGMatrix(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0));
}

wxSVGMatrix wxSVGMatrix::RotateFromVector(float x, float y)
{
  return Multiply(wxSVGMatrix(1, 0, 0, 1, 0, 0));
}

wxSVGMatrix wxSVGMatrix::FlipX()
{
  return Multiply(wxSVGMatrix(-1, 0, 0, 1, 0, 0));
}

wxSVGMatrix wxSVGMatrix::FlipY()
{
  return Multiply(wxSVGMatrix(1, 0, 0, -1, 0, 0));
}

wxSVGMatrix wxSVGMatrix::SkewX(float angle)
{
  return Multiply(wxSVGMatrix(1, 0, tan(angle), 1, 0, 0));
}

wxSVGMatrix wxSVGMatrix::SkewY(float angle)
{
  return Multiply(wxSVGMatrix(1, tan(angle), 0, 1, 0, 0));
}

