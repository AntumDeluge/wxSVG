//////////////////////////////////////////////////////////////////////////////
// Name:        SVGSVGElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGSVGElement.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
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

float wxSVGSVGElement::GetCurrentTime()
{
  return 0;
}

void wxSVGSVGElement::SetCurrentTime(float seconds)
{

}

wxNodeList wxSVGSVGElement::GetIntersectionList(const wxSVGRect& rect, const wxSVGElement& referenceElement)
{
  wxNodeList res;
  return res;
}

wxNodeList wxSVGSVGElement::GetEnclosureList(const wxSVGRect& rect, const wxSVGElement& referenceElement)
{
  wxNodeList res;
  return res;
}

bool wxSVGSVGElement::CheckIntersection(const wxSVGElement& element, const wxSVGRect& rect)
{
  bool res = 0;
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

wxXmlElement* wxSVGSVGElement::GetElementById(const wxString& elementId)
{
  return NULL;
}
