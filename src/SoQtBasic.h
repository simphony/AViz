/* src/Inventor/Qt/SoQtBasic.h.  Generated automatically by configure.  */
/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// Generated automatically from SoGuiBasic.h.in by configure.
// $Id: SoGuiBasic.h.in,v 1.11 2000/09/11 14:58:47 larsa Exp $

#ifndef SOQT_BASIC_H
#define SOQT_BASIC_H

/*!
  \defgroup devices Qt Device Classes
*/

/*!
  \defgroup components Qt Components
*/

/*!
  \defgroup viewers Qt Viewer Components
*/

/*!
  \defgroup editors Qt Editor Components
*/

/*!
  \defgroup slidersets Qt Slider Set Components
*/

/*!
  \defgroup misc Miscellaneous Classes
*/

/*!
  \defgroup common Common Code
*/

// *************************************************************************

template <class Type>
inline Type SoQtMax( Type a, Type b ) { return (b > a) ? b : a; }

template <class Type>
inline Type SoQtMin( Type a, Type b ) { return (b < a) ? b : a; }

template <class Type>
inline void SoQtSwap( Type & a, Type & b ) { Type t = a; a = b; b = t; }

// *************************************************************************

#define __COIN_SOQT__

#if ! defined(SOQT_MAJOR_VERSION)
#define SOQT_MAJOR_VERSION 0
#endif /* ! SOQT_MAJOR_VERSION */
#if ! defined(SOQT_MINOR_VERSION)
#define SOQT_MINOR_VERSION 8
#endif /* ! SOQT_MINOR_VERSION */
#if ! defined(SOQT_MICRO_VERSION)
#define SOQT_MICRO_VERSION 1
#endif /* ! SOQT_MICRO_VERSION */
#if ! defined(SOQT_VERSION)
#define SOQT_VERSION "0.8.1"
#endif /* ! SOQT_VERSION */

#ifdef SOQT_MAKE_DLL
#define SOQT_DLL_EXPORT __declspec(dllexport)
#else
#define SOQT_DLL_EXPORT
#endif

// *************************************************************************

#endif // ! SOQT_BASIC_H
