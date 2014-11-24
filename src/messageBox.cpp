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

#include <Q3GridLayout>

// Make a popup dialog box 
MessageBox::MessageBox( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Message" );

	// Insert a grid that will hold a label 
	// and a button
	const int numCols = 1;
        const int numRows = 2;
        Q3GridLayout * messageBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "messageBox" );

	// Create a hbox that will contain a label 
	Q3HBox * hb1 = new Q3HBox( this, "hb1" );
	hb1->setMargin( SPACE );
	messageBox->addWidget( hb1, 0, 0 );

	// Create a label
	messageL = new QLabel( hb1, "messageL" );

	// Create a hbox that will contain a button
	Q3HBox * hb2 = new Q3HBox( this, "hb2" );
	hb2->setMargin( SPACE );
	messageBox->addWidget( hb2, 1, 0 );

	// Create a placeholder 
	QLabel * emptyL1 = new QLabel( hb2, "emptyL1" );

	// Create a pushbutton that will go into the lowest row
	QPushButton * ok = new QPushButton( hb2, "ok" );
	ok->setText( "OK" ); 

	 // Define a callback for that button
        QObject::connect( ok, SIGNAL(clicked()), this, SLOT(ok()) );

	// Create a placeholder 
	QLabel * emptyL2 = new QLabel( hb2, "emptyL2" );

	hb2->setStretchFactor( emptyL1, 10 );
	hb2->setStretchFactor( emptyL2, 10 );
}


// Set the label text
void MessageBox::setText( char * mess )
{
	if (messageL) 
		messageL->setText( mess );
}


// Set the label text (overloaded function)
void MessageBox::setText( char * mess1, char * mess2 )
{
	char * buffer = (char *)malloc(BUFSIZ);
	sprintf(buffer, "%s %s", mess1, mess2);
	if (messageL) 
		messageL->setText( buffer );
	free(buffer);	
}


// Hide the board
void MessageBox::ok()
{
	// Hide now
        hide();
}
