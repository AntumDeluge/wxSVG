//////////////////////////////////////////////////////////////////////////////
// Name:        Animated.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: Animated.h,v 1.1.1.1 2005-05-10 17:51:11 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef WXSVG_ANIMATED_H
#define WXSVG_ANIMATED_H

#include "String.h"

#define WXSVG_MAKE_ANIMATED(X, T) class wxSVGAnimated##X\
{\
  public:\
    T& GetBaseVal() { return m_baseVal; };\
    T& GetAnimVal() { return m_animVal; }\
    \
    T GetBaseVal() const { return m_baseVal; };\
    T GetAnimVal() const { return m_animVal; }\
    \
  public:\
    operator T() { return GetBaseVal(); }\
    \
  protected:\
    T m_baseVal;\
    T m_animVal;\
};

#define WXSVG_MAKE_ANIMATED_NUMBER(X, T) class wxSVGAnimated##X\
{\
  public:\
    wxSVGAnimated##X(): m_baseVal(0), m_animVal(0) {}\
    T& GetBaseVal() { return m_baseVal; };\
    T& GetAnimVal() { return m_animVal; }\
    \
    T GetBaseVal() const { return m_baseVal; };\
    T GetAnimVal() const { return m_animVal; }\
    \
  public:\
    operator T() { return GetBaseVal(); }\
    \
  protected:\
    T m_baseVal;\
    T m_animVal;\
};


#endif // WXSVG_ANIMATED_H

