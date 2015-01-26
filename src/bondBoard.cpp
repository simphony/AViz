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

#include <cmath>

#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QPixmap>
#include <QButtonGroup>
#include <QGroupBox>

#include "./pixmaps/whiteSquare2.xpm"
#include "./pixmaps/blackSquare2.xpm"
#include "./pixmaps/redSquare2.xpm"
#include "./pixmaps/greenSquare2.xpm"
#include "./pixmaps/blueSquare2.xpm"
#include "./pixmaps/yellowSquare2.xpm"
#include "./pixmaps/goldSquare2.xpm"
#include "./pixmaps/silverSquare2.xpm"
#include "./pixmaps/bronzeSquare2.xpm"

#include "floatSpin.h"
#include "mainForm.h"
#include "fileFunctions.h"
#include "defaultParticles.h"
#include "widgets/doneapplycancelwidget.h"

namespace {
QRadioButton* createColorRadioButton(const QPixmap &icon) {
    QRadioButton *rb = new QRadioButton();
    rb->setMinimumWidth( COLORLABEL_WIDTH );
    rb->setIcon(icon);
    return rb;
}
}

// Make a popup dialog box 
BondBoard::BondBoard(MainForm *mainForm, QWidget * parent)
    : QDialog( parent, Qt::WType_TopLevel ), mainForm(mainForm)
{
    setWindowTitle( "AViz: Set Bonds" );

    // Insert a grid that will hold combo boxes and
    // a spin box, plus a row of control buttons

    QGridLayout * bondBox = new QGridLayout(this);

    {
        // Create a hboxlayout that will fill the first row
        QWidget *w = new QWidget(this);
        QHBoxLayout *hBox = new QHBoxLayout(w);

        bondBox->addWidget(w, 0 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);

        // Create a label
        hBox->addWidget(new QLabel(" From Particle Type: "));

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        fParticleCob = new QComboBox(w);
        hBox->addWidget(fParticleCob);
        connect( fParticleCob, SIGNAL(activated(int)), SLOT(setFAtom()) );

        // Create a label
        hBox->addWidget(new QLabel("   To Particle Type: "));

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        tParticleCob = new QComboBox(w);
        hBox->addWidget(tParticleCob);

        // Define a callback for this combo boxes
        connect( tParticleCob, SIGNAL(activated(int)), SLOT(setTAtom()) );

        // Add radiobuttons and a label
        hBox->addWidget(new QLabel("   Bond: " ));

        bondMode = new QGroupBox();
        hBox->addWidget(bondMode);

        bondMode0 = new QRadioButton("Present" );
        bondMode1 = new QRadioButton( "Absent" );

        QHBoxLayout *bondModeLayout = new QHBoxLayout(bondMode);
        bondModeLayout->addWidget(bondMode0);
        bondModeLayout->addWidget(bondMode1);

        // Define a callback for these radio buttons
        QButtonGroup *bondModeButtonGroup = new QButtonGroup();
        bondModeButtonGroup->addButton(bondMode0);
        bondModeButtonGroup->addButton(bondMode1);
        connect( bondModeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setBond(int)) );
    }

    {
        // Create a hbox that will contain various switches
        QWidget *w = new QWidget(this);
        QHBoxLayout *hBox = new QHBoxLayout(w);
        hBox->setMargin( SPACE );

        bondBox->addWidget(w, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);

        // Create a label
        colorL = new QLabel(" Color: ");
        hBox->addWidget(colorL);

        {
            // Create radiobuttons to set the color
            bondColor = new QGroupBox();
            hBox->addWidget(bondColor);

            bondColor0 = createColorRadioButton(QPixmap( whiteSquare2 ));
            bondColor1 = createColorRadioButton(QPixmap( blackSquare2 ));
            bondColor2 = createColorRadioButton(QPixmap( redSquare2 ));
            bondColor3 = createColorRadioButton(QPixmap( greenSquare2 ));
            bondColor4 = createColorRadioButton(QPixmap( blueSquare2 ));
            bondColor5 = createColorRadioButton(QPixmap( yellowSquare2 ));
            bondColor6 = createColorRadioButton(QPixmap( goldSquare2 ));
            bondColor7 = createColorRadioButton(QPixmap( silverSquare2 ));
            bondColor8 = createColorRadioButton(QPixmap( bronzeSquare2 ));

            QGridLayout *grid = new QGridLayout(bondColor);
            grid->addWidget(bondColor0, 0, 0);
            grid->addWidget(bondColor1, 0, 1);
            grid->addWidget(bondColor2, 0, 2);
            grid->addWidget(bondColor3, 1, 0);
            grid->addWidget(bondColor4, 1, 1);
            grid->addWidget(bondColor5, 1, 2);
            grid->addWidget(bondColor6, 2, 0);
            grid->addWidget(bondColor7, 2, 1);
            grid->addWidget(bondColor8, 2, 2);
        }

        // Create a label
        thickL = new QLabel("  Thickness: ");
        hBox->addWidget(thickL);

        {
            // Create radiobuttons to set the thickness
            bondThickness = new QGroupBox();
            hBox->addWidget(bondThickness);

            bondThickness0 = new QRadioButton("Line");
            bondThickness1 = new QRadioButton("Thin" );
            bondThickness2 = new QRadioButton("Medium" );
            bondThickness3 = new QRadioButton("Thick" );
            bondThickness4 = new QRadioButton("Linear Variation" );
            bondThickness5 = new QRadioButton("Quadratic Variation" );

            QGridLayout *grid = new QGridLayout(bondThickness);
            grid->addWidget(bondThickness0, 0, 0);
            grid->addWidget(bondThickness1, 0, 1);
            grid->addWidget(bondThickness2, 1, 0);
            grid->addWidget(bondThickness3, 1, 1);
            grid->addWidget(bondThickness4, 2, 0);
            grid->addWidget(bondThickness5, 2, 1);
        }

        // Create a grid to hold a check box and a spin box plus labels
        QWidget *gridW = new QWidget();
        QGridLayout *grid = new QGridLayout(gridW);
        hBox->addWidget(gridW);
        grid->setHorizontalSpacing(SPACE);
        grid->setVerticalSpacing(SPACE);

        // Create a label and a check box
        antiAlL = new QLabel(" AntiAliasing ");
        antiAliasCb = new QCheckBox();

        // Create a label and a check box
        seqBond = new QLabel(" Sequential Bonding ");
        seqBondCb = new QCheckBox();

        // Create a hbox with a label and a spin box
        bondEmL = new QLabel(" Bond Emission ");
        bondEmissionSb = new QSpinBox();
        bondEmissionSb->setMinimum(0);
        bondEmissionSb->setMaximum(100);
        bondEmissionSb->setSingleStep(10);
        bondEmissionSb->setSuffix( "%" );
        bondEmissionSb->setValue( 0 );

        grid->addWidget(antiAlL, 0, 0);
        grid->addWidget(antiAliasCb, 0, 1);
        grid->addWidget(seqBond, 1, 0);
        grid->addWidget(seqBondCb, 1, 1);
        grid->addWidget(bondEmL, 2, 0);
        grid->addWidget(bondEmissionSb, 2, 1);

        minLengthL = new QLabel("Min Length: " );

        // Create a spin box to set the min length
        bondMinLengthSb = new QFSpinBox();

        maxLengthL = new QLabel("Max Length: " );

        // Create a spin box to set the max length
        bondMaxLengthSb = new QFSpinBox();

        grid->addWidget(minLengthL, 0, 2);
        grid->addWidget(bondMinLengthSb, 0, 3);
        grid->addWidget(maxLengthL, 1, 2);
        grid->addWidget(bondMaxLengthSb, 1, 3);
    }

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));
    bondBox->addWidget(doneApplyCancel, 2 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);

    bondBox->addWidget(doneApplyCancel);

    // Set flags
    haveFEntry = false;
    haveTEntry = false;
}

// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the color
// board is launched
void BondBoard::setData( void )
{
    int i;
    aggregateData * ad = NULL;

    // Get the current settings and register
    // it using a local particle data structure
    thisPd = mainForm->getParticleData();

    // Get a list of particles that are currently rendered
    ad = mainForm->getAggregateData();

    // Make entries in the combo box -- use only particle
    // types that are really needed; otherwise the list
    // gets too long
    if (fParticleCob && tParticleCob && (*ad).numberOfParticles > 0) {

        fParticleCob->clear();
        tParticleCob->clear();

        for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
            // Check: is this particle type really needed?
            bool needed = false;
            for (int j=0;j<(*ad).numberOfParticles;j++) {
                if (typeCmp( (char *)&(*ad).particles[j].type, (char *)&(*thisPd).type[i]) == true) {
                    // Yes it is needed: set a flag
                    needed = true;
                    break;
                }
            }

            // Add the item to the list
            if (needed) {
                fParticleCob->addItem(QString( (char *)&(*thisPd).type[i]));
                tParticleCob->addItem(QString( (char *)&(*thisPd).type[i]));
                // Set flags
                haveFEntry = true;
                haveTEntry = true;
            }
        }
        fParticleCob->setMinimumSize( fParticleCob->sizeHint() );
        tParticleCob->setMinimumSize( tParticleCob->sizeHint() );
    }

    // Sort the entries alphabetically, at least approximately
    if (haveFEntry && haveTEntry) {
        for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
            for (int j=0;j<fParticleCob->count()-1;j++) {
                if (QString::compare(fParticleCob->itemText(j), fParticleCob->itemText(j+1)) > 0) {
                    fParticleCob->removeItem(j);
                    fParticleCob->insertItem(j+1,tParticleCob->itemText(j));
                    tParticleCob->removeItem(j);
                    tParticleCob->insertItem(j+1, fParticleCob->itemText(j+1));
                }
            }
        }
    }

    // Now go through the bond data structure and make new entries
    // if needed
    for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        for (int j=0;j<(*thisPd).numberOfParticleTypes;j++) {
            bool flag = false;
            for (int k=0;k<(*thisPd).particleBonds[i].numberOfNeighborBonds;k++) {
                if (typeCmp( (char *)&(*thisPd).type[j], (char *)&(*thisPd).particleBonds[i].neighborParticleType[k]) == true) {
                    // OK this bond is already defined:
                    // set a flag
                    flag = true;
                }
            }

            // If the bond is not defined, define it now
            if (flag == false) {
                int index = (*thisPd).particleBonds[i].numberOfNeighborBonds;
                if (index < BONDS_MAX) {
                    typeCopy( (char *)&(*thisPd).type[j], (char *)&(*thisPd).particleBonds[i].neighborParticleType[index] );
                    // Set some defaults
                    (*thisPd).particleBonds[i].bShowBond[index] = false;
                    (*thisPd).particleBonds[i].bMinLength[index] = 0.0;
                    (*thisPd).particleBonds[i].bMaxLength[index] = 10.0;
                    (*thisPd).particleBonds[i].bColor[index] = GREEN;
                    (*thisPd).particleBonds[i].bThickness[index] = LINE;
                    (*thisPd).particleBonds[i].bEmission[index] = 0;
                    (*thisPd).particleBonds[i].bAntiAlias[index] = true;

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
        fParticleCob->setCurrentIndex(0);
    if (haveTEntry)
        tParticleCob->setCurrentIndex(0);

    // Find out what combination of particles is set
    if (haveFEntry)
        typeCopy( qPrintable(fParticleCob->currentText()), (char *)&thisFParticle );
    if (haveTEntry)
        typeCopy( qPrintable(tParticleCob->currentText()), (char *)&thisTParticle );

    for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == true) {
                    // Found the correct entry now;
                    // adjust the presence/absence and set
                    // toggles
                    switch ((*thisPd).particleBonds[i].bShowBond[j]) {
                    case true:
                        bondMode0->setChecked( true );
                        break;
                    case false:
                        bondMode1->setChecked( true );
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
        typeCopy( qPrintable(fParticleCob->currentText()), (char *)&thisFParticle );
    if (haveTEntry)
        typeCopy( qPrintable(tParticleCob->currentText()), (char *)&thisTParticle );

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisFParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisTParticle ) == true) {

                    // Found the correct entry now:
                    // adjust the presence/absent toggle
                    switch ((*thisPd).particleBonds[i].bShowBond[j]) {
                    case true:
                        bondMode0->setChecked( true );
                        break;
                    case false:
                        bondMode1->setChecked( true );
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
        typeCopy( qPrintable(fParticleCob->currentText()), (char *)&thisFParticle );
    if (haveTEntry)
        typeCopy( qPrintable(tParticleCob->currentText()), (char *)&thisTParticle );

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == true) {

                    // Found the correct entry now:
                    // adjust the presence/absent toggle
                    switch ((*thisPd).particleBonds[i].bShowBond[j]) {
                    case true:
                        bondMode0->setChecked( true );
                        break;
                    case false:
                        bondMode1->setChecked( true );
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
        typeCopy( qPrintable(fParticleCob->currentText()), (char *)&thisFParticle );
    if (haveTEntry)
        typeCopy( qPrintable(tParticleCob->currentText()), (char *)&thisTParticle );

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == true) {
                    // Found the correct entry now;
                    // adjust the presence/absence
                    switch (radio) {
                    case 0:
                        (*thisPd).particleBonds[i].bShowBond[j] = true;
                        break;
                    case 1:
                        (*thisPd).particleBonds[i].bShowBond[j] = false;
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
    case true:
        // Indicate that a bond exists
        colorL->setDisabled( false );
        bondColor->setDisabled( false );
        thickL->setDisabled( false );
        bondThickness->setDisabled( false );
        antiAlL->setDisabled( false );
        antiAliasCb->setDisabled( false );
        seqBond->setDisabled( false );
        seqBondCb->setDisabled( false );
        bondEmL->setDisabled( false );
        bondEmissionSb->setDisabled( false );
        minLengthL->setDisabled( false );
        bondMinLengthSb->setDisabled( false );
        maxLengthL->setDisabled( false );
        bondMaxLengthSb->setDisabled( false );

        // Adjust the toggles now
        switch ((*thisPd).particleBonds[thisAtom].bColor[neighborAtom]) {
        case WHITE:
            bondColor0->setChecked( true );
            break;
        case BLACK:
            bondColor1->setChecked( true );
            break;
        case RED:
            bondColor2->setChecked( true );
            break;
        case GREEN:
            bondColor3->setChecked( true );
            break;
        case BLUE:
            bondColor4->setChecked( true );
            break;
        case YELLOW:
            bondColor5->setChecked( true );
            break;
        case GOLD:
            bondColor6->setChecked( true );
            break;
        case SILVER:
            bondColor7->setChecked( true );
            break;
        case BRONZE:
            bondColor8->setChecked( true );
            break;
        }

        switch ((*thisPd).particleBonds[thisAtom].bThickness[neighborAtom]) {
        case LINE:
            bondThickness0->setChecked( true );
            break;
        case THIN:
            bondThickness1->setChecked( true );
            break;
        case MEDIUM:
            bondThickness2->setChecked( true );
            break;
        case THICK:
            bondThickness3->setChecked( true );
            break;
        case VARYING_LINEAR:
            bondThickness4->setChecked( true );
            break;
        case VARYING_QUADRATIC:
            bondThickness5->setChecked( true );
            break;
        }

        antiAliasCb->setChecked ((*thisPd).particleBonds[thisAtom].bAntiAlias[neighborAtom] );
        seqBondCb->setChecked ((*thisPd).particleBonds[thisAtom].bSeqBond[neighborAtom] );
        bondEmissionSb->setValue( (int)floor(100.0*(*thisPd).particleBonds[thisAtom].bEmission[neighborAtom]+0.5) );
        bondMinLengthSb->setValue( (int)floor(10.0*(*thisPd).particleBonds[thisAtom].bMinLength[neighborAtom]+0.5) );
        bondMaxLengthSb->setValue( (int)floor(10.0*(*thisPd).particleBonds[thisAtom].bMaxLength[neighborAtom]+0.5) );
        break;
    case false:
        // Indicate that no bond exists
        colorL->setDisabled( true );
        bondColor->setDisabled( true );
        thickL->setDisabled( true );
        bondThickness->setDisabled( true );
        antiAlL->setDisabled( true );
        antiAliasCb->setDisabled( true );
        seqBond->setDisabled( true );
        seqBondCb->setDisabled( true );
        bondEmL->setDisabled( true );
        bondEmissionSb->setDisabled( true );
        minLengthL->setDisabled( true );
        bondMinLengthSb->setDisabled( true );
        maxLengthL->setDisabled( true );
        bondMaxLengthSb->setDisabled( true );
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
        typeCopy( qPrintable(fParticleCob->currentText()), (char *)&thisFParticle );
    if (haveTEntry)
        typeCopy( qPrintable(tParticleCob->currentText()), (char *)&thisTParticle );

    // Find out the combination of atoms to which these
    // settings apply
    for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisFParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisTParticle ) == true) {
                    // Found the correct entry now
                    fAtom = i;
                    neighborAtom = j;
                }
            }
        }
    }

    for (i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisTParticle ) == true) {
            for (int j=0;j<(*thisPd).particleBonds[i].numberOfNeighborBonds;j++) {
                if (typeCmp( (char *)&(*thisPd).particleBonds[i].neighborParticleType[j], (char *)&thisFParticle ) == true) {
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
    if (bondMode0->isChecked() == true) {
        (*thisPd).particleBonds[fAtom].bShowBond[neighborAtom] = true;
        (*thisPd).particleBonds[tAtom].bShowBond[neighborAtom2] = true;
    }
    if (bondMode1->isChecked() == true) {
        (*thisPd).particleBonds[fAtom].bShowBond[neighborAtom] = false;
        (*thisPd).particleBonds[tAtom].bShowBond[neighborAtom2] = false;
    }

    if ( bondColor0->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = WHITE;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = WHITE;
    }
    if ( bondColor1->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BLACK;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BLACK;
    }
    if ( bondColor2->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = RED;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = RED;
    }
    if ( bondColor3->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = GREEN;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = GREEN;
    }
    if ( bondColor4->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BLUE;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BLUE;
    }
    if ( bondColor5->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = YELLOW;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = YELLOW;
    }
    if ( bondColor6->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = GOLD;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = GOLD;
    }
    if ( bondColor7->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = SILVER;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = SILVER;
    }
    if ( bondColor8->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bColor[neighborAtom] = BRONZE;
        (*thisPd).particleBonds[tAtom].bColor[neighborAtom2] = BRONZE;
    }
    if ( bondThickness0->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = LINE;
        (*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = LINE;
    }
    if ( bondThickness1->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = THIN;
        (*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = THIN;
    }
    if ( bondThickness2->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = MEDIUM;
        (*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = MEDIUM;
    }
    if ( bondThickness3->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = THICK;
        (*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = THICK;
    }
    if ( bondThickness4->isChecked() == true ) {
        (*thisPd).particleBonds[fAtom].bThickness[neighborAtom] = VARYING_LINEAR;
        (*thisPd).particleBonds[tAtom].bThickness[neighborAtom2] = VARYING_LINEAR;
    }
    if ( bondThickness5->isChecked() == true ) {
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
        mainForm->statusMessage( "Saved particle data in ", filename );
    }
    free(filename);

    // Re-do the graphics, using the new data
    mainForm->updateView();
    mainForm->updateRendering();

    // Hide now
    hide();
}


// Accept the bond setting: update the rendering 
void BondBoard::bapply() {
    // Complete the current settings
    readToggles();

    // Save the particle data
    char * filename = (char *)malloc(BUFSIZ);
    sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
    if (saveParticleDataFunction( filename, thisPd ) ) {
        mainForm->statusMessage( "Saved particle data in ", filename );
    }
    free(filename);

    // Re-do the graphics, using the new data
    mainForm->updateView();
    mainForm->updateRendering();
}


// Cancel the bond setting: hide the board
void BondBoard::bcancel()
{
    // Hide now
    hide();
}
