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

#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QRadioButton>

TypeColorNumberBox::TypeColorNumberBox( QWidget * parent, int thisMaxNumberOfColors )
    : QWidget(parent), maxNumberOfColors(thisMaxNumberOfColors) {
    QHBoxLayout *hbox = new QHBoxLayout(this);

    hbox->addWidget(new QLabel(" Type Colors: "));

    {
        // Create group of radio buttons
        QGroupBox *numberGroupBox = new QGroupBox();
        hbox->addWidget(numberGroupBox);

        QHBoxLayout *numberHBox = new QHBoxLayout(numberGroupBox);

        one= new QRadioButton("One");
        numberHBox->addWidget(one);

        if (maxNumberOfColors > 1) {
            two= new QRadioButton("Two");
            numberHBox->addWidget(two);
        }
        if (maxNumberOfColors > 2) {
            three= new QRadioButton("Three");
            numberHBox->addWidget(three);
        }
    }

}


// Adjust the controls
void TypeColorNumberBox::setParticle( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0) {
        int typeColorNumber = (*thisPd).line[thisIndex].typeColorNumber;
        switch (typeColorNumber) {
        case 1:
            one->setChecked( true );
            break;
        case 2:
            if (maxNumberOfColors > 1)
                two->setChecked( true );
            break;
        case 3:
            if (maxNumberOfColors > 2)
                three->setChecked( true );
            break;
        default:
            one->setChecked( true );
            break;
        }
    }
}


// Read the controls
void TypeColorNumberBox::readToggles( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0 && thisPd) {
        if (one->isChecked()) {
            (*thisPd).line[thisIndex].typeColorNumber = 1;
        }
        if (maxNumberOfColors > 1 && two->isChecked()) {
            (*thisPd).line[thisIndex].typeColorNumber = 2;
        }
        if (maxNumberOfColors > 2 && three->isChecked()) {
            (*thisPd).line[thisIndex].typeColorNumber = 3;
        }
    }
}
