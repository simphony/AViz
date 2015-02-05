//
// Implementation of color label widget class
//

/**********************************************************************
Copyright (C) 2001 - 2003  Geri Wagner

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

Contact address: Computational Physics Group, Dept. of Physics,
                 Technion. 32000 Haifa Israel
                 gery@tx.technion.ac.il
***********************************************************************/

#include "colorLabel.h"

#include <cmath>

#include <QPalette>

#include "defaults.h"

// Construct the color label
ColorLabel::ColorLabel(QWidget *parent)
        : QWidget(parent)
{
    setAutoFillBackground(true);

    setColor(1.0, 1.0, 1.0);
}


// Set the label color
void ColorLabel::setColor( float red, float green, float blue )
{
    int redi = (int)floor(red*255.0);
    int greeni = (int)floor(green*255.0);
    int bluei = (int)floor(blue*255.0);

	setPalette( QPalette( QColor( redi, greeni, bluei) ) );
}


// Set colors that indicate that the label is not active
void ColorLabel::switchOff() {
	setPalette( QPalette( QColor( 255, 255, 255 ) ) );
}

// Set size of the label
QSize ColorLabel::minimumSizeHint() const {
	return QSize( SMALL_LABEL_WIDTH, LABEL_HEIGHT );
}
