//
// Implementation of bond board dialog widget class
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

#include "bondBoard.h"

#include "./pixmaps/whiteSquare2.xpm"
#include "./pixmaps/blackSquare2.xpm"
#include "./pixmaps/redSquare2.xpm"
#include "./pixmaps/greenSquare2.xpm"
#include "./pixmaps/blueSquare2.xpm"
#include "./pixmaps/yellowSquare2.xpm"
#include "./pixmaps/goldSquare2.xpm"
#include "./pixmaps/silverSquare2.xpm"
#include "./pixmaps/bronzeSquare2.xpm"

// Make a popup dialog box 
BondBoard::BondBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, WType_TopLevel )
{
	this->setCaption( "AViz: Set Bonds" );

	// Insert a grid that will hold combo boxes and
	// a spin box, plus a row of control buttons
	const int numCols = 3;
        const int numRows = 3;
        QGridLayout * bondBox = new QGridLayout( this, numCols, numRows, SPACE, SPACE, "bondBox" );

        // Create a hboxlayout that will fill the first row
        QHBox * hb0 = new QHBox( this, "hb0" );
        bondBox->addMultiCellWidget( hb0, 0, 0, 0, -1);

        // Create a label
        QLabel * particleL0 = new QLabel( hb0, "particleL0" );
        particleL0->setText( " From Particle Type: ");

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        fParticleCob = new QComboBox( FALSE, hb0, "particleSelection0" );

        // Define a callback for this combo boxes
        connect( fParticleCob, SIGNAL(activated(int)), SLOT(setFAtom()) );

        // Create a label
        QLabel * particleL1 = new QLabel( hb0, "particleL1" );
        particleL1->setText( "   To Particle Type: ");

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        tParticleCob = new QComboBox( FALSE, hb0, "particleSelection1" );

        // Define a callback for this combo boxes
        connect( tParticleCob, SIGNAL(activated(int)), SLOT(setTAtom()) );

        // Add radiobuttons and a label
        QLabel * bondL = new QLabel( hb0, "bondL" );
        bondL->setText( "   Bond: " );

		bondMode = new QButtonGroup( 2, QGroupBox::Horizontal, hb0, "bondMode");
        bondMode0 = new QRadioButton( bondMode, "on" );
        bondMode0->setText( "Present" );
        bondMode1 = new QRadioButton( bondMode, "off" );
        bondMode1->setText( "Absent" );

	// Define a callback for these radio buttons
        connect( bondMode, SIGNAL(clicked(int)), this, SLOT(setBond(int)) );

	// Create a hbox that will contain various switches
	QHBox * hb1 = new QHBox( this, "hb1" );
	hb1->setMargin( SPACE );
        bondBox->addMultiCellWidget( hb1, 1, 1, 0, -1);

	// Create a label
	colorL = new QLabel( hb1, "colorL" );
	colorL->setText( " Color: " );

	// Create radiobuttons to set the color
	bondColor = new QButtonGroup( 3, QGroupBox::Horizontal, hb1, "bondColor");

        QPixmap blackIcon = QPixmap( blackSquare2 );
        QPixmap whiteIcon = QPixmap( whiteSquare2 );
        QPixmap redIcon = QPixmap( redSquare2 );
        QPixmap greenIcon = QPixmap( greenSquare2 );
        QPixmap blueIcon = QPixmap( blueSquare2 );
        QPixmap yellowIcon = QPixmap( yellowSquare2 );
        QPixmap goldIcon = QPixmap( goldSquare2 );
        QPixmap silverIcon = QPixmap( silverSquare2 );
        QPixmap bronzeIcon = QPixmap( bronzeSquare2 );
//        QPixmap graphiteIcon = QPixmap( graphiteSquare2 );

        bondColor0 = new QRadioButton( bondColor, "white" );
        bondColor0->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor0->setPixmap( whiteIcon );
        bondColor1 = new QRadioButton( bondColor, "black" );
        bondColor1->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor1->setPixmap( blackIcon );
        bondColor2 = new QRadioButton( bondColor, "red" );
        bondColor2->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor2->setPixmap( redIcon );
        bondColor3 = new QRadioButton( bondColor, "green" );
        bondColor3->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor3->setPixmap( greenIcon );
        bondColor4 = new QRadioButton( bondColor, "blue" );
        bondColor4->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor4->setPixmap( blueIcon );
        bondColor5 = new QRadioButton( bondColor, "yellow" );
        bondColor5->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor5->setPixmap( yellowIcon );
        bondColor6 = new QRadioButton( bondColor, "gold" );
        bondColor6->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor6->setPixmap( goldIcon );
        bondColor7 = new QRadioButton( bondColor, "silver" );
        bondColor7->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor7->setPixmap( silverIcon );
        bondColor8 = new QRadioButton( bondColor, "bronze" );
        bondColor8->setMinimumWidth( COLORLABEL_WIDTH );
        bondColor8->setPixmap( bronzeIcon );

	// Create a label
	thickL = new QLabel( hb1, "thickL" );
	thickL->setText( "  Thickness: " );

	// Create radiobuttons to set the thickness 
	bondThickness = new QButtonGroup( 2, QGroupBox::Horizontal, hb1, "bondThickness");

        bondThickness0 = new QRadioButton( bondThickness, "line" );
        bondThickness0->setText( "Line" );
        bondThickness1 = new QRadioButton( bondThickness, "thin" );
        bondThickness1->setText( "Thin" );
        bondThickness2 = new QRadioButton( bondThickness, "medium" );
        bondThickness2->setText( "Medium" );
        bondThickness3 = new QRadioButton( bondThickness, "thick" );
        bondThickness3->setText( "Thick" );
        bondThickness4 = new QRadioButton( bondThickness, "varying_linear" );
        bondThickness4->setText( "Linear Variation" );
        bondThickness5 = new QRadioButton( bondThickness, "varying_quadratic" );
        bondThickness5->setText( "Quadratic Variation" );

	// Create a grid to hold a check box and a spin box plus labels
        QGrid * gr0 = new QGrid( 2, hb1, "gr0" );
        gr0->setMargin( SPACE );
        gr0->setSpacing( SPACE );

	// Create a label and a check box
	antiAlL = new QLabel( gr0, "antiAlL" );
	antiAlL->setText( " AntiAliasing " );
	antiAliasCb = new QCheckBox( gr0, "antiAliasCb" );

	// Create a label and a check box
	seqBond = new QLabel( gr0, "antiAlL" );
	seqBond->setText( " Sequential Bonding " );
	seqBondCb = new QCheckBox( gr0, "seqBond" );

	// Create a hbox with a label and a spin box
	bondEmL = new QLabel( gr0, "bondEmL" );
	bondEmL->setText( " Bond Emission " );
	bondEmissionSb = new QSpinBox( 0, 100, 10, gr0 );
	bondEmissionSb->setSuffix( "%" );
	bondEmissionSb->setValue( 0 );


	// Create a grid to hold two labels and spin boxes
        QGrid * gr1 = new QGrid( 2, hb1, "gr1" );
        gr1->setMargin( SPACE );
        gr1->setSpacing( SPACE );

	// Create a label
	minLengthL = new QLabel( gr1, "minLengthL" );
	minLengthL->setText( "Min Length: " );

	// Create a spin box to set the min length
 	bondMinLengthSb = new QFSpinBox( gr1, "minLengthSb" );

	// Create a label
	maxLengthL = new QLabel( gr1, "maxLengthL" );
	maxLengthL->setText( "Max Length: " );

	// Create a spin box to set the max length
	bondMaxLengthSb = new QFSpinBox( gr1, "maxLengthSb" );

	// Create a placeholder 
	QLabel * emptyL0 = new QLabel( gr1, "emptyL1" );

	// Create a hboxlayout that will fill the lowest row
	QHBox * hb3 = new QHBox( this, "hb3" );
	bondBox->addMultiCellWidget( hb3, numRows-1, numRows-1, 0, -1);
	


	// Create a placeholder 
	QLabel * emptyL1 = new QLabel( hb3, "emptyL1" );

	// Create pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( hb3, "done" );
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

	QPushButton * apply = new QPushButton( hb3, "apply" );
	apply->setText( "Apply" ); 

	 // Define a callback for that button
        QObject::connect( apply, SIGNAL(clicked()), this, SLOT(bapply()) );

	QPushButton * cancel = new QPushButton( hb3, "cancel" );
	cancel->setText( "Cancel" ); 

	 // Define a callback for that button
        QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(bcancel()) );

	hb3->setStretchFactor( emptyL1, 10 );
	hb1->setStretchFactor( emptyL0, 10 );

	// Set flags
	haveFEntry = FALSE;
	haveTEntry = FALSE;
}


