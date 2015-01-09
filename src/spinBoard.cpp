//
// Implementation of spin board dialog widget class
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

#include "spinBoard.h"

#include "mainForm.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "sizeBox.h"
#include "lineTypeBox.h"
#include "codeBox.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "fileFunctions.h"
#include "defaultParticles.h" // typeCopy, typeCmp

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>

// Make a popup dialog box 
SpinBoard::SpinBoard(QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel)
{
    setWindowTitle( "AViz: Set Spins" );

    // Create a hboxlayout that will fill the first row
    hb1 = new Q3HBox( this, "hb1" );

    // Create a label
    QLabel * spinL = new QLabel( hb1, "spinL" );
    spinL->setText( " Type: ");

    // Create a combo box that will go into the
    // second column; entries in the combo box will
    // be made later
    spinCob = new QComboBox(hb1);

    // Define a callback for this combo box
    connect( spinCob, SIGNAL(activated(int)), SLOT(setSpin()) );

    // Create a placeholder
    QLabel * emptyL0 = new QLabel( hb1, "emptyL0" );
    emptyL0->setText("");

    // Add a check box button
    showSpinCb = new QCheckBox( hb1, "showSpin" );
    showSpinCb->setText( "Show Spins" );
    showSpinCb->setChecked( TRUE );

    // Define a callback for this toggle switch
    connect( showSpinCb, SIGNAL(clicked()), this, SLOT(adjustSpin()) );

    // Create a hboxlayout that will fill the next row
    hb2 = new Q3HBox( this, "hb2" );

    // Add a label and color labels
    colorL = new QLabel(" Color: ", hb2);
    colorLabel0 = new ColorLabel(hb2);
    colorLabel1 = new ColorLabel(hb2);
    colorLabel2 = new ColorLabel(hb2);
    colorLabel3 = new ColorLabel(hb2);
    colorLabel4 = new ColorLabel(hb2);
    colorLabel5 = new ColorLabel(hb2);
    colorLabel0->setFixedHeight( LABEL_HEIGHT );
    colorLabel1->setFixedHeight( LABEL_HEIGHT );
    colorLabel2->setFixedHeight( LABEL_HEIGHT );
    colorLabel3->setFixedHeight( LABEL_HEIGHT );
    colorLabel4->setFixedHeight( LABEL_HEIGHT );
    colorLabel5->setFixedHeight( LABEL_HEIGHT );

    // Add a push button
    colorButton = new QPushButton( hb2, "colorButton" );
    colorButton->setText( "Set Color..." );

    // Define a callback for this button
    QObject::connect( colorButton, SIGNAL(clicked()), this, SLOT(setColorCb(
                                                                     )) );

    // Create a hboxlayout that will fill the next row
    sizeBox = new SizeBox(this);

    // Create a hboxlayout that will fill the next row
    hb4 = new Q3HBox( this, "hb4" );

    // Add radiobuttons and a label
    modeL = new QLabel( hb4, "modeL" );
    modeL->setText( " Color Criterion: " );

    colorMode = new Q3ButtonGroup( 4, Qt::Horizontal, hb4, "colorMode" );
    colorMode0 = new QRadioButton( colorMode, "type" );
    colorMode0->setText( "Type" );
    colorMode1 = new QRadioButton( colorMode, "position" );
    colorMode1->setText( "Position" );
    colorMode2 = new QRadioButton( colorMode, "property" );
    colorMode2->setText( "Property" );
    colorMode3 = new QRadioButton( colorMode, "colorcode" );
    colorMode3->setText( "ColorCode" );
    colorMode->insert( colorMode0, 0 );
    colorMode->insert( colorMode1, 1 );
    colorMode->insert( colorMode2, 2 );
    colorMode->insert( colorMode3, 3 );

    // Define a callback for these radio buttons
    connect( colorMode, SIGNAL(clicked(int)), this, SLOT(adjustCriterion()) );

    // Create hboxlayouts that will fill the next row; these
    // are shown only when appropriate
    positionBox = new PositionBox( this, "positionBox" );
    propertyBox = new PropertyBox( this, "positionBox" );
    codeBox = new CodeBox(this);

    // Create a box that will fill the next row
    lineTypeBox = new LineTypeBox( this );

    // Create a hboxlayout that will fill the lowest row
    hb5 = new Q3HBox( this, "hb5" );

    // Create a placeholder
    QLabel * emptyL1 = new QLabel( hb5, "emptyL1" );

    // Create pushbuttons that will go into the lowest row
    QPushButton * done = new QPushButton( hb5, "done" );
    done->setText( "Done" );

    // Define a callback for that button
    QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

    QPushButton * apply = new QPushButton( hb5, "apply" );
    apply->setText( "Apply" );

    // Define a callback for that button
    QObject::connect( apply, SIGNAL(clicked()), this, SLOT(bapply()) );

    QPushButton * cancel = new QPushButton( hb5, "cancel" );
    cancel->setText( "Cancel" );

    // Define a callback for that button
    QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(bcancel()) );

    hb5->setStretchFactor( emptyL1, 10 );

    // Clear the board pointer
    spinBox = NULL;

    // Clear the color board pointer
    cb = NULL;

    // Clear internal variables
    colors = 1;
    colorPosX = colorPosY = 0;
    showColorBoard = FALSE;
    spinRenderStyle = SLINE;
    renderQuality = LOW;

    // Set defaults
    lineTypeBox->setDisabled( TRUE );

    // Set defaults appropriate for startup without data
    colorButton->setDisabled( TRUE );
    sizeBox->setDisabled( TRUE );
    modeL->setDisabled( TRUE );
    colorMode->setDisabled( TRUE );
    colorMode0->setChecked( TRUE );

    // Build default layout
    this->buildLayout( TYPE );
}


