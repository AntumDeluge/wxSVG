//////////////////////////////////////////////////////////////////////////////
// Name:        SVGAnimationElement.cpp
// Purpose:     Implementation of wxSVGAnimationElement
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGAnimationElement.cpp,v 1.7 2016-01-09 23:31:14 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGAnimationElement.h"
#include "SVGDocument.h"
#include "SVGSVGElement.h"
#include <wx/log.h>

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

wxSVGElement* wxSVGAnimationElement::GetTargetElement() const {
	if (m_href.length() && GetOwnerSVGElement()) {
		return (wxSVGElement*) GetOwnerSVGElement()->GetElementById(m_href);
	}
	return (wxSVGElement*) GetParent();
}

void wxSVGAnimationElement::ApplyAnimation() {
	wxSVGElement* targetElement = GetTargetElement();
	if (targetElement == NULL || GetDur() <= 0)
		return;
	if (GetCurrentTime() >= GetStartTime() + GetDur()) {
		targetElement->SetAnimatedValue(GetAttributeName(), GetTo());
	} else if (GetCurrentTime() >= GetStartTime() && GetFrom().GetPropertyType() == wxSVG_ANIMATED_LENGTH
			&& GetTo().GetPropertyType() == wxSVG_ANIMATED_LENGTH) {
		wxSVGAnimatedType value(wxSVGLength(GetTo().GetLength().GetUnitType(), GetFrom().GetLength().GetValue()
				+ (GetTo().GetLength().GetValue() - GetFrom().GetLength().GetValue())*
				(GetCurrentTime() - GetStartTime())/GetDur()));
		value.GetValueAsString();
		targetElement->SetAnimatedValue(GetAttributeName(), value);
	}
}
