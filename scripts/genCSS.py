##############################################################################
## Name:        genCSS.py
## Purpose:     generates CSSStyleDeclaration
## Author:      Alex Thuering
## Created:     2005/06/06
## RCS-ID:      $Id: genCSS.py,v 1.8 2005-09-29 09:54:35 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
##############################################################################

import config
import string
import cpp
import cppHeader
import cppImpl
import sys
from xml.dom.ext.reader import Sax2

class Attribute:
    def __init__(self, dtdName, name, cssType, valueType, function, defValue):
        self.dtdName = dtdName
        self.name = name
        self.cssType = cssType
        self.valueType = valueType
        self.function = function
        self.defValue = defValue

## cssType, valueType, getFunction, name, dtdName, defValue
attibutes = [
Attribute('clip-path', 'ClipPath', 'wxCSSPrimitiveValue', 'wxCSSPrimitiveValue', '', '*s_emptyCSSValue'),
Attribute('color', 'Color', 'wxCSSPrimitiveValue', 'wxRGBColor', 'RGBColorValue', 'wxRGBColor()'),
Attribute('display', 'Display', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_INLINE'),
Attribute('fill', 'Fill', 'wxSVGPaint', 'wxSVGPaint', '', '*s_blackSVGPaint'),
Attribute('fill-opacity', 'FillOpacity', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '1'),
Attribute('fill-rule', 'FillRule', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_NONZERO'),
Attribute('filter', 'Filter', 'wxCSSPrimitiveValue', 'wxCSSPrimitiveValue', '', '*s_emptyCSSValue'),
Attribute('font-family', 'FontFamily', 'wxCSSPrimitiveValue', 'wxString', 'StringValue', 'wxT("")'),
Attribute('font-size', 'FontSize', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '20'),
Attribute('font-stretch', 'FontStretch', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_NORMAL'),
Attribute('font-style', 'FontStyle', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_NORMAL'),
Attribute('font-variant', 'FontVariant', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_NORMAL'),
Attribute('font-weight', 'FontWeight', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_NORMAL'),
Attribute('marker', 'Marker', 'wxCSSPrimitiveValue', 'wxString', 'StringValue', 'wxT("")'),
Attribute('marker-end', 'MarkerEnd', 'wxCSSPrimitiveValue', 'wxCSSPrimitiveValue', '', '*s_emptyCSSValue'),
Attribute('marker-mid', 'MarkerMid', 'wxCSSPrimitiveValue', 'wxCSSPrimitiveValue', '', '*s_emptyCSSValue'),
Attribute('marker-start', 'MarkerStart', 'wxCSSPrimitiveValue', 'wxCSSPrimitiveValue', '', '*s_emptyCSSValue'),
Attribute('opacity', 'Opacity', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '1'),
Attribute('stop-color', 'StopColor', 'wxSVGColor', 'wxSVGColor', '', '*s_emptySVGColor'),
Attribute('stop-opacity', 'StopOpacity', 'wxCSSPrimitiveValue', 'double', 'FloatValue', ' 1'),
Attribute('stroke', 'Stroke', 'wxSVGPaint', 'wxSVGPaint', '', '*s_emptySVGPaint'),
##Attribute('stroke-dasharray', 'StrokeDasharray', 'wxCSSValueList', 'wxCSSValueList', '', 'wxCSSValueList()'),
Attribute('stroke-dashoffset', 'StrokeDashoffset', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '0'),
Attribute('stroke-linecap', 'StrokeLinecap', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_BUTT'),
Attribute('stroke-linejoin', 'StrokeLinejoin', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_MITER'),
Attribute('stroke-miterlimit', 'StrokeMiterlimit', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '4'),
Attribute('stroke-opacity', 'StrokeOpacity', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '1'),
Attribute('stroke-width', 'StrokeWidth', 'wxCSSPrimitiveValue', 'double', 'FloatValue', '1'),
Attribute('text-anchor', 'TextAnchor', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_START'),
Attribute('visibility', 'Visibility', 'wxCSSPrimitiveValue', 'wxCSS_VALUE', 'IdentValue', 'wxCSS_VALUE_VISIBLE')]

def propId(name):
    return 'wxCSS_PROPERTY_' + string.upper(cpp.make_name(name))
    
def generate():
    genCSSStyleDeclaration()
    genStyles()
    #genValues()

######################### CSSStyleDeclaration.h ##############################
def genCSSStyleDeclaration():
    enum = ''
    for attr in attibutes:
        if (len(enum)):
            enum = enum + ',\n'
        enum = enum + '  ' + propId(attr.dtdName)
    
    methods = ''
    for attr in attibutes:
        valueType = attr.valueType
        get = '((%s&)*it->second)'%attr.cssType
        if len(attr.function):
            get = get + '.Get' + attr.function + '()'
        else:
            valueType = "const " + valueType + "&"
        get = '''\
    inline %s Get%s()
    {
      iterator it = find(%s);
      return it != end() ? %s : %s;
    }
    '''%(valueType, attr.name, propId(attr.dtdName), get, attr.defValue)
        
        has = 'inline bool Has%s() { return HasProperty(%s); }\n'%(attr.name, propId(attr.dtdName))
        
        if len(attr.function):
            valueType = attr.valueType
            if valueType not in cpp.builtin_types and valueType != "wxCSS_VALUE":
                valueType = "const " + valueType + "&"
            ptype = ""
            if attr.valueType == "wxString":
                ptype = "wxCSS_STRING, "
            elif attr.valueType == "double":
                ptype = "wxCSS_NUMBER, "
            set = '''\
    inline void Set%s(%s value)
    {
      iterator it = find(%s);
      if (it != end())
        ((%s*)it->second)->Set%s(%svalue);
      else
        (*this)[%s] = new %s(value);
    }
    '''%(attr.name, valueType, propId(attr.dtdName), attr.cssType, attr.function, ptype, propId(attr.dtdName), attr.cssType)
        else:
            set = '''\
    inline void Set%s(const %s& value)
    {
      iterator it = find(%s);
      if (it != end())
      {
        delete it->second;
        it->second = new %s(value);
      }
      else
        (*this)[%s] = new %s(value);
    }
    '''%(attr.name, attr.valueType, propId(attr.dtdName), attr.valueType, propId(attr.dtdName), attr.valueType)
            
        
        if len(methods):
            methods = methods + '\n\n';
        methods = methods + get + has + set;
        
    output = '''
#include "CSSValue.h"
#include "SVGPaint.h"
#include <wx/hashmap.h>

enum wxCSS_PROPERTY
{
  wxCSS_PROPERTY_UNKNOWN,
%s
};

WX_DECLARE_HASH_MAP(wxCSS_PROPERTY, wxCSSValue*, wxIntegerHash, wxIntegerEqual, wxHashMapCSSValue);

typedef wxString wxCSSStyler;

class wxCSSStyleDeclaration: public wxHashMapCSSValue
{
  public:
    wxCSSStyleDeclaration() {}
    wxCSSStyleDeclaration(const wxCSSStyleDeclaration& src) { Add(src); }
    ~wxCSSStyleDeclaration();
    void Add(const wxCSSStyleDeclaration& style);

  public:
    wxString GetCSSText();
    void SetCSSText(const wxString& text);
    
    inline wxString GetPropertyValue(const wxString& propertyName) const
    { return GetPropertyValue(GetPropertyId(propertyName)); }
    
    inline const wxCSSValue& GetPropertyCSSValue(const wxString& propertyName) const
    { return GetPropertyCSSValue(GetPropertyId(propertyName)); }
    
    void SetProperty(const wxString& propertyName, const wxString& value)
    { SetProperty(GetPropertyId(propertyName), value); }
    
    inline bool HasProperty(const wxString& propertyName) const
    { return HasProperty(GetPropertyId(propertyName)); }
    
    inline wxString RemoveProperty(const wxString& propertyName)
    { return RemoveProperty(GetPropertyId(propertyName)); }
  
  public:
    inline wxString GetPropertyValue(wxCSS_PROPERTY propertyId) const
    { const_iterator it = find(propertyId); if (it != end()) return it->second->GetCSSText(); return wxT(""); }
    
    inline const wxCSSValue& GetPropertyCSSValue(wxCSS_PROPERTY propertyId) const
    { const_iterator it = find(propertyId); if (it != end()) return *it->second; return *s_emptyCSSValue; }
    
    void SetProperty(wxCSS_PROPERTY propertyId, const wxString& value);
    inline bool HasProperty(wxCSS_PROPERTY propertyId) const { return find(propertyId) != end(); }
    inline wxString RemoveProperty(wxCSS_PROPERTY propertyId) { erase(propertyId); return wxT(""); }
    
    static wxCSS_PROPERTY GetPropertyId(const wxString& propertyName);
    static wxString GetPropertyName(wxCSS_PROPERTY propertyId);
  
  public:
%s
  
  protected:
    static wxCSSPrimitiveValue* s_emptyCSSValue;
    static wxSVGColor* s_emptySVGColor;
    static wxSVGPaint* s_emptySVGPaint;
    static wxSVGPaint* s_blackSVGPaint;
    static double ParseNumber(const wxString& value);
    static wxRGBColor ParseColor(const wxString& value);
    static void ParseSVGPaint(wxSVGPaint& paint, const wxString& value);
};

/* this class copy only references of css values */
class wxCSSStyleRef: public wxCSSStyleDeclaration
{
  public:
    wxCSSStyleRef() {}
    wxCSSStyleRef(const wxCSSStyleDeclaration& src) { Add(src); }
    ~wxCSSStyleRef();
    void Add(const wxCSSStyleDeclaration& style);
};
'''%(enum, methods) 
    
    header = cppHeader.Header("CSSStyleDeclaration", "genCSS.py")
    header.add_content(output)
    header.dump(path=config.include_dir)

####################### CSSStyleDeclaration_styles.h #########################
def genStyles():
    properties = ''
    for attr in attibutes:
        if len(properties):
            properties = properties + ',\n'
        properties = properties + '  wxT("%s")'%attr.dtdName
    
    output = '''
wxString s_cssPropertyStrings[] = 
{
%s
};
    '''%properties
    
    impl = cppImpl.Impl("css_properties", "genCSS.py")
    impl.add_content(output)
    impl.dump(path=config.src_dir)

############################### CSSValues.h ##################################
cssValues = []
cssProperties = {}
def parseCSSProps():
    reader = Sax2.Reader()
    doc = reader.fromStream(config.share_dir + "/CSS21propidx.html")
    table = doc.getElementsByTagName('HTML')[0].getElementsByTagName('BODY')[0].getElementsByTagName('table')[0]
    for tr in table.childNodes:
        if tr.nodeName == "tr":
            attrName = ''
            attrValue = ''
            i = 0
            for td in tr.childNodes:
                if td.nodeName == "td":
                    i = i + 1
                    if i == 1:
                        attrName = td.childNodes[0].childNodes[0].childNodes[0].nodeValue
                    else:
                        for child in td.childNodes:
                            if child.nodeName == "#text":
                                attrValue = attrValue + child.nodeValue
                        break
            while 1:
                beg = attrValue.find("{")
                end = attrValue.find("}")
                if beg != -1 and end != -1 and beg < end:
                    attrValue = attrValue[:beg] + attrValue[end+1:]
                else:
                    break
            tmp = ''
            for c in attrValue:
                if c in string.ascii_letters or c in string.digits or c in "|-_":
                    tmp = tmp + c
            attrValue = tmp
            cssProperties[attrName] = []
            for val in attrValue.split('|'):
                if len(val) and val not in cssProperties[attrName]:
                    cssProperties[attrName].append(val)
    cssProperties['fill-rule'] = ['nonzero', 'evenodd']
    cssProperties['stroke-linejoin'] = ['miter', 'round', 'bevel']
    cssProperties['stroke-linecap'] = ['butt', 'round', 'square']
    cssProperties['text-anchor'] = ['start', 'middle', 'end']
    cssProperties['font-stretch'] = ['normal', 'wider', 'narrower', 'ultra-condensed',\
        'extra-condensed', 'condensed', 'semi-condensed', 'semi-expanded',\
        'expanded', 'extra-expanded', 'ultra-expanded']
    for attrName in cssProperties.keys():
        for val in cssProperties[attrName]:
            if val not in cssValues:
                cssValues.append(val)
    cssValues.sort()
    
def genValues():
    parseCSSProps()
    
    values = ''
    for value in cssValues:
        if len(values):
            values = values + ',\n'
        values = values + '  wxCSS_VALUE_%s'%cpp.make_name(value).upper()
    
    output = '''\
enum wxCSS_VALUE
{
  wxCSS_VALUE_UNKNOWN,
%s
};'''%values
    
    header = cppHeader.Header("CSSValues", "genCSS.py")
    header.add_content(output)
    header.dump(path=config.include_dir)
    
    values = ''
    for value in cssValues:
        if len(values):
            values = values + ',\n'
        values = values + '  wxT("%s")'%value
    
    output = '''\
wxString s_cssValueStrings[] =
{
%s
};'''%values
    impl = cppImpl.Impl("css_values", "genCSS.py")
    impl.add_content(output)
    impl.dump(path=config.src_dir)