// Set a pointer to the main form
void SpinBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Build the layout
void SpinBoard::buildLayout( colorCriterion crit )
{
        int numCols = 6;
        int numRows = 7;

	// Adjust number of rows
	if (crit == TYPE) 
		numRows = 6;

        // Destroy existing layout
        if (spinBox) {
                spinBox->~Q3GridLayout();
        }

        // Insert a grid that will hold control buttons
        spinBox = new Q3GridLayout( this, numRows, numCols, SPACE, SPACE, "spinBox" );

        // Add components that are always needed
        spinBox->addMultiCellWidget( hb1, 0, 0, 0, -1);
	spinBox->addMultiCellWidget( hb2, 1, 1, 0, -1);
        spinBox->addMultiCellWidget( sizeBox, 2, 2, 0, -1);
        spinBox->addMultiCellWidget( hb4, 3, 3, 0, -1);
        spinBox->addMultiCellWidget( lineTypeBox, numRows-2, numRows-2, 0, -1);
        spinBox->addMultiCellWidget( hb5, numRows-1, numRows-1, 0, -1);

        // Add additional components
        switch (crit) {
                case TYPE:
                        positionBox->hide();
                        propertyBox->hide();
                        codeBox->hide();
                break;
                case POSITION:
                        positionBox->show();
                        propertyBox->hide();
                        codeBox->hide();
                        spinBox->addMultiCellWidget( positionBox, 4, 4, 0, -1);
                break;
                case PROPERTY:
                        positionBox->hide();
                        propertyBox->show();
                        codeBox->hide();
                        spinBox->addMultiCellWidget( propertyBox, 4, 4, 0, -1);
                break;
                case COLORCODE:
                        positionBox->hide();
                        propertyBox->hide();
                        codeBox->show();
                        spinBox->addMultiCellWidget( codeBox, 4, 4, 0, -1);
                break;
        }

        this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void SpinBoard::setData( void )
{
        aggregateData * ad = NULL;
	tag tmp;

        if (mainForm) {
                // Get the current switch settings and register
                // it using a local particle data structure
                thisPd = mainForm->getParticleData();

                // Get a list of particles that are currently rendered
                ad = mainForm->getAggregateData();

	        // Make entries in the combo box -- use only particle
	        // types that are really needed; otherwise the list
	        // gets too long
                if (spinCob) {
                        spinCob->clear();
                        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                                // Check: is this particle type really needed?
                                bool needed = FALSE;
                                for (int j=0;j<(*ad).numberOfParticles;j++) {
                                        if (typeCmp( (char *)&(*ad).particles[j].type, (char *)&(*thisPd).type[i]) == TRUE) {
                                                // Yes it is needed: set a flag
                                                needed = TRUE;
                                                break;
                                        }
                                }

                                // Add the item to the list
                                if (needed) {
                                        spinCob->addItem( QString( (char *)&(*thisPd).type[i]));
                                }
                        }
                        spinCob->setMinimumSize( spinCob->sizeHint() );
                }
	}

        // Sort the entries alphabetically, at least approximately
        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                for (int j=0;j<spinCob->count()-1;j++) {
                        if (QString::compare(spinCob->itemText(j), spinCob->itemText(j+1)) > 0) {
                                typeCopy( qPrintable(spinCob->itemText(j+1)), (char *)&tmp );
                                spinCob->insertItem(j, QString( (char *)&tmp));
                                spinCob->removeItem(j+2);
                        }
                }
        }

        // Set the first spin as the current spin
        if ((*ad).numberOfParticles > 0) {
                thisSpinIndex = 0;
                this->setSpin();
        }
        else {
                thisSpinIndex = -1;
        }
}


