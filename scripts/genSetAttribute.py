##############################################################################
## Name:        genSetAttribute.py
## Purpose:     generates Elements_SetAttribute.cpp
##              -> SetAttribute() methods for all svg elements
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: genSetAttribute.py,v 1.7 2005-11-06 17:37:28 ntalex Exp $
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
        
        set_attr = cpp.make_attr_name(attr.name)
        typestr =attr.type.name
        anim_pos = string.find(typestr, 'Animated')
        if anim_pos>=0: # SVGAnimatedTypename
            typestr = typestr[anim_pos+len('Animated'):]
            if typestr not in ["float", "Number", "Integer", "Boolean", "Enumeration", "unsigned short"]:
                set_attr = set_attr + '.GetBaseVal()'
        conv = ''
        #print classdecl.name, typestr
        if typestr in ["Integer", "Boolean", "Enumeration", "unsigned short"]:
            etype = ''
            if typestr == "Integer":
                etype = '(long int) '
            elif typestr == "Boolean":
                etype = '(bool) '
            elif typestr == "Enumeration":
                etype = '(char) '
            elif typestr == "unsigned short":
                if classdecl.name == "SVGZoomAndPan":
                    etype = '(wxSVG_ZOOMANDPAN) '
                elif classdecl.name == "SVGColorProfileElement":
                    etype = '(wxRENDERING_INTENT) '
            if anim_pos>=0:
                set_attr = '%s.SetBaseVal(%svalue)'%(set_attr,etype)
            else:
                set_attr = '%s = %svalue'%(set_attr,etype)
            set_attr = '''  {
    long value;
    if (attrValue.ToLong(&value))
      %s;
  }'''%set_attr
        elif typestr in ["float", "Number"]:
            if anim_pos>=0:
                set_attr = '%s.SetBaseVal(value)'%set_attr
            else:
                set_attr = '%s = value'%set_attr
            set_attr = '''  {
    double value;
    if (attrValue.ToDouble(&value))
      %s;
  }'''%set_attr
        elif typestr == "css::CSSStyleDeclaration":
            set_attr = '    %s.SetCSSText(attrValue);'%set_attr
        elif typestr in ["Length", "Rect", "PreserveAspectRatio"] or typestr[-4:] == "List":
            set_attr = '    %s.SetValueAsString(attrValue);'%set_attr
        else:
            set_attr = '\t' + set_attr + ' = attrValue;'
        func_body = func_body + 'if (attrName == wxT("%s"))\n'%entity_name
        func_body = func_body + set_attr + '\n  else '

    for inh in classdecl.inherits:
        try:
            res = process(parse_idl.class_decls[inh])
        except AlreadyProcessed:
            res=1
        except KeyError:
            if inh not in ["Element", "events::EventTarget", "events::DocumentEvent",
                           "css::ViewCSS", "css::DocumentCSS", "css::CSSValue",
                           "smil::ElementTimeControl", "Document", "events::UIEvent",
                           "css::CSSRule", "events::Event"]:
                raise
            else:
                continue
        if res>0:
            if nattr==0:
                includes.append(classdecl.name)

            nattr = nattr+res
            func_body = func_body + '''if (wx%s::SetAttribute(attrName, attrValue));
  else '''%(inh)

    if nattr>0:
        if classdecl.name in customParser:
             #print classdecl.name
            func_body = func_body + '''if (SetCustomAttribute(attrName, attrValue));
  else'''

        #if it's an element
        #if string.find(classdecl.name,"Element")>0 and classdecl.name != "SVGElement":
        if mapDtdIdl.elements_idl_dtd.has_key(classdecl):
            func_body = func_body + '''
  {
    //wxLogDebug(wxT("unknown attribute %s::") + attrName);
    return false;
  }
'''%(classdecl.name)

        else:
            func_body = func_body +'''
      return false;
'''

        output_cpp = '''
// wx%s
bool wx%s::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  %s
  return true;
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

parse_includestr='#include "String.h"\n#include <wx/tokenzr.h>\n'

impl = cppImpl.Impl("Elements_SetAttribute", "genSetAttribute.py")
impl.add_content(parse_includestr +  includestr + output_cpp)
impl.dump(path=config.src_dir)