// Set a pointer to the main form
void BondBoard::setMainFormAddress( MainForm * thisMF )
{
	mainForm = thisMF;
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the color
// board is launched
void BondBoard::setData( void )
{
	int i;
	aggregateData * ad = NULL;

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
		if (fParticleCob && tParticleCob && (*ad).numberOfParticles > 0) {
		
			fParticleCob->clear();
			tParticleCob->clear();

			for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
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
					fParticleCob->insertItem( QString( (char *)&(*thisPd).type[i]), item );
					tParticleCob->insertItem( QString( (char *)&(*thisPd).type[i]), item );
					// Set flags
					haveFEntry = TRUE;
					haveTEntry = TRUE;

					item++;
				}
			}
			fParticleCob->setMinimumSize( fParticleCob->sizeHint() );
			tParticleCob->setMinimumSize( tParticleCob->sizeHint() );
		}

	}	

	// Sort the entries alphabetically, at least approximately
	if (haveFEntry && haveTEntry) {
		for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
			for (int j=0;j<fParticleCob->count()-1;j++) {
				if (QString::compare(fParticleCob->text(j), fParticleCob->text(j+1)) > 0) {
					fParticleCob->removeItem(j);
					fParticleCob->insertItem( tParticleCob->text(j), j+1);
					tParticleCob->removeItem(j);
					tParticleCob->insertItem( fParticleCob->text(j+1), j+1);
				}
			}
		}
	}

	// Now go through the bond data structure and make new entries 
	// if needed
	for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		for (int j=0;j<(*thisPd).numberOfParticleTypes;j++) {
			bool flag = FALSE;
			for (int k=0;k<(*thisPd).particleBonds[i].numberOfNeighborBonds;k++) {
				if (typeCmp( (char *)&(*thisPd).type[j], (char *)&(*thisPd).particleBonds[i].neighborParticleType[k]) == TRUE) {
					// OK this bond is already defined:
					// set a flag
					flag = TRUE;
				}
			}
			
			// If the bond is not defined, define it now
			if (flag == FALSE) {
				int index = (*thisPd).particleBonds[i].numberOfNeighborBonds;
				if (index < BONDS_MAX) {
					typeCopy( (char *)&(*thisPd).type[j], (char *)&(*thisPd).particleBonds[i].neighborParticleType[index] );
					// Set some defaults
					(*thisPd).particleBonds[i].bShowBond[index] = FALSE;
					(*thisPd).particleBonds[i].bMinLength[index] = 0.0;
					(*thisPd).particleBonds[i].bMaxLength[index] = 10.0;
					(*thisPd).particleBonds[i].bColor[index] = GREEN;
					(*thisPd).particleBonds[i].bThickness[index] = LINE;
					(*thisPd).particleBonds[i].bEmission[index] = 0;
					(*thisPd).particleBonds[i].bAntiAlias[index] = TRUE;

					// Increment the number of bonds emerging from 
					// this particle
					(*thisPd).particleBonds[i].numberOfNeighborBonds++;
				}
				else {
					printf("Error -- Cannot add bonds -- recompile with larger BONDS_MAX (%d)\n", BONDS_MAX);
				}
			}
		}
	}

	// Set the first particle as the current particle
	if (haveFEntry)
		fParticleCob->setCurrentItem(0);
	if (haveTEntry)
		tParticleCob->setCurrentItem(0);

        // Find out what combination of particles is set
	if (haveFEntry)
        	typeCopy( (const char *)fParticleCob->currentText(), (char *)&thisFParticle );
	if (haveTEntry)
        	typeCopy( (const char *)tParticleCob->currentText(), (char *)&thisTParticle );

        for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == TRUE) {
                        for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == TRUE) {
                                        // Found the correct entry now;
                                        // adjust the presence/absence and set 
					// toggles
                                        switch ((*thisPd).particleBonds[i].bShowBond[j]) {
                                                case TRUE:
							bondMode0->setChecked( TRUE );
                                                break;
                                                case FALSE:
							bondMode1->setChecked( TRUE );
                                                break;
                                        }

                                        // Update the board
                                        setToggles( i, j );
                                }
                        }
                }
        }
}


