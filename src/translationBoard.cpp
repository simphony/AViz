//
// Implementation of translation board dialog widget class
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

#include "translationBoard.h"
#include "floatSpin.h"
#include "mainForm.h"

#include <Q3GridLayout>
#include <QLabel>
#include <QPushButton>

#include <math.h>

TranslationBoard::TranslationBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Set Data Translation" );

	// Insert a grid that will hold control buttons
	const int numCols = 4;
        const int numRows = 4;
        Q3GridLayout * transBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "transBox" );

	// Create labels and spin boxes
	QLabel * labelx = new QLabel( this, "labelx" );
	labelx->setText( " Translate X: ");
	transBox->addWidget( labelx, 0, 0);
    panSBX = new QFSpinBox( this );
	panSBX->setMaxValue( MAXPAN );
	panSBX->setMinValue( MINPAN );
	transBox->addWidget( panSBX, 0, 1);

	QLabel * labely = new QLabel( this, "labely" );
	labely->setText( " Translate Y: ");
	transBox->addWidget( labely, 1, 0);
    panSBY = new QFSpinBox( this );
	panSBY->setMaxValue( MAXPAN );
	panSBY->setMinValue( MINPAN );
	transBox->addWidget( panSBY, 1, 1);

	QLabel * labelz = new QLabel( this, "labelz" );
	labelz->setText( " Translate Z: ");
	transBox->addWidget( labelz, 2, 0);
    panSBZ = new QFSpinBox( this );
	panSBZ->setMaxValue( MAXPAN );
	panSBZ->setMinValue( MINPAN );
	transBox->addWidget( panSBZ, 2, 1);

	// Create labels and spin boxes
	QLabel * labelright = new QLabel( this, "labelright" );
	labelright->setText( " Translate Right: ");
	transBox->addWidget( labelright, 0, 2);
    panSBRight = new QFSpinBox( this );
	panSBRight->setMaxValue( MAXPAN );
	panSBRight->setMinValue( MINPAN );
	transBox->addWidget( panSBRight, 0, 3);

	QLabel * labeltop = new QLabel( this, "labeltop" );
	labeltop->setText( " Translate Top: ");
	transBox->addWidget( labeltop, 1, 2);
    panSBTop = new QFSpinBox( this );
	panSBTop->setMaxValue( MAXPAN );
	panSBTop->setMinValue( MINPAN );
	transBox->addWidget( panSBTop, 1, 3);

	QLabel * labelforward = new QLabel( this, "labelforward" );
	labelforward->setText( " Translate Forward: ");
	transBox->addWidget( labelforward, 2, 2);
    panSBForward = new QFSpinBox( this );
	panSBForward->setMaxValue( MAXPAN );
	panSBForward->setMinValue( MINPAN );
	transBox->addWidget( panSBForward, 2, 3);

	// Create a hboxlayout that will fill the lowest row
	Q3HBox * hb = new Q3HBox( this, "hb" );
	transBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);
	
	// Create a pushbutton
	QPushButton * reset = new QPushButton( hb, "reset" );
	reset->setText( "Reset Translation" );

	// Create a placeholder 
	QLabel * emptyL = new QLabel( hb, "emptyL" );

	// Define a callback for this pushbutton
        connect( reset, SIGNAL(clicked()), SLOT(doReset()) );

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
}


// Set a pointer to the main form
void TranslationBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Receive the current view parameters
void TranslationBoard::setData( viewParam vp )
{
	panSBX->setValue( (int)floor(10.0*vp.panX+0.5) ); 
	panSBY->setValue( (int)floor(10.0*vp.panY+0.5) ); 
	panSBZ->setValue( (int)floor(10.0*vp.panZ+0.5) ); 
	panSBRight->setValue( (int)floor(10.0*vp.panRight+0.5) ); 
	panSBTop->setValue( (int)floor(10.0*vp.panTop+0.5) ); 
	panSBForward->setValue( (int)floor(10.0*vp.panForward+0.5) ); 
}


// Read the current settings
void TranslationBoard::registerSettings( void )
{
        // Get the current settings
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		// Update the values
		(*vp).panX = (float)panSBX->value()/10.0;
		(*vp).panY = (float)panSBY->value()/10.0;
		(*vp).panZ = (float)panSBZ->value()/10.0;
		(*vp).panRight = (float)panSBRight->value()/10.0;
		(*vp).panTop = (float)panSBTop->value()/10.0;
		(*vp).panForward = (float)panSBForward->value()/10.0;
	}
}


// Reset the translations
void TranslationBoard::doReset( void )
{
	viewParam thisVp;

	thisVp.panX = thisVp.panY = thisVp.panZ = 0.0;
	thisVp.panRight = thisVp.panTop = thisVp.panForward = 0.0;

	this->setData( thisVp );
}


// Complete the setting: save the view parameter and hide the board
void TranslationBoard::bdone()
{
	this->registerSettings();

	// Re-do the graphics, using the new parameters
	if (mainForm) 
		mainForm->updateRendering();

	// Hide now
        hide();
}


// Accept the setting: save the view parameter
void TranslationBoard::bapply()
{
	this->registerSettings();

	// Re-do the graphics, using the new parameters
	if (mainForm) 
		mainForm->updateRendering();
}


// Cancel the setting: hide the board
void TranslationBoard::bcancel()
{
	// Hide now
	hide();
}
