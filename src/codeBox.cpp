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

#include <QLabel>
#include <QSpinBox>
#include <QFrame>
#include <QHBoxLayout>


// Make a box
CodeBox::CodeBox(QWidget * parent)
    : QFrame(parent) {
    setFrameStyle(QFrame::Box | QFrame::Sunken );

    QHBoxLayout *hBox = new QHBoxLayout(this);
    hBox->setMargin( SPACE );

    hBox->addWidget(new QLabel("  Red: Property"));
    codeRedSb = new QSpinBox(this);
    hBox->addWidget(codeRedSb);
    codeRedSb->setMinimum( 1 );
    codeRedSb->setMaximum( 8 );
    codeRedSb->setFixedWidth( SPIN_WIDTH );
    hBox->addWidget(new QLabel("  Green: Property"));
    codeGreenSb = new QSpinBox(this);
    hBox->addWidget(codeGreenSb);
    codeGreenSb->setMinimum( 1 );
    codeGreenSb->setMaximum( 8 );
    codeGreenSb->setFixedWidth( SPIN_WIDTH );
    hBox->addWidget(new QLabel("  Blue: Property"));
    codeBlueSb = new QSpinBox(this);
    hBox->addWidget(codeBlueSb);
    codeBlueSb->setMinimum( 1 );
    codeBlueSb->setMaximum( 8 );
    codeBlueSb->setFixedWidth( SPIN_WIDTH );
}


// Adjust the controls
void CodeBox::setParticle( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0) {
        codeRedSb->setValue( (int)(*thisPd).colorCritCodeRed[thisIndex]+1 );
        codeGreenSb->setValue( (int)(*thisPd).colorCritCodeGreen[thisIndex]+1 );
        codeBlueSb->setValue( (int)(*thisPd).colorCritCodeBlue[thisIndex]+1 );
    }
}


// Read the controls
void CodeBox::readToggles( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0 && thisPd) {
        // Read the settings and
        // update the particle data entry
        (*thisPd).colorCritCodeRed[thisIndex] = (colorCriterionColumn)(codeRedSb->value()-1);
        (*thisPd).colorCritCodeGreen[thisIndex] = (colorCriterionColumn)(codeGreenSb->value()-1);
        (*thisPd).colorCritCodeBlue[thisIndex] = (colorCriterionColumn)(codeBlueSb->value()-1);
    }
}
