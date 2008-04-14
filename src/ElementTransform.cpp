//////////////////////////////////////////////////////////////////////////////
// Name:        ElementTransform.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: ElementTransform.cpp,v 1.3 2008-04-14 15:44:05 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "ElementTransform.h"

void InitElementMatrix(wxSVGElement* elem)
{
  wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
  wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform;
  transforms.Add(t);
  wxSVGMatrix matrix;
  transforms[transforms.Count()-1].SetMatrix(matrix);
  element->SetTransform(transforms);
}

void ResetElementMatrix(wxSVGElement* elem)
{
  wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
  wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
  wxSVGMatrix matrix;
  transforms[transforms.Count()-1].SetMatrix(matrix);
  element->SetTransform(transforms);
}

void MoveElement(wxSVGElement* elem, double Xposition, double Yposition)
{
  if (elem->GetDtd() == wxSVG_RECT_ELEMENT)
  {
    double stroke_width = 0;
      if (((wxSVGRectElement*)elem)->GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE)
        stroke_width = ((wxSVGRectElement*)elem)->GetStrokeWidth();
    wxSVGMatrix CTM = ((wxSVGRectElement*)elem)->GetCTM();
    double denom = CTM.GetB()*CTM.GetC() - CTM.GetA()*CTM.GetD();
    double x = (CTM.GetC()*(Yposition-CTM.GetF()) - CTM.GetD()*(Xposition-CTM.GetE())) / denom;
    double y = (CTM.GetB()*(Xposition-CTM.GetE()) - CTM.GetA()*(Yposition-CTM.GetF())) / denom;
    //wxSVGLength Xvalue(x + stroke_width / 2);
    //wxSVGLength Yvalue(y + stroke_width / 2);
    wxSVGLength Xvalue(x);
    wxSVGLength Yvalue(y);
    ((wxSVGRectElement*)elem)->SetX(Xvalue);
    ((wxSVGRectElement*)elem)->SetY(Yvalue);
  }
  else
  {
    wxSVGRect bbox = wxSVGLocatable::GetElementBBox(elem);
    wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
    wxSVGMatrix CTM = element->GetCTM();
    wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
    wxSVGMatrix matrix = transforms[(int)transforms.Count()-1].GetMatrix();
    wxSVGPoint LeftUp = wxSVGPoint(bbox.GetX(), bbox.GetY());
    
    wxSVGMatrix new_matrix = wxSVGMatrix();
    new_matrix = new_matrix.Translate(Xposition - LeftUp.GetX(), Yposition - LeftUp.GetY());
    new_matrix = matrix.Multiply(CTM.Inverse().Multiply(new_matrix.Multiply(CTM)));
    transforms[transforms.Count()-1].SetMatrix(new_matrix);
    element->SetTransform(transforms);
  }
}

