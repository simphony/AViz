//
// Implementation of liquid crystal board dialog widget class
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
                 . 32000 Haifa Israel
                 gery@tx.technion.ac.il
***********************************************************************/

#include "lcBoard.h"

#include "mainForm.h"
#include "codeBox.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "floatSpin.h"
#include "typeColorNumberBox.h"
#include "sizeBox.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "fileFunctions.h"
#include "defaultParticles.h" // typeCopy, typeCmp

#include <Q3GridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>


// Make a popup dialog box 
LcBoard::LcBoard(QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel )
{
	setWindowTitle( "AViz: Set Liquid Crystals" );

	// Create a hboxlayout that will fill the first row
	hb1 = new Q3HBox( this, "hb1" );

	// Create a label 
	QLabel * lcL = new QLabel( hb1, "lcL" );
	lcL->setText( " Type: ");

	// Create a combo box that will go into the
	// second column; entries in the combo box will
	// be made later
    lcCob = new QComboBox(hb1);

	// Define a callback for this combo box
	connect( lcCob, SIGNAL(activated(int)), SLOT(setLc()) );

	// Create a placeholder 
	QLabel * emptyL0 = new QLabel( hb1, "emptyL0" );
	emptyL0->setText("");

        // Add a check box button
        showLcCb = new QCheckBox( hb1, "showLc" );
	showLcCb->setText( "Show Liquid Crystals" );
	showLcCb->setChecked( TRUE );

	// Define a callback for this toggle switch
	connect( showLcCb, SIGNAL(clicked()), this, SLOT(adjustLc()) );

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
        positionBox = new PositionBox(this);
        propertyBox = new PropertyBox(this);
        codeBox = new CodeBox(this);
        typeColorNumberBox = new TypeColorNumberBox( this, 3 );

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
	lcBox = NULL;

	// Clear the color board pointer
	cb = NULL;

	// Clear internal variables
        colors = 1;
        colorPosX = colorPosY = 0;
        showColorBoard = FALSE;
	lcRenderStyle = LDOT;
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
void LcBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Build the layout
void LcBoard::buildLayout( colorCriterion crit )
{
        int numCols = 6;
        int numRows = 7;

        // Destroy existing layout
        if (lcBox) {
                lcBox->~Q3GridLayout();
        }

        // Destroy existing layout
        // Adjust height of dialog board
        this->setFixedHeight( numRows*ROW_HEIGHT );

        // Insert a grid that will hold control buttons
        lcBox = new Q3GridLayout( this, numRows, numCols, SPACE, SPACE, "lcBox" );

        // Add components that are always needed
        lcBox->addMultiCellWidget( hb1, 0, 0, 0, -1);
        lcBox->addMultiCellWidget( hb2, 1, 1, 0, -1);
        lcBox->addMultiCellWidget( sizeBox, 2, 2, 0, -1);
        lcBox->addMultiCellWidget( hb4, 3, 3, 0, -1);
	lcBox->addMultiCellWidget( lineTypeBox, numRows-2, numRows-2, 0, -1);
	lcBox->addMultiCellWidget( hb5, numRows-1, numRows-1, 0, -1);

        // Add additional components
        switch (crit) {
                case TYPE:
                        positionBox->hide();
                        propertyBox->hide();
                        codeBox->hide();
                        typeColorNumberBox->show();
        		lcBox->addMultiCellWidget( typeColorNumberBox, 4, 4, 0, -1);
                break;
                case POSITION:
                        positionBox->show();
			propertyBox->hide();
                        codeBox->hide();
                        typeColorNumberBox->hide();
        		lcBox->addMultiCellWidget( typeColorNumberBox, 4, 4, 0, -1);
                        lcBox->addMultiCellWidget( positionBox, 4, 4, 0, -1);
                break;
                case PROPERTY:
                        positionBox->hide();
                        propertyBox->show();
                        codeBox->hide();
                        typeColorNumberBox->hide();
                        lcBox->addMultiCellWidget( propertyBox, 4, 4, 0, -1);
                break;
                case COLORCODE:
                        positionBox->hide();
                        propertyBox->hide();
                        codeBox->show();
                        typeColorNumberBox->hide();
                        lcBox->addMultiCellWidget( codeBox, 4, 4, 0, -1);
                break;
        }

        this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void LcBoard::setData( void )
{
        aggregateData * ad = NULL;
	tag tmp;

        if (mainForm) {
                // Get the current switch settings and register
                // it using a local particle data structure
                thisPd = mainForm->getParticleData();

                // Get a list of particles that are currently rendered
                ad = mainForm->getAggregateData();

	        // Make entries in the combo box -- use only atoms
	        // types that are really needed; otherwise the list
	        // gets too long
                if (lcCob) {
                        lcCob->clear();
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
                                        lcCob->addItem( QString( (char *)&(*thisPd).type[i]));
                                }
                        }
                        lcCob->setMinimumSize( lcCob->sizeHint() );
                }
	}

        // Sort the entries alphabetically, at least approximately
        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                for (int j=0;j<lcCob->count()-1;j++) {
                        if (QString::compare(lcCob->itemText(j), lcCob->itemText(j+1)) > 0) {
                                typeCopy( qPrintable(lcCob->itemText(j+1)), (char *)&tmp );
                                lcCob->insertItem(j, QString( (char *)&tmp));
                                lcCob->removeItem(j+2);
                        }
                }
        }

        // Set the first liquid crystal as the current liquid crystal
        if ((*ad).numberOfParticles > 0) {
                thisLcIndex = 0;
                this->setLc();
        }
        else {
                thisLcIndex = -1;
        }
}


