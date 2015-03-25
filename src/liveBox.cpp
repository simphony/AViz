//
// Implementation of live update box class
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

#include "liveBox.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "mainForm.h"


// Make a box that will hold a horizontal row of buttons, used to 
// take snapshots of the scene
LiveBox::LiveBox(MainForm *mainForm, QWidget *parent)
    : QWidget(parent), mainForm(mainForm)
{
    // Insert pushbuttons
    QPushButton *snapPb = new QPushButton("Snap" );

    autoSnapPb= new QPushButton("AutoSnap");
    autoSnapPb->setCheckable(true);

    // Define callbacks for these pushbuttons
    connect( snapPb, SIGNAL(clicked()), this, SLOT(snapCB()) );
    connect( autoSnapPb, SIGNAL(clicked()), this, SLOT(autoSnapCB()) );

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setMargin( 0 );
    hbox->addWidget(snapPb);
    hbox->addWidget(autoSnapPb);
}

// Trigger a snapshot
void LiveBox::snapCB() {
    mainForm->snapRendering();
}

// Set a flag 
void LiveBox::autoSnapCB() {
    viewParam * vp = mainForm->getViewParam();
    (*vp).autoSnap = this->getAutoSnap();
}

// Start auto snap mode at start-up -- simply indicate this by 
// the button state
void LiveBox::startAutoSnap() {
    this->autoSnapPb->setChecked(true);
}

// Return the auto snap button toggle state
bool LiveBox::getAutoSnap()
{
    return autoSnapPb->isChecked();
}
