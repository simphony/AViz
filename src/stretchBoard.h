//
// Declaration of stretch board class
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

#ifndef STRETCHB_H
#define STRETCHB_H 

#include <QDialog>

#include "data.h"

class QFSpinBox;
class MainForm;

// Stretch board dialog widget
class StretchBoard: public QDialog
{
    Q_OBJECT
public:
    StretchBoard(MainForm * , QWidget * parent=0);

public slots:
    void setData( viewParam );

private slots:
    void stretchChanged();
    void registerSettings();
    void doResetX();
    void doResetY();
    void doResetZ();
    void bdone();
    void bapply();
    void bcancel();

private:
    MainForm *m_mainForm;
    QFSpinBox * stretchSBX;
    QFSpinBox * stretchSBY;
    QFSpinBox * stretchSBZ;
    bool stretchChange;
};

#endif // STRETCHB_H
