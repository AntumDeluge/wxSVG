##############################################################################
## Name:        genGetAttributes.py
## Purpose:     generates Elements_GetAttributes.cpp
##              -> GetAttributes() methods for all svg elements
## Author:      Alex Thuering
## Created:     2005/09/27
## RCS-ID:      $Id: genGetAttributes.py,v 1.8 2012-01-08 02:42:14 ntalex Exp $
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

customParser = ["SVGStylable", "SVGFEGaussianBlurElement"] ##TODO["SVGMarkerElement"]

includes = ["String_wxsvg"]
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
        
        #print classdecl.name + '::' + attr.name, typestr
        check = ''
        if typestr in ["String", "DOMString"]:
            check = '!%s.IsEmpty()'%get_attr
        elif typestr in ["Integer", "Boolean", "Enumeration", "unsigned short"]:
            etype = ''
            if typestr == "Integer":
                etype = '(long int) '
            elif typestr == "Boolean":
                check = get_attr
                etype = '(bool) '
            elif typestr == "Enumeration":
                check = '%s != 0'%get_attr
                etype = '(char) '
            elif typestr == "unsigned short":
                if classdecl.name == "SVGZoomAndPan":
                    check = '%s != wxSVG_ZOOMANDPAN_UNKNOWN'%get_attr
                    etype = '(wxSVG_ZOOMANDPAN) '
                elif classdecl.name == "SVGColorProfileElement":
                    etype = '(wxRENDERING_INTENT) '
            if classdecl.name == "SVGGradientElement" and attr.name == "gradientUnits":
                get_attr = '%s == wxSVG_UNIT_TYPE_USERSPACEONUSE ? wxT("userSpaceOnUse") : wxT("objectBoundingBox")'%get_attr
            else:
                get_attr = etype + get_attr
                get_attr = 'wxString::Format(wxT("%%d"), %s)'%get_attr
        elif typestr in ["float", "Number"]:
            check = '%s > 0'%get_attr
            get_attr = 'wxString::Format(wxT("%%g"), %s)'%get_attr
        elif typestr == "css::CSSStyleDeclaration":
            check = '!%s.empty()'%get_attr
            get_attr = '%s.GetCSSText()'%get_attr
        elif typestr in ["Length", "Rect", "PreserveAspectRatio"]  or typestr[-4:] == "List":
            if typestr == "Length":
                check = '%s.GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN'%get_attr
            elif typestr == "PreserveAspectRatio":
                check = '%s.GetAlign() != wxSVG_PRESERVEASPECTRATIO_UNKNOWN && %s.GetMeetOrSlice() != wxSVG_MEETORSLICE_UNKNOWN'%(get_attr,get_attr)
            else:
                check = '!%s.IsEmpty()'%get_attr
            get_attr = '%s.GetValueAsString()'%get_attr
        if len(check)>0:
            func_body = func_body + '  if (%s)\n  '%check
        func_body = func_body + '  attrs.Add(wxT("%s"), %s);\n'%(entity_name, get_attr)
		if classdecl.name in customParser:
			func_body = func_body + '  attrs.Add(GetCustomAttributes());\n'
		
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
            nattr = nattr+res
            func_body = func_body + '  attrs.Add(wx%s::GetAttributes());\n'%inh

    if nattr>0:
        output_cpp = '''
// wx%s
wxSvgXmlAttrHash wx%s::GetAttributes() const
{
  wxSvgXmlAttrHash attrs;
%s\
  return attrs;
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

impl = cppImpl.Impl("Elements_GetAttributes", "genGetAttributes.py")
impl.add_content(includestr + output_cpp)
impl.dump(path=config.src_dir)

