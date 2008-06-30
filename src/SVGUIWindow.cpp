//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIWindow.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIWindow.cpp,v 1.6 2008-06-30 13:06:53 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIWindow.h"

BEGIN_EVENT_TABLE(SVGUIWindow, wxSVGCtrl)
  EVT_LEFT_DOWN (SVGUIWindow::OnLeftDown)
  EVT_MOTION (SVGUIWindow::OnMotion)
  EVT_LEFT_UP (SVGUIWindow::OnLeftUp)
  EVT_CHAR (SVGUIWindow::OnChar)
  EVT_COMMAND(-1, wxEVT_COMMAND_ENTER, SVGUIWindow::OnRefresh)
END_EVENT_TABLE()

SVGUIWindow::SVGUIWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos,
  const wxSize& size, long style, const wxString& name): 
  wxSVGCtrl(parent, id, pos, size, style, name)
{
}

bool SVGUIWindow::LoadFiles(const wxString& svgfile, const wxString& deffile)
{
  bool res = wxSVGCtrl::Load(svgfile);
  
  if (res)
  {
    wxSvgXmlDocument::Load(deffile);
    ClearList();
    Update_Elements();
  }
  return res;
}

SVGUIElement* SVGUIWindow::CreateElement(const wxString& tagName)
{
  return CreateElementNS(wxT(""), tagName);
}

SVGUIElement* SVGUIWindow::CreateElementNS(const wxString& namespaceURI,
  const wxString& qualifiedName)
{
  SVGUIElement* res = NULL;
  if (qualifiedName == wxT("Container"))
    res = new SVGUIContainer(GetSVG());
  else if (qualifiedName == wxT("Button"))
    res = new SVGUIButton(GetSVG(), (wxEvtHandler*)this);
  else if (qualifiedName == wxT("TextCtrl"))
    res = new SVGUITextCtrl(GetSVG(), (wxEvtHandler*)this);
  else if (qualifiedName == wxT("ScrollBar"))
    res = new SVGUIScrollBar(GetSVG(), (wxEvtHandler*)this);
  else if (qualifiedName == wxT("RotatingCtrl"))
    res = new SVGUIRotatingCtrl(GetSVG(), (wxEvtHandler*)this);
  else if (qualifiedName == wxT("NoteBook"))
    res = new SVGUINoteBook(GetSVG(), (wxEvtHandler*)this);
  else if (qualifiedName == wxT("NoteBookContent"))
    res = new SVGUINoteBookContent(GetSVG());
  else if (qualifiedName == wxT("Transform"))
    res = new SVGUITransform(GetSVG(), (wxEvtHandler*)this);
  else
    res = new SVGUIContainer(GetSVG());
  return res;
}

wxWindow* SVGUIWindow::CreateWindow(const wxString& classname)
{
  return NULL;
}

SVGUIElement* RecurseElementId(SVGUIElement* elem, const wxString& id)
{
  wxString found_id = elem->GetId();
  if (elem->GetId() == id) return elem;
  SVGUIElement* n = (SVGUIElement*)elem->GetChildren();
  while (n)
  {
    SVGUIElement* res = RecurseElementId(n, id);
    if (res)
       return res;
    else
       n = (SVGUIElement*)n->GetNext();
  }
  return NULL;
} 

SVGUIElement* RecurseElementName(SVGUIElement* elem, const wxString& name)
{
  if (elem->GetName() == name) return elem;
  SVGUIElement* n = (SVGUIElement*)elem->GetChildren();
  while (n)
  {
    SVGUIElement* res = RecurseElementName(n, name);
    if (res)
       return res;
    else
       n = (SVGUIElement*)n->GetNext();
  }
  return NULL;
} 

SVGUIContainer* SVGUIWindow::GetSVGUIRootElement()
{
  return (SVGUIContainer*)GetRoot();
}

SVGUIElement* SVGUIWindow::GetElementById(const wxString& id)
{
  SVGUIElement* my_elem;
  if (m_selected_element && id == m_selected_id )
    my_elem = m_selected_element;
  else
  {
    my_elem = RecurseElementId((SVGUIElement*)GetRoot(), id);
    m_selected_id = id;
    m_selected_element = my_elem;
  }
  return my_elem;
}

SVGUIElement* SVGUIWindow::GetElementByName(const wxString& name)
{
  return RecurseElementName((SVGUIElement*)GetRoot(), name);
}

SVGUIScrollBar* SVGUIWindow::GetScrollBarById(const wxString& id)
{
  return (SVGUIScrollBar*)RecurseElementId((SVGUIElement*)GetRoot(), id);
}

void SVGUIWindow::Update_Elements()
{
  if (GetRoot())
    ((SVGUIElement*)GetRoot())->Update_Elements();
}

void SVGUIWindow::OnLeftDown(wxMouseEvent& event)
{
  if (GetRoot())
  {
    event.m_x = (long)((float)event.m_x / GetScale());
    event.m_y = (long)((float)event.m_y / GetScale());
    ((SVGUIElement*)GetRoot())->HitTest(event.GetPosition());
    //CaptureMouse();
    ((SVGUIElement*)GetRoot())->OnLeftDown(event);
    m_clicked = true;
  }
  
  event.Skip();
}

void SVGUIWindow::OnMotion(wxMouseEvent& event)
{
  if (GetRoot())
  {
    event.m_x = (long)((float)event.m_x / GetScale());
    event.m_y = (long)((float)event.m_y / GetScale());
    ((SVGUIElement*)GetRoot())->OnMotion(event);
    if (m_clicked)
    {
      Refresh();
    }
  }
  event.Skip();
}

void SVGUIWindow::OnLeftUp(wxMouseEvent& event)
{
  if (GetRoot())
  {
    event.m_x = (long)((float)event.m_x / GetScale());
    event.m_y = (long)((float)event.m_y / GetScale());
    ((SVGUIElement*)GetRoot())->OnLeftUp(event);
    m_clicked = false;
    //ReleaseMouse();
  }
  
  event.Skip();
}

void SVGUIWindow::OnChar(wxKeyEvent& event)
{
  ((SVGUIElement*)GetRoot())->OnChar(event);
  event.Skip();
}

void SVGUIWindow::OnRefresh(wxCommandEvent& event)
{
  wxString elem_name = event.GetString();
  if (elem_name.length())
  {
    SVGUIElement* elem = GetElementById(elem_name);
    if (elem){
      Refresh();
    }
  }
  else
    Refresh();
}
