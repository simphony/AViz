/**************************************************************************
 *
 *  This file is part of the Coin SoQt GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoQt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOQT_DEBUG
static const char rcsid[] =
  "$Id: SoQtThumbWheel.cpp,v 1.7 2000/09/11 13:15:19 larsa Exp $";
#endif // SOQT_DEBUG

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <QPainter>
#include <QImage>
#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QPalette>

#include "SoAnyThumbWheel.h"
#include "SoQtThumbWheel.h"

// *************************************************************************

static const int SHADEBORDERWIDTH = 2;

SoQtThumbWheel::SoQtThumbWheel(
  QWidget * parent)
: QWidget( parent )
{
  this->constructor( SoQtThumbWheel::Vertical );
} // SoQtThumbWheel()

SoQtThumbWheel::SoQtThumbWheel(
  Orientation orientation,
  QWidget * parent)
: QWidget( parent )
{
  this->constructor( orientation );
} // SoQtThumbWheel()

void
SoQtThumbWheel::constructor( // private
  Orientation orientation )
{
  this->orient = orientation;
  this->state = SoQtThumbWheel::Idle;
  this->wheelValue = this->tempWheelValue = 0.0f;
  this->wheel = new SoAnyThumbWheel;
  this->wheel->SetWheelMotionMethod( SoAnyThumbWheel::UNIFORM );
  this->wheel->SetGraphicsByteOrder( SoAnyThumbWheel::ARGB );
  this->wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::MODULATE );
  this->pixmaps = NULL;
  this->numPixmaps = 0;
  this->currentPixmap = -1;
} // constructor()

SoQtThumbWheel::~SoQtThumbWheel(
  void )
{
  delete this->wheel;
  if ( this->pixmaps ) {
    for ( int i = 0; i < this->numPixmaps; i++ )
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }
} // ~SoQtThumbWheel()

void
SoQtThumbWheel::setOrientation(
  Orientation orientation )
{
  this->orient = orientation;
  this->repaint();
} // setOrientation()

void
SoQtThumbWheel::paintEvent(
  QPaintEvent * event )
{
  QPainter p( this );
  QRect paintRect = event->rect();
  p.setClipRect(paintRect);

  int w, d;
  if ( this->orient == SoQtThumbWheel::Vertical ) {
    w = this->width() - 2*SHADEBORDERWIDTH - 6;
    d = this->height() - 2*SHADEBORDERWIDTH - 12;
  } else {
    w = this->height() - 2*SHADEBORDERWIDTH - 6;
    d = this->width() - 2*SHADEBORDERWIDTH - 12;
  }

  // Handle resizing to too small dimensions gracefully.
  if ( ( d <= 0 ) || ( w <= 0 ) ) return;

  this->initWheel( d, w );

  int pixmap = this->wheel->GetBitmapForValue( this->tempWheelValue,
     (this->state == SoQtThumbWheel::Disabled) ?
        SoAnyThumbWheel::DISABLED : SoAnyThumbWheel::ENABLED );


  QRect wheelrect( SHADEBORDERWIDTH, SHADEBORDERWIDTH,
                   this->width() - 2*SHADEBORDERWIDTH,
                   this->height() - 2*SHADEBORDERWIDTH );

  qDrawShadePanel(&p, 0, 0, this->width(), this->height(),
                  palette(), false, SHADEBORDERWIDTH);

  if ( this->orient == Vertical ) {
    wheelrect.setTop(    wheelrect.top() + 5 );
    wheelrect.setBottom( wheelrect.bottom() - 5 );
    wheelrect.setLeft(   wheelrect.left() + 2 );
    wheelrect.setRight(  wheelrect.right() - 2 );
  } else {
    wheelrect.setTop(    wheelrect.top() + 2 );
    wheelrect.setBottom( wheelrect.bottom() - 2 );
    wheelrect.setLeft(   wheelrect.left() + 5 );
    wheelrect.setRight(  wheelrect.right() - 5 );
  }
  
  qDrawPlainRect( &p, wheelrect.left(), wheelrect.top(), wheelrect.width(),
                  wheelrect.height(), QColor( 0, 0, 0 ), 1 );
  
  wheelrect.setTop(    wheelrect.top() + 1 );
  wheelrect.setBottom( wheelrect.bottom() - 1 );
  wheelrect.setLeft(   wheelrect.left() + 1 );
  wheelrect.setRight(  wheelrect.right() - 1 );
  // wheelrect is now wheel-only

  if ( this->orient == Vertical )
    p.drawPixmap(wheelrect.left(), wheelrect.top(), *this->pixmaps[pixmap],
            0, 0, w, d);
  else
    p.drawPixmap(wheelrect.left(), wheelrect.top(), *this->pixmaps[pixmap],
            0, 0, d, w);
  this->currentPixmap = pixmap;
} // paintEvent()

/*!
  \internal
*/

void
SoQtThumbWheel::mousePressEvent(
  QMouseEvent * event )
{
  if ( this->state != SoQtThumbWheel::Idle )
    return;

  if ( event->button() != Qt::LeftButton )
    return;

  QRect wheel;
  if ( this->orient == Vertical ) {
    wheel.setLeft( SHADEBORDERWIDTH + 3 );
    wheel.setTop( SHADEBORDERWIDTH + 6 );
    wheel.setRight( this->width() - SHADEBORDERWIDTH - 3 );
    wheel.setBottom( this->height() - SHADEBORDERWIDTH - 6 );
  } else {
    wheel.setLeft( SHADEBORDERWIDTH + 6 );
    wheel.setTop( SHADEBORDERWIDTH + 3 );
    wheel.setRight( this->width() - SHADEBORDERWIDTH - 6 );
    wheel.setBottom( this->height() - SHADEBORDERWIDTH - 3 );
  }

  if ( ! wheel.contains( event->pos() ) )
    return;

  this->state = SoQtThumbWheel::Dragging;

  if ( this->orient == SoQtThumbWheel::Vertical )
    this->mouseDownPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseDownPos = event->pos().x() - SHADEBORDERWIDTH - 6;

  this->mouseLastPos = this->mouseDownPos;

  emit wheelPressed();
} // mousePressEvent()

