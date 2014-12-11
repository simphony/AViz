//
// Implementation of stretch board dialog widget class
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

#include "stretchBoard.h"

#include <cmath>

#include <Q3GridLayout>
#include <q3hbox.h>
#include <QPushButton>
#include <QLabel>

#include "floatSpin.h"
#include "mainForm.h"
#include "parameterLimits.h"

// Make a popup dialog box 
StretchBoard::StretchBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	setWindowTitle( "AViz: Set Data Stretching" );

	// Insert a grid that will hold control buttons
	const int numCols = 3;
        const int numRows = 4;
        Q3GridLayout * stretchBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "stretchBox" );

	// Create labels and spin boxes
	QLabel * labelx = new QLabel( this, "labelx" );
	labelx->setText( " Stretch X: ");
	stretchBox->addWidget( labelx, 0, 0);
    stretchSBX = new QFSpinBox( this );
	stretchSBX->setMaximum( MAXSTRETCH );
	stretchSBX->setMinimum( MINSTRETCH );
	stretchBox->addWidget( stretchSBX, 0, 1);

	QLabel * labely = new QLabel( this, "labely" );
	labely->setText( " Stretch Y: ");
	stretchBox->addWidget( labely, 1, 0);
    stretchSBY = new QFSpinBox( this );
	stretchSBY->setMaximum( MAXSTRETCH );
	stretchSBY->setMinimum( MINSTRETCH );
	stretchBox->addWidget( stretchSBY, 1, 1);

    QLabel * labelz = new QLabel( this, "labelz" );
	labelz->setText( " Stretch Z: ");
	stretchBox->addWidget( labelz, 2, 0);
    stretchSBZ = new QFSpinBox( this );
	stretchSBZ->setMaximum( MAXSTRETCH );
	stretchSBZ->setMinimum( MINSTRETCH );
	stretchBox->addWidget( stretchSBZ, 2, 1);

	// Define a callback for the spin boxes
        QObject::connect( stretchSBX, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );
        QObject::connect( stretchSBY, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );
        QObject::connect( stretchSBZ, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );

	// Create pushbuttons
	QPushButton * resetx = new QPushButton( this, "resetx" );
	resetx->setText( "Reset Stretch X" );
	stretchBox->addWidget( resetx, 0, 2);
	QPushButton * resety = new QPushButton( this, "resety" );
	resety->setText( "Reset Stretch Y" );
	stretchBox->addWidget( resety, 1, 2);
	QPushButton * resetz = new QPushButton( this, "resetz" );
	resetz->setText( "Reset Stretch Z" );
	stretchBox->addWidget( resetz, 2, 2);

	// Define a callback for the pushbuttons
        connect( resetx, SIGNAL(clicked()), SLOT(doResetX()) );
        connect( resety, SIGNAL(clicked()), SLOT(doResetY()) );
        connect( resetz, SIGNAL(clicked()), SLOT(doResetZ()) );

	// Create a hboxlayout that will fill the lowest row
	Q3HBox * hb = new Q3HBox( this, "hb" );
	stretchBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);
	
	// Create a placeholder 
	QLabel * emptyL = new QLabel( hb, "emptyL" );

	// Create more pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( hb, "done" );
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

	QPushButton * apply = new QPushButton( hb, "apply" );
	apply->setText( "Apply" ); 

	 // Define a callback for that button
        QObject::connect( apply, SIGNAL(clicked()), this, SLOT(bapply()) );

	QPushButton * cancel = new QPushButton( hb, "cancel" );
	cancel->setText( "Cancel" ); 

	 // Define a callback for that button
        QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(bcancel()) );

	hb->setStretchFactor( emptyL, 10 );

	// Reset a flag
	stretchChange = FALSE;
}


// Set a pointer to the main form
void StretchBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Receive the current view parameters
void StretchBoard::setData( viewParam vp )
{
	stretchSBX->setValue( (int)floor(10.0*vp.stretchX+0.5) ); 
	stretchSBY->setValue( (int)floor(10.0*vp.stretchY+0.5) ); 
	stretchSBZ->setValue( (int)floor(10.0*vp.stretchZ+0.5) ); 
}


// Set a flag if the stretching parameters change so that
// the view object must be re-defined
void StretchBoard::stretchChanged()
{
	stretchChange = TRUE;
}


// Read the current settings
void StretchBoard::registerSettings( void )
{
        // Get the current settings
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		// Update the values
		(*vp).stretchX = (float)stretchSBX->value()/10.0;
		(*vp).stretchY = (float)stretchSBY->value()/10.0;
		(*vp).stretchZ = (float)stretchSBZ->value()/10.0;
	}
}


// Reset the translations
void StretchBoard::doResetX()
{
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).stretchX = 1.0;
		stretchChange = TRUE;
		this->setData( (*vp) );
	}
}


// Reset the translations
void StretchBoard::doResetY()
{
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).stretchY = 1.0;
		stretchChange = TRUE;
		this->setData( (*vp) );
	}
}


// Reset the translations
void StretchBoard::doResetZ()
{
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).stretchZ = 1.0;
		stretchChange = TRUE;
		this->setData( (*vp) );
	}
}


// Complete the setting: save the view parameter and hide the board
void StretchBoard::bdone()
{
	registerSettings();

	// Re-do the graphics, using the new parameters
	if (mainForm) {
		if (stretchChange) {
			// Re-define the view object and re-draw
			mainForm->updateView();
			mainForm->updateRendering();

			// Reset flag
			stretchChange = FALSE;
		}
		else {
			// Merely re-draw the slice planes
			mainForm->updateRendering();
		}
	}

	// Hide now
        hide();
}


// Accept the setting: save the view parameter
void StretchBoard::bapply()
{
	registerSettings();

	// Re-do the graphics, using the new parameters
	if (mainForm) {
		if (stretchChange) {
			// Re-define the view object and re-draw
			mainForm->updateView();
			mainForm->updateRendering();

			// Reset flag
			stretchChange = FALSE;
		}
		else {
			// Merely re-draw the slice planes
			mainForm->updateRendering();
		}
	}
}


// Cancel the setting: hide the board
void StretchBoard::bcancel()
{
	// Hide now
	hide();
}
