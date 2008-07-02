//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIContainer.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/05
// RCS-ID:      $Id: SVGUIContainer.cpp,v 1.6 2008-07-02 14:18:33 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIContainer.h"

SVGUIContainer::SVGUIContainer(wxSVGDocument* doc):
  SVGUIElement(doc)
{
  m_visible = true;
  m_FocusedElement = NULL;
  SetName(wxT("Container"));
}

SVGUIContainer::~SVGUIContainer() {
  m_FocusedElement = NULL;
}

bool SVGUIContainer::HitTest(wxPoint pt)
{
  if (!m_enable)
    return false;
  bool res = false;
  SVGUIElement* elem = NULL;
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n && !elem)
  {
    if (n->HitTest(pt))
      elem = n;
    n = (SVGUIElement*)n->GetNext();
  }
  if (m_FocusedElement != elem && m_FocusedElement)
    m_FocusedElement->SetSelected(false);
  m_FocusedElement = elem;
  if (m_FocusedElement)
  {
    m_FocusedElement->SetSelected(true);
    res = true;
  }
  else if (m_BackgroundElement)
  {
    wxSVGRect rect(pt.x, pt.y, 1, 1);
    res = m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  }
  return res;
}

void SVGUIContainer::SetSelected(bool selected)
{
  if (m_FocusedElement)
    m_FocusedElement->SetSelected(selected);
}

wxSVGRect SVGUIContainer::GetBBox()
{
  wxSVGRect res(0, 0, 0, 0);
  if (m_BackgroundElement)
    res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
  return res;
}

void SVGUIContainer::Update_Elements()
{
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n)
  {
    n->Update_Elements();
    n = (SVGUIElement*)n->GetNext();
  }
}

wxString SVGUIContainer::GetFocusedElement()
{
  if (m_FocusedElement)
    if (m_FocusedElement->GetName() == wxT("Container"))
      return ((SVGUIContainer*)m_FocusedElement)->GetFocusedElement();
    else
      return m_FocusedElement->GetId();
  else
    return GetId();
}

wxString SVGUIContainer::GetFocusedElementName()
{
  if (m_FocusedElement)
    if (m_FocusedElement->GetName() == wxT("Container"))
      return ((SVGUIContainer*)m_FocusedElement)->GetFocusedElementName();
    else
      return m_FocusedElement->GetName();
  else
    return GetName();
}

void SVGUIContainer::Hide()
{
  m_visible = false;
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n)
  {
    n->Hide();
    n = (SVGUIElement*)n->GetNext();
  }
}

void SVGUIContainer::Show()
{
  m_visible = true;
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n)
  {
    n->Show();
    n = (SVGUIElement*)n->GetNext();
  }
  Update_Elements();
}

void SVGUIContainer::Enable()
{
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n)
  {
    n->Enable();
    n = (SVGUIElement*)n->GetNext();
  }
}

void SVGUIContainer::Disable()
{
  SVGUIElement* n = (SVGUIElement*) GetChildren();
  while (n)
  {
    n->Disable();
    n = (SVGUIElement*)n->GetNext();
  }
}

void SVGUIContainer::OnLeftDown(wxMouseEvent& event)
{
  if (m_FocusedElement)
    m_FocusedElement->OnLeftDown(event);
  event.Skip();
}
    
void SVGUIContainer::OnLeftUp(wxMouseEvent& event)
{
  if (m_FocusedElement)
    m_FocusedElement->OnLeftUp(event);
  event.Skip();
}

void SVGUIContainer::OnMotion(wxMouseEvent& event)
{
  if (m_FocusedElement)
    m_FocusedElement->OnMotion(event);
  event.Skip();
}

void SVGUIContainer::OnChar(wxKeyEvent& event)
{
  if (m_FocusedElement)
    m_FocusedElement->OnChar(event);
  event.Skip();
}