void MoveElementByCenter(wxSVGElement* elem, double Xposition, double Yposition)
{
  wxSVGRect bbox = wxSVGLocatable::GetElementBBox(elem);
  if (elem->GetDtd() == wxSVG_RECT_ELEMENT)
  {
    double stroke_width = 0;
      if (((wxSVGRectElement*)elem)->GetStroke().GetPaintType() != wxSVG_PAINTTYPE_NONE)
        stroke_width = ((wxSVGRectElement*)elem)->GetStrokeWidth();
    wxSVGMatrix CTM = ((wxSVGRectElement*)elem)->GetCTM();
    double denom = CTM.GetB()*CTM.GetC() - CTM.GetA()*CTM.GetD();
    double xcenter = (CTM.GetC()*(Yposition-CTM.GetF()) - CTM.GetD()*(Xposition-CTM.GetE())) / denom;
    double ycenter = (CTM.GetB()*(Xposition-CTM.GetE()) - CTM.GetA()*(Yposition-CTM.GetF())) / denom;
    //wxSVGLength Xvalue(xcenter + (stroke_width - bbox.GetWidth()) / 2);
    //wxSVGLength Yvalue(ycenter + (stroke_width - bbox.GetHeight()) / 2);
    wxSVGLength Xvalue(xcenter - bbox.GetWidth() / 2);
    wxSVGLength Yvalue(ycenter - bbox.GetHeight() / 2);
    ((wxSVGRectElement*)elem)->SetX(Xvalue);
    ((wxSVGRectElement*)elem)->SetY(Yvalue);
  }
  else
  {
    wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
        wxSVGMatrix CTM = element->GetCTM();
    wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
    wxSVGMatrix matrix = transforms[(int)transforms.Count()-1].GetMatrix();
    wxSVGPoint Center = wxSVGPoint(bbox.GetX() + bbox.GetWidth(), bbox.GetY() + bbox.GetHeight());
    wxSVGMatrix new_matrix = wxSVGMatrix();
    new_matrix = new_matrix.Translate(Xposition - Center.GetX(), Yposition - Center.GetY());
    new_matrix = matrix.Multiply(CTM.Inverse().Multiply(new_matrix.Multiply(CTM)));
    transforms[transforms.Count()-1].SetMatrix(new_matrix);
    element->SetTransform(transforms);
  }
}

void ScaleElement(wxSVGElement* elem, double Xscale, double Yscale)
{
  if (elem->GetDtd() == wxSVG_RECT_ELEMENT)
  {
    wxSVGLength Width(((wxSVGRectElement*)elem)->GetWidth().GetBaseVal()*Xscale);
    wxSVGLength Height(((wxSVGRectElement*)elem)->GetHeight().GetBaseVal()*Yscale);
    ((wxSVGRectElement*)elem)->SetWidth(Width);
    ((wxSVGRectElement*)elem)->SetHeight(Height);
  }
  else
  {
    wxSVGRect bbox = wxSVGLocatable::GetElementBBox(elem);
    wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
        wxSVGMatrix CTM = element->GetCTM();
    wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
    wxSVGMatrix matrix = transforms[(int)transforms.Count()-1].GetMatrix();
    wxSVGPoint LeftUp = wxSVGPoint(bbox.GetX(), bbox.GetY());
    wxSVGMatrix new_matrix = wxSVGMatrix();
    new_matrix = new_matrix.Translate(LeftUp.GetX(),LeftUp.GetY());
    new_matrix = new_matrix.ScaleNonUniform(Xscale,Yscale);
    new_matrix = new_matrix.Translate(-LeftUp.GetX(),-LeftUp.GetY());
    new_matrix = matrix.Multiply(CTM.Inverse().Multiply(new_matrix.Multiply(CTM)));
    transforms[transforms.Count()-1].SetMatrix(new_matrix);
    element->SetTransform(transforms);
  }
}

void RotateElement(wxSVGElement* elem, double angle, wxSVGPoint* center)
{
  wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
    wxSVGMatrix CTM = element->GetCTM();
  wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
  wxSVGMatrix matrix = transforms[(int)transforms.Count()-1].GetMatrix();
  wxSVGPoint Center;
  if (center)
    Center = *center;
  else
  {
    wxSVGRect bbox = wxSVGLocatable::GetElementBBox(elem);
    Center = wxSVGPoint(bbox.GetX() + bbox.GetWidth()/2, bbox.GetY() + bbox.GetHeight()/2);
  }
  wxSVGMatrix new_matrix = wxSVGMatrix();
  new_matrix = new_matrix.Translate(Center.GetX(),Center.GetY());
  new_matrix = new_matrix.Rotate(-angle);
  new_matrix = new_matrix.Translate(-Center.GetX(),-Center.GetY());
  new_matrix = matrix.Multiply(CTM.Inverse().Multiply(new_matrix.Multiply(CTM)));
  transforms[transforms.Count()-1].SetMatrix(new_matrix);
  element->SetTransform(transforms);
}
