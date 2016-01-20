//
// Declaration of property box class used by atom board etc.
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

#ifndef PROPERTY_BOX_H
#define PROPERTY_BOX_H

#include <QWidget>

#include "data.h"

class QRadioButton;
class PropertyInformation;

/*! @class PropertyBox
    @brief Widget to select one of the 8 properties

*/
class PropertyBox: public QWidget
{
public:
    PropertyBox(QWidget * parent=0);

    void setParticle( particleData *, int );
    void readToggles( particleData *, int );

    /// set property information
    void setPropertyInformation(const QList<PropertyInformation>& propertyInformation);

private:
    QList<QRadioButton*> m_propertyRadioButtons;
};

#endif // PROPBO_H
