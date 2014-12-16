//
// Implementation of polymer board dialog widget class (like atom board class)
//

/**********************************************************************
Copyright (C) 2001  Geri Wagner

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

#include "polymerBoard.h"

#include <cmath>

#include <Q3GridLayout>
#include <QLabel>

// Make a popup dialog box 
PolymerBoard::PolymerBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	setWindowTitle( "AViz: Set Polymer Atom Types" );

	// Create a hboxlayout that will fill the first row
	hb1 = new Q3HBox( this, "hb1" );

	// Create a label 
	QLabel * atomL = new QLabel( hb1, "atomL" );
	atomL->setText( " Type: ");

	// Create a combo box that will go into the
	// second column; entries in the combo box will
	// be made later
	atomCob = new QComboBox( FALSE, hb1, "atomSelection" );

	// Define a callback for this combo box
	connect( atomCob, SIGNAL(activated(int)), SLOT(setPolymerAtom()) );

	// Create a placeholder 
	QLabel * emptyL0 = new QLabel( hb1, "emptyL0" );
	emptyL0->setText("");

        // Add a check box button
        showPolymerAtomCb = new QCheckBox( hb1, "showAtom" );
	showPolymerAtomCb->setText( "Show Polymer Atoms" );
	showPolymerAtomCb->setChecked( TRUE );

	// Define a callback for this toggle switch
	connect( showPolymerAtomCb, SIGNAL(clicked()), this, SLOT(adjustPolymer()) );

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
	colorLabel0->setColor( 1.0, 1.0, 1.0);
	colorLabel1->setColor( 1.0, 1.0, 1.0);
	colorLabel2->setColor( 1.0, 1.0, 1.0);
	colorLabel3->setColor( 1.0, 1.0, 1.0);
	colorLabel4->setColor( 1.0, 1.0, 1.0);
	colorLabel5->setColor( 1.0, 1.0, 1.0);

	// Add a push button
        colorButton = new QPushButton( hb2, "colorButton" );
        colorButton->setText( "Set Color..." );

	// Define a callback for this button
        QObject::connect( colorButton, SIGNAL(clicked()), this, SLOT(setColorCb()) );

	// Create a hboxlayout that will fill the next row
	sizeBox = new SizeBox( this, "sizeBox" );

	// Create a hboxlayout that will fill the next row
        hb4 = new Q3HBox( this, "hb4" );

        // Add radiobuttons and a label
        modeL = new QLabel( hb4, "modeL" );
        modeL->setText( " Color Criterion: " );

        colorMode = new Q3ButtonGroup( 4, Qt::Horizontal, hb4, "colorMode"
 );
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
	codeBox = new CodeBox( this, "codeBox" );

	// Create a hboxlayout that will fill the lowest row
	hb5 = new Q3HBox( this, "hb5" );
	
	// Create pushbuttons that will go into the lowest row
	QPushButton * bonds = new QPushButton( hb5, "bonds" );
	bonds->setText( "Bonds..." ); 

	 // Define a callback for that button
        QObject::connect( bonds, SIGNAL(clicked()), this, SLOT(bbonds()) );

	// Create a placeholder 
	QLabel * emptyL1 = new QLabel( hb5, "emptyL1" );

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
	polymerBox = NULL;

	// Clear the color board pointer
	cb = NULL;

	// Clear internal variables
	colors = 1;
	colorPosX = colorPosY = 0;
	showColorBoard = FALSE;
	polymerAtomRenderStyle = PADOT;
        renderQuality = LOW;

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
void PolymerBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Build the layout
void PolymerBoard::buildLayout( colorCriterion crit ) 
{
	int numCols = 6;
        int numRows = 6;

	// Adjust the number of rows
	if (crit == TYPE) 
		numRows = 5;
	
	// Destroy existing layout
	if (polymerBox) {
		delete polymerBox;
	}

	// Adjust height of dialog board
	this->setFixedHeight( numRows*ROW_HEIGHT );

	// Insert a grid that will hold control buttons
        polymerBox = new Q3GridLayout( this, numRows, numCols, SPACE, SPACE, "polymerBox" );

	// Add components that are always needed
	polymerBox->addMultiCellWidget( hb1, 0, 0, 0, -1);
	polymerBox->addMultiCellWidget( hb2, 1, 1, 0, -1);
	polymerBox->addMultiCellWidget( sizeBox, 2, 2, 0, -1);
	polymerBox->addMultiCellWidget( hb4, 3, 3, 0, -1);
	polymerBox->addMultiCellWidget( hb5, numRows-1, numRows-1, 0, -1);

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
			polymerBox->addMultiCellWidget( positionBox, 4, 4, 0, -1);
		break;
		case PROPERTY:
			positionBox->hide();
			propertyBox->show();
			codeBox->hide();
			polymerBox->addMultiCellWidget( propertyBox, 4, 4, 0, -1);
		break;
		case COLORCODE:
			positionBox->hide();
			propertyBox->hide();
			codeBox->show();
			polymerBox->addMultiCellWidget( codeBox, 4, 4, 0, -1);
		break;
	}

	this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void PolymerBoard::setData( void )
{
        aggregateData * ad = NULL;
	tag tmp;

        if (mainForm) {
                // Get the current settings and register
                // it using a local particle data structure
                thisPd = mainForm->getParticleData();

                // Get a list of particles that are currently rendered
                ad = mainForm->getAggregateData();

	        // Make entries in the combo box -- use only particle
	        // types that are really needed; otherwise the list
	        // gets too long
                int item = 0;
                if (atomCob) {
                        atomCob->clear();
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
                                        atomCob->insertItem( QString( (char *)&(*thisPd).type[i]), item );
                                        item++;

                                }
                        }
                        atomCob->setMinimumSize( atomCob->sizeHint() );
                }
	}

        // Sort the entries alphabetically, at least approximately
        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                for (int j=0;j<atomCob->count()-1;j++) {
                        if (QString::compare(atomCob->text(j), atomCob->text(j+1)) > 0) {
                                typeCopy( (const char *)atomCob->text(j+1), (char *)&tmp );
                                atomCob->insertItem( QString( (char *)&tmp), j);
                                atomCob->removeItem(j+2);
                        }
                }
        }

        // Set the first particle type as the current type 
        if ((*ad).numberOfParticles > 0) {
                thisPolymerAtomIndex = 0;
                this->setPolymerAtom();
        }
        else {
                thisPolymerAtomIndex = -1;
        }
}


// Set the atom type under consideration: adjust the toggle switch
void PolymerBoard::setPolymerAtom( void )
{
        tag thisAtom;
        thisPolymerAtomIndex = -1;

        // Find out what combination of atoms is set
        typeCopy( (const char *)atomCob->currentText(), (char *)&thisAtom);

        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisAtom) == TRUE) {
                        // Found the correct entry
                        thisPolymerAtomIndex = i;

			// Adjust the color board
			this->setColors();

			// Adjust the toggles
			bool thisShowParticle = (*thisPd).showParticle[thisPolymerAtomIndex];
			showPolymerAtomCb->setChecked( thisShowParticle );

			switch( thisShowParticle) {
				case TRUE:
					if (thisPolymerAtomIndex >= 0) {
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
			if (polymerAtomRenderStyle != PADOT && polymerAtomRenderStyle != PALINE && thisShowParticle) {
				sizeBox->setDisabled( FALSE );
		        }
		        else {
		                sizeBox->setDisabled( TRUE );
		        }

			// Adjust size settings
			sizeBox->setParticle( thisPd, thisPolymerAtomIndex);

			colorCriterion colorCrit = (*thisPd).colorCrit[thisPolymerAtomIndex];
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
	
			positionBox->setParticle( thisPd, thisPolymerAtomIndex);
			propertyBox->setParticle( thisPd, thisPolymerAtomIndex);
			codeBox->setParticle( thisPd, thisPolymerAtomIndex);
		}
	}
}


// Adjust the settings, using the local
// copy of the particle data structure 
void PolymerBoard::adjustPolymer( void )
{
	// Read the toggle switch values and
	// update the particle data entry
	if (thisPolymerAtomIndex >= 0)
		(*thisPd).showParticle[thisPolymerAtomIndex] = showPolymerAtomCb->isChecked();

	switch ( showPolymerAtomCb->isChecked() ) {
		case TRUE:	
			if (thisPolymerAtomIndex >= 0) {
				this->buildLayout( (*thisPd).colorCrit[thisPolymerAtomIndex] );
			}
			else {
				this->buildLayout( TYPE );
				colorButton->setDisabled( FALSE );
			}
		break;
		case FALSE:
			this->buildLayout( TYPE );
			colorButton->setDisabled( TRUE );
		break;
	}
}



// Adjust the board after change of color criterion
void PolymerBoard::adjustCriterion( void )
{
	this->readToggles();
	this->setPolymerAtom();
}


// Read the toggle switches
void PolymerBoard::readToggles( void )
{
	if (thisPolymerAtomIndex >= 0) {
                // Read the settings and
                // update the particle data entry
		sizeBox->readToggles( thisPd, thisPolymerAtomIndex);

                if (colorMode0->isChecked() == TRUE) {
			(*thisPd).colorCrit[thisPolymerAtomIndex] = TYPE;
		}
                if (colorMode1->isChecked() == TRUE) {
			(*thisPd).colorCrit[thisPolymerAtomIndex] = POSITION;
		}
                if (colorMode2->isChecked() == TRUE) {
			(*thisPd).colorCrit[thisPolymerAtomIndex] = PROPERTY;
		}
                if (colorMode3->isChecked() == TRUE) {
			(*thisPd).colorCrit[thisPolymerAtomIndex] = COLORCODE;
		}
        }

	if (thisPolymerAtomIndex >= 0) {
		positionBox->readToggles( thisPd, thisPolymerAtomIndex );
		propertyBox->readToggles( thisPd, thisPolymerAtomIndex );
		codeBox->readToggles( thisPd, thisPolymerAtomIndex );
	}
}


// Launch the color board to adjust colors
void PolymerBoard::setColorCb()
{
	// Set a flag
	showColorBoard = TRUE;

	// Adjust the color board
	this->setColors();
}


// Adjust the colors of the color board
void PolymerBoard::setColors()
{
	float red, green, blue;
	float redMin, redMax;
	float greenMin, greenMax;
	float blueMin, blueMax;
	red = green = blue = 0.0;
	redMin = redMax = 0.0;
	greenMin = greenMax = 0.0;
	blueMin = blueMax = 0.0;

	if (thisPolymerAtomIndex >= 0) {
		// Get the current particle data and show the 
		// color panel
		switch ((*thisPd).colorCrit[thisPolymerAtomIndex]) {
			case TYPE:
				// Only one color is needed
				red = (*thisPd).particleColors[thisPolymerAtomIndex].red;
				green = (*thisPd).particleColors[thisPolymerAtomIndex].green;
				blue = (*thisPd).particleColors[thisPolymerAtomIndex].blue;
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
	
					cb = new ColorBoard( this, "colorBoard" );
	
					if (cb) {
						// This is an ugly hack, intended to 
						// propagate the address of the calling class
						cb->setPolymerBoardAddress( (char *)this );
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
				redMin = (*thisPd).particleColors[thisPolymerAtomIndex].redMin;
				greenMin = (*thisPd).particleColors[thisPolymerAtomIndex].greenMin;
				blueMin = (*thisPd).particleColors[thisPolymerAtomIndex].blueMin;
				redMax = (*thisPd).particleColors[thisPolymerAtomIndex].redMax;
				greenMax = (*thisPd).particleColors[thisPolymerAtomIndex].greenMax;
				blueMax = (*thisPd).particleColors[thisPolymerAtomIndex].blueMax;
				colorLabel0->setColor( redMin + 0.0*(redMax-redMin), greenMin + 0.0*(greenMax-greenMin), blueMin + 0.0*(blueMax-blueMin) );
				colorLabel1->setColor( redMin + 0.2*(redMax-redMin), greenMin + 0.2*(greenMax-greenMin), blueMin + 0.2*(blueMax-blueMin) );
				colorLabel2->setColor( redMin + 0.4*(redMax-redMin), greenMin + 0.4*(greenMax-greenMin), blueMin + 0.4*(blueMax-blueMin) );
				colorLabel3->setColor( redMin + 0.6*(redMax-redMin), greenMin + 0.6*(greenMax-greenMin), blueMin + 0.6*(blueMax-blueMin) );
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
	
					cb = new ColorBoard( this, "colorBoard" );
		
					if (cb) {
						// This is an ugly hack, intended to 
						// propagate the address of the calling class
						cb->setPolymerBoardAddress( (char *)this );
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
void PolymerBoard::getColors( float r0, float g0, float b0, float r1, float g1, float b1 )
{
	if (thisPolymerAtomIndex >= 0) {
		if ((*thisPd).colorCrit[thisPolymerAtomIndex] == TYPE) {
			// Only one color are sent
				(*thisPd).particleColors[thisPolymerAtomIndex].red = r0;
			(*thisPd).particleColors[thisPolymerAtomIndex].green = g0;
			(*thisPd).particleColors[thisPolymerAtomIndex].blue = b0;
			colorLabel0->setColor( r0, g0, b0 );
			colorLabel1->setColor( r0, g0, b0 );
			colorLabel2->setColor( r0, g0, b0 );
			colorLabel3->setColor( r0, g0, b0 );
			colorLabel4->setColor( r0, g0, b0 );
			colorLabel5->setColor( r0, g0, b0 );
		}
		else {
			// A spectrum of colors is sent
			(*thisPd).particleColors[thisPolymerAtomIndex].redMin = r0;
			(*thisPd).particleColors[thisPolymerAtomIndex].greenMin = g0;
			(*thisPd).particleColors[thisPolymerAtomIndex].blueMin = b0;
			(*thisPd).particleColors[thisPolymerAtomIndex].redMax = r1;
			(*thisPd).particleColors[thisPolymerAtomIndex].greenMax = g1; 
			(*thisPd).particleColors[thisPolymerAtomIndex].blueMax = b1; 
			colorLabel0->setColor( r0 + 0.0*(r1-r0), g0 + 0.0*(g1-g0), b0 + 0.0*(b1-b0) );
			colorLabel1->setColor( r0 + 0.2*(r1-r0), g0 + 0.2*(g1-g0), b0 + 0.2*(b1-b0) );
			colorLabel2->setColor( r0 + 0.4*(r1-r0), g0 + 0.4*(g1-g0), b0 + 0.4*(b1-b0) );
			colorLabel3->setColor( r0 + 0.6*(r1-r0), g0 + 0.6*(g1-g0), b0 + 0.6*(b1-b0) );
			colorLabel4->setColor( r0 + 0.8*(r1-r0), g0 + 0.8*(g1-g0), b0 + 0.8*(b1-b0) );
			colorLabel5->setColor( r0 + 1.0*(r1-r0), g0 + 1.0*(g1-g0), b0 + 1.0*(b1-b0) );
		}

		this->bapply();
	}
}


// Adjust the board
void PolymerBoard::setDotStyle( void )
{	
	polymerAtomRenderStyle = PADOT;
	sizeBox->setDisabled( TRUE );
}


// Adjust the board
void PolymerBoard::setLineStyle( void )
{	
	polymerAtomRenderStyle = PALINE;
	sizeBox->setDisabled( TRUE );
}


// Adjust the board
void PolymerBoard::setCubeStyle( void )
{	
	polymerAtomRenderStyle = PACUBE;
	sizeBox->setDisabled( FALSE );
}


// Adjust the board
void PolymerBoard::setCylinderStyle( void )
{	
	polymerAtomRenderStyle = PACYLINDER;
	sizeBox->setDisabled( FALSE );
}


// Adjust the board
void PolymerBoard::setConeStyle( void )
{	
	polymerAtomRenderStyle = PACONE;
	sizeBox->setDisabled( FALSE );
}


// Adjust the board
void PolymerBoard::setSphereStyle( void )
{	
	polymerAtomRenderStyle = PASPHERE;
	sizeBox->setDisabled( FALSE );
}


// Adjust the board
void PolymerBoard::setLowQuality( void )
{
	renderQuality = LOW;
}


// Adjust the board
void PolymerBoard::setHighQuality( void )
{
	renderQuality = HIGH;
}


// Adjust the board
void PolymerBoard::setFinalQuality( void )
{
	renderQuality = FINAL;
}


// Set a flag when the color board is closed
void PolymerBoard::closeColorBoard( void )
{
	showColorBoard = FALSE;

	if (cb)
		cb->~ColorBoard();

	cb = NULL;
	colors = 0;
}


// Remember the board position when the color board is closed
void PolymerBoard::getColorBoardPos( int posX, int posY )
{
	colorPosX = posX;
	colorPosY = posY;
}


// Launch the bond board
void PolymerBoard::bbonds()
{
	if (mainForm)
		mainForm->launchBonds();
}


// Complete the particle switch setting: save the particle data and hide 
// the board
void PolymerBoard::bdone()
{
        // Read the toggle switches
        this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved particle data in ", filename );
	}
	free(filename);

	// Re-do the graphics, using the new particle data
	if (mainForm) {
		mainForm->updateView();
		mainForm->updateRendering();
	}

	// Set a flag
	this->closeColorBoard();

	// Hide now
        hide();
}


// Accept the particle switch setting: save the particle data
void PolymerBoard::bapply()
{
        // Read the toggle switches
        this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved particle data in ", filename );
	}
	free(filename);

	// Re-do the graphics, using the new particle data
	if (mainForm) {
		mainForm->updateView();
		mainForm->updateRendering();
	}
}


// Cancel the setting: hide the board
void PolymerBoard::bcancel()
{
	// Set a flag
	this->closeColorBoard();

	// Hide now
	hide();
}
