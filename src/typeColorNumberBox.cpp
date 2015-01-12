//
// Implementation of type color number box widget class used by atom board etc.
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

#include "typeColorNumberBox.h"

// Make a box 
TypeColorNumberBox::TypeColorNumberBox( QWidget * parent, const char * name, int thisMaxNumberOfColors )
    : Q3HBox( parent, name )
{
    // Create hboxlayouts that will fill the next row; these
    // are shown only when appropriate
    Q3HBox * typeColorNumberBox = new Q3HBox( this, "typeColorNumberBox" );
    QLabel * numberL = new QLabel( typeColorNumberBox, "number" );
    numberL->setText( " Type Colors: " );

    // Register max number of color options
    maxNumberOfColors = thisMaxNumberOfColors;

    // Create radio box
    number = new Q3ButtonGroup( maxNumberOfColors, Qt::Horizontal, typeColorNumberBox, "coordinate" );
    one= new QRadioButton( number, "one" );
    one->setText( "One" );
    if (maxNumberOfColors > 1) {
        two= new QRadioButton( number, "two" );
        two->setText( "Two" );
    }
    if (maxNumberOfColors > 2) {
        three= new QRadioButton( number, "three" );
        three->setText( "Three" );
    }
    number->insert( one, 0 );
    if (maxNumberOfColors > 1)
        number->insert( two, 1 );
    if (maxNumberOfColors > 2)
        number->insert( three, 2 );
}


// Adjust the controls
void TypeColorNumberBox::setParticle( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0) {
        int typeColorNumber = (*thisPd).line[thisIndex].typeColorNumber;
        switch (typeColorNumber) {
        case 1:
            one->setChecked( TRUE );
            break;
        case 2:
            if (maxNumberOfColors > 1)
                two->setChecked( TRUE );
            break;
        case 3:
            if (maxNumberOfColors > 2)
                three->setChecked( TRUE );
            break;
        default:
            one->setChecked( TRUE );
            break;
        }
    }
}


// Read the controls
void TypeColorNumberBox::readToggles( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0 && thisPd) {
        if (one->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].typeColorNumber = 1;
        }
        if (maxNumberOfColors > 1) {
            if (two->isChecked() == TRUE) {
                (*thisPd).line[thisIndex].typeColorNumber = 2;
            }
        }
        if (maxNumberOfColors > 2) {
            if (three->isChecked() == TRUE) {
                (*thisPd).line[thisIndex].typeColorNumber = 3;
            }
        }
    }
}
