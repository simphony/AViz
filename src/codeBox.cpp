//
// Implementation of code box widget class used by atom board etc.
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

#include "codeBox.h"

#include <Q3Frame>

// Make a box
CodeBox::CodeBox( QWidget * parent, const char * name )
    : Q3HBox( parent, name )
{
	this->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
	this->setMargin( SPACE );

        codeRedL = new QLabel( this, "codeRedL" );
	codeRedL->setText( "  Red: Property" ); 
	codeRedSb = new QSpinBox( this, "codeRedSb" ); 
	codeRedSb->setMinValue( 1 );
	codeRedSb->setMaxValue( 8 );
	codeRedSb->setFixedWidth( SPIN_WIDTH );
        codeGreenL = new QLabel( this, "codeGreenL" );
	codeGreenL->setText( "  Green: Property" ); 
	codeGreenSb = new QSpinBox( this, "codeGreenSb" ); 
	codeGreenSb->setMinValue( 1 );
	codeGreenSb->setMaxValue( 8 );
	codeGreenSb->setFixedWidth( SPIN_WIDTH );
        codeBlueL = new QLabel( this, "codeBlueL" );
	codeBlueL->setText( "  Blue: Property" ); 
	codeBlueSb = new QSpinBox( this, "codeBlueSb" ); 
	codeBlueSb->setMinValue( 1 );
	codeBlueSb->setMaxValue( 8 );
	codeBlueSb->setFixedWidth( SPIN_WIDTH );
}


// Adjust the controls
void CodeBox::setParticle( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0) {
		codeRedSb->setValue( (int)(*thisPd).colorCritCodeRed[thisIndex]+1 );
		codeGreenSb->setValue( (int)(*thisPd).colorCritCodeGreen[thisIndex]+1 );
		codeBlueSb->setValue( (int)(*thisPd).colorCritCodeBlue[thisIndex]+1 );
	}
}


// Read the controls
void CodeBox::readToggles( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0 && thisPd) {
        	// Read the settings and
	        // update the particle data entry
                (*thisPd).colorCritCodeRed[thisIndex] = (colorCriterionColumn)(codeRedSb->value()-1);
                (*thisPd).colorCritCodeGreen[thisIndex] = (colorCriterionColumn)(codeGreenSb->value()-1);
                (*thisPd).colorCritCodeBlue[thisIndex] = (colorCriterionColumn)(codeBlueSb->value()-1);
		
	}
}
