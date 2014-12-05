//
// Implementation of slice board dialog widget class
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

#include "sliceBoard.h"

#include <cmath>

#include "mainForm.h"
#include "floatSpin.h"
#include "parameterLimits.h"

#include <q3grid.h>
#include <q3hbox.h>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>



// Make a popup dialog box 
SliceBoard::SliceBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Set Clipping" );

	// Insert a grid that will hold spin boxes
	// plus a row of control buttons
	const int numCols = 5;
        const int numRows = 5;
        Q3GridLayout * sliceBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "sliceBox" );

	// Create a check box
	sliceCb = new QCheckBox( this, "sliceCb" );
	sliceCb->setText( "Slicing On" );
	sliceBox->addWidget( sliceCb, 0, 0 );

	// Create a pushbutton to adjust to boundaries
	QPushButton * boundaryPb = new QPushButton( this, "boundaryPb" );
	boundaryPb->setText( "Set to Contour" ); 
	sliceBox->addWidget( boundaryPb, 0, 4 );

	 // Define a callback for that button
        QObject::connect( boundaryPb, SIGNAL(clicked()), this, SLOT(autoSlice()) );

	// Create spin boxes, labels and check boxes
        QLabel * sliceLX1 = new QLabel( this, "sliceXL1" );
        sliceLX1->setText( " Slice X From: ");
	sliceBox->addWidget( sliceLX1, 1, 0 );

    sliceSbX1 = new QFSpinBox( this );
	sliceSbX1->setMinValue( MINSLI );
	sliceSbX1->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbX1, 1, 1 );

        QLabel * sliceLX2 = new QLabel( this, "sliceXL2" );
        sliceLX2->setText( " To: ");
	sliceBox->addWidget( sliceLX2, 1, 2 );

    sliceSbX2 = new QFSpinBox( this );
	sliceSbX2->setMinValue( MINSLI );
	sliceSbX2->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbX2, 1, 3 );

        QLabel * sliceLY1 = new QLabel( this, "sliceYL1" );
        sliceLY1->setText( " Slice Y From: ");
	sliceBox->addWidget( sliceLY1, 2, 0 );

    sliceSbY1 = new QFSpinBox( this );
	sliceSbY1->setMinValue( MINSLI );
	sliceSbY1->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbY1, 2, 1 );

        QLabel * sliceLY2 = new QLabel( this, "sliceYL2" );
        sliceLY2->setText( " To: ");
	sliceBox->addWidget( sliceLY2, 2, 2 );

    sliceSbY2 = new QFSpinBox( this );
	sliceSbY2->setMinValue( MINSLI );
	sliceSbY2->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbY2, 2, 3 );

        QLabel * sliceLZ1 = new QLabel( this, "sliceZL1" );
        sliceLZ1->setText( " Slice Z From: ");
	sliceBox->addWidget( sliceLZ1, 3, 0 );

    sliceSbZ1 = new QFSpinBox( this );
	sliceSbZ1->setMinValue( MINSLI );
	sliceSbZ1->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbZ1, 3, 1 );

        QLabel * sliceLZ2 = new QLabel( this, "sliceZL2" );
        sliceLZ2->setText( " To: ");
	sliceBox->addWidget( sliceLZ2, 3, 2 );

    sliceSbZ2 = new QFSpinBox( this );
	sliceSbZ2->setMinValue( MINSLI );
	sliceSbZ2->setMaxValue( MAXSLI );
	sliceBox->addWidget( sliceSbZ2, 3, 3 );

	 // Define a callback for the spin boxes
        QObject::connect( sliceSbX1, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );
        QObject::connect( sliceSbX2, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );
        QObject::connect( sliceSbY1, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );
        QObject::connect( sliceSbY2, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );
        QObject::connect( sliceSbZ1, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );
        QObject::connect( sliceSbZ2, SIGNAL(valueChanged(int)), this, SLOT(sliceChanged()) );

	// Create check boxes
	indicateXCb = new QCheckBox( this, "indicateXSb" );
	indicateXCb->setText( "Indicate Only" );
	sliceBox->addWidget( indicateXCb, 1, 4 );

	indicateYCb = new QCheckBox( this, "indicateYSb" );
	indicateYCb->setText( "Indicate Only" );
	sliceBox->addWidget( indicateYCb, 2, 4 );

	indicateZCb = new QCheckBox( this, "indicateZSb" );
	indicateZCb->setText( "Indicate Only" );
	sliceBox->addWidget( indicateZCb, 3, 4 );

	 // Define a callback for these check boxes
        QObject::connect( indicateXCb, SIGNAL(clicked()), this, SLOT(sliceChangedX()) );
        QObject::connect( indicateYCb, SIGNAL(clicked()), this, SLOT(sliceChangedY()) );
        QObject::connect( indicateZCb, SIGNAL(clicked()), this, SLOT(sliceChangedZ()) );

	// Create a hboxlayout that will fill the lowest row
	Q3HBox * hb = new Q3HBox( this, "hb" );
	sliceBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);
	
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
void SliceBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current settings from the main form
void SliceBoard::setSlice( viewParam vp )
{
	// Update the controls
        sliceCb->setChecked( vp.slicing );

        indicateXCb->setChecked( vp.showSlicePlaneX );
        indicateYCb->setChecked( vp.showSlicePlaneY );
        indicateZCb->setChecked( vp.showSlicePlaneZ );

	sliceSbX1->setValue( (int)floor(10.0*vp.sliceXMin+0.5) );
	sliceSbX2->setValue( (int)floor(10.0*vp.sliceXMax+0.5) );
	sliceSbY1->setValue( (int)floor(10.0*vp.sliceYMin+0.5) );
	sliceSbY2->setValue( (int)floor(10.0*vp.sliceYMax+0.5) );
	sliceSbZ1->setValue( (int)floor(10.0*vp.sliceZMin+0.5) );
	sliceSbZ2->setValue( (int)floor(10.0*vp.sliceZMax+0.5) );
}


