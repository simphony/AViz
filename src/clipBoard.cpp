//
// Implementation of clip board dialog widget class
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

#include "clipBoard.h"

#include "floatSpin.h"
#include "mainForm.h"
#include "parameterLimits.h" // for MAXCLIP

#include <QCheckBox>
#include <q3hbox.h>
#include <QPushButton>

#include <Q3GridLayout>
#include <QLabel>

#include <cmath>

// Make a popup dialog box 
ClipBoard::ClipBoard(QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel )
{
	setWindowTitle( "AViz: Set Clipping" );

	// Insert a grid that will hold spin boxes
	// plus a row of control buttons
	const int numCols = 3;
        const int numRows = 3;
        Q3GridLayout * clipBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "clipBox" );

        // Create a label and a spin box
        clipNearL = new QLabel( this, "clipNearL" );
        clipNearL->setText( " Near Clip Dist: ");
	clipBox->addWidget( clipNearL, 0, 0 );

    clipNearSb = new QFSpinBox( this );
	clipNearSb->setMaximum( MAXCLIP );
	clipBox->addWidget( clipNearSb, 0, 1 );

        // Create a label and a spin box
        clipFarL = new QLabel( this, "clipFarL" );
        clipFarL->setText( " Far Clip Dist: ");
	clipBox->addWidget( clipFarL, 1, 0 );

    clipFarSb = new QFSpinBox( this );
	clipFarSb->setMaximum( MAXCLIP );
	clipBox->addWidget( clipFarSb, 1, 1 );

	// Create check boxes to control auto clipping
	autoNearCb = new QCheckBox( this, "autoNearCb" );
	autoNearCb->setText( " Auto Clip " );
	clipBox->addWidget( autoNearCb, 0, 2 );

	autoFarCb = new QCheckBox( this, "autoFarCb" );
	autoFarCb->setText( " Auto Clip " );
	clipBox->addWidget( autoFarCb, 1, 2 );

	// Define callbacks for these checkboxes
        QObject::connect( autoNearCb, SIGNAL(clicked()), this, SLOT(autoClip()) );
        QObject::connect( autoFarCb, SIGNAL(clicked()), this, SLOT(autoClip()) );

	// Create a hboxlayout that will fill the lowest row
	Q3HBox * hb = new Q3HBox( this, "hb" );
	clipBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);
	
	// Create a placeholder 
	QLabel * emptyL = new QLabel( hb, "emptyL" );

	// Create pushbuttons that will go into the lowest row
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
}


// Set a pointer to the main form
void ClipBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current settings from the main form
void ClipBoard::setClip( viewParam vp )
{
	// Update the controls
	autoNearCb->setChecked( vp.autoClipNear );
	autoFarCb->setChecked( vp.autoClipFar );
	
	clipNearSb->setValue( (int)floor(10.0*vp.clipNear+0.5) );
	clipFarSb->setValue( (int)floor(10.0*vp.clipFar+0.5) );

	// Activate or disactivate the controls
	this->autoClip();
}


// Callback function to respond to auto clipping
void ClipBoard::autoClip()
{
	switch( autoNearCb->isChecked() ) {
		case TRUE:
			clipNearL->setDisabled( TRUE );
			clipNearSb->setDisabled( TRUE );
		break;
		case FALSE:
			clipNearL->setDisabled( FALSE );
			clipNearSb->setDisabled( FALSE );
		break;
	}

	switch( autoFarCb->isChecked() ) {
		case TRUE:
			clipFarL->setDisabled( TRUE );
			clipFarSb->setDisabled( TRUE );
		break;
		case FALSE:
			clipFarL->setDisabled( FALSE );
			clipFarSb->setDisabled( FALSE );
		break;
	}
}


// Read the current clip settings
void ClipBoard::registerSettings()
{
        // Get the current settings
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).clipNear = (float)clipNearSb->value()/10.0;
		(*vp).clipFar = (float)clipFarSb->value()/10.0;

		(*vp).autoClipNear = autoNearCb->isChecked();
		(*vp).autoClipFar = autoFarCb->isChecked();
	}
}


// Accept the setting: update the rendering and hide the board
void ClipBoard::bdone()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm) 
                mainForm->updateRendering();

	// Hide now
        hide();
}


// Accept the setting: update the rendering 
void ClipBoard::bapply()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm) 
                mainForm->updateRendering();
}


// Cancel the setting: hide the board
void ClipBoard::bcancel()
{
	// Hide now
	hide();
}