// Set the spin under consideration: adjust the toggle switch
void SpinBoard::setSpin( void )
{
        tag thisSpin;
        thisSpinIndex = -1;

        // Find out what combination of particles is set
        typeCopy( qPrintable(spinCob->currentText()), (char *)&thisSpin );

        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisSpin ) == TRUE) {
                        // Found the correct entry
                        thisSpinIndex = i;

			// Adjust the color board
			this->setColors();

			// Adjust the toggles
			bool thisShowParticle = (*thisPd).showParticle[thisSpinIndex];
			showSpinCb->setChecked( thisShowParticle);

			switch( thisShowParticle) {
                                case TRUE:
                                        if (thisSpinIndex >= 0) {
                                                modeL->setDisabled( FALSE );
                                                colorMode->setDisabled( FALSE );
                                        }
                                        else {
                                                modeL->setDisabled( TRUE );
                                                colorMode->setDisabled( TRUE );
                                        }
                                break;
                                case FALSE:
                                        sizeBox->setDisabled( TRUE );
                                        modeL->setDisabled( TRUE );
                                        colorMode->setDisabled( TRUE );
                                break;
                        }

			// Adjust the toggles
                        if (spinRenderStyle != SDOT && spinRenderStyle != SLINE && thisShowParticle) {
                                sizeBox->setDisabled( FALSE );
                        }
                        else {
                                sizeBox->setDisabled( TRUE );
                        }

			// Adjust size settings
			sizeBox->setParticle( thisPd, thisSpinIndex );

			colorCriterion colorCrit = (*thisPd).colorCrit[thisSpinIndex];
			switch (colorCrit) {
				case TYPE:
					this->buildLayout( TYPE );
					colorMode0->setChecked( TRUE );
					colorButton->setDisabled( FALSE );
				break;
				case POSITION:
					this->buildLayout( POSITION );
					colorMode1->setChecked( TRUE );
					colorButton->setDisabled( FALSE );
				break;
				case PROPERTY:
					this->buildLayout( PROPERTY );
					colorMode2->setChecked( TRUE );
					colorButton->setDisabled( FALSE );
				break;
				case COLORCODE:
					this->buildLayout( COLORCODE );
					colorMode3->setChecked( TRUE );
					colorButton->setDisabled( TRUE );
					colorLabel0->switchOff();
					colorLabel1->switchOff();
					colorLabel2->switchOff();
					colorLabel3->switchOff();
					colorLabel4->switchOff();
					colorLabel5->switchOff();
				break;
			}
	
			positionBox->setParticle( thisPd, thisSpinIndex );
			propertyBox->setParticle( thisPd, thisSpinIndex );
			codeBox->setParticle( thisPd, thisSpinIndex );
			lineTypeBox->setParticle( thisPd, thisSpinIndex );
		}
	}
}


