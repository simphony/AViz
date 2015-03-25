//
// Declaration of line type box class used by spin board etc.
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

#ifndef LINETBO_H
#define LINETBO_H 

#include <QWidget>

#include "data.h"

class QLabel;
class QRadioButton;
class QSpinBox;
class QGroupBox;

// Line type box widget
class LineTypeBox: public QWidget
{
    Q_OBJECT
public:
    LineTypeBox( QWidget * parent);

public:
    void setParticle( particleData *, int );
    void readToggles( particleData *, int );
    void setDisabled( bool );
    void setDisabled( bool, bool );

private:
    QLabel * thickL;
    QGroupBox * footThickness;
    QRadioButton * footThickness0;
    QRadioButton * footThickness1;
    QRadioButton * footThickness2;
    QRadioButton * footThickness3;
    QRadioButton * footThickness4;
    QRadioButton * footThickness5;
    QLabel * footEmL;
    QSpinBox * footEmSb;
};

#endif // LINETBO_H
