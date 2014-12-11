//
// Declaration of light board class
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

#ifndef LIGHTB_H
#define LIGHTB_H 

#include "mainForm.h"
#include "data.h"

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <q3grid.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <q3vbox.h>
#include <qwidget.h>

#include <cmath>

// Lights board dialog widget
class LightsBoard: public QDialog
{
    Q_OBJECT
public:
    LightsBoard( QWidget * parent=0, const char * name=0 );

public slots:
    void setMainFormAddress( MainForm * );
    void setLights( viewParam );

private slots:
    void registerSettings();
    void setLightDepth( int );
    void bdone();
    void bapply();
    void bcancel();

private:
    MainForm * mainForm;
    QCheckBox * light0;
    QCheckBox * light1;
    QCheckBox * light2;
    QCheckBox * light3;
    QCheckBox * light4;
    QCheckBox * light5;
    QCheckBox * light6;
    QCheckBox * light7;
    QComboBox * lightCob;
    QSlider * depthS;
    QSlider * ambientS;
    QSlider * shininessS;
    QLabel * depthL0;
    QLabel * depthL1;
};

#endif // LIGHTB_H