// Adjust the spin switch settings, using the local
// copy of the particle data structure 
void SpinBoard::adjustSpin( void )
{
	// Read the toggle switch values and
	// update the particle data entry
	(*thisPd).showParticle[thisSpinIndex] = showSpinCb->isChecked();

	// Activate or deactivate the remaining radio buttons
	switch ((*thisPd).showParticle[thisSpinIndex]) {
		case TRUE:
			if (thisSpinIndex >= 0) {	
				this->buildLayout( (*thisPd).colorCrit[thisSpinIndex] );
			}
			else {
				this->buildLayout( TYPE );
				colorButton->setDisabled( FALSE );
			}
		break;
		case FALSE:
			this->buildLayout( TYPE );
			colorButton->setDisabled( FALSE );
		break;
	}
}


// Adjust the spin switch settings, using the local
// copy of the particle data structure
void SpinBoard::adjustCriterion( void )
{
	this->readToggles();
	this->setSpin();
}


// Read the toggle switches and the emission text window
void SpinBoard::readToggles( void )
{
	if (thisSpinIndex >= 0) {
	        // Read the toggle switch values and
		// update the particle data entry
		sizeBox->readToggles( thisPd, thisSpinIndex );

                if (colorMode0->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisSpinIndex] = TYPE;
                }
                if (colorMode1->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisSpinIndex] = POSITION;
                }
                if (colorMode2->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisSpinIndex] = PROPERTY;
                }
                if (colorMode3->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisSpinIndex] = COLORCODE;
                }
        }
	
	if (thisSpinIndex >= 0) {
		positionBox->readToggles( thisPd, thisSpinIndex );
		propertyBox->readToggles( thisPd, thisSpinIndex );
		codeBox->readToggles( thisPd, thisSpinIndex );
		lineTypeBox->readToggles( thisPd, thisSpinIndex );
		// Spins have always only one type color: set it 
		// manually (it's a hack...)
		(*thisPd).line[thisSpinIndex].typeColorNumber = 1;
	}
}


// Launch the color board to adjust colors
void SpinBoard::setColorCb()
{
        // Set a flag
        showColorBoard = TRUE;

        // Adjust the color board
        this->setColors();
}


