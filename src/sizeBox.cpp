//
// Implementation of size box widget class used by atom board etc.
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

#include "sizeBox.h"

// Make a box
SizeBox::SizeBox( QWidget * parent, const char * name )
    : Q3HBox( parent, name )
{
	this->setMargin( SPACE );

	// Add a label and radio buttons to adjust size
	sizeL = new QLabel( this, "sizeL" );
        sizeL->setText( " Rel. Render Size: " );

        rSize = new Q3ButtonGroup( 5, Qt::Horizontal, this, "rSize" );
        rSize0 = new QRadioButton( rSize, "regular" );
        rSize0->setText( "Regular" );
        rSize1 = new QRadioButton( rSize, "tiny" );
        rSize1->setText( "Tiny" );
        rSize2 = new QRadioButton( rSize, "small" );
        rSize2->setText( "Small" );
        rSize3 = new QRadioButton( rSize, "large" );
        rSize3->setText( "Large" );
        rSize4 = new QRadioButton( rSize, "huge" );
        rSize4->setText( "Huge" );

	// Set a default
	rSize0->setChecked( TRUE );
	this->setDisabled( TRUE );
}


// Adjust the controls
void SizeBox::setParticle( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0) {
		relativeSize relSize = (*thisPd).relSize[thisIndex];

	        switch (relSize) {
	        	case REGULAR:
				rSize0->setChecked( TRUE );
			break;
			case TINY:
				rSize1->setChecked( TRUE );
			break;
			case SMALL:
				rSize2->setChecked( TRUE );
			break;
       			case LARGE:
				rSize3->setChecked( TRUE );
			break;
			case HHUGE:
				rSize4->setChecked( TRUE );
			break;
        	}
        }
}


// Read the controls
void SizeBox::readToggles( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0 && thisPd) {
		if (rSize0->isChecked() == TRUE) {
			(*thisPd).relSize[thisIndex] = REGULAR;
                }
                if (rSize1->isChecked() == TRUE) {
                        (*thisPd).relSize[thisIndex] = TINY;
                }
                if (rSize2->isChecked() == TRUE) {
                        (*thisPd).relSize[thisIndex] = SMALL;
                }
                if (rSize3->isChecked() == TRUE) {
                        (*thisPd).relSize[thisIndex] = LARGE;
                }
                if (rSize4->isChecked() == TRUE) {
                        (*thisPd).relSize[thisIndex] = HHUGE;
                }
	}
}


// Switch on or off the entire box
void SizeBox::setDisabled( bool on ) 
{
	sizeL->setDisabled( on );
	rSize->setDisabled( on );
}