// Set the liquid crystal under consideration: adjust the toggle switch
void LcBoard::setLc( void )
{
        tag thisLc;
        thisLcIndex = -1;

        // Find out what combination of atoms is set
        typeCopy( qPrintable(lcCob->currentText()), (char *)&thisLc );

        for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisLc ) == TRUE) {
                        // Found the correct entry
                        thisLcIndex = i;

			// Adjust the color board
			this->setColors();

			// Adjust the toggles
			bool thisShowParticle = (*thisPd).showParticle[thisLcIndex];
			showLcCb->setChecked( thisShowParticle);

			switch( thisShowParticle ) {
				case TRUE: 
					if (thisLcIndex >= 0) {
						modeL->setDisabled( FALSE );
						colorMode->setDisabled( FALSE );
					}
					else {
						modeL->setDisabled( TRUE );
						colorMode->setDisabled( TRUE );
					}
				break;
				case FALSE:
					modeL->setDisabled( TRUE );
       					colorMode->setDisabled( TRUE );
				break;
			}

			// Adjust the toggles
                        if (lcRenderStyle != LDOT && lcRenderStyle != LLINE 
&& thisShowParticle) {
                                sizeBox->setDisabled( FALSE );
                        }
                        else {
                                sizeBox->setDisabled( TRUE );
                        }

			colorCriterion colorCrit = (*thisPd).colorCrit[thisLcIndex];
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

			positionBox->setParticle( thisPd, thisLcIndex );
                        propertyBox->setParticle( thisPd, thisLcIndex );
                        codeBox->setParticle( thisPd, thisLcIndex );
                        typeColorNumberBox->setParticle( thisPd, thisLcIndex );
                        lineTypeBox->setParticle( thisPd, thisLcIndex );
		}
	}
}


