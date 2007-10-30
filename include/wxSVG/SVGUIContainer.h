//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIContainer.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/05
// RCS-ID:      $Id: SVGUIContainer.h,v 1.3 2007-10-30 21:59:21 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_CONTAINER_H
#define SVGUI_CONTAINER_H

#include <wx/wx.h>
#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "SVGUIElement.h"
#include "SVGUITextCtrl.h"

class SVGUIContainer:
	public SVGUIElement
{
	protected:
		SVGUIElement* m_FocusedElement;
		
	public:
		SVGUIContainer(){};
		SVGUIContainer(wxSVGDocument* doc);
		wxString GetFocusedElement();
		wxString GetFocusedElementName();
		virtual void Update_Elements();
		
		void Hide();
		void Show();
		void Enable();
		void Disable();
		
		virtual bool HitTest(wxPoint pt);
		virtual wxSVGRect GetBBox();
		virtual void SendMouseEvent(wxMouseEvent& event);
		virtual void SendKeyEvent(wxKeyEvent& event);
		//virtual void SendNotebookEvent(wxNotebookEvent& event);
	  virtual void SendScrollEvent(wxScrollEvent& event);
	  virtual void SendCommandEvent(wxCommandEvent& event);
};

#endif //SVGUI_CONTAINER_H
