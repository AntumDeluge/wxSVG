##############################################################################
## Name:        genList.py
## Purpose:     
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: genList.py,v 1.1.1.1 2005-05-10 17:51:21 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:		some modules adapted from svgl project
##############################################################################

import config
import genAnimated
import cpp
import cppHeader
import cppImpl

def make(name):
	typename = genAnimated.getBaseType(name)
	include = ""
	if name == "String":
		include = '#include "%s.h"\n'%name
	elif typename not in cpp.builtin_types:
		include = '#include "SVG%s.h"\n'%name
	
	output = '''%s#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY(%s, wxSVG%sList);'''%(include,typename,name)
	header = cppHeader.Header("SVG%sList"%name, "genList.py")
	header.add_content(output)
	header.dump(path=config.include_dir)
	
	output = '''#include "SVG%sList.h"
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxSVG%sList);'''%(name,name)
	impl = cppImpl.Impl("SVG%sList"%name, "genList.py")
	impl.add_content(output)
	impl.dump(path=config.src_dir)

