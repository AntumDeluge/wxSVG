//////////////////////////////////////////////////////////////////////////////
// Name:        NodeListCls.h
// Purpose:     
// Author:      Jonathan Hurtrel
// Created:     2007/06/08
// RCS-ID:      $Id: NodeListCls.h,v 1.1 2007-06-08 08:50:14 gusstdie Exp $
// Copyright:   (c) 2005 Jonathan Hurtrel
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef wxSVG_NODE_LIST_CLS_H
#define wxSVG_NODE_LIST_CLS_H

#include "SVGElement.h"
#include "NodeList.h"



#ifdef WXMAKINGDLL_WXSVG
    #define WXDLLIMPEXP_WXSVG WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXSVG WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXSVG
#endif

class WXDLLIMPEXP_WXSVG wxNodeListCls
{
	protected:
		int list_size;
		wxNodeList my_list;
		
	public:
		wxNodeListCls(){ list_size=0;}
		wxNodeListCls(wxNodeList m_list){ my_list = m_list; list_size = my_list.size(); }
		~wxNodeListCls();
		
		wxSVGElement *GetFirstElement(){return my_list[0];}
		wxSVGElement *GetElement(int i){return my_list[i];}
		int GetSize(){return list_size;}
};
	
#endif //wxSVG_NODE_LIST_CLS_H