// Adjust the colors of the color board
void SpinBoard::setColors()
{
	float red, green, blue;
        float redMin, redMax;
        float greenMin, greenMax;
        float blueMin, blueMax;
        red = green = blue = 0.0;
	redMin = redMax = 0.0;
	greenMin = greenMax = 0.0;
	blueMin = blueMax = 0.0;

	if (thisSpinIndex > 0) {
	        // Get the current particle data and show the
		// color panel
		switch ((*thisPd).colorCrit[thisSpinIndex]) {
			case TYPE:
				// Only one color is needed
				red = (*thisPd).particleColors[thisSpinIndex].red;
				green = (*thisPd).particleColors[thisSpinIndex].green;
				blue = (*thisPd).particleColors[thisSpinIndex].blue;
				colorLabel0->setColor( red, green, blue );
				colorLabel1->setColor( red, green, blue );
				colorLabel2->setColor( red, green, blue );
				colorLabel3->setColor( red, green, blue );
				colorLabel4->setColor( red, green, blue );
				colorLabel5->setColor( red, green, blue );

		                // Destroy the color board and re-build if
		                // the settings are not right
		                if (colors != 1 || !cb) {
		                        colors = 1;
		
		                        if (cb) {
		                                colorPosX = cb->pos().x();
		                                colorPosY = cb->pos().y();
		                                cb->~ColorBoard();
		                        }

                                cb = new ColorBoard(this);
		
		                        if (cb) {
		                                // This is an ugly hack, intended to
		                                // propagate the address of the calling class
                                        cb->setSpinBoardAddress(this);
		                                cb->buildLayout( colors );
		                        }
		                }

		                // Position the color board correctly
		                if (colorPosX > 0 && colorPosY > 0) {
		                        cb->setGeometry( colorPosX, colorPosY, cb->width(), cb->height() );
		                }
		
		                if (cb  && showColorBoard)
		                        cb->show();

		                // Do the color settings now
		                if (cb) {
		                        cb->setColor( red, green, blue );
		                        cb->setLabel( "Type Color" );
		                }
			break;
			case COLORCODE: 
				// No coloring is specified
				colorLabel0->switchOff();
				colorLabel1->switchOff();
				colorLabel2->switchOff();
				colorLabel3->switchOff();
				colorLabel4->switchOff();
				colorLabel5->switchOff();
			break;
			default:
				// A spectrum of colors is needed
				redMin = (*thisPd).particleColors[thisSpinIndex].redMin;
				greenMin = (*thisPd).particleColors[thisSpinIndex].greenMin;
				blueMin = (*thisPd).particleColors[thisSpinIndex].blueMin;
				redMax = (*thisPd).particleColors[thisSpinIndex].redMax;
				greenMax = (*thisPd).particleColors[thisSpinIndex].greenMax;
		       		blueMax = (*thisPd).particleColors[thisSpinIndex].blueMax;

				colorLabel0->setColor( redMin + 0.0*(redMax-redMin), greenMin + 0.0*(greenMax-greenMin), blueMin + 0.0*(blueMax-blueMin) );
				colorLabel1->setColor( redMin + 0.2*(redMax-redMin), greenMin + 0.2*(greenMax-greenMin), blueMin + 0.2*(blueMax-blueMin) );
				colorLabel2->setColor( redMin + 0.4*(redMax-redMin), greenMin + 0.6*(greenMax-greenMin), blueMin + 0.4*(blueMax-blueMin) );
				colorLabel3->setColor( redMin + 0.6*(redMax-redMin), greenMin + 0.4*(greenMax-greenMin), blueMin + 0.6*(blueMax-blueMin) );
				colorLabel4->setColor( redMin + 0.8*(redMax-redMin), greenMin + 0.8*(greenMax-greenMin), blueMin + 0.8*(blueMax-blueMin) );
				colorLabel5->setColor( redMin + 1.0*(redMax-redMin), greenMin + 1.0*(greenMax-greenMin), blueMin + 1.0*(blueMax-blueMin) );
	
				// Destroy the color board and re-build if
		                // the settings are not right
		                if (colors != 2 || !cb) {
		                        colors = 2;
		
		                        if (cb) {
		                                colorPosX = cb->pos().x();
		                                colorPosY = cb->pos().y();
		                                cb->~ColorBoard();
		                        }
		
                                cb = new ColorBoard(this);
		
		                        if (cb) {
		                                // This is an ugly hack, intended to
		                                // propagate the address of the calling class
                                        cb->setSpinBoardAddress(this);
		                                cb->buildLayout( colors );
		                        }
		                }
		
		                // Position the color board correctly
		                if (colorPosX > 0 && colorPosY > 0) {
		                        cb->setGeometry( colorPosX, colorPosY, cb->width(), cb->height() );
		                }

		                if (cb  && showColorBoard)
		                        cb->show();
		
		                // Do the color settings now
		                if (cb) {
		                        cb->setColor( redMin, greenMin, blueMin, redMax, greenMax, blueMax );
		                        cb->setLabel( "Min Ramp Color", "Max Ramp Color" );
		                }
			break;
		}
	}
}


