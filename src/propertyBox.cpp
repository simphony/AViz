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

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>


// Make a box 
PropertyBox::PropertyBox( QWidget * parent)
    : QWidget( parent ) {
    QHBoxLayout *propertyBox = new QHBoxLayout( this);

    propertyBox->addWidget(new QLabel("  Property: "));

    QGroupBox *property = new QGroupBox();
    propertyBox->addWidget(property);

    prop1 = new QRadioButton("1");
    prop2 = new QRadioButton("2");
    prop3 = new QRadioButton("3");
    prop4 = new QRadioButton("4");
    prop5 = new QRadioButton("5");
    prop6 = new QRadioButton("6");
    prop7 = new QRadioButton("7");
    prop8 = new QRadioButton("8");

    QHBoxLayout *hBox = new QHBoxLayout(property);
    hBox->addWidget(prop1);
    hBox->addWidget(prop2);
    hBox->addWidget(prop3);
    hBox->addWidget(prop4);
    hBox->addWidget(prop5);
    hBox->addWidget(prop6);
    hBox->addWidget(prop7);
}


// Adjust the controls
void PropertyBox::setParticle( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0) {
        colorCriterionColumn colorCritProp = (*thisPd).colorCritProp[thisIndex];
        switch (colorCritProp) {
        case PROP1:
            prop1->setChecked(true);
            break;
        case PROP2:
            prop2->setChecked(true);
            break;
        case PROP3:
            prop3->setChecked(true);
            break;
        case PROP4:
            prop4->setChecked(true);
            break;
        case PROP5:
            prop5->setChecked(true);
            break;
        case PROP6:
            prop6->setChecked(true);
            break;
        case PROP7:
            prop7->setChecked(true);
            break;
        case PROP8:
            prop8->setChecked(true);
            break;
        }
    }
}


// Read the controls
void PropertyBox::readToggles( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0 && thisPd) {
        if (prop1->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP1;
        }
        if (prop2->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP2;
        }
        if (prop3->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP3;
        }
        if (prop4->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP4;
        }
        if (prop5->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP5;
        }
        if (prop6->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP6;
        }
        if (prop7->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP7;
        }
        if (prop8->isChecked()) {
            (*thisPd).colorCritProp[thisIndex] = PROP8;
        }
    }
}
