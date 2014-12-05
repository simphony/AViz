//
// Implementation of explicit view point settings board dialog widget class
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

#include "explicitBoard.h"

#include "parameterLimits.h"
#include "floatSpin.h"
#include "mainForm.h"

#include "./pixmaps/phi.xpm"
#include "./pixmaps/theta.xpm"
#include "./pixmaps/chi.xpm"
#include "./pixmaps/dolly.xpm"
#include "./pixmaps/fovy.xpm"

#include <QPixmap>
#include <Q3GridLayout>
#include <q3hbox.h>
#include <QLabel>
#include <QPushButton>

#include <cmath>

// Make a popup dialog box
ExplicitBoard::ExplicitBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Set Viewpoint" );

	// Insert a grid that will hold spin boxes
	// plus a row of control buttons
	const int numCols = 3;
        const int numRows = 6;
        Q3GridLayout * explicitBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "explicitBox" );

        // Create a label and a spin box - PHI
        phiL = new QLabel( this, "phiL" );
	QPixmap phiPm = QPixmap( phi );
        phiL->setPixmap(  phiPm );
	explicitBox->addWidget( phiL, 0, 0 );

    phiSb = new QFSpinBox( this );
	phiSb->setMinValue( (int)floor(10.0*PHI_MIN+0.5) );
	phiSb->setMaxValue( (int)floor(10.0*PHI_MAX+0.5) );
	phiSb->setWrapping( TRUE );
	explicitBox->addWidget( phiSb, 0, 1 );

	char strPhiRange[30];
	sprintf(strPhiRange, "Range: [%d, %d]", (int)PHI_MIN, (int)PHI_MAX);
	phiRangeL = new QLabel( this, "phiRangeL" );
	phiRangeL->setText(strPhiRange);
	explicitBox->addWidget( phiRangeL, 0, 2 );

        // Create a label and a spin box - THETA
        thetaL = new QLabel( this, "thetaL" );
	QPixmap thetaPm = QPixmap( theta );
        thetaL->setPixmap(  thetaPm );
	explicitBox->addWidget( thetaL, 1, 0 );

    thetaSb = new QFSpinBox( this );
	thetaSb->setMinValue( (int)floor(10.0*THETA_MIN+0.5) );
	thetaSb->setMaxValue( (int)floor(10.0*THETA_MAX+0.5) );
	thetaSb->setWrapping( TRUE );
	explicitBox->addWidget( thetaSb, 1, 1 );

	char strThetaRange[30];
	sprintf(strThetaRange, "Range: [%d, %d]", (int)THETA_MIN, (int)THETA_MAX);
	thetaRangeL = new QLabel( this, "thetaRangeL" );
	thetaRangeL->setText(strThetaRange);
	explicitBox->addWidget( thetaRangeL, 1, 2 );

        // Create a label and a spin box - CHI
        chiL = new QLabel( this, "chiL" );
	QPixmap chiPm = QPixmap( chi );
        chiL->setPixmap(  chiPm );
	explicitBox->addWidget( chiL, 2, 0 );

    chiSb = new QFSpinBox( this );
	chiSb->setMinValue( (int)floor(10.0*CHI_MIN+0.5) );
	chiSb->setMaxValue( (int)floor(10.0*CHI_MAX+0.5) );
	chiSb->setWrapping( TRUE );
	explicitBox->addWidget( chiSb, 2, 1 );

	char strChiRange[30];
	sprintf(strChiRange, "Range: [%d, %d]", (int)CHI_MIN, (int)CHI_MAX);
	chiRangeL = new QLabel( this, "chiRangeL" );
	chiRangeL->setText(strChiRange);
	explicitBox->addWidget( chiRangeL, 2, 2 );

        // Create a label and a spin box - DOLLY
        dollyL = new QLabel( this, "dollyL" );
	QPixmap dollyPm = QPixmap( dolly );
        dollyL->setPixmap(  dollyPm );
	explicitBox->addWidget( dollyL, 3, 0 );

    dollySb = new QFSpinBox( this );
	dollySb->setMinValue( (int)floor(10.0*DOLLY_MIN+0.5) );
	dollySb->setMaxValue( (int)floor(10.0*DOLLY_MAX+0.5) );
	explicitBox->addWidget( dollySb, 3, 1 );

	char strDollyRange[30];
	sprintf(strDollyRange, "Range: [%d, %d]", (int)DOLLY_MIN, (int)DOLLY_MAX);
	dollyRangeL = new QLabel( this, "dollyRangeL" );
	dollyRangeL->setText(strDollyRange);
	explicitBox->addWidget( dollyRangeL, 3, 2 );

        // Create a label and a spin box - FOVY
        fovyL = new QLabel( this, "fovyL" );
	QPixmap fovyPm = QPixmap( fovy );
        fovyL->setPixmap(  fovyPm );
	explicitBox->addWidget( fovyL, 4, 0 );

    fovySb = new QFSpinBox( this );
	fovySb->setMinValue( (int)floor(10.0*FOVY_MIN+0.5) );
	fovySb->setMaxValue( (int)floor(10.0*FOVY_MAX+0.5) );
	explicitBox->addWidget( fovySb, 4, 1 );

	char strFovyRange[30];
	sprintf(strFovyRange, "Range: [%d, %d]", (int)FOVY_MIN, (int)FOVY_MAX);
	fovyRangeL = new QLabel( this, "fovyRangeL" );
	fovyRangeL->setText(strFovyRange);
	explicitBox->addWidget( fovyRangeL, 4, 2 );

	// Define common callbacks
        QObject::connect( phiSb, SIGNAL(valueChanged(int)), this, SLOT(registerSettings()) );
        QObject::connect( thetaSb, SIGNAL(valueChanged(int)), this, SLOT(registerSettings()) );
        QObject::connect( chiSb, SIGNAL(valueChanged(int)), this, SLOT(registerSettings()) );
        QObject::connect( dollySb, SIGNAL(valueChanged(int)), this, SLOT(registerSettings()) );
        QObject::connect( fovySb, SIGNAL(valueChanged(int)), this, SLOT(registerSettings()) );

	// Create a hboxlayout that will fill the lowest row
	Q3HBox * hb = new Q3HBox( this, "hb" );
	explicitBox->addMultiCellWidget( hb, numRows-1, numRows-1, 0, -1);

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
void ExplicitBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current settings from the main form
void ExplicitBoard::setExplicit( viewParam vp )
{
	while (vp.phi > PHI_MAX) {
		vp.phi -= 360.0;
	}
	while (vp.phi < PHI_MIN) {
		vp.phi += 360.0;
	}
	phiSb->setValue( (int)floor(10.0*vp.phi+0.5) );

	while (vp.theta > THETA_MAX) {
		vp.theta -= 360.0;
	}
	while (vp.theta < THETA_MIN) {
		vp.theta += 360.0;
	}
	thetaSb->setValue( (int)floor(10.0*vp.theta+0.5) );

	while (vp.chi > CHI_MAX) {
		vp.chi -= 360.0;
	}
	while (vp.chi < CHI_MIN) {
		vp.chi += 360.0;
	}
	chiSb->setValue( (int)floor(10.0*vp.chi+0.5) );

	dollySb->setValue( (int)floor(10.0*vp.dolly+0.5) );

	while (vp.fovy > FOVY_MAX) {
		vp.fovy -= 360.0;
	}
	while (vp.fovy < FOVY_MIN) {
		vp.fovy += 360.0;
	}
	fovySb->setValue( (int)floor(10.0*vp.fovy+0.5) );
}


// Read the current clip settings
void ExplicitBoard::registerSettings()
{
        // Get the current settings
        if (mainForm) {
                viewParam * vp = mainForm->getViewParam( );

		(*vp).phi = (float)phiSb->value()/10.0;
		(*vp).theta = (float)thetaSb->value()/10.0;
		(*vp).chi = (float)chiSb->value()/10.0;
		(*vp).dolly = (float)dollySb->value()/10.0;
		(*vp).fovy = ((float)fovySb->value()/10.0 > 0.001)? (float)fovySb->value()/10.0 : 0.001; // Avoid a zero fovy
	}
}


// Accept the setting: update the rendering and hide the board
void ExplicitBoard::bdone()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm)
                mainForm->updateRendering();

	// Hide now
        hide();
}


// Accept the setting: update the rendering
void ExplicitBoard::bapply()
{
        this->registerSettings();

        // Re-do the graphics, using the new view settings
        if (mainForm)
                mainForm->updateRendering();
}


// Cancel the setting: hide the board
void ExplicitBoard::bcancel()
{
	// Hide now
	hide();
}