// Fill in slice values corresponding to viewing the entire set
void SliceBoard::autoSlice()
{
        // Get the current settings
        if (mainForm) {
                viewParam thisVp = (*mainForm->getViewParam());
                viewObject thisVo = (*mainForm->getViewObject());

		thisVp.sliceXMin = thisVo.xmin-SLICEEPS;
		thisVp.sliceXMax = thisVo.xmax+SLICEEPS;
		thisVp.sliceYMin = thisVo.ymin-SLICEEPS;
		thisVp.sliceYMax = thisVo.ymax+SLICEEPS;
		thisVp.sliceZMin = thisVo.zmin-SLICEEPS;
		thisVp.sliceZMax = thisVo.zmax+SLICEEPS;

		this->setSlice( thisVp );
	}
}


// Respond to changes of the boundary values
void SliceBoard::sliceChanged()
{
	// Always activate the slicing-on toggle when values change
        sliceCb->setChecked( TRUE );
}


// Respond to changes of the boundary values
void SliceBoard::sliceChangedX()
{
	// Always activate the slicing-on toggle when values change
        sliceCb->setChecked( TRUE );
}


// Respond to changes of the boundary values
void SliceBoard::sliceChangedY()
{
	// Always activate the slicing-on toggle when values change
        sliceCb->setChecked( TRUE );
}


// Respond to changes of the boundary values
void SliceBoard::sliceChangedZ()
{
	// Always activate the slicing-on toggle when values change
        sliceCb->setChecked( TRUE );
}


// Read the current clip settings
void SliceBoard::registerSettings( void )
{
        // Get the current settings
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).slicing = sliceCb->isChecked();

		(*vp).showSlicePlaneX = indicateXCb->isChecked();
		(*vp).showSlicePlaneY = indicateYCb->isChecked();
		(*vp).showSlicePlaneZ = indicateZCb->isChecked();

	        (*vp).sliceXMin = (float)sliceSbX1->value()/10.0;
	        (*vp).sliceXMax = (float)sliceSbX2->value()/10.0;
	        (*vp).sliceYMin = (float)sliceSbY1->value()/10.0;
	        (*vp).sliceYMax = (float)sliceSbY2->value()/10.0;
	        (*vp).sliceZMin = (float)sliceSbZ1->value()/10.0;
	        (*vp).sliceZMax = (float)sliceSbZ2->value()/10.0;

		(*vp).slicingSet = TRUE;
	}
}


// Accept the setting: update the rendering and hide the board
void SliceBoard::bdone()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm) {

		// Re-define the view object and re-draw
                mainForm->updateView();
                mainForm->updateRendering();
	}

	// Hide now
        hide();
}


// Accept the setting: update the rendering 
void SliceBoard::bapply()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm) {

		// Re-define the view object and re-draw
                mainForm->updateView();
                mainForm->updateRendering();
	}
}


// Cancel the setting: hide the board
void SliceBoard::bcancel()
{
	// Hide now
	hide();
}
