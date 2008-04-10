//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUINoteBook.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/09/08
// RCS-ID:      $Id: SVGUINoteBook.cpp,v 1.5 2008-04-10 17:34:21 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUINoteBook.h"
#include "SVGUIWindow.h"

SVGUINoteBook::SVGUINoteBook(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
  m_initialised = false;
  m_current_page = -1;
  m_current_content = NULL;
  m_GroupElement = NULL;
  m_ContentElement = NULL;
  SetName(wxT("NoteBook"));
}

bool SVGUINoteBook::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("group_id"))
    m_GroupElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("content_id"))
    m_ContentElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else
    return false;
  return true;
}

bool SVGUINoteBook::HitTest(wxPoint pt)
{
  if (!m_enable)
    return false;
  wxSVGRect rect(pt.x, pt.y, 1, 1);
  
  if (m_current_content && m_current_content->HitTest(pt))
    return true;
  
  bool res=false;
  if (m_BackgroundElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  wxSVGElement* tab_element;
  SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
  while (n)
  {
    tab_element = n->GetTabElement();
    res |= m_doc->GetRootElement()->CheckIntersection(*tab_element, rect);
    n = (SVGUINoteBookContent*)n->GetNext();
  }
  return res;
}

#define UpdateBBox_macro(element)\
    if (element)\
    {\
      if (res.IsEmpty())\
        res = wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT);\
      else\
        res = SumBBox(res, wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT));\
    }

wxSVGRect SVGUINoteBook::GetBBox()
{
  wxSVGRect res;
  UpdateBBox_macro(m_BackgroundElement)
  wxSVGElement* tab_element;
  SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
  while (n)
  {
    tab_element = n->GetTabElement();
    UpdateBBox_macro(tab_element)
    n = (SVGUINoteBookContent*)n->GetNext();
  }
  return res;
}

void SVGUINoteBook::Initialize()
{
  if (m_current_page < 0) {
    m_current_page = 0;
    m_current_content = (SVGUINoteBookContent*) GetChildren();
  }
  if (m_ContentElement)
  {
    wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_ContentElement, wxSVG_COORDINATES_VIEWPORT);
    wxSVGElement* content_element;
    SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
    while (n)
    {
      content_element = n->GetContentElement();
      MoveElement(content_element, rect.GetX(), rect.GetY());
      n = (SVGUINoteBookContent*)n->GetNext();
    }
  }
  m_initialised = true;
}

#define AddChain_element(element)\
    if (current_tab)\
      current_tab->SetNext((wxSvgXmlElement*)element);\
    else\
      m_GroupElement->SetChildren((wxSvgXmlElement*)element);\
    current_tab = element;

void SVGUINoteBook::Update_Elements()
{
  SetDisplay(m_ContentElement, wxCSS_VALUE_NONE);
  if (!m_initialised)
    Initialize();
  wxSVGElement* current_tab = NULL;
  wxSVGElement* end_tab = NULL;
  int current_page = 0;
  SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
  while (n)
  {
    if (current_page == m_current_page) {
      end_tab = n->GetTabElement();
      SetDisplay(n->GetContentElement(), wxCSS_VALUE_INLINE);
      n->Update_Elements();
      n->Enable();
    }
    else {
      wxSVGElement* tab_element = n->GetTabElement();
      AddChain_element(tab_element)
      SetDisplay(n->GetContentElement(), wxCSS_VALUE_NONE);
      n->Disable();
    }
    n = (SVGUINoteBookContent*)n->GetNext();
    current_page++;
  }
  AddChain_element(m_BackgroundElement)
  if (end_tab) {
    AddChain_element(end_tab)
  }
  AddChain_element(NULL)
}

void SVGUINoteBook::SetCurrentPage(int page)
{
  m_current_page = page;
  int current_page = 0;
  SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
  while (n)
  {
    if (current_page == page) {
      m_current_content = n;
      break;
    }
    n = (SVGUINoteBookContent*)n->GetNext();
  }
  Update_Elements();
  Refresh();
}

void SVGUINoteBook::OnLeftDown(wxMouseEvent &event)
{
  wxSVGRect rect(event.GetX(), event.GetY(), 1, 1);
  wxSVGElement* tab_element;
  
  int new_page = 0;
  SVGUINoteBookContent* n = (SVGUINoteBookContent*) GetChildren();
  while (n)
  {
    tab_element = n->GetTabElement();
    if (tab_element && m_doc->GetRootElement()->CheckIntersection(*tab_element, rect))
    {
      if (m_current_page != new_page) {
        m_current_page = new_page;
        m_current_content = n;
        Update_Elements();
        Refresh();
        wxCommandEvent evt(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, SVGUIWindow::GetSVGUIID(GetName()));
        m_window->ProcessEvent(evt);
      }
      break;
    }
    n = (SVGUINoteBookContent*)n->GetNext();
    new_page++;
  }
  if (!n && m_current_content)
    m_current_content->OnLeftDown(event);
  event.Skip();
}

void SVGUINoteBook::OnLeftUp(wxMouseEvent& event)
{
  if (m_current_content)
    m_current_content->OnLeftUp(event);
  event.Skip();
}

void SVGUINoteBook::OnMotion(wxMouseEvent& event)
{
  if (m_current_content)
    m_current_content->OnMotion(event);
  event.Skip();
}
