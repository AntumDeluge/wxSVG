//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIElement.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/18
// RCS-ID:      $Id: SVGUIElement.h,v 1.5 2008-04-03 17:02:48 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_ELEMENT_H
#define SVGUI_ELEMENT_H

#include <wx/wx.h>
//#include <wx/notebook.h>
#include <wxSVGXML/svgxml.h>
#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "ElementTransform.h"

void SetDisplay(wxSVGElement* elem, wxCSS_VALUE value);

class SVGUIElement:
	public wxSvgXmlElement
{
	protected:
		wxSVGDocument* m_doc;
		wxSVGElement* m_BackgroundElement;
		bool m_enable;
		bool m_visible;
		
	public:
		SVGUIElement();
		SVGUIElement(wxSVGDocument* doc);
		wxString GetId();
		wxString GetName();
		
		virtual void DefineCenter(wxSVGPoint* pt){Update_Elements();}
		virtual wxSVGPoint* GetCenter(){return NULL;}
		virtual void Initialize(){Update_Elements();}		
		virtual bool HitTest(wxPoint pt) = 0;
		virtual wxSVGRect GetBBox() = 0;
		virtual void SendMouseEvent(wxMouseEvent& event) = 0;
		virtual void SendKeyEvent(wxKeyEvent& event) = 0;
		//virtual void SendNotebookEvent(wxNotebookEvent& event) = 0;
		virtual void SendScrollEvent(wxScrollEvent& event) = 0;
		virtual void SendCommandEvent(wxCommandEvent& event) = 0;
		virtual void Update_Elements() = 0;
		bool IsVisible(){return m_visible;}
    bool IsEnabled(){return m_enable;}
		
		void Hide();
		void Show();
		void Enable(){m_enable = true;}
		void Disable(){m_enable = false;}
		
		virtual void AddProperty(const wxString& name, const wxString& value);
		virtual bool SetAttribute(const wxString& attrName, const wxString& attrValue); 
};

#endif //SVGUI_ELEMENT_H
