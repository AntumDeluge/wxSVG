//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIButton.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIButton.h,v 1.1 2007-08-31 08:56:17 gusstdie Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_BUTTON_H
#define SVGUI_BUTTON_H

#include "SVGUIControl.h"
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

#ifdef WXMAKINGDLL_WXSVG
    #define WXDLLIMPEXP_WXSVG WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXSVG WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXSVG
#endif

class WXDLLIMPEXP_WXSVG SVGUIButton:
	public SVGUIControl
{
	protected:
		bool m_up;
		wxSVGElement* m_SelectedElement;
		wxSVGElement* m_UnselectedElement;		
	
	public:
		SVGUIButton(wxSVGDocument* doc, wxEvtHandler* window);
		
		bool HitTest(wxPoint pt);
		wxSVGRect GetBBox();
		bool SetAttribute(const wxString& attrName, const wxString& attrValue);
		void Update_Elements();
		
		void OnLeftDown(wxMouseEvent &event);
		void OnMotion(wxMouseEvent &event);
		void OnLeftUp(wxMouseEvent &event);
};

#endif //SVGUI_BUTTON_H
