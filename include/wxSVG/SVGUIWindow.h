//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIWindow.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIWindow.h,v 1.4 2008-04-10 17:37:19 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_WINDOW_H
#define SVGUI_WINDOW_H

#include <wx/wx.h>
#include <wxSVG/svgctrl.h>
#include <wx/string.h>
#include <wxSVGXML/svgxml.h>
#include <wxSVG/SVGElement.h>
#include "SVGUIButton.h"
#include "SVGUITextCtrl.h"
#include "SVGUIScrollBar.h"
#include "SVGUIRotatingCtrl.h"
#include "SVGUINoteBook.h"
#include "SVGUIElement.h"
#include "SVGUITransform.h"
#include "SVGUIContainer.h"

class SVGUIWindow:
  public wxSvgXmlDocument,
  public SVGUIContainer,
  public wxSVGCtrl
{
  protected:
    float m_scale;
    bool m_clicked;
  
  public:
    wxString m_selected_id;
    SVGUIElement* m_selected_element;

    DECLARE_EVENT_TABLE()

  public:
    SVGUIWindow(wxWindow* parent, wxWindowID id = wxID_ANY,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = 0, const wxString& name = wxPanelNameStr);
      
    SVGUIElement* CreateElement(const wxString& tagName);
    SVGUIElement* CreateElementNS(const wxString& namespaceURI, const wxString& qualifiedName);
    
    virtual wxWindow* CreateWindow(const wxString& classname);
    
    bool LoadFiles(const wxString& svgfile, const wxString& deffile);
    void Update_Elements();
    void RefreshScale();
    float GetScale(){return m_scale;}
    
    SVGUIContainer* GetSVGUIRootElement();
    SVGUIElement* GetElementById(const wxString& id);
    SVGUIElement* GetElementByName(const wxString& name);
    SVGUIScrollBar* GetScrollBarById(const wxString& id);
    static int GetSVGUIID(const wxChar *str_id);

    virtual void OnLeftDown(wxMouseEvent& event);
    virtual void OnLeftUp(wxMouseEvent& event);
    virtual void OnMotion(wxMouseEvent& event);
    virtual void OnChar(wxKeyEvent& event);
    virtual void OnSize(wxSizeEvent& event);
    virtual void OnRefresh(wxCommandEvent& event);
};

#define SVGUIID(str_id)  SVGUIWindow::GetSVGUIID(wxT(str_id))

#endif // SVGUI_WINDOW_H
