//
// Implementation of property box widget class used by atom board etc.
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

#include "propertyBox.h"

// Make a box 
PropertyBox::PropertyBox( QWidget * parent, const char * name )
    : Q3HBox( parent, name )
{
	// Create hboxlayouts that will fill the next row; these
	// are shown only when appropriate
	Q3HBox * propertyBox = new Q3HBox( this, "propertyBox" );

        QLabel * propL = new QLabel( propertyBox, "propL" );
        propL->setText( "  Property: " );

        property = new Q3ButtonGroup( 8, Qt::Horizontal, propertyBox, "property" );
        prop1 = new QRadioButton( property, "1" );
        prop1->setText( "1" );
        prop2 = new QRadioButton( property, "1" );
        prop2->setText( "2" );
        prop3 = new QRadioButton( property, "3" );
        prop3->setText( "3" );
        prop4 = new QRadioButton( property, "4" );
        prop4->setText( "4" );
        prop5 = new QRadioButton( property, "5" );
        prop5->setText( "5" );
        prop6 = new QRadioButton( property, "6" );
        prop6->setText( "6" );
        prop7 = new QRadioButton( property, "7" );
        prop7->setText( "7" );
        prop8 = new QRadioButton( property, "8" );
        prop8->setText( "8" );
        property->insert( prop1, 0 );
        property->insert( prop2, 1 );
        property->insert( prop3, 2 );
        property->insert( prop4, 3 );
        property->insert( prop5, 4 );
        property->insert( prop6, 5 );
        property->insert( prop7, 6 );
}


// Adjust the controls
void PropertyBox::setParticle( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0) {
		colorCriterionColumn colorCritProp = (*thisPd).colorCritProp[thisIndex];
		switch (colorCritProp) {
       			case PROP1:
				prop1->setChecked( TRUE );
			break;
			case PROP2:
				prop2->setChecked( TRUE );
			break;
			case PROP3:
				prop3->setChecked( TRUE );
			break;
			case PROP4:
				prop4->setChecked( TRUE );
			break;
               		case PROP5:
				prop5->setChecked( TRUE );
			break;
			case PROP6:
				prop6->setChecked( TRUE );
			break;
			case PROP7:
				prop7->setChecked( TRUE );
			break;
			case PROP8:
				prop8->setChecked( TRUE );
	                break;
		}
	}
}


// Read the controls
void PropertyBox::readToggles( particleData * thisPd, int thisIndex )
{
	if (thisIndex >= 0 && thisPd) {
		if (prop1->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP1;
                }
                if (prop2->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP2;
                }
                if (prop3->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP3;
                }
                if (prop4->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP4;
                }
                if (prop5->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP5;
                }
                if (prop6->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP6;
                }
                if (prop7->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP7;
                }
                if (prop8->isChecked() == TRUE) {
                        (*thisPd).colorCritProp[thisIndex] = PROP8;
                }
	}
}
