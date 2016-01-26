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
#include <QComboBox>

#include "aggregateData.h"

PropertyBox::PropertyBox( QWidget * parent)
    : QWidget( parent ) {
    QHBoxLayout *propertyBox = new QHBoxLayout( this);

    propertyBox->addWidget(new QLabel("  Property: "));

    QGroupBox *property = new QGroupBox();
    propertyBox->addWidget(property);

    m_propertyCB = new QComboBox(property);

    QHBoxLayout *hBox = new QHBoxLayout(property);
    hBox->addWidget(m_propertyCB);

    // initialize combo box with default entries
    setPropertyInformation(QList<PropertyInformation>());
}

void PropertyBox::setPropertyInformation(const QList<PropertyInformation>& propertyInformation){
    m_propertyCB->clear();
    for(auto i=0; i<8; i++) {
        QString info = QString("Property %1").arg(i);
        if (i<propertyInformation.size()) {
            info += QString(": %1").arg(propertyInformation[i].name);
        }
        m_propertyCB->addItem(info);
    }
}

// Adjust the controls
void PropertyBox::setParticle( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0) {
        colorCriterionColumn colorCritProp = (*thisPd).colorCritProp[thisIndex];
        m_propertyCB->setCurrentIndex(colorCritProp);
    }
}

// Read the controls
void PropertyBox::readToggles( particleData * thisPd, int thisIndex ) {
    if (thisIndex >= 0) {
        (*thisPd).colorCritProp[thisIndex] = colorCriterionColumn(m_propertyCB->currentIndex());
    }
}
