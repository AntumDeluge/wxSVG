//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIWindow.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIWindow.cpp,v 1.5 2008-05-23 13:47:53 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIWindow.h"

BEGIN_EVENT_TABLE(SVGUIWindow, wxSVGCtrl)
  EVT_LEFT_DOWN (SVGUIWindow::OnLeftDown)
  EVT_MOTION (SVGUIWindow::OnMotion)
  EVT_LEFT_UP (SVGUIWindow::OnLeftUp)
  EVT_CHAR (SVGUIWindow::OnChar)
  EVT_SIZE (SVGUIWindow::OnSize)
  EVT_COMMAND(-1, wxEVT_COMMAND_ENTER, SVGUIWindow::OnRefresh)
END_EVENT_TABLE()

SVGUIWindow::SVGUIWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos,
  const wxSize& size, long style, const wxString& name): 
  wxSVGCtrl(parent, id, pos, size, style, name)
{
  m_scale = 1.;
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
  /*printf("MY_ID : %s\n",id.c_str());
  printf("FOUND_ID : %s\n",found_id.c_str());*/
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
  /*printf("MY_ID : %s\n",id.c_str());
  printf("FOUND_ID : %s\n",found_id.c_str());*/
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

void SVGUIWindow::RefreshScale()
{
  if (wxSVGCtrl::m_doc)
  {
    if (m_fitToFrame)
    {
      wxSVGRect rect = wxSVGCtrl::m_doc->GetRootElement()->GetBBox();
      float width = (float)wxSVGCtrl::m_doc->GetRootElement()->GetWidth().GetBaseVal().GetValue();
      float height = (float)wxSVGCtrl::m_doc->GetRootElement()->GetHeight().GetBaseVal().GetValue();
      wxSize size = GetClientSize();
      if ( ((width < (float)rect.GetWidth()) && (width < (float)rect.GetWidth())) || (width  > (float)rect.GetWidth() && height > (float)rect.GetHeight() ) ) 
      {
        rect.SetX(0);
        rect.SetY(0);
        if (m_fitToFrame == true)
        {
          rect.SetHeight(height);
          rect.SetWidth(width);
        }
        else
        {
          rect.SetHeight((float)size.GetHeight());
          rect.SetWidth((float)size.GetWidth());
        }
      }
      //printf("Rect : %f | Client : %f | Root : %f\n",(float)rect.GetWidth(),(float)size.GetWidth(), width);
      if ((float)rect.GetWidth()/(float)rect.GetHeight() > (float)size.GetWidth()/(float)size.GetHeight())
        m_scale = (float)rect.GetWidth() / (float)size.GetWidth();
      else
        m_scale = (float)rect.GetHeight() / (float)size.GetHeight();
    }
    else
      m_scale = 1;
  }
}

void SVGUIWindow::Update_Elements()
{
  if (GetRoot())
    ((SVGUIElement*)GetRoot())->Update_Elements();
}

void SVGUIWindow::OnLeftDown(wxMouseEvent& event)
{
  if (m_scale && GetRoot())
  {
    event.m_x = (long)((float)event.m_x * m_scale);
    event.m_y = (long)((float)event.m_y * m_scale);
    ((SVGUIElement*)GetRoot())->HitTest(event.GetPosition());
    //CaptureMouse();
    ((SVGUIElement*)GetRoot())->OnLeftDown(event);
    m_clicked = true;
  }
  
  event.Skip();
}

void SVGUIWindow::OnMotion(wxMouseEvent& event)
{
  if (m_scale && GetRoot())
  {
    event.m_x = (long)((float)event.m_x * m_scale);
    event.m_y = (long)((float)event.m_y * m_scale);
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
  if (m_scale && GetRoot())
  {
    event.m_x = (long)((float)event.m_x * m_scale);
    event.m_y = (long)((float)event.m_y * m_scale);
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
  /*wxRect *rect = new wxRect(0,0,300,300);
  Refresh(true,rect);*/
  wxString elem_name = event.GetString();
  if (elem_name.length())
  {
    SVGUIElement* elem = GetElementById(elem_name);
    if (elem){
      //wxSVGRect rect = elem->GetBBox();
      //rect.SetX(rect.GetX()*(m_scale));
      //rect.SetY(rect.GetY()*(m_scale));
      //rect.SetHeight(rect.GetHeight()*1/m_scale);
      //rect.SetWidth(rect.GetWidth()*1/m_scale);
      //Refresh(&rect);
      Refresh();
      //RefreshRect(elem->GetBBox());
    }
  }
  else
    Refresh();
}

void SVGUIWindow::OnSize(wxSizeEvent& event)
{
  RefreshScale();
  event.Skip();
}
