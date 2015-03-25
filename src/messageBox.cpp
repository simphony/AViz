//
// Implementation of message box dialog widget class
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

#include "messageBox.h"
#include "defaults.h" // SPACE

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

MessageBox::MessageBox(const QString &message, QWidget * parent)
    : QDialog( parent) {
    setWindowTitle( "AViz: Message" );

    // delete when closed
    setAttribute(Qt::WA_DeleteOnClose);

    QPushButton *ok = new QPushButton("OK");
    ok->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(SPACE);
    vbox->addWidget(new QLabel(message));
    vbox->addStretch(1);
    vbox->addWidget(ok, 0 /*stretch*/, Qt::AlignHCenter);
}
