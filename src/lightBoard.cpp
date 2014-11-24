//
// Implementation of light board dialog widget class
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

#include "lightBoard.h"

// Make a popup dialog box that will hold a vertical row of buttons
LightsBoard::LightsBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Set Lights" );

	// Insert a grid that will hold sliders, labels and 
	// check boxes, plus a row of control buttons
	const int numCols = 2;
        const int numRows = 4;
        Q3GridLayout * lightsBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "lightsBox" );

	// Create check boxes in the first row
	Q3HBox * hb0 = new Q3HBox( this, "hb0" );
	lightsBox->addMultiCellWidget( hb0, 0, 0, 0, -1);

	QLabel * spotL0 = new QLabel( hb0, "spotL0" );
	spotL0->setText( " Spotlights:  ");	

	// Create a vbox and a grid to hold the checkboxes
	Q3VBox * vb0 = new Q3VBox( hb0, "vb0" );
	
	Q3Grid * gr0 = new Q3Grid( 4, vb0, "gr0" );
        gr0->setSpacing( SPACE );
        gr0->setMargin( SPACE );

        light0 = new QCheckBox( gr0, "light0" );
        light0->setText( "Left" );
        light0->setChecked( FALSE );

        light1 = new QCheckBox( gr0, "light1" );
        light1->setText( "Right" );
        light1->setChecked( FALSE );

	light2 = new QCheckBox( gr0, "light2" );
	light2->setText( "Top" );
	light2->setChecked( FALSE );

	light3 = new QCheckBox( gr0, "light3" );
        light3->setText( "Bottom" );
	light3->setChecked( FALSE );

	light4 = new QCheckBox( gr0, "light4" );
	light4->setText( "Top Left" );
	light4->setChecked( FALSE );

	light5 = new QCheckBox( gr0, "light5" );
	light5->setText( "Top Right" );
	light5->setChecked( FALSE );

	light6 = new QCheckBox( gr0, "light6" );
	light6->setText( "Bottom Left" );
	light6->setChecked( FALSE );

	light7 = new QCheckBox( gr0, "light7" );
	light7->setText( "Bottom Right" );
	light7->setChecked( FALSE );

	// Create another hbox below the checkboxes
	Q3HBox * hb1 = new Q3HBox( vb0, "hb1" );

	// Create a combo box to set light position depth
	lightCob = new QComboBox( FALSE, hb1, "lightSelection" );	
	lightCob->clear();
	lightCob->insertItem( "Left" );
	lightCob->insertItem( "Right" );
	lightCob->insertItem( "Top" );
	lightCob->insertItem( "Bottom" );
	lightCob->insertItem( "Top Left" );
	lightCob->insertItem( "Top Right" );
	lightCob->insertItem( "Bottom Left" );
	lightCob->insertItem( "Bottom Right" );

	// Define a callback for this combo box
        connect( lightCob, SIGNAL(activated(int)), SLOT(setLightDepth(int)) );

	// Create a horizontal slider and some labels 
	depthL0 = new QLabel( hb1, "depthL0" );
	depthL0->setText( " Position Depth:  Near  ");	
	depthS = new QSlider( Qt::Horizontal, hb1, "depthS" );
	depthS->setMinValue( 0 );
	depthS->setMaxValue( 100 );
	depthS->setTickInterval( 10 );
        depthS->setTickmarks( QSlider::TicksAbove );
        depthS->setFixedHeight( SLIDER_HEIGHT );
	depthL1 = new QLabel( hb1, "depthL1" );
	depthL1->setText( "  Far  ");	
	
	// Create a vertical box to hold two sliders and labels
	Q3Grid * gr1 = new Q3Grid( 2, this, "gr1" );
	gr1->setSpacing( SPACE );
	gr1->setMargin( SPACE );
	gr1->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
	lightsBox->addMultiCellWidget( gr1, 2, 2, 0, -1);

	// Create a large label and slider that will go into the 
	// top column
	QLabel * ambientL = new QLabel( gr1, "ambientL" );
	ambientL->setText( " Global Ambient Light:  ");	

	ambientS = new QSlider( Qt::Horizontal, gr1, "ambientS" );
	ambientS->setMinValue( LIGHT_MIN );
	ambientS->setMaxValue( LIGHT_MAX );
	ambientS->setTickInterval( 10 );
        ambientS->setTickmarks( QSlider::TicksAbove );
        ambientS->setFixedHeight( SLIDER_HEIGHT );

	// Create a large label and slider that will go into the 
	// bottom column
	QLabel * shininessL = new QLabel( gr1, "shininessL" );
	shininessL->setText( " Surface Shininess:  ");	

	shininessS = new QSlider( Qt::Horizontal, gr1, "shininessS" );
	shininessS->setMinValue( SHINE_MIN );
	shininessS->setMaxValue( SHINE_MAX );
	shininessS->setTickInterval( 10 );
        shininessS->setTickmarks( QSlider::TicksAbove );
        shininessS->setFixedHeight( SLIDER_HEIGHT );

	Q3HBox * hb2 = new Q3HBox( this, "hb2" );
	lightsBox->addMultiCellWidget( hb2, numRows-1, numRows-1, 0, -1);
	
	// Create a placeholder 
	QLabel * emptyL = new QLabel( hb2, "emptyL" );

	// Create pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( hb2, "done" );
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

	QPushButton * apply = new QPushButton( hb2, "apply" );
	apply->setText( "Apply" ); 

	 // Define a callback for that button
        QObject::connect( apply, SIGNAL(clicked()), this, SLOT(bapply()) );

	QPushButton * cancel = new QPushButton( hb2, "cancel" );
	cancel->setText( "Cancel" ); 

	 // Define a callback for that button
        QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(bcancel()) );

	hb2->setStretchFactor( emptyL, 10 );
}