// Adjust the liquid crystal switch settings, using the local
// copy of the particle data structure 
void LcBoard::adjustLc( void )
{
	// Read the toggle switch values and
	// update the atom data entry
	(*thisPd).showParticle[thisLcIndex] = showLcCb->isChecked();

	// Activate or deactivate the remaining radio buttons
	switch ( showLcCb->isChecked() ) {
		case TRUE:	
			if (thisLcIndex >= 0) {
				this->buildLayout( (*thisPd).colorCrit[thisLcIndex] );
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


// Adjust the atom switch settings, using the local
// copy of the atom data structure
void LcBoard::adjustCriterion( void )
{
	this->readToggles();
	this->setLc();
}


// Read the toggle switches and the emission text window
void LcBoard::readToggles( void )
{
	if (thisLcIndex >= 0) {
		// Read the settings and
                // update the particle data entry
                sizeBox->readToggles( thisPd, thisLcIndex );

                if (colorMode0->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisLcIndex] = TYPE;
                }
                if (colorMode1->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisLcIndex] = POSITION;
                }
                if (colorMode2->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisLcIndex] = PROPERTY;
                }
                if (colorMode3->isChecked() == TRUE) {
                        (*thisPd).colorCrit[thisLcIndex] = COLORCODE;
                }
        }

        if (thisLcIndex >= 0) {
                positionBox->readToggles( thisPd, thisLcIndex );
                propertyBox->readToggles( thisPd, thisLcIndex );
                codeBox->readToggles( thisPd, thisLcIndex );
                typeColorNumberBox->readToggles( thisPd, thisLcIndex );
		lineTypeBox->readToggles( thisPd, thisLcIndex );
        }

	// Update the color board -- if the number of type colors has
	// been changed, it must be adjusted
	this->setColors();
}


// Launch the color board to adjust colors
void LcBoard::setColorCb()
{
        // Set a flag
        showColorBoard = TRUE;

        // Adjust the color board
        this->setColors();
}


