//
// Implementation of floating point spin box class
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

#include "floatSpin.h"

QFSpinBox::QFSpinBox( QWidget* parent)
    : QSpinBox( parent )
{
    m_validator = new QDoubleValidator(this);
}


// This is used to build a floating-point spin box
/*virtual*/ QString QFSpinBox::textFromValue( int value ) const {
    if (value >= 0)
        return QString("%1.%2").arg(value/10).arg(value%10);
    else
        return QString("-%1.%2").arg(-value/10).arg(-value%10);
}


// This is used to build a floating-point spin box
/*virtual*/ int QFSpinBox::valueFromText(const QString &text) const {
    return int(text.toFloat()*10);
}

/*virtual*/ QValidator::State QFSpinBox::validate(QString & text, int &pos) const {
    QValidator::State state =  m_validator->validate(text, pos);
    return state;
}
