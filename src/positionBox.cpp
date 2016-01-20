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

#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>

// Make a box 
PositionBox::PositionBox(QWidget * parent)
    : QWidget(parent)
{
    // Create hboxlayouts that will fill the next row; these
    // are shown only when appropriate
    QHBoxLayout *positionBox = new QHBoxLayout(this);
    positionBox->addWidget(new QLabel(" Coordinate: "));

    QGroupBox *coordinate = new QGroupBox();
    positionBox->addWidget(coordinate);

    xcoord = new QRadioButton("X", coordinate);
    ycoord = new QRadioButton("Y", coordinate);
    zcoord = new QRadioButton("Z", coordinate);

    QHBoxLayout *hBox = new QHBoxLayout(coordinate);
    hBox->addWidget(xcoord);
    hBox->addWidget(ycoord);
    hBox->addWidget(zcoord);
}


// Adjust the controls
void PositionBox::setParticle( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0) {
        colorCriterionPos colorCritPos = (*thisPd).colorCritPos[thisIndex];
        switch (colorCritPos) {
        case XPOS:
            xcoord->setChecked(true);
            break;
        case YPOS:
            ycoord->setChecked(true);
            break;
        case ZPOS:
            zcoord->setChecked(true);
            break;
        }
    }
}


// Read the controls
void PositionBox::readToggles( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0 && thisPd) {
        if (xcoord->isChecked()) {
            (*thisPd).colorCritPos[thisIndex] = XPOS;
        }
        if (ycoord->isChecked()) {
            (*thisPd).colorCritPos[thisIndex] = YPOS;
        }
        if (zcoord->isChecked()) {
            (*thisPd).colorCritPos[thisIndex] = ZPOS;
        }
    }
}
