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

#ifndef PROPBO_H
#define PROPBO_H 

#include "data.h"

#include <q3buttongroup.h>
#include <q3hbox.h>
#include <qlabel.h>
#include <qradiobutton.h>


// Property box widget
class PropertyBox: public Q3HBox
{
    Q_OBJECT
public:
    PropertyBox( QWidget * parent=0, const char * name=0 );

public slots:
    void setParticle( particleData *, int );
    void readToggles( particleData *, int );

private slots:

private:
    Q3ButtonGroup * property;
    QRadioButton * prop1;
    QRadioButton * prop2;
    QRadioButton * prop3;
    QRadioButton * prop4;
    QRadioButton * prop5;
    QRadioButton * prop6;
    QRadioButton * prop7;
    QRadioButton * prop8;

};

#endif // PROPBO_H
