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

#include "aggregateData.h"

PropertyBox::PropertyBox( QWidget * parent)
    : QWidget( parent ) {
    QHBoxLayout *propertyBox = new QHBoxLayout( this);

    propertyBox->addWidget(new QLabel("  Property: "));

    QGroupBox *property = new QGroupBox();
    propertyBox->addWidget(property);

    QHBoxLayout *hBox = new QHBoxLayout(property);
    for(auto i=0; i<8; i++) {
        QRadioButton *rb = new QRadioButton(QString::number(i));
        m_propertyRadioButtons.push_back(rb);
        hBox->addWidget(rb);
    }
}

void PropertyBox::setPropertyInformation(const QList<PropertyInformation>& propertyInformation){
    for(auto i=0; i<8; i++) {
        QString info = (i<propertyInformation.size()) ? propertyInformation[0].name : "";
        m_propertyRadioButtons[i]->setToolTip(info);
    }
}

// Adjust the controls
void PropertyBox::setParticle( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0) {
        colorCriterionColumn colorCritProp = (*thisPd).colorCritProp[thisIndex];
        m_propertyRadioButtons[colorCritProp]->setChecked(true);
    }
}

// Read the controls
void PropertyBox::readToggles( particleData * thisPd, int thisIndex )
{
    if (thisIndex >= 0 && thisPd) {
        for(auto i=0; i<8; i++) {
            if (m_propertyRadioButtons[i]->isChecked()) {
                (*thisPd).colorCritProp[thisIndex] = colorCriterionColumn(i);
            }
        }
    }
}
