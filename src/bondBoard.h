//
// Declaration of bond board class
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

#ifndef BONDB_H
#define BONDB_H 

#include "data.h"

#include <QDialog>

class MainForm;

class QFSpinBox;
class QRadioButton;
class QSpinBox;
class QLabel;
class QCheckBox;
class QComboBox;
class Q3ButtonGroup;

// Bond board dialog widget
class BondBoard: public QDialog
{
    Q_OBJECT
public:
    BondBoard( QWidget * parent=0);

public slots:
    void setMainFormAddress( MainForm * );
    void setData();

private slots:
    void setFAtom();
    void setTAtom();
    void setBond( int );
    void bdone();
    void bapply();
    void bcancel();

private:
    void setToggles( int, int );
    void readToggles();

    MainForm * mainForm;
    QComboBox * fParticleCob;
    QComboBox * tParticleCob;
    Q3ButtonGroup * bondMode;
    Q3ButtonGroup * bondColor;
    Q3ButtonGroup * bondThickness;
    QRadioButton * bondMode0;
    QRadioButton * bondMode1;
    QRadioButton * bondColor0;
    QRadioButton * bondColor1;
    QRadioButton * bondColor2;
    QRadioButton * bondColor3;
    QRadioButton * bondColor4;
    QRadioButton * bondColor5;
    QRadioButton * bondColor6;
    QRadioButton * bondColor7;
    QRadioButton * bondColor8;
    QRadioButton * bondThickness0;
    QRadioButton * bondThickness1;
    QRadioButton * bondThickness2;
    QRadioButton * bondThickness3;
    QRadioButton * bondThickness4;
    QRadioButton * bondThickness5;
    QSpinBox * bondEmissionSb;
    QCheckBox * antiAliasCb;
    QCheckBox *seqBondCb;
    QFSpinBox * bondMinLengthSb;
    QFSpinBox * bondMaxLengthSb;
    QLabel * colorL, * thickL, * minLengthL, * maxLengthL;
    QLabel * antiAlL, * bondEmL;
    QLabel * seqBond;
    particleData * thisPd;
    tag thisFParticle, thisTParticle;
    bool haveFEntry;
    bool haveTEntry;
};

#endif // BONDB_H
