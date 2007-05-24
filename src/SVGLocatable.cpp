//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLocatable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLocatable.cpp,v 1.10 2007-05-24 08:59:09 etisserant Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLocatable.h"
#include "svg.h"
#include <math.h>
#include <wx/log.h>

wxSVGRect wxSVGLocatable::GetElementBBox(const wxSVGElement* element,
  wxSVG_COORDINATES coordinates)
{
  wxSVGRect elemBBox;
  if (element == NULL || element->GetType() != wxSVGXML_ELEMENT_NODE)
    return elemBBox;
  // SVGLocatable
  if (element->GetDtd() == wxSVG_SVG_ELEMENT)
    return ((wxSVGSVGElement*)element)->GetBBox(coordinates);
  // SVGTransformable
  wxSVGTransformable* transformable =
    wxSVGTransformable::GetSVGTransformable(*(wxSVGElement*)element);
  if (!transformable)
      return elemBBox;
  // GetBBox and transfrom it to parent coordinate system
  if (coordinates != wxSVG_COORDINATES_USER)
    return transformable->GetBBox(coordinates);
  wxSVGMatrix matrix;
  transformable->UpdateMatrix(matrix);
  return transformable->GetBBox().MatrixTransform(matrix);
}

wxSVGRect wxSVGLocatable::GetElementResultBBox(const wxSVGElement* element,
  wxSVG_COORDINATES coordinates)
{
  wxSVGRect elemBBox;
  if (element == NULL || element->GetType() != wxSVGXML_ELEMENT_NODE)
    return elemBBox;
  // SVGLocatable
  if (element->GetDtd() == wxSVG_SVG_ELEMENT)
    return ((wxSVGSVGElement*)element)->GetResultBBox(coordinates);
  // SVGTransformable
  wxSVGTransformable* transformable =
    wxSVGTransformable::GetSVGTransformable(*(wxSVGElement*)element);
  if (!transformable)
      return elemBBox;
  // GetResultBBox and transfrom it to parent coordinate system
  if (coordinates != wxSVG_COORDINATES_USER)
    return transformable->GetResultBBox(coordinates);
  wxSVGMatrix matrix;
  transformable->UpdateMatrix(matrix);
  return transformable->GetResultBBox().MatrixTransform(matrix);
}

wxSVGRect wxSVGLocatable::GetChildrenBBox(const wxSVGElement* element,
  wxSVG_COORDINATES coordinates)
{
  wxSVGRect bbox;
  wxSVGElement* child = (wxSVGElement*) element->GetChildren();
  for (; child != NULL; child = (wxSVGElement*) child->GetNext())
  {
    wxSVGRect childBBox = wxSVGLocatable::GetElementBBox(child, coordinates);
    
    if (childBBox.IsEmpty())
      continue;
    
    if (bbox.IsEmpty())
    {
      bbox = childBBox;
      continue;
    }
    
    if (bbox.GetX() > childBBox.GetX())
    {
      bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - childBBox.GetX());
      bbox.SetX(childBBox.GetX());
    }
    if (bbox.GetY() > childBBox.GetY())
    {
      bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - childBBox.GetY());
      bbox.SetY(childBBox.GetY());
    }
    if (bbox.GetX() + bbox.GetWidth() < childBBox.GetX() + childBBox.GetWidth())
      bbox.SetWidth(childBBox.GetX() + childBBox.GetWidth() - bbox.GetX());
    if (bbox.GetY() + bbox.GetHeight() < childBBox.GetY() + childBBox.GetHeight())
      bbox.SetHeight(childBBox.GetY() + childBBox.GetHeight() - bbox.GetY());
  }
  return bbox;
}

wxSVGRect wxSVGLocatable::GetChildrenResultBBox(const wxSVGElement* element,
  wxSVG_COORDINATES coordinates)
{
  wxSVGRect bbox;
  wxSVGElement* child = (wxSVGElement*) element->GetChildren();
  for (; child != NULL; child = (wxSVGElement*) child->GetNext())
  {
    wxSVGRect childBBox =
      wxSVGLocatable::GetElementResultBBox(child, coordinates);
    
    if (childBBox.IsEmpty())
      continue;
    
    if (bbox.IsEmpty())
    {
      bbox = childBBox;
      continue;
    }
    
    if (bbox.GetX() > childBBox.GetX())
    {
      bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - childBBox.GetX());
      bbox.SetX(childBBox.GetX());
    }
    if (bbox.GetY() > childBBox.GetY())
    {
      bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - childBBox.GetY());
      bbox.SetY(childBBox.GetY());
    }
    if (bbox.GetX() + bbox.GetWidth() < childBBox.GetX() + childBBox.GetWidth())
      bbox.SetWidth(childBBox.GetX() + childBBox.GetWidth() - bbox.GetX());
    if (bbox.GetY() + bbox.GetHeight() < childBBox.GetY() + childBBox.GetHeight())
      bbox.SetHeight(childBBox.GetY() + childBBox.GetHeight() - bbox.GetY());
  }
  return bbox;
}

wxSVGMatrix wxSVGLocatable::GetCTM(const wxSVGElement* element)
{  
  if (element == NULL || element->GetType() != wxSVGXML_ELEMENT_NODE ||
      element->GetDtd() == wxSVG_SVG_ELEMENT)
      return wxSVGMatrix();
  wxSVGMatrix matrix = GetCTM((wxSVGElement*)(element->GetParent()));
  const wxSVGTransformable* transformable =
    wxSVGTransformable::GetSVGTransformable(*element);
  if (!transformable)
      return matrix;
  transformable->UpdateMatrix(matrix);
  return matrix;
}

wxSVGMatrix wxSVGLocatable::GetScreenCTM(const wxSVGElement* element)
{  
  if (element == NULL || element->GetType() != wxSVGXML_ELEMENT_NODE)
      return wxSVGMatrix();
  
  wxSVGMatrix matrix;
  if (element->GetParent())
    matrix = GetScreenCTM((wxSVGElement*)(element->GetParent()));
  else if (element->GetOwnerDocument())
    matrix = ((wxSVGDocument*) element->GetOwnerDocument())->GetScreenCTM();
  
  if (element->GetDtd() == wxSVG_SVG_ELEMENT)
  {
    ((wxSVGSVGElement*)element)->UpdateMatrix(matrix);
  }
  else
  {
    const wxSVGTransformable* transformable =
      wxSVGTransformable::GetSVGTransformable(*element);
    if (transformable)
      transformable->UpdateMatrix(matrix);
  }
  return matrix;
}

wxSVGMatrix wxSVGLocatable::GetTransformToElement(const wxSVGElement& element)
{
  wxSVGMatrix res;
  return res;
}
