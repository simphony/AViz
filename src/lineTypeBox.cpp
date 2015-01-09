//
// Implementation of line type box widget class used by spin board etc.
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

#include "lineTypeBox.h"

#include <cmath>

#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>


// Make a box
LineTypeBox::LineTypeBox( QWidget * parent )
    : QWidget( parent ) {

    QHBoxLayout *hbox = new QHBoxLayout(this);

    // Add radiobuttons and a label
    thickL =new QLabel("  Thickness: ");
    hbox->addWidget(thickL);

    // Create radiobuttons to set the thickness
    footThickness = new QGroupBox();
    hbox->addWidget(footThickness);

    footThickness0 = new QRadioButton( footThickness );
    footThickness0->setText( "Thin" );
    footThickness1 = new QRadioButton( footThickness );
    footThickness1->setText( "Medium" );
    footThickness2 = new QRadioButton( footThickness );
    footThickness2->setText( "Thick" );
    footThickness3 = new QRadioButton( footThickness );
    footThickness3->setText( "Fat" );
    footThickness4 = new QRadioButton( footThickness );
    footThickness4->setText( "Linear Variation" );
    footThickness5 = new QRadioButton( footThickness );
    footThickness5->setText( "Quadratic Variation" );

    QGridLayout *grid = new QGridLayout(footThickness);
    grid->addWidget(footThickness0, 0, 0);
    grid->addWidget(footThickness1, 0, 1);
    grid->addWidget(footThickness2, 1, 0);
    grid->addWidget(footThickness3, 1, 1);
    grid->addWidget(footThickness4, 2, 0);
    grid->addWidget(footThickness5, 2, 1);

    // Create a label and a spin box
    footEmL = new QLabel("  Emission:");
    hbox->addWidget(footEmL);

    footEmSb = new QSpinBox();
    hbox->addWidget(footEmSb);
    footEmSb->setMinimum(0);
    footEmSb->setMaximum(100);
    footEmSb->setSingleStep(10);
    footEmSb->setSuffix( "%" );
    footEmSb->setValue( 0 );
}


// Adjust the controls
void LineTypeBox::setParticle( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0) {
        fixedThickness fThickness = (*thisPd).line[thisIndex].fThickness;
        switch (fThickness) {
        case THIN:
            footThickness0->setChecked( TRUE );
            break;
        case MEDIUM:
            footThickness1->setChecked( TRUE );
            break;
        case THICK:
            footThickness2->setChecked( TRUE );
            break;
        case FAT:
            footThickness3->setChecked( TRUE );
            break;
        case VARYING_LINEAR:
            footThickness4->setChecked( TRUE );
            break;
        case VARYING_QUADRATIC:
            footThickness4->setChecked( TRUE );
            break;
        }

        footEmSb->setValue( (int)floor(100.0*(*thisPd).line[thisIndex].fEmission+0.5) );
    }
}


// Read the controls
void LineTypeBox::readToggles( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0 && thisPd) {
        // Read the settings and
        // update the particle data entry
        if (footThickness0->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = THIN;
        }
        if (footThickness1->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = MEDIUM;
        }
        if (footThickness2->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = THICK;
        }
        if (footThickness3->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = FAT;
        }
        if (footThickness4->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = VARYING_LINEAR;
        }
        if (footThickness5->isChecked() == TRUE) {
            (*thisPd).line[thisIndex].fThickness = VARYING_QUADRATIC;
        }

        (*thisPd).line[thisIndex].fEmission = (float)footEmSb->value()/100.0;
    }
}


// Switch on or off the entire box
// (overloaded function)
void LineTypeBox::setDisabled( bool on ) 
{
    thickL->setDisabled( on );
    footThickness->setDisabled( on );
    footEmL->setDisabled( on );
    footEmSb->setDisabled( on );
}


// Switch on or off the entire box
// (overloaded function)
void LineTypeBox::setDisabled( bool on1, bool on2 ) 
{
    thickL->setDisabled( on1 );
    footThickness->setDisabled( on1 );
    footEmL->setDisabled( on2 );
    footEmSb->setDisabled( on2 );
}
