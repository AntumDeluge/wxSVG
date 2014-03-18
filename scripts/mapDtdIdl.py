##############################################################################
## Name:        mapDtdIdl.py
## Purpose:     
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: mapDtdIdl.py,v 1.2 2014-03-18 13:08:39 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:		some modules adapted from svgl project
##############################################################################

import idl
import parse_idl
import parse_dtd
import string

__doc__=='''
this module should map DOM interfaces to DTD Elements
'''

attributes_dtd_idl = {}
attributes_idl_dtd = {}
elements_dtd_idl = {}
elements_idl_dtd = {}

#first, lower the idl class name
parse_idl_class_decls={}
if len(parse_idl.class_decls):
    for key,val in parse_idl.class_decls.items():
        parse_idl_class_decls[string.lower(key)]=val


def make_cppname(name):
    beg=0
    while 1:
        pos = string.find(name, '-', beg)
        if pos==-1:
            pos = string.find(name, ':', beg)
        if pos>0:
            res = name[:pos]+ string.upper(name[pos+1]) + name[pos+2:]
            name=res
            beg=pos
        else:
            break

    if name=='class':
        name = 'className'
    elif name=='in':
        name = 'in1'
    elif name=='d':
        name='pathSegList'
    elif name=='xlinkHref':
        name='href'
        

    return name


def find_name_in_inherit(name, classdecl):
    if len(classdecl.attributes):
        for attr in classdecl.attributes:
            if string.lower(attr.name)==name:
                return (classdecl, attr)

    for inh in classdecl.inherits:
        try:
            (c, e) = find_name_in_inherit(name, parse_idl.class_decls[inh])
            if c and e:
                return (c,e)
        except KeyError:
            pass
    return (None,None)


elements = parse_dtd.attlists
if len(elements):
    for name, entity_types in elements.items():
        classname = make_cppname(string.upper(name[0]) + name[1:])
        classname = "SVG" + classname + "Element"
        try:
            classdecl = parse_idl_class_decls[string.lower(classname)]
        except KeyError, arg:
            if classname=="SVGMpathElement":
                continue
            else:
                raise

        elements_dtd_idl[name] = classdecl

        for entity_type in entity_types:
            ltypes = entity_type.expand(parse_idl.class_decls, parse_dtd.entity_common_attrs)

            for i in ltypes:
                attrname = make_cppname(i.name)
                (c, attr) = find_name_in_inherit(string.lower(attrname), classdecl)

                if c!=None :
                    if attributes_dtd_idl.has_key(i.name):
                        attributes_dtd_idl[i.name].append(attr)
                    else:
                        attributes_dtd_idl[i.name]=[attr]
                else:
                    pass
 

if len(attributes_dtd_idl):
    for key, lval in attributes_dtd_idl.items():
        for val in lval:
            attributes_idl_dtd[val]=key
			#print key, val.name

if len(elements_dtd_idl):
    for key, val in elements_dtd_idl.items():
        elements_idl_dtd[val]=key
        #print key, val.name

