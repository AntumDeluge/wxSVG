//////////////////////////////////////////////////////////////////////////////
// Name:        SVGSVGElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGSVGElement.cpp,v 1.10 2007-05-23 15:15:19 etisserant Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGSVGElement.h"
#include <wx/log.h>

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
  wxSVGRect elemBBox = GetElementResultBBox(&element, wxSVG_COORDINATES_VIEWPORT);
  return  elemBBox.GetX() + elemBBox.GetWidth() > rect.GetX() &&
    elemBBox.GetX() < rect.GetX() + rect.GetWidth() &&
    elemBBox.GetY() + elemBBox.GetHeight() > rect.GetY() &&
    elemBBox.GetY() < rect.GetY() + rect.GetHeight();
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
    if (root->GetId() == elementId)
        return root;
    // check childs
    wxSVGElement* child = (wxSVGElement*)root->GetChildren();
    while (child)
    {
        if (child->GetType() == wxXML_ELEMENT_NODE)
        {
            if (child->GetDtd() == wxSVG_SVG_ELEMENT)
            {
              if (child->GetId() == elementId)
                return child;
            }
            else
            {
                wxSVGElement* res = RecurseElementId(child, elementId);
                if (res)
                    return res;
            }
        }
        child = (wxSVGElement*)child->GetNext();
    }
    return NULL;
}

wxSvgXmlElement* wxSVGSVGElement::GetElementById(const wxString& elementId) const
{
    return RecurseElementId((wxSVGElement*)this,elementId);
}
