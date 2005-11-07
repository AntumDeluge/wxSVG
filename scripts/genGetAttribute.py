##############################################################################
## Name:        genGetAttribute.py
## Purpose:     generates Elements_GetAttribute.cpp
##              -> GetAttribute() methods for all svg elements
## Author:      Alex Thuering
## Created:     2005/09/27
## RCS-ID:      $Id: genGetAttribute.py,v 1.3 2005-11-07 17:47:43 ntalex Exp $
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

includes = ["String"]
already_done={}
output_cpps = {}

def process(classdecl):
    if classdecl.name in already_done.keys():
        return already_done[classdecl.name];
    
    already_done[classdecl.name] = 0
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
        
        get_attr = cpp.make_attr_name(attr.name)
        typestr =attr.type.name
        anim_pos = string.find(typestr, 'Animated')
        if anim_pos>=0: # SVGAnimatedTypename
            typestr = typestr[anim_pos+len('Animated'):]
            get_attr = get_attr + '.GetBaseVal()'
        
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
            get_attr = etype + get_attr
            get_attr = '    return wxString::Format(wxT("%%d"), %s);'%get_attr
        elif typestr in ["float", "Number"]:
            get_attr = '    return wxString::Format(wxT("%%d"), %s);'%get_attr
        elif typestr == "css::CSSStyleDeclaration":
            get_attr = '    return %s.GetCSSText();'%get_attr
        elif typestr in ["Length", "Rect", "PreserveAspectRatio"]  or typestr[-4:] == "List":
            get_attr = '    return %s.GetValueAsString();'%get_attr
        else:
            get_attr = '    return ' + get_attr + ';'
        func_body = func_body + 'if (attrName == wxT("%s"))\n'%entity_name
        func_body = func_body + get_attr + '\n  else '

    for inh in classdecl.inherits:
        if inh in ["Element", "events::EventTarget", "events::DocumentEvent",
                   "css::ViewCSS", "css::DocumentCSS", "css::CSSValue",
                   "smil::ElementTimeControl", "Document", "events::UIEvent",
                   "css::CSSRule", "events::Event"]:
            continue
        res = process(parse_idl.class_decls[inh])
        if res>0:
            if nattr==0:
                includes.append(classdecl.name)

            nattr = nattr + res
            func_body = func_body + 'if (wx%s::HasAttribute(attrName))\n'%inh
            func_body = func_body + '    return wx%s::GetAttribute(attrName);\n'%inh
            func_body = func_body + '  else '

    if nattr>0:
        if classdecl.name in customParser:
            func_body = func_body + 'if (HasCustomAttribute(attrName))\n'
            func_body = func_body + '    return GetCustomAttribute(attrName);\n'
            func_body = func_body + '  else'
        if mapDtdIdl.elements_idl_dtd.has_key(classdecl):
            func_body = func_body + '\n    return wxT("");\n' #wxLogDebug(wxT("unknown attribute %s::") + attrName);%(classdecl.name)
        else:
            func_body = func_body + '\n    return wxT("");\n'

        output_cpp = '''
// wx%s
wxString wx%s::GetAttribute(const wxString& attrName)
{
  %s
  return wxT("");
}      
'''%(classdecl.name, classdecl.name, func_body)

        output_cpps[classdecl.name]=output_cpp
    
    already_done[classdecl.name] = nattr
    return nattr


if len(parse_idl.class_decls):
    cnames = parse_idl.class_decls.keys()
    cnames.sort()
    for name in cnames:
        process(parse_idl.class_decls[name])

output_cpp=""
if len(output_cpps):
    for value in output_cpps.values():
        output_cpp = output_cpp + value

includestr=''
for i in includes:
    includestr = includestr + '#include "%s.h"\n'%i

impl = cppImpl.Impl("Elements_GetAttribute", "genGetAttribute.py")
impl.add_content(includestr + output_cpp)
impl.dump(path=config.src_dir)