// Set the atom under consideration: adjust the toggle switches
void BondBoard::setFAtom( void )
{
	// Find out what combination of atoms is set
	if (haveFEntry)
		typeCopy( (const char *)fParticleCob->currentText(), (char *)&thisFParticle ); 
	if (haveTEntry)
		typeCopy( (const char *)tParticleCob->currentText(), (char *)&thisTParticle ); 

	for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisFParticle ) == TRUE) {
			for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
				if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisTParticle ) == TRUE) {

					// Found the correct entry now:
					// adjust the presence/absent toggle
					switch ((*thisPd).particleBonds[i].bShowBond[j]) {
						case TRUE:
							bondMode0->setChecked( TRUE );
						break;
						case FALSE:
							bondMode1->setChecked( TRUE );
						break;
					}

					// Adjust the remaining toggles
					setToggles( i, j );
				}
			}
		}
	}
}


// Set the atom under consideration: adjust the toggle switch
void BondBoard::setTAtom( void )
{
	// Find out what combination of atoms is set
	if (haveFEntry)
		typeCopy( (const char *)fParticleCob->currentText(), (char *)&thisFParticle ); 
	if (haveTEntry)
		typeCopy( (const char *)tParticleCob->currentText(), (char *)&thisTParticle ); 

	for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == TRUE) {
			for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
				if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == TRUE) {
					
					// Found the correct entry now:
					// adjust the presence/absent toggle
					switch ((*thisPd).particleBonds[i].bShowBond[j]) {
						case TRUE:
							bondMode0->setChecked( TRUE );
						break;
						case FALSE:
							bondMode1->setChecked( TRUE );
						break;
					}

					// Adjust the remaining toggles
					setToggles( i, j );
				}
			}
		}
	}
}


