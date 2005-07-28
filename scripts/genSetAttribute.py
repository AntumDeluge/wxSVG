##############################################################################
## Name:        genSetAttribute.py
## Purpose:     generates Elements_SetAttribute.cpp
##              -> SetAttribute() methods for all svg elements
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: genSetAttribute.py,v 1.5 2005-07-28 20:10:44 ntalex Exp $
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
          set_attr = '\t' + set_attr + '.SetCSSText(attrValue);'
        elif typestr == "Length":
            set_attr = '\t' + set_attr + '.SetValueAsString(attrValue);'
        elif typestr == "Rect":
            set_attr = '''  {
    double value;
    wxStringTokenizer tkz(attrValue, wxT(", \\t"));
    int pi = 0;
    while (tkz.HasMoreTokens()) 
    { 
      wxString token = tkz.GetNextToken(); 
      token.ToDouble(&value);
      switch (pi)
      {
        case 0: %s.SetX(value); break;
        case 1: %s.SetY(value); break;
        case 2: %s.SetWidth(value); break;
        case 3: %s.SetHeight(value); break;
      }
      pi++;
    }
  }'''%(set_attr,set_attr,set_attr,set_attr)
        elif typestr == "PreserveAspectRatio":
            set_attr = '''  {
    wxString value = attrValue.Strip(wxString::both).Lower();
    wxString val = value.BeforeFirst(wxT(' '));
    if (val == wxT("defer"))
    {
      value = value.AfterFirst(wxT(' '));
      val = value.BeforeFirst(wxT(' '));
    }
    if (!val.length())
      return true;
    else if (val == wxT("none"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_NONE);
    else if (val == wxT("xminymin"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMINYMIN);
    else if (val == wxT("xmidymin"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMIDYMIN);
    else if (val == wxT("xmaxymin"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMAXYMIN);
    else if (val == wxT("xminymid"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMINYMID);
    else if (val == wxT("xmidymid"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMIDYMID);
    else if (val == wxT("xmaxymid"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMAXYMID);
    else if (val == wxT("xminymax"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMINYMAX);
    else if (val == wxT("xmidymax"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMIDYMAX);
    else if (val == wxT("xmaxymax"))
      %s.SetAlign(wxSVG_PRESERVEASPECTRATIO_XMAXYMAX);
    value = value.AfterFirst(wxT(' '));
    val = value.BeforeFirst(wxT(' '));
    if (val == wxT("meet"))
      %s.SetMeetOrSlice(wxSVG_MEETORSLICE_MEET);
    else if (val == wxT("slice"))
      %s.SetMeetOrSlice(wxSVG_MEETORSLICE_SLICE);
  }'''%(set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr,set_attr)
        elif typestr == "SVGStringList":
            set_attr = '''  {
    wxStringTokenizer tkz(attrValue, wxT(","));
    while (tkz.HasMoreTokens()) 
      %s.Add(tkz.GetNextToken()); 
  }'''%set_attr
        elif typestr == "LengthList":
            set_attr = '''  {
    wxStringTokenizer tkz(attrValue, wxT(", \\t"));
    while (tkz.HasMoreTokens())
    {
	  wxString token = tkz.GetNextToken();
	  if (!token.length())
		continue;
      wxSVGLength* length = new wxSVGLength();
      length->SetValueAsString(token);
      %s.Add(length);
    }
  }'''%set_attr
        elif typestr == "NumberList":
            set_attr = '''  {
    double value;
    wxStringTokenizer tkz(attrValue, wxT(", \\t"));
    while (tkz.HasMoreTokens()) 
    { 
      wxString token = tkz.GetNextToken(); 
      if (token.length() && token.ToDouble(&value))
        %s.Add(value); 
    }
  }'''%set_attr
        elif typestr == "SVGPointList":  ######### SVGPointList ##########
            set_attr = '''  {
    int num = 0;
    double numbers[2];
    wxStringTokenizer tkz(attrValue, wxT(", \\t"));
    while (tkz.HasMoreTokens()) 
    { 
      wxString token = tkz.GetNextToken(); 
      if (token.length() && token.ToDouble(&numbers[num]))
      {
        num++;
        if (num == 2)
        {
          wxSVGPoint point;
          point.SetX(numbers[0]);
          point.SetY(numbers[1]);
          %s.Add(point);
		  num = 0;
        }
      }
    }
  }'''%set_attr
        elif typestr == "SVGPathSegList":  ######### SVGPathSegList ##########
            set_attr1 = '''  {
    const wxString s_commands = wxT("MmZzLlHhVvCcSsQqTtAa");
    const wxString s_numeric = wxT(".Ee0123456789");
    const wxString s_numericFirst = wxT("+-") + s_numeric;
    const wxString s_separators = wxT(" ,\\t\\n\\r");
    wxChar type = 0;
    double number;
    wxSVGNumberList numbers;
    wxString value = attrValue;
    while (value.Length() && s_separators.Contains(value.Left(1)))
      value.Remove(0,1);
    while (value.Length())
    {
      if (!s_commands.Contains(value.Left(1)))
        break;
      type = value.GetChar(0);
      value.Remove(0,1);
      
      while (value.Length())
      {
        while (value.Length() && s_separators.Contains(value.Left(1)))
          value.Remove(0,1);
        
        unsigned int pos;
        if (value.Length()>0 && s_numericFirst.Contains(value.Left(1)))
        {
          pos = 1;
          while (value.Length()>pos && s_numeric.Contains(value.Mid(pos,1)))
            pos++;
          value.Mid(0,pos).ToDouble(&number);
          numbers.Add(number);
          value.Remove(0,pos);
        }
        else
          break;
      }
        
      // new path segement
      if (type == wxT('Z') || type == wxT('z'))
        %s.Add(new wxSVGPathSegClosePath);
      '''%set_attr
            paths = [["M", "wxSVGPathSegMovetoAbs", ["X", "Y"]],
              ["m", "wxSVGPathSegMovetoRel", ["X", "Y"]],
              ["L", "wxSVGPathSegLinetoAbs", ["X", "Y"]],
              ["l", "wxSVGPathSegLinetoRel", ["X", "Y"]],
              ["H", "wxSVGPathSegLinetoHorizontalAbs", ["X"]],
              ["h", "wxSVGPathSegLinetoHorizontalRel", ["X"]],
              ["V", "wxSVGPathSegLinetoVerticalAbs", ["Y"]],
              ["v", "wxSVGPathSegLinetoVerticalRel", ["Y"]],
              ["C", "wxSVGPathSegCurvetoCubicAbs", ["X1", "Y1", "X2", "Y2", "X", "Y"]],
              ["c", "wxSVGPathSegCurvetoCubicRel", ["X1", "Y1", "X2", "Y2", "X", "Y"]],
              ["S", "wxSVGPathSegCurvetoCubicSmoothAbs", ["X2", "Y2", "X", "Y"]],
              ["s", "wxSVGPathSegCurvetoCubicSmoothRel", ["X2", "Y2", "X", "Y"]],
              ["Q", "wxSVGPathSegCurvetoQuadraticAbs", ["X1", "Y1", "X", "Y"]],
              ["q", "wxSVGPathSegCurvetoQuadraticRel", ["X1", "Y1", "X", "Y"]],
              ["T", "wxSVGPathSegCurvetoQuadraticSmoothAbs", ["X", "Y"]],
              ["t", "wxSVGPathSegCurvetoQuadraticSmoothRel", ["X", "Y"]],
              ["A", "wxSVGPathSegArcAbs", ["R1", "R2", "Angle", "LargeArcFlag", "SweepFlag", "X", "Y"]],
              ["a", "wxSVGPathSegArcRel", ["R1", "R2", "Angle", "LargeArcFlag", "SweepFlag", "X", "Y"]]]
            for path in paths:
              (seg_type, seg_class, seg_methods) = path
              set_attr1 = set_attr1 + '''
      else if (type == wxT('%s'))
      {
        int pos = 0;
        while (numbers.Count()-pos>=%d)
        {
          %s* pathSeg = new %s;'''%(seg_type,len(seg_methods),seg_class,seg_class)
              for seg_method in seg_methods:
                  set_attr1 = set_attr1 + '\n\t\t  pathSeg->Set%s(numbers[pos++]);'%seg_method
              set_attr1 = set_attr1 + '''
          %s.Add(pathSeg);
        }
      }'''%set_attr
      
            set_attr = set_attr1 + '''
      type = 0;
      numbers.Clear();
    }
  }'''
        elif typestr == "TransformList": ######### TransformList ##########
            set_attr = '''  {
    wxStringTokenizer tkz(attrValue, wxT(" \\t"));
    while (tkz.HasMoreTokens()) 
    { 
      wxString token = tkz.GetNextToken(); 
      if (token.length())
      {
        wxSVGTransform transform;
        double params[6] = { 0, 0, 0, 0, 0, 0 };
        wxStringTokenizer tkz2(
          token.AfterFirst(wxT('(')).BeforeLast(wxT(')')), wxT(","));
        int pi = 0;
        while (tkz2.HasMoreTokens()) 
        {
          tkz2.GetNextToken().ToDouble(&params[pi]);
          pi++;
        }
        if (pi == 0)
          continue;
        if (token.substr(0,9) == wxT("translate"))
          transform.SetTranslate(params[0],params[1]);
        else if (token.substr(0,5) == wxT("scale"))
          transform.SetScale(params[0],params[0]);
        else if (token.substr(0,6) == wxT("rotate"))
          transform.SetRotate(params[0],params[1],params[2]);
        else if (token.substr(0,5) == wxT("skewX"))
          transform.SetSkewX(params[0]);
        else if (token.substr(0,5) == wxT("skewY"))
          transform.SetSkewY(params[0]);
        else if (token.substr(0,6) == wxT("matrix"))
		  transform.SetMatrix(wxSVGMatrix(params[0], params[1], params[2], params[3], params[4], params[5]));
		else
		  continue;
        %s.Add(transform);
      }
    }
  }'''%set_attr
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

