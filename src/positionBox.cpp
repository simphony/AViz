//
// Implementation of position box widget class used by atom board etc.
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

#include "positionBox.h"

// Make a box 
PositionBox::PositionBox( QWidget * parent, const char * name )
    : Q3HBox( parent, name )
{
	// Create hboxlayouts that will fill the next row; these
	// are shown only when appropriate
	Q3HBox * positionBox = new Q3HBox( this, "positionBox" );
        QLabel * posL = new QLabel( positionBox, "posL" );
	posL->setText( " Coordinate: " ); 

	coordinate = new Q3ButtonGroup( 3, Qt::Horizontal, positionBox, "coordinate" );
	xcoord = new QRadioButton( coordinate, "x" );
	xcoord->setText( "X" );
	ycoord = new QRadioButton( coordinate, "y" );
	ycoord->setText( "Y" );
	zcoord = new QRadioButton( coordinate, "z" );
	zcoord->setText( "Z" );
	coordinate->insert( xcoord, 0 );
	coordinate->insert( ycoord, 1 );
	coordinate->insert( zcoord, 2 );
}


// Adjust the controls
void PositionBox::setParticle( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0) {
		colorCriterionPos colorCritPos = (*thisPd).colorCritPos[thisIndex];
		switch (colorCritPos) {
			case XPOS:
				xcoord->setChecked( TRUE );
			break;
			case YPOS:
				ycoord->setChecked( TRUE );
			break;
			case ZPOS:
				zcoord->setChecked( TRUE );
			break;
		}
	}
}


// Read the controls
void PositionBox::readToggles( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0 && thisPd) {
	        if (xcoord->isChecked() == TRUE) {
	                (*thisPd).colorCritPos[thisIndex] = XPOS;
	        }
	        if (ycoord->isChecked() == TRUE) {
	                (*thisPd).colorCritPos[thisIndex] = YPOS;
	        }
	        if (zcoord->isChecked() == TRUE) {
	                (*thisPd).colorCritPos[thisIndex] = ZPOS;
	        }
	}
}
