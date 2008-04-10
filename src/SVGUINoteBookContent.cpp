//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUINoteBookContent.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/09/08
// RCS-ID:      $Id: SVGUINoteBookContent.cpp,v 1.1 2008-04-10 17:34:21 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUINoteBookContent.h"

SVGUINoteBookContent::SVGUINoteBookContent(wxSVGDocument* doc):
  SVGUIContainer(doc)
{
  m_TabElement = NULL;
  m_ContentElement = NULL;
  SetName(wxT("NoteBookContent"));
}

bool SVGUINoteBookContent::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIContainer::SetAttribute(attrName, attrValue))
    return true;
  if (attrName == wxT("tab_id")) {
    m_TabElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  }
  else if (attrName == wxT("content_id")) {
    m_ContentElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  }
  else
    return false;
  return true;
}