// Callback for bond on/off radio buttons; this is equivalent 
// to adjusting the combos
void BondBoard::setBond( int radio )
{
	
	// Find out what combination of atoms is set
	if (haveFEntry)
		typeCopy( (const char *)fParticleCob->currentText(), (char *)&thisFParticle ); 
	if (haveTEntry)
		typeCopy( (const char *)tParticleCob->currentText(), (char *)&thisTParticle ); 

	for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == TRUE) {
			for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
				if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == TRUE) {
					// Found the correct entry now; 
					// adjust the presence/absence
					switch (radio) {
						case 0:
							(*thisPd).particleBonds[i].bShowBond[j] = TRUE;
						break;
						case 1:
							(*thisPd).particleBonds[i].bShowBond[j] = FALSE;
						break;
					}

					// Update the board

					setToggles( i, j );
				}
			}
		}
	}
}
 

// Adjust the toggle switches
void BondBoard::setToggles( int thisAtom, int neighborAtom )
{
	switch ((*thisPd).particleBonds[thisAtom].bShowBond[neighborAtom]) {
		case TRUE:
			// Indicate that a bond exists
			colorL->setDisabled( FALSE );
			bondColor->setDisabled( FALSE );
			thickL->setDisabled( FALSE );
			bondThickness->setDisabled( FALSE );
			antiAlL->setDisabled( FALSE );
			antiAliasCb->setDisabled( FALSE );
			seqBond->setDisabled( FALSE );
			seqBondCb->setDisabled( FALSE );
			bondEmL->setDisabled( FALSE );
			bondEmissionSb->setDisabled( FALSE );
			minLengthL->setDisabled( FALSE );
			bondMinLengthSb->setDisabled( FALSE );
			maxLengthL->setDisabled( FALSE );
			bondMaxLengthSb->setDisabled( FALSE );

			// Adjust the toggles now
			switch ((*thisPd).particleBonds[thisAtom].bColor[neighborAtom]) {
				case WHITE:
					bondColor0->setChecked( TRUE );
				break;
				case BLACK:
					bondColor1->setChecked( TRUE );
				break;
				case RED:
					bondColor2->setChecked( TRUE );
				break;
				case GREEN:
					bondColor3->setChecked( TRUE );
				break;
				case BLUE:
					bondColor4->setChecked( TRUE );
				break;
				case YELLOW:
					bondColor5->setChecked( TRUE );
				break;
				case GOLD:
					bondColor6->setChecked( TRUE );
				break;
				case SILVER:
					bondColor7->setChecked( TRUE );
				break;
				case BRONZE:
					bondColor8->setChecked( TRUE );
				break;
			}		

			switch ((*thisPd).particleBonds[thisAtom].bThickness[neighborAtom]) {
				case LINE:
					bondThickness0->setChecked( TRUE );
				break;
				case THIN:
					bondThickness1->setChecked( TRUE );
				break;
				case MEDIUM:
					bondThickness2->setChecked( TRUE );
				break;
				case THICK:
					bondThickness3->setChecked( TRUE );
				break;
				case VARYING_LINEAR:
					bondThickness4->setChecked( TRUE );
				break;
				case VARYING_QUADRATIC:
					bondThickness5->setChecked( TRUE );
				break;
			}
			
			antiAliasCb->setChecked ((*thisPd).particleBonds[thisAtom].bAntiAlias[neighborAtom] );	
			seqBondCb->setChecked ((*thisPd).particleBonds[thisAtom].bSeqBond[neighborAtom] );	
			bondEmissionSb->setValue( (int)floor(100.0*(*thisPd).particleBonds[thisAtom].bEmission[neighborAtom]+0.5) );
			bondMinLengthSb->setValue( (int)floor(10.0*(*thisPd).particleBonds[thisAtom].bMinLength[neighborAtom]+0.5) );
			bondMaxLengthSb->setValue( (int)floor(10.0*(*thisPd).particleBonds[thisAtom].bMaxLength[neighborAtom]+0.5) );
		break;
		case FALSE:	
			// Indicate that no bond exists
			colorL->setDisabled( TRUE );
			bondColor->setDisabled( TRUE );
			thickL->setDisabled( TRUE );
			bondThickness->setDisabled( TRUE );
			antiAlL->setDisabled( TRUE );
			antiAliasCb->setDisabled( TRUE );
			seqBond->setDisabled( TRUE );
			seqBondCb->setDisabled( TRUE );
			bondEmL->setDisabled( TRUE );
			bondEmissionSb->setDisabled( TRUE );
			minLengthL->setDisabled( TRUE );
			bondMinLengthSb->setDisabled( TRUE );
			maxLengthL->setDisabled( TRUE );
			bondMaxLengthSb->setDisabled( TRUE );
		break;	
	}
}


