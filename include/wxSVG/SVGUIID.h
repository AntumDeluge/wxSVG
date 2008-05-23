//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIElement.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2008/04/30
// RCS-ID:      $Id: SVGUIID.h,v 1.1 2008-05-23 13:47:22 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUIID_H_
#define SVGUIID_H_

#include <wx/wx.h>

int GetSVGUIID(const wxChar *str_id);
int SVGUIID(const char *str_id);

#define SVGUIID(str_id) GetSVGUIID(wxT(str_id))

#endif /*SVGUIID_H_*/
