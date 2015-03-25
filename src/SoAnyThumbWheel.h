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

// Generated automatically from SoAnyThumbWheel.h.in by configure.
// $Id: SoAnyThumbWheel.h.in,v 1.5 2000/09/11 14:58:47 larsa Exp $

#ifndef SOANY_THUMBWHEEL_H
#define SOANY_THUMBWHEEL_H

// ************************************************************************

class SoAnyThumbWheel {
public:
  SoAnyThumbWheel();
  ~SoAnyThumbWheel();

  enum RenderDirection {
    VERTICAL, HORIZONTAL
  };

  enum WheelState {
    DISABLED, ENABLED
  };

  void SetWheelSize( int diameter, int width );
  void GetWheelSize( int & diameter, int & width ) const;

  void SetColor( int red, int green, int blue );
  void GetColor( int & red, int & green, int & blue ) const;
  void SetColorFactors( float light, float front, float normal, float shade );
  void GetColorFactors( float & light, float & front, float & normal, float & shade );

  int BitmapsRequired() const;
  void DrawBitmap( int number, void * bitmap, RenderDirection direction );
  float CalculateValue( float origValue, int origPosition, int deltaPosition );
  int GetBitmapForValue( float value, WheelState state );

  enum GraphicsByteOrder {
    ABGR,
    RGBA,
    ARGB,
    BGRA
  };

  void SetGraphicsByteOrder( const GraphicsByteOrder byteorder );
  GraphicsByteOrder GetGraphicsByteOrder() const;

  enum WheelMotionMethod {
    UNIFORM, AUTHENTIC
  };

  void SetWheelMotionMethod( const WheelMotionMethod method );
  WheelMotionMethod GetWheelMotionMethod() const;

  enum WheelRangeBoundaryHandling {
    MODULATE, ACCUMULATE, CLAMP
  };

  void SetWheelRangeBoundaryHandling( const WheelRangeBoundaryHandling handling );
  WheelRangeBoundaryHandling GetWheelRangeBoundaryHandling() const;

private:
  unsigned int swapWord(unsigned int) const;
  int diameter, width;
  int disabledred, disabledgreen, disabledblue;
  int red, green, blue;
  float light, front, normal, shade;

  GraphicsByteOrder           byteorder;
  WheelRangeBoundaryHandling  handling;
  WheelMotionMethod           method;

  void Validate();

  enum Tables {
    SIN,
    COS,
    RAD,
    NUMTABLES
  };

  float * tables[NUMTABLES];
  int dirtyTables;
  int dirtyVariables;
  float squarelength, squarespacing, shadelength, unistep, numsquares;

  void DrawDisabledWheel(int number, void * bitmap, RenderDirection direction );
  void DrawEnabledWheel( int number, void * bitmap, RenderDirection direction );
}; // class SoAnyThumbWheel

// ************************************************************************

#endif // ! SOANY_THUMBWHEEL_H
