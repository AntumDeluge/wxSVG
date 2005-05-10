##############################################################################
## Name:        parse_dtd.py
## Purpose:     
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: parse_dtd.py,v 1.1.1.1 2005-05-10 17:51:21 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:		some modules adapted from svgl project
##############################################################################

import re, string
import parse_attr
import os.path
import config

pathtosvg=os.path.expanduser(config.share_dir + "/svg.dtd")

f=open(pathtosvg,'r')
content=f.read()

# mmm... they should not be different, especially with a so thin
# difference: there is an \s before the > in the second case...

entity_type_decl_re = re.compile('<!ENTITY\s+%\s+([\w\-_]+)\s"([^"]+)"\s*>')
entity_common_attr_re = re.compile('<!ENTITY\s+%\s+([\w\-_]+)\s+"([^"]+)"\s>', re.MULTILINE)

element_re = re.compile('<!ELEMENT\s+([\w\-_]+)\s+([^>]+)>', re.MULTILINE)
attlist_re = re.compile('<!ATTLIST\s+([\w\-_]+)\s+([^>]+)>', re.MULTILINE)

entity_type_decls={}
entity_common_attrs={}
elements={}
attlists={}

beg=0
while 1:
	m = entity_type_decl_re.search(content, beg)
	if m==None:
		break
	beg = m.end()
	name = m.group(1)
	defi = m.group(2)
	
	if defi[0]=='(':
		enums, pos = parse_attr.get_enums(defi,1)
		theattr = parse_attr.attr_named_enum(name, enums)
	else:
		theattr =  parse_attr.attr_named_simple_type(name,defi)

	entity_type_decls[name]=theattr


beg=0
while 1:
	m = entity_common_attr_re.search(content, beg)
	if m==None:
		break
	beg = m.end()
	name = string.strip(m.group(1))
	defi = m.group(2)
	attributes=parse_attr.parse_attr(defi)
	entity_common_attrs[name]=attributes


beg=0
while 1:
	m = element_re.search(content, beg)
	if m==None:
		break
	beg = m.end()

	name = string.strip(m.group(1))
	elements[name]=m.group(2)

beg=0
while 1:
	m = attlist_re.search(content, beg)
	if m==None:
		break
	beg = m.end()

	name = string.strip(m.group(1))
	defi = m.group(2)

	attributes=parse_attr.parse_attr(defi)
	attlists[name] = attributes

	continue
	print '---'
	print name
	for i in attributes:
		l = i.expand(entity_type_decls, entity_common_attrs)
		for j in l:
			print j

