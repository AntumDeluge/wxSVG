//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUINoteBook.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/09/08
// RCS-ID:      $Id: SVGUINoteBook.h,v 1.2 2007-10-30 21:59:22 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_NOTEBOOK_H
#define SVGUI_NOTEBOOK_H

#include "SVGUIControl.h"
#include <math.h>
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>
#include <wx/notebook.h>

class SVGUINoteBook:
	public SVGUIControl
{
	protected:
		unsigned int m_current_page;
		wxSVGElement* m_GroupElement;
		wxSVGElement* m_Page0Element;
		wxSVGElement* m_Page1Element;
		wxSVGElement* m_ContentElement;
		wxSVGElement* m_Page0Content;
		wxSVGElement* m_Page1Content;
	
	public:
		SVGUINoteBook(wxSVGDocument* doc, wxEvtHandler* window);
		
		bool HitTest(wxPoint pt);
		wxSVGRect GetBBox();
		bool SetAttribute(const wxString& attrName, const wxString& attrValue);
		void Update_Elements();
		void RefreshContentSize(double scale);
		void MoveTabs(int new_page);
		unsigned int GetCurrentPage(){return m_current_page;}
		void SetCurrentPage(int page);
		
		void OnLeftDown(wxMouseEvent &event);
		void OnMotion(wxMouseEvent& event);
		void OnLeftUp(wxMouseEvent& event);
};

#endif //SVGUI_NOTEBOOK_H