// Adjust the colors of the color board
void LcBoard::setColors()
{
	float red, green, blue;
	float redTwo, greenTwo, blueTwo;
	float redThree, greenThree, blueThree;
	float redMin, redMax;
	float greenMin, greenMax;
	float blueMin, blueMax;
	red = green = blue = 0.0;
	redTwo = greenTwo = blueTwo = 0.0;
	redThree = greenThree = blueThree = 0.0;
	redMin = redMax = 0.0;
	greenMin = greenMax = 0.0;
	blueMin = blueMax = 0.0;

	if (thisLcIndex > 0) {
	        // Get the current particle data and show the
	        // color panel
		switch ((*thisPd).colorCrit[thisLcIndex]) {
			case TYPE:
				// One, two, or three colors are needed
				red = (*thisPd).particleColors[thisLcIndex].red;
				green = (*thisPd).particleColors[thisLcIndex].green;
				blue = (*thisPd).particleColors[thisLcIndex].blue;
				redTwo = (*thisPd).particleColors[thisLcIndex].redTwo;
				greenTwo = (*thisPd).particleColors[thisLcIndex].greenTwo;
				blueTwo = (*thisPd).particleColors[thisLcIndex].blueTwo;
				redThree = (*thisPd).particleColors[thisLcIndex].redThree;
				greenThree = (*thisPd).particleColors[thisLcIndex].greenThree;
				blueThree = (*thisPd).particleColors[thisLcIndex].blueThree;
	
				switch ((*thisPd).line[thisLcIndex].typeColorNumber) {
					case 1:
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
				                                // This is an ugly hack, 
								// intended to propagate the 
								// address of the calling class
                                                cb->setLcBoardAddress(this );
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
					case 2:
						colorLabel0->setColor( red, green, blue );
						colorLabel1->setColor( red, green, blue );
						colorLabel2->setColor( red, green, blue );
						colorLabel3->setColor( redTwo, greenTwo, blueTwo );
						colorLabel4->setColor( redTwo, greenTwo, blueTwo );
						colorLabel5->setColor( redTwo, greenTwo, blueTwo );
		
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
				                                // This is an ugly hack, 
								// intended to propagate the 
								// address of the calling class
                                                cb->setLcBoardAddress(this );
				                                cb->buildLayout( colors );
				                        }
		       	                 	}
	
				                // Position the color board correctly
				                if (colorPosX > 0 && colorPosY > 0) {
				                        cb->setGeometry( colorPosX, colorPosY, cb->width(), cb->height() );
				                }
		
				                if (cb  && showColorBoard)
				                        cb->show();
		
						if (cb) {
							cb->setColor( red, green, blue, redTwo, greenTwo, blueTwo );
							cb->setLabel( "Type Color North", "Type Color South" );
						}
					break;
					case 3:
						colorLabel0->setColor( red, green, blue );
						colorLabel1->setColor( red, green, blue );
						colorLabel2->setColor( redTwo, greenTwo, blueTwo );
						colorLabel3->setColor( redTwo, greenTwo, blueTwo );
						colorLabel4->setColor( redThree, greenThree, blueThree );
						colorLabel5->setColor( redThree, greenThree, blueThree );
		
				                // Destroy the color board and re-build if
				                // the settings are not right
				                if (colors != 3 || !cb) {
				                        colors = 3;
				
				                        if (cb) {
				                                colorPosX = cb->pos().x();
				                                colorPosY = cb->pos().y();
				                                cb->~ColorBoard();
				                        }
				
                                        cb = new ColorBoard(this);
				
				                        if (cb) {
				                                // This is an ugly hack, 
								// intended to propagate the 
								// address of the calling class
                                                cb->setLcBoardAddress(this );
				                                cb->buildLayout( colors );
				                        }
			                        }
		
				                // Position the color board correctly
				                if (colorPosX > 0 && colorPosY > 0) {
				                        cb->setGeometry( colorPosX, colorPosY, cb->width(), cb->height() );
				                }
		
				                if (cb  && showColorBoard)
				                        cb->show();
		
						if (cb) {
							cb->setColor( red, green, blue, redTwo, greenTwo, blueTwo, redThree, greenThree, blueThree );
							cb->setLabel( "Type Color North", "Type Color Center", "Type Color South" );
						}
					break;
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
				 redMin = (*thisPd).particleColors[thisLcIndex].redMin;
				greenMin = (*thisPd).particleColors[thisLcIndex].greenMin;
				blueMin = (*thisPd).particleColors[thisLcIndex].blueMin;
				redMax = (*thisPd).particleColors[thisLcIndex].redMax;
				greenMax = (*thisPd).particleColors[thisLcIndex].greenMax;
       				blueMax = (*thisPd).particleColors[thisLcIndex].blueMax;
	
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
	
                                cb = new ColorBoard(this);
	
		                        if (cb) {
		                                // This is an ugly hack, intended to
		                                // propagate the address of the calling class
                                        cb->setLcBoardAddress(this);
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
void LcBoard::getColors( float r0, float g0, float b0, float r1, float g1, float b1, float r2, float g2, float b2 )
{
        if ((*thisPd).colorCrit[thisLcIndex] == TYPE) {
		switch ((*thisPd).line[thisLcIndex].typeColorNumber) {
			case 1:
				// Only one color is set
				(*thisPd).particleColors[thisLcIndex].red = r0;
				(*thisPd).particleColors[thisLcIndex].green = g0;
				(*thisPd).particleColors[thisLcIndex].blue = b0;
				colorLabel0->setColor( r0, g0, b0 );
				colorLabel1->setColor( r0, g0, b0 );
				colorLabel2->setColor( r0, g0, b0 );
				colorLabel3->setColor( r0, g0, b0 );
				colorLabel4->setColor( r0, g0, b0 );
				colorLabel5->setColor( r0, g0, b0 );
			break;
			case 2:
				// Two colors are set
				(*thisPd).particleColors[thisLcIndex].red = r0;
				(*thisPd).particleColors[thisLcIndex].green = g0;
				(*thisPd).particleColors[thisLcIndex].blue = b0;
				(*thisPd).particleColors[thisLcIndex].redTwo = r1;
				(*thisPd).particleColors[thisLcIndex].greenTwo = g1;
				(*thisPd).particleColors[thisLcIndex].blueTwo = b1;
				colorLabel0->setColor( r0, g0, b0 );
				colorLabel1->setColor( r0, g0, b0 );
				colorLabel2->setColor( r0, g0, b0 );
				colorLabel3->setColor( r1, g1, b1 );
				colorLabel4->setColor( r1, g1, b1 );
				colorLabel5->setColor( r1, g1, b1 );
			break;
			case 3:
				// Three colors are set
				(*thisPd).particleColors[thisLcIndex].red = r0;
				(*thisPd).particleColors[thisLcIndex].green = g0;
				(*thisPd).particleColors[thisLcIndex].blue = b0;
				(*thisPd).particleColors[thisLcIndex].redTwo = r1;
				(*thisPd).particleColors[thisLcIndex].greenTwo = g1;
				(*thisPd).particleColors[thisLcIndex].blueTwo = b1;
				(*thisPd).particleColors[thisLcIndex].redThree = r2;
				(*thisPd).particleColors[thisLcIndex].greenThree = g2;
				(*thisPd).particleColors[thisLcIndex].blueThree = b2;
				colorLabel0->setColor( r0, g0, b0 );
				colorLabel1->setColor( r0, g0, b0 );
				colorLabel2->setColor( r1, g1, b1 );
				colorLabel3->setColor( r1, g1, b1 );
				colorLabel4->setColor( r2, g2, b2 );
				colorLabel5->setColor( r2, g2, b2 );
			break;
		}
        }
	else {
		// A spectrum of colors is sent
		(*thisPd).particleColors[thisLcIndex].redMin = r0;
		(*thisPd).particleColors[thisLcIndex].greenMin = g0;
		(*thisPd).particleColors[thisLcIndex].blueMin = b0;
		(*thisPd).particleColors[thisLcIndex].redMax = r1;
		(*thisPd).particleColors[thisLcIndex].greenMax = g1;
		(*thisPd).particleColors[thisLcIndex].blueMax = b1;

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
void LcBoard::setDotStyle( void )
{
	lcRenderStyle = LDOT;
        sizeBox->setDisabled( TRUE );
	lineTypeBox->setDisabled( TRUE  );
}


// Adjust the board
void LcBoard::setLineStyle( void )
{
	lcRenderStyle = LLINE;
        sizeBox->setDisabled( TRUE );
	lineTypeBox->setDisabled( FALSE, TRUE  );
}


// Adjust the board
void LcBoard::setCubeStyle( void )
{
	lcRenderStyle = LCUBE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void LcBoard::setCylinderStyle( void )
{
	lcRenderStyle = LCYLINDER;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void LcBoard::setConeStyle( void )
{
	lcRenderStyle = LCONE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void LcBoard::setSphereStyle( void )
{
	lcRenderStyle = LSPHERE;
        sizeBox->setDisabled( FALSE );
	lineTypeBox->setDisabled( FALSE );
}


// Adjust the board
void LcBoard::setLowQuality( void )
{
        renderQuality = LOW;
}


// Adjust the board
void LcBoard::setHighQuality( void )
{
        renderQuality = HIGH;
}


// Adjust the board
void LcBoard::setFinalQuality( void )
{
        renderQuality = FINAL;
}


// Set a flag when the color board is closed
void LcBoard::closeColorBoard( void )
{
        showColorBoard = FALSE;

        if (cb)
                cb->~ColorBoard();

        cb = NULL;
        colors = 0;
}


// Remember the board position when the color board is closed
void LcBoard::getColorBoardPos( int posX, int posY )
{
        colorPosX = posX;
        colorPosY = posY;
}


// Complete the liquid crystal switch setting: save the particle data and 
// hide the board
void LcBoard::bdone()
{
	// Read the toggle switches
	this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
        sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved liquid crystal data in ", filename );
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


// Accept the liquid crystal switch setting: save the particle data
void LcBoard::bapply()
{
	// Read the toggle switches
	this->readToggles();

	// Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
        sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
	if (saveParticleDataFunction( filename, thisPd ) ) {
		if (mainForm) 
			mainForm->statusMessage( "Saved liquid crystal data in ", filename );
	}
	free(filename);

	// Re-do the graphics, using the new particle data
	if (mainForm) {
		mainForm->updateView();
		mainForm->updateRendering();
	}
}


// Cancel the atom switch setting: hide the board
void LcBoard::bcancel()
{
        // Set a flag
        this->closeColorBoard();

	// Hide now
	hide();
}