// Receive colors from the color board
void SpinBoard::getColors( float r0, float g0, float b0, float r1, float g1, float b1 )
{
        if ((*thisPd).colorCrit[thisSpinIndex] == TYPE) {
		// Only one color are sent
		(*thisPd).particleColors[thisSpinIndex].red = r0;
		(*thisPd).particleColors[thisSpinIndex].green = g0;
		(*thisPd).particleColors[thisSpinIndex].blue = b0;
		colorLabel0->setColor( r0, g0, b0 );
		colorLabel1->setColor( r0, g0, b0 );
		colorLabel2->setColor( r0, g0, b0 );
		colorLabel3->setColor( r0, g0, b0 );
		colorLabel4->setColor( r0, g0, b0 );
		colorLabel5->setColor( r0, g0, b0 );
        }
	else {
		// A spectrum of colors is sent
		(*thisPd).particleColors[thisSpinIndex].redMin = r0;
		(*thisPd).particleColors[thisSpinIndex].greenMin = g0;
		(*thisPd).particleColors[thisSpinIndex].blueMin = b0;
		(*thisPd).particleColors[thisSpinIndex].redMax = r1;
		(*thisPd).particleColors[thisSpinIndex].greenMax = g1;
		(*thisPd).particleColors[thisSpinIndex].blueMax = b1;
		colorLabel0->setColor( r0 + 0.0*(r1-r0), g0 + 0.0*(g1-g0), b0 + 0.0*(b1-b0) );
		colorLabel1->setColor( r0 + 0.2*(r1-r0), g0 + 0.2*(g1-g0), b0 + 0.2*(b1-b0) );
		colorLabel2->setColor( r0 + 0.4*(r1-r0), g0 + 0.4*(g1-g0), b0 + 0.4*(b1-b0) );
		colorLabel3->setColor( r0 + 0.6*(r1-r0), g0 + 0.6*(g1-g0), b0 + 0.6*(b1-b0) );
		colorLabel4->setColor( r0 + 0.8*(r1-r0), g0 + 0.8*(g1-g0), b0 + 0.8*(b1-b0) );
		colorLabel5->setColor( r0 + 1.0*(r1-r0), g0 + 1.0*(g1-g0), b0 + 1.0*(b1-b0) );
	}

	this->bapply();
}

// Adjust the board
void SpinBoard::setDotStyle( void )
{
	spinRenderStyle = SDOT;
        sizeBox->setDisabled( TRUE );
}


// Adjust the board
void SpinBoard::setLineStyle( void )
{
	spinRenderStyle = SLINE;
        sizeBox->setDisabled( TRUE );
	lineTypeBox->setDisabled( FALSE, TRUE );
}


// Adjust the board
void SpinBoard::setCubeStyle( void )
{
	spinRenderStyle = SCUBE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void SpinBoard::setCylinderStyle( void )
{
	spinRenderStyle = SCYLINDER;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void SpinBoard::setConeStyle( void )
{
	spinRenderStyle = SCONE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void SpinBoard::setSphereStyle( void )
{
	spinRenderStyle = SSPHERE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void SpinBoard::setLowQuality( void )
{
	renderQuality = LOW;
}


// Adjust the board
void SpinBoard::setHighQuality( void )
{
	renderQuality = HIGH;
}


// Adjust the board
void SpinBoard::setFinalQuality( void )
{
	renderQuality = FINAL;
}


// Set a flag when the color board is closed
void SpinBoard::closeColorBoard( void )
{
        showColorBoard = FALSE;

        if (cb)
                cb->~ColorBoard();

        cb = NULL;
        colors = 0;
}


// Remember the board position when the color board is closed
void SpinBoard::getColorBoardPos( int posX, int posY )
{
        colorPosX = posX;
        colorPosY = posY;
}


// Complete the spin switch setting: save the particle data and hide the board
void SpinBoard::bdone()
{
	// Read the toggle switches
	this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved spin data in ", filename );
	}
	free(filename);

	// Re-do the graphics, using the new particle data
	if (mainForm) {
		mainForm->updateView();
		mainForm->updateRendering();
	}

	// Hide now
        hide();
}


// Accept the spin switch setting: save the particle data
void SpinBoard::bapply()
{
	// Read the toggle switches
	this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved spin data in ", filename );
	}
	free(filename);

	// Re-do the graphics, using the new particle data
	if (mainForm) {
		mainForm->updateView();
		mainForm->updateRendering();
	}
}


// Cancel the spin switch setting: hide the board
void SpinBoard::bcancel()
{
	// Hide now
	hide();
}
