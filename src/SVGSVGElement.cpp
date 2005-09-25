//////////////////////////////////////////////////////////////////////////////
// Name:        SVGSVGElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGSVGElement.cpp,v 1.5 2005-09-25 11:41:06 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGSVGElement.h"

unsigned long wxSVGSVGElement::SuspendRedraw(unsigned long max_wait_milliseconds)
{
  return 0;
}

void wxSVGSVGElement::UnsuspendRedraw(unsigned long suspend_handle_id)
{

}

void wxSVGSVGElement::UnsuspendRedrawAll()
{

}

void wxSVGSVGElement::ForceRedraw()
{

}

void wxSVGSVGElement::PauseAnimations()
{

}

void wxSVGSVGElement::UnpauseAnimations()
{

}

bool wxSVGSVGElement::AnimationsPaused()
{
  return false;
}

double wxSVGSVGElement::GetCurrentTime()
{
  return 0;
}

void wxSVGSVGElement::SetCurrentTime(double seconds)
{

}

void RecurseIntersectionList(const wxSVGSVGElement& root, const wxSVGElement& element, const wxSVGRect& rect, wxNodeList& res)
{
  if (((wxSVGSVGElement*)&root)->CheckIntersection(element, rect))
  {
    res.Add((wxSVGElement*)&element);
    wxSVGElement* n = (wxSVGElement*)element.GetChildren();
    while (n && n->GetType() == wxXML_ELEMENT_NODE)
    {
      RecurseIntersectionList(root, *n, rect, res);
      n = (wxSVGElement*)n->GetNext();
    }
  }
}

wxNodeList wxSVGSVGElement::GetIntersectionList(const wxSVGRect& rect, const wxSVGElement& referenceElement)
{
  wxNodeList res;
  RecurseIntersectionList(*this, referenceElement, rect, res);
  return res;
}

wxNodeList wxSVGSVGElement::GetEnclosureList(const wxSVGRect& rect, const wxSVGElement& referenceElement)
{
  wxNodeList res;
  return res;
}

bool wxSVGSVGElement::CheckIntersection(const wxSVGElement& element, const wxSVGRect& rect)
{
  wxSVGRect elemBBox = GetElementBBox(element);

  float rect_x1 = rect.GetX();
  float rect_x2 = rect_x1 + rect.GetWidth();
  float rect_y1 = rect.GetY();
  float rect_y2 = rect_y1 + rect.GetHeight();

  float elemBBox_x1 = elemBBox.GetX();
  float elemBBox_x2 = elemBBox_x1 + elemBBox.GetWidth();
  float elemBBox_y1 = elemBBox.GetY();
  float elemBBox_y2 = elemBBox_y1 + elemBBox.GetHeight();

  // test if rect overlap elemBBox
  bool res = rect_x1 < elemBBox_x2 && rect_x2 > elemBBox_x1 && 
  		rect_y1 < elemBBox_y2 && rect_y2 > elemBBox_y1;

  return res;
}

bool wxSVGSVGElement::CheckEnclosure(const wxSVGElement& element, const wxSVGRect& rect)
{
  bool res = 0;
  return res;
}

void wxSVGSVGElement::DeselectAll()
{

}

wxSVGNumber wxSVGSVGElement::CreateSVGNumber()
{
  return wxSVGNumber();
}

wxSVGLength wxSVGSVGElement::CreateSVGLength()
{
  return wxSVGLength();
}

wxSVGAngle wxSVGSVGElement::CreateSVGAngle()
{
  return wxSVGAngle();
}

wxSVGPoint wxSVGSVGElement::CreateSVGPoint()
{
  return wxSVGPoint();
}

wxSVGMatrix wxSVGSVGElement::CreateSVGMatrix()
{
  return wxSVGMatrix();
}

wxSVGRect wxSVGSVGElement::CreateSVGRect()
{
  return wxSVGRect();
}

wxSVGTransform wxSVGSVGElement::CreateSVGTransform()
{
  return wxSVGTransform();
}

wxSVGTransform wxSVGSVGElement::CreateSVGTransformFromMatrix(const wxSVGMatrix& matrix)
{
  return wxSVGTransform(matrix);
}

wxSVGElement* RecurseElementId(wxSVGElement* root, const wxString& elementId)
{
    if (root->GetId() == elementId) return root;
    wxSVGElement* n = (wxSVGElement*)root->GetChildren();
    while (n)
    {
        if (n->GetType() == wxXML_ELEMENT_NODE)
        {
            wxSVGElement* res = RecurseElementId(n, elementId);
            if (res)
               return res;
        }
        n = (wxSVGElement*)n->GetNext();
    }
    return NULL;
}

wxXmlElement* wxSVGSVGElement::GetElementById(const wxString& elementId)
{
    return RecurseElementId((wxSVGElement*)this,elementId);
}
