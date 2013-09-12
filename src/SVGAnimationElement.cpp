//////////////////////////////////////////////////////////////////////////////
// Name:        SVGAnimationElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGAnimationElement.cpp,v 1.3 2013-09-12 08:42:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGAnimationElement.h"
#include "SVGDocument.h"

double wxSVGAnimationElement::GetStartTime() {
  return m_begin;
}

double wxSVGAnimationElement::GetCurrentTime() {
	wxSVGDocument* doc = (wxSVGDocument*) GetOwnerDocument();
	return doc != NULL ? doc->GetCurrentTime() : 0;
}

double wxSVGAnimationElement::GetSimpleDuration() {
	return m_dur;
}

void wxSVGAnimationElement::ApplyAnimation() {
	if (GetDur() <= 0)
		return;
	if (GetTargetElement() == NULL)
		SetTargetElement((wxSVGElement*) GetParent());
	if (GetCurrentTime() >= GetStartTime() + GetDur())
		GetTargetElement()->SetAttribute(GetAttributeName(), m_to.GetValueAsString());
	else if (GetCurrentTime() >= GetStartTime()) {
		wxSVGLength val(m_to.GetUnitType(), GetFrom().GetValue()
				+ (GetTo().GetValue() - GetFrom().GetValue())*(GetCurrentTime() - GetStartTime())/GetDur());
		GetTargetElement()->SetAttribute(GetAttributeName(), val.GetValueAsString());
	}
}
