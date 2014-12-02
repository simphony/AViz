//
// Implementation of annotation board dialog widget class
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

#include "annotationBoard.h"

// Make a popup dialog box that will hold a vertical row of buttons
AnnotationBoard::AnnotationBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, WType_TopLevel )
{
	this->setCaption( "AViz: Set Annotation" );

	// Insert a grid that will hold sliders and text windoes,
	// plus a row of control buttons
	const int numCols = 7;
        const int numRows = 3;
        QGridLayout * annotBox = new QGridLayout( this, numCols, numRows, SPACE, SPACE, "annotBox" );
	
	// Create a label
	QLabel * textL = new QLabel( this, "textL" );
	textL->setText( "Annotation:" ); 
	annotBox->addWidget( textL, 0, 0);

	// Create a text window
	annotationTextLe = new QLineEdit( this, "annotationText" );
	annotBox->addMultiCellWidget( annotationTextLe, 0, 0, 1, numCols-3);

	// Create a check box
	showAnnotationCb = new QCheckBox( this, "showAnnotation" );
	showAnnotationCb->setText( "Show Annotation" );
	annotBox->addMultiCellWidget( showAnnotationCb, 0, 0, numCols-2, -1);

	// Create three sliders and labels
	QLabel * sizeL = new QLabel( this, "sizeL" );
	sizeL->setText( "Size:" ); 
	annotBox->addWidget( sizeL, 1, 1);

	annotationSizeSb = new QSpinBox( this, "annotationSizeSl" );
	annotationSizeSb->setMinValue( 1 );
	annotationSizeSb->setMaxValue( 4 );
	annotationSizeSb->setLineStep( 1 );
	annotBox->addWidget( annotationSizeSb, 1, 2);

	QLabel * xL = new QLabel( this, "xL" );
	xL->setText( "X Position:" ); 
	annotBox->addWidget( xL, 1, 3);

	annotationXSb = new QSpinBox( this, "xSl" );
	annotationXSb->setMinValue( 0 );
	annotationXSb->setMaxValue( 1000 );
	annotationXSb->setLineStep( 10 );
	annotBox->addWidget( annotationXSb, 1, 4);

	QLabel * yL = new QLabel( this, "yL" );
	yL->setText( "Y Position:" ); 
	annotBox->addWidget( yL, 1, 5);

	annotationYSb = new QSpinBox( this, "ySl" );
	annotationYSb->setMinValue( 0 );
	annotationYSb->setMaxValue( 1000 );
	annotationYSb->setLineStep( 10 );
	annotBox->addWidget( annotationYSb, 1, 6);

	QHBox * hb = new QHBox( this, "hb" );
	annotBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);
	
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
void AnnotationBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current view settings from main form, register
// it using a local copy, and update the settings
void AnnotationBoard::setAnnotation( viewParam vp )
{
	annotationTextLe->setText( vp.annotationText );
	showAnnotationCb->setChecked( vp.showAnnotation );
	annotationSizeSb->setValue( vp.annotationSize );
	annotationXSb->setValue( vp.annotationCoordX );
	annotationYSb->setValue( vp.annotationCoordY );
}


// Register the current settings
void AnnotationBoard::registerSettings( void )
{
	// Get the current settings 
	if (mainForm) {
		viewParam * vp = mainForm->getViewParam( );

		// Update the values
		QString text = annotationTextLe->text();
		sprintf( (*vp).annotationText, "%s", (const char *)text );
		(*vp).showAnnotation = showAnnotationCb->isChecked();
		(*vp).annotationSize = annotationSizeSb->value();
		(*vp).annotationCoordX = annotationXSb->value();
		(*vp).annotationCoordY = annotationYSb->value();
	}
}


// Complete the annotation setting
void AnnotationBoard::bdone()
{
	this->registerSettings();

	// Re-do the graphics, using the new view settings
	if (mainForm) {
		mainForm->updateRendering();
	}

	// Hide now
        hide();
}


// Accept the annotation setting
void AnnotationBoard::bapply()
{
	this->registerSettings();

	// Re-do the graphics, using the new view settings
	if (mainForm) {
		mainForm->updateRendering();
	}
}


// Cancel the annotation setting: hide the board
void AnnotationBoard::bcancel()
{
	// Hide now
	hide();
}