// Set a pointer to the main form
void LightsBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current view settings from main form, register
// it using a local copy, and update the settings
void LightsBoard::setLights( viewParam vp )
{
	light0->setChecked(vp.light[0] );
	light1->setChecked(vp.light[1] );
	light2->setChecked(vp.light[2] );
	light3->setChecked(vp.light[3] );
	light4->setChecked(vp.light[4] );
	light5->setChecked(vp.light[5] );
	light6->setChecked(vp.light[6] );
	light7->setChecked(vp.light[7] );

	ambientS->setValue( (int)(100.0*vp.ambientLight+0.5) );

	shininessS->setValue( (int)(vp.shininess+0.5) );

	// Display the depth of the first ligth
	lightCob->setCurrentItem(0);
	this->setLightDepth(0);
}


// Read the current settings
void LightsBoard::registerSettings( void )
{
	// Get the current settings 
	if (mainForm) {
		viewParam * vp = mainForm->getViewParam( );

		// Update the values
		(*vp).light[0] = light0->isChecked();
		(*vp).light[1] = light1->isChecked();
		(*vp).light[2] = light2->isChecked();
		(*vp).light[3] = light3->isChecked();
		(*vp).light[4] = light4->isChecked();
		(*vp).light[5] = light5->isChecked();
		(*vp).light[6] = light6->isChecked();
		(*vp).light[7] = light7->isChecked();
	
		// Set the depth position as well	
		int thisLight = lightCob->currentItem();
		if (thisLight >= 0 && thisLight < LIGHTS_MAX) {
			(*vp).lightDepth[thisLight] = (float)depthS->value()/100.0;
		}

		(*vp).ambientLight = (float)ambientS->value()/100.0;

		(*vp).shininess = (float)shininessS->value();
	}
}


// Update the setting of the depth slider
void LightsBoard::setLightDepth( int thisLight )
{
	// Get the current settings 
	if (mainForm) {
		viewParam * vp = mainForm->getViewParam( );

		// Update the values
		if (thisLight >= 0 && thisLight < LIGHTS_MAX)
			depthS->setValue( (int)floor(100.0*(*vp).lightDepth[thisLight] + 0.5) );
	}
}


// Complete the lights setting
void LightsBoard::bdone()
{
	this->registerSettings();

	// Re-do the graphics, using the new view settings
	if (mainForm) 
		mainForm->updateRendering();

	// Hide now
        hide();
}


// Accept the lights setting
void LightsBoard::bapply()
{
	this->registerSettings();

	// Re-do the graphics, using the new view settings
	if (mainForm) 
		mainForm->updateRendering();
}


// Cancel the lights setting: hide the board
void LightsBoard::bcancel()
{
	// Hide now
	hide();
}
