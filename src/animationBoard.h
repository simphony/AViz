//
// Declaration of animation board class
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

#ifndef ANIMB_H
#define ANIMB_H 

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;

// File list board dialog widget
class AnimationBoard: public QDialog
{
    Q_OBJECT
public:
    AnimationBoard( QWidget * parent=0, const char * name=0 );

private slots:
    void createAGIF();
    void browseCB();
    void bdone();

private:
    QLineEdit * fileLine;
    QPushButton * animGIF;
    QLabel * numberL;
    QString targetDir;
    QSizePolicy sizePolicy() const;
    bool haveTarget;
};

#endif // ANIMB_H
