//////////////////////////////////////////////////////////////////////////////
// Name:        ElementTransform.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: ElementTransform.cpp,v 1.4 2008-05-23 16:36:49 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include <list>
#include "ElementTransform.h"

typedef struct
{
  int id;
} ELEMENT_INIT;

std::list<ELEMENT_INIT> element_init_list;

void ClearList(void) {
  element_init_list.clear();
}

bool HasBeenInitialised(wxSVGElement* elem)
{
  int svguiid = GetSVGUIID(elem->GetId());
  std::list<ELEMENT_INIT>::iterator pt;
  for(pt = element_init_list.begin(); pt != element_init_list.end(); pt++) {
    if (pt->id == svguiid)
      return true;
  }
  return false;
}

void _InitElementMatrix(wxSVGElement* elem)
{
  wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
  wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
  wxSVGTransform* t = new wxSVGTransform;
  wxSVGMatrix matrix;
  transforms.Add(t);
  t->SetMatrix(matrix);
  element->SetTransform(transforms);
  ELEMENT_INIT *element_init;
  element_init = new ELEMENT_INIT;
  element_init->id = GetSVGUIID(elem->GetId());
  element_init_list.push_back(*element_init);
}

void _ResetElementMatrix(wxSVGElement* elem)
{
  wxSVGTransformable* element = wxSVGTransformable::GetSVGTransformable(*elem);
  wxSVGTransformList transforms = element->GetTransform().GetBaseVal();
  wxSVGMatrix matrix;
  transforms[transforms.Count()-1].SetMatrix(matrix);
  element->SetTransform(transforms);
}

void ResetElementMatrix(wxSVGElement* elem)
{
  if (HasBeenInitialised(elem))
    _ResetElementMatrix(elem);
  else
    _InitElementMatrix(elem);
}

void MoveElement(wxSVGElement* elem, double Xposition, double Yposition)
{
  if (!HasBeenInitialised(elem))
    _InitElementMatrix(elem);
  
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
  if (!HasBeenInitialised(elem))
    _InitElementMatrix(elem);
  
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
  if (!HasBeenInitialised(elem))
    _InitElementMatrix(elem);
  
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
  if (!HasBeenInitialised(elem))
    _InitElementMatrix(elem);
  
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
