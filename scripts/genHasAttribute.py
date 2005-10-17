##############################################################################
## Name:        genHasAttribute.py
## Purpose:     generates Elements_HasAttribute.cpp
##              -> HasAttribute() methods for all svg elements
## Author:      Alex Thuering
## Created:     2005/09/27
## RCS-ID:      $Id: genHasAttribute.py,v 1.1 2005-10-17 13:51:33 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:		some modules adapted from svgl project
##############################################################################

import parse_idl
import mapDtdIdl
import string
import config
import genFile
import cpp
import cppImpl

customParser = ["SVGStylable"] ##TODO["SVGFEGaussianBlurElement", "SVGMarkerElement"]
output_cpps = {}

includes = ["String"]
already_done=[]

def make_cpp(name):
    if name in ['operator']:
        return name + '_'
    return name

class AlreadyProcessed:
    def __init__(self):
        pass

def process(classdecl):

    if classdecl.name in includes:
        raise AlreadyProcessed

    if classdecl in already_done:
        return 0;
    already_done.append(classdecl)

    nattr=0
    func_body = ''

    for attr in classdecl.attributes:
        try:
            entity_name = mapDtdIdl.attributes_idl_dtd[attr]
        except KeyError:
            #print classdecl.name, attr ###### TODO
            #print classdecl
            #print mapDtdIdl.attributes_idl_dtd
            #raise ""
            continue

        if nattr == 0:
            includes.append(classdecl.name)
        nattr = nattr + 1
        
        if len(func_body):
            func_body = func_body + " ||\n    "
        func_body = func_body + 'attrName == wxT("%s")'%entity_name

    for inh in classdecl.inherits:
        try:
            res = process(parse_idl.class_decls[inh])
        except AlreadyProcessed:
            res=1
        except KeyError:
            if inh not in ["Element", "events::EventTarget", "events::DocumentEvent", "css::ViewCSS", "css::DocumentCSS",
                           "css::CSSValue", "smil::ElementTimeControl", "Document", "events::UIEvent", "css::CSSRule",
                           "events::Event"]:
                raise
            else:
                continue
        if res>0:
            if nattr==0:
                includes.append(classdecl.name)

            nattr = nattr+res
            if len(func_body):
                func_body = func_body + " ||\n    "
            func_body = func_body + 'wx%s::HasAttribute(attrName)'%inh

    if nattr>0:
        if classdecl.name in customParser:
            func_body = func_body + ' ||\n    HasCustomAttribute(attrName)'

        output_cpp = '''
// wx%s
bool wx%s::HasAttribute(const wxString& attrName)
{
  return %s;
}      
'''%(classdecl.name, classdecl.name, func_body)

        output_cpps[classdecl.name]=output_cpp

    else:
        #print classdecl
        pass

    
    return nattr


if len(parse_idl.class_decls):
    w = parse_idl.class_decls.keys()
    v = []
    for name in w:
	v.append(name)
    v.sort()
    for name in v:
        classdecl = parse_idl.class_decls[name]
        try:
            process(classdecl)
        except AlreadyProcessed:
            continue


output_cpp=""
if len(output_cpps):
    for key,out in output_cpps.items():
        output_cpp=output_cpp+out

includestr=''
for i in includes:
    includestr = includestr+'#include "%s.h"\n'%i

impl = cppImpl.Impl("Elements_HasAttribute", "genHasAttribute.py")
impl.add_content(includestr + output_cpp)
impl.dump(path=config.src_dir)

