//
// Declaration of live box class
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

#ifndef LIVEBOX_H
#define LIVEBOX_H

#include "data.h"
#include "mainForm.h"

#include <qcheckbox.h>
#include <q3hbox.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <q3vbox.h>

#include <stdio.h>

// Live box in main form
class LiveBox: public Q3HBox
{
    Q_OBJECT
public:
    LiveBox( QWidget * parent=0, const char * name=0 );

    void setFormAddress( MainForm * );

public slots:
    void startAutoSnap();

private slots:
    void snapCB();
    void autoSnapCB();
    bool getAutoSnap();

private:
    MainForm * mainForm;
    QPushButton * snapPb;
    QPushButton * autoSnapPb;

    QSizePolicy sizePolicy() const;
};


#endif // LIVEBOX_H