// Read the current settings
void BondBoard::readToggles( void ) 
{
	int i;
	int fAtom = -1; 
	int tAtom = -1; 
	int neighborAtom = -1; 
	int neighborAtom2 = -1;

	// Find out what combination of atoms is set
	if (haveFEntry)
		typeCopy( (const char *)fParticleCob->currentText(), (char *)&thisFParticle ); 
	if (haveTEntry)
		typeCopy( (const char *)tParticleCob->currentText(), (char *)&thisTParticle ); 

	// Find out the combination of atoms to which these 
	// settings apply
	for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisFParticle ) == TRUE) {
			for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
				if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisTParticle ) == TRUE) {
					// Found the correct entry now
					fAtom = i;
					neighborAtom = j;
				}
			}
		}
	}

	for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
		if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == TRUE) {
			for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
				if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == TRUE) {
					// Found the second correct entry now
					tAtom = i;
					neighborAtom2 = j;
				}
			}
		}
	}

	// Begin with some checks
	if (fAtom < 0 || tAtom < 0) 
		return;
	if (neighborAtom < 0 || neighborAtom2 < 0) 
		return;

	// Read the settings -- there should be 
	// an entry in the list of bonds
	if (bondMode0->isChecked() == TRUE) {
		(*thisPd).particleBonds[fAtom].bShowBond[neighborAtom] = TRUE;
		(*thisPd).particleBonds[tAtom].bShowBond[neighborAtom2] = TRUE;
	}
	if (bondMode1->isChecked() == TRUE) {
		(*thisPd).particleBonds[fAtom].bShowBond[neighborAtom] = FALSE;
		(*thisPd).particleBonds[tAtom].bShowBond[neighborAtom2] = FALSE;
	}

	if ( bondColor0->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = WHITE;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = WHITE;
	}
	if ( bondColor1->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BLACK;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BLACK;
	}
	if ( bondColor2->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = RED;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = RED;
	}
	if ( bondColor3->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = GREEN;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = GREEN;
	}
	if ( bondColor4->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BLUE;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BLUE;
	}
	if ( bondColor5->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = YELLOW;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = YELLOW;
	}
	if ( bondColor6->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = GOLD;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = GOLD;
	}
	if ( bondColor7->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = SILVER;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = SILVER;
	}
	if ( bondColor8->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BRONZE;
		(*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BRONZE;
	}
	if ( bondThickness0->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = LINE;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = LINE;
	}
	if ( bondThickness1->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = THIN;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = THIN;
	}
	if ( bondThickness2->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = MEDIUM;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = MEDIUM;
	}
	if ( bondThickness3->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = THICK;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = THICK;
	}
	if ( bondThickness4->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = VARYING_LINEAR;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = VARYING_LINEAR;
	}
	if ( bondThickness5->isChecked() == TRUE ) {
		(*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = VARYING_QUADRATIC;
		(*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = VARYING_QUADRATIC;
	}

	(*thisPd).particleBonds[fAtom].bAntiAlias[neighborAtom] = antiAliasCb->isChecked();
	(*thisPd).particleBonds[tAtom].bAntiAlias[neighborAtom2] = antiAliasCb->isChecked();
	(*thisPd).particleBonds[fAtom].bSeqBond[neighborAtom] = seqBondCb->isChecked();
	(*thisPd).particleBonds[tAtom].bSeqBond[neighborAtom2] = seqBondCb->isChecked();
	(*thisPd).particleBonds[fAtom].bEmission[neighborAtom] = (float)bondEmissionSb->value()/100.0;
	(*thisPd).particleBonds[tAtom].bEmission[neighborAtom] = (float)bondEmissionSb->value()/100.0;
	(*thisPd).particleBonds[fAtom].bMinLength[neighborAtom] = (float)bondMinLengthSb->value()/10.0;
	(*thisPd).particleBonds[tAtom].bMinLength[neighborAtom2] = (float)bondMinLengthSb->value()/10.0;
	(*thisPd).particleBonds[fAtom].bMaxLength[neighborAtom] = (float)bondMaxLengthSb->value()/10.0;
	(*thisPd).particleBonds[tAtom].bMaxLength[neighborAtom2] = (float)bondMaxLengthSb->value()/10.0;
}


// Complete the bond setting: update the rendering and hide the board
void BondBoard::bdone()
{
	// Complete the current settings
	readToggles();

        // Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
        if (saveParticleDataFunction( filename, thisPd ) ) {
                if (mainForm)
                        mainForm->statusMessage( "Saved particle data in ", filename );
        }
	free(filename);

        // Re-do the graphics, using the new data
        if (mainForm) {
                mainForm->updateView();
                mainForm->updateRendering();
	}

	// Hide now
        hide();
}


// Accept the bond setting: update the rendering 
void BondBoard::bapply()
{
	// Complete the current settings
	readToggles(); 

        // Save the particle data
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
        if (saveParticleDataFunction( filename, thisPd ) ) {
                if (mainForm)
                        mainForm->statusMessage( "Saved particle data in ", filename );
        }
	free(filename);

        // Re-do the graphics, using the new data
        if (mainForm) {
                mainForm->updateView();
                mainForm->updateRendering();
	}
}


// Cancel the bond setting: hide the board
void BondBoard::bcancel()
{
	// Hide now
	hide();
}
