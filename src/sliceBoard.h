//
// Declaration of slice  board class
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

#ifndef SLICEB_H
#define SLICEB_H 

#include <QDialog>

#include <data.h>

class MainForm;
class QLabel;
class QCheckBox;
class QFSpinBox;

// Slice board dialog widget
class SliceBoard: public QDialog
{
    Q_OBJECT
public:
    SliceBoard( QWidget * parent=0, const char * name=0 );

public slots:
    void setMainFormAddress( MainForm * );
    void setSlice( viewParam );
    void autoSlice( void );

private slots:
    void sliceChanged( void );
    void sliceChangedX( void );
    void sliceChangedY( void );
    void sliceChangedZ( void );
    void registerSettings( void );
    void bdone();
    void bapply();
    void bcancel();

private:
    MainForm * mainForm;
    QCheckBox * sliceCb;
    QCheckBox * indicateXCb;
    QCheckBox * indicateYCb;
    QCheckBox * indicateZCb;
    QLabel * sliceLX1, * sliceLX2;
    QLabel * sliceLY1, * sliceLY2;
    QLabel * sliceLZ1, * sliceLZ2;
    QFSpinBox * sliceSbX1, * sliceSbX2;
    QFSpinBox * sliceSbY1, * sliceSbY2;
    QFSpinBox * sliceSbZ1, * sliceSbZ2;
};

#endif // SLICEB_H
