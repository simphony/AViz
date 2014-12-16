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

// Make a box that will hold a horizontal row of buttons, used to 
// take snapshots of the scene
LiveBox::LiveBox( QWidget * parent, const char * name )
    : Q3HBox( parent, name )
{
	// Create some space
	this->setMargin( 0 );

	// Insert pushbuttons 
	snapPb = new QPushButton( this, "Snap" );
	snapPb->setText( "Snap" );
	
	autoSnapPb= new QPushButton( this, "AutoSnap" );
	autoSnapPb->setText( "AutoSnap" );
	autoSnapPb->setCheckable(true);
	
	// Define callbacks for these pushbuttons
	connect( snapPb, SIGNAL(clicked()), this, SLOT(snapCB()) );
	connect( autoSnapPb, SIGNAL(clicked()), this, SLOT(autoSnapCB()) );

	// Reset the form pointer
	mainForm = NULL;
}


// Set a pointer to the main form
void LiveBox::setFormAddress( MainForm * thisForm )
{
        mainForm = thisForm;
}


// Trigger a snapshot
void LiveBox::snapCB( void )
{
        if (mainForm)
                mainForm->snapRendering();
}


// Set a flag 
void LiveBox::autoSnapCB( void )
{
        if (mainForm) {
		viewParam * vp = mainForm->getViewParam();

		(*vp).autoSnap = this->getAutoSnap();
	}
}


// Start auto snap mode at start-up -- simply indicate this by 
// the button state
void LiveBox::startAutoSnap( void )
{
	this->autoSnapPb->setOn( TRUE );	
	
}

// Return the auto snap button toggle state
bool LiveBox::getAutoSnap( void )
{
        return autoSnapPb->isOn( );
}


// Give size hints and define size policy
QSizePolicy LiveBox::sizePolicy() const
{
	return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
}
