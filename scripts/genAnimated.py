##############################################################################
## Name:        genAnimated.py
## Purpose:     generates all SVGAnimated*.h
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: genAnimated.py,v 1.2 2005-05-16 11:00:17 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:       some modules adapted from svgl project
##############################################################################

import config
import cpp
import cppHeader

def getBaseType(name):
	typename = name
	if typename != "String":
	  typename = "SVG" + typename
	typename = cpp.fix_typename(typename)
	if name=="Enumeration":
	  typename = "char"
	elif name=="Boolean":
	  typename = "bool"
	elif name=="Integer":
	  typename = "long"
	elif name=="Number":
	  typename = "float"
	return typename


def generate(name):
	typename = getBaseType(name)
	include = ""
	if typename not in cpp.builtin_types and name != "String":
		include = '#include "SVG%s.h"'%name
	if len(include):
		include = include + '\n'
	animType = ""
	if typename in cpp.number_types:
		animType = "_NUMBER"
	output = '''#include "Animated.h"
%s
WXSVG_MAKE_ANIMATED%s(%s, %s)'''%(include,animType,name,typename)
	header = cppHeader.Header("SVGAnimated%s"%name, "genAnimated.py")
	header.add_content(output)
	header.dump(path=config.include_dir)