/*!
  \internal
*/

void
SoQtThumbWheel::mouseMoveEvent(
  QMouseEvent * event )
{
  if ( this->state != SoQtThumbWheel::Dragging )
    return;

  if ( this->orient == SoQtThumbWheel::Vertical )
    this->mouseLastPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseLastPos = event->pos().x() - SHADEBORDERWIDTH - 6;
 

  this->tempWheelValue = this->wheel->CalculateValue( this->wheelValue,
      this->mouseDownPos, this->mouseLastPos - this->mouseDownPos );

  emit wheelMoved( this->tempWheelValue );

  this->repaint();
} // mouseMoveEvent()

/*!
  \internal
*/

void
SoQtThumbWheel::mouseReleaseEvent(
  QMouseEvent * event )
{
  if ( this->state != SoQtThumbWheel::Dragging )
    return;

  if ( event->button() != Qt::LeftButton )
    return;

  this->wheelValue = this->tempWheelValue;
  this->mouseLastPos = this->mouseDownPos;
  this->state = SoQtThumbWheel::Idle;
  emit wheelReleased();
} // mouseReleaseEvent()

/*
float
SoQtThumbWheel::getNormalizedValue(int pos) const
{
  int relativepos = pos - this->mouseDownPos;
  return (float) relativepos / (float)this->getWheelLength() * 2.0f;
} // getNormalizedValue()
*/

/*
int
SoQtThumbWheel::getWheelLength() const
{
  return this->orient == SoQtThumbWheel::Vertical ?
    this->height() : this->width();
} // getWheelLength()
*/

/*
int
SoQtThumbWheel::orientedCoord(const QPoint &p) const
{
  return (this->orient == SoQtThumbWheel::Horizontal) ?  p.x() : p.y();
} // orientedCoord()
*/

QSize
SoQtThumbWheel::sizeHint() const
{
  const int length = 88;
  int thick = 24;

  if (this->orient == SoQtThumbWheel::Horizontal)
    return QSize(length, thick);
  else
    return QSize(thick, length);
} // sizeHint()

SoQtThumbWheel::Orientation
SoQtThumbWheel::orientation() const
{
  return this->orient;
} // orientation()

float
SoQtThumbWheel::value() const
{
  return this->wheelValue;
} // value()

// *************************************************************************

void
SoQtThumbWheel::initWheel(
  int diameter,
  int width )
{
  int d, w;
  this->wheel->GetWheelSize( d, w );
  if ( d == diameter && w == width ) return;

  this->wheel->SetWheelSize( diameter, width );

  int pwidth = width;
  int pheight = diameter;
  if ( this->orient == Horizontal ) {
    pwidth = diameter;
    pheight = width;
  }

  if ( this->pixmaps != NULL ) {
    for ( int i = 0; i < this->numPixmaps; i++ )
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }

  this->numPixmaps = this->wheel->BitmapsRequired();
  this->pixmaps = new QPixmap * [this->numPixmaps];
  QImage image( pwidth, pheight, QImage::Format_RGB32);
  for ( int i = 0; i < this->numPixmaps; i++ ) {
    this->wheel->DrawBitmap( i, image.bits(), (this->orient == Vertical) ?
      SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL );
    this->pixmaps[i] = new QPixmap( QSize( pwidth, pheight) );
    this->pixmaps[i]->convertFromImage( image );
  }
} // initWheel()

// *************************************************************************

void
SoQtThumbWheel::setEnabled(
  bool enable )
{
  if ( enable )
    this->state = SoQtThumbWheel::Idle;
  else
    this->state = SoQtThumbWheel::Disabled;
  this->repaint();
} // setEnabled()

bool
SoQtThumbWheel::isEnabled(
  void ) const
{
  return ( this->state != SoQtThumbWheel::Disabled );
} // isEnabled()

void
SoQtThumbWheel::setValue(
  float value )
{
  this->wheelValue = this->tempWheelValue = value;
  this->mouseDownPos = this->mouseLastPos;
  this->repaint();
} // setValue()

// *************************************************************************

void
SoQtThumbWheel::setRangeBoundaryHandling(
  boundaryHandling handling )
{
  switch ( handling ) {
  case CLAMP:
    this->wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::CLAMP );
    break;
  case MODULATE:
    this->wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::MODULATE );
    break;
  case ACCUMULATE:
    this->wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::ACCUMULATE );
    break;
  default:
    assert( 0 && "impossible" );
  }
} // setRangeBoundaryHandling()

// *************************************************************************

SoQtThumbWheel::boundaryHandling
SoQtThumbWheel::getRangeBoundaryHandling(
  void ) const
{
  switch ( this->wheel->GetWheelRangeBoundaryHandling() ) {
  case SoAnyThumbWheel::CLAMP:
    return CLAMP;
  case SoAnyThumbWheel::MODULATE:
    return MODULATE;
  case SoAnyThumbWheel::ACCUMULATE:
    return ACCUMULATE;
  default:
    assert( 0 && "impossible" );
  }
  return CLAMP; // never reached
} // getRangeBoundaryHandling()

// *************************************************************************
