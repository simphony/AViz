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

#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include "codeBox.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "defaultParticles.h"
#include "defaults.h"
#include "fileFunctions.h"
#include "mainForm.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "sizeBox.h"
#include "widgets/doneapplycancelwidget.h"
#include "aggregateData.h"

// Make a popup dialog box 
PolymerBoard::PolymerBoard(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle( "AViz: Set Polymer Atom Types" );

    {
        // Create a hboxlayout that will fill the first row
        hb1 = new QWidget(this);

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        atomCob = new QComboBox();
        connect( atomCob, SIGNAL(activated(int)), SLOT(setPolymerAtom()) );

        // Add a check box button
        showPolymerAtomCb = new QCheckBox("Show Polymer Atoms");
        showPolymerAtomCb->setChecked(true);
        connect( showPolymerAtomCb, SIGNAL(clicked()), this, SLOT(adjustPolymer()) );

        QHBoxLayout *hbox = new QHBoxLayout(hb1);
        hbox->addWidget(new QLabel(" Type: "));
        hbox->addWidget(atomCob);
        hbox->addStretch(1);
        hbox->addWidget(showPolymerAtomCb);
    }

    {
        // Create a hboxlayout that will fill the next row
        hb2 = new QWidget(this);

        // Add color labels
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
        colorButton = new QPushButton("Set Color...");

        // Define a callback for this button
        connect(colorButton, SIGNAL(clicked()), SLOT(setColorCb()));

        QHBoxLayout *hbox = new QHBoxLayout(hb2);
        hbox->setSpacing(0);
        hbox->addWidget(new QLabel(" Color: "));
        hbox->addWidget(colorLabel0);
        hbox->addWidget(colorLabel1);
        hbox->addWidget(colorLabel2);
        hbox->addWidget(colorLabel3);
        hbox->addWidget(colorLabel4);
        hbox->addWidget(colorLabel5);
        hbox->addWidget(colorButton);
    }


    // Create a hboxlayout that will fill the next row
    sizeBox = new SizeBox(this);

    {
        // Create a hboxlayout that will fill the next row
        hb4 = new QWidget(this);

        // Add radiobuttons and a label
        modeL = new QLabel(" Color Criterion: ");

        colorMode = new QGroupBox();
        QHBoxLayout *colorModeLayout = new QHBoxLayout(colorMode);

        colorMode0 = new QRadioButton("Type");
        colorMode1 = new QRadioButton("Position");
        colorMode2 = new QRadioButton("Property");
        colorMode3 = new QRadioButton("ColorCode");
        colorModeLayout->addWidget(colorMode0);
        colorModeLayout->addWidget(colorMode1);
        colorModeLayout->addWidget(colorMode2);
        colorModeLayout->addWidget(colorMode3);

        QButtonGroup *colorModeButtonGroup = new QButtonGroup(this);
        colorModeButtonGroup->addButton(colorMode0);
        colorModeButtonGroup->addButton(colorMode1);
        colorModeButtonGroup->addButton(colorMode2);
        colorModeButtonGroup->addButton(colorMode3);

        // Define a callback for these radio buttons
        connect(colorModeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(adjustCriterion()) );

        QHBoxLayout *hBox = new QHBoxLayout(hb4);
        hBox->addWidget(modeL);
        hBox->addWidget(colorMode);
    }

    // Create hboxlayouts that will fill the next row; these
    // are shown only when appropriate
    positionBox = new PositionBox(this);
    propertyBox = new PropertyBox(this);
    codeBox = new CodeBox(this);

    {
        hb5 = new QWidget(this);

        // Create pushbuttons that will go into the lowest row
        QPushButton * bonds = new QPushButton("Bonds...");
        connect( bonds, SIGNAL(clicked()), this, SLOT(bbonds()) );

        DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
        connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
        connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
        connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));

        QHBoxLayout *hbox = new QHBoxLayout(hb5);
        hbox->addWidget(bonds);
        hbox->addStretch(1);
        hbox->addWidget(doneApplyCancel);
    }

    // Clear the board pointer
    polymerBox = NULL;

    // Clear the color board pointer
    cb = NULL;

    // Clear internal variables
    colors = 1;
    colorPosX = colorPosY = 0;
    showColorBoard = false;
    polymerAtomRenderStyle = PADOT;
    renderQuality = LOW;

    // Set defaults appropriate for startup without data
    colorButton->setDisabled(true);
    sizeBox->setDisabled(true);
    modeL->setDisabled(true);
    colorMode->setDisabled(true);
    colorMode0->setChecked(true);

    // Build default layout
    this->buildLayout( TYPE );
}


// Set a pointer to the main form
void PolymerBoard::setMainFormAddress( MainForm * thisMF )
{
    mainForm = thisMF;
}


// Build the layout
void PolymerBoard::buildLayout( colorCriterion crit ) {

    // Destroy existing layout
    if (polymerBox) {
        delete polymerBox;
    }

    // Insert a grid that will hold control buttons
    polymerBox = new QGridLayout(this);
    polymerBox->setHorizontalSpacing(SPACE);
    polymerBox->setVerticalSpacing(SPACE);

    // Add components that are always needed
    polymerBox->addWidget(hb1, 0 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    polymerBox->addWidget(hb2, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    polymerBox->addWidget(sizeBox, 2 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    polymerBox->addWidget(hb4, 3 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    polymerBox->addWidget(hb5, 5 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);

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
        polymerBox->addWidget(positionBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case PROPERTY:
        positionBox->hide();
        propertyBox->show();
        codeBox->hide();
        polymerBox->addWidget(propertyBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case COLORCODE:
        positionBox->hide();
        propertyBox->hide();
        codeBox->show();
        polymerBox->addWidget(codeBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    }

    this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void PolymerBoard::setData()
{
    // Get a list of particles that are currently rendered
    AggregateData *ad = mainForm->getAggregateData();

    propertyBox->setPropertyInformation(ad->propertiesInformation);

    if (mainForm) {
        // Get the current settings and register
        // it using a local particle data structure
        thisPd = mainForm->getParticleData();



        // Make entries in the combo box -- use only particle
        // types that are really needed; otherwise the list
        // gets too long
        if (atomCob) {
            atomCob->clear();
            for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                // Check: is this particle type really needed?
                for (int j=0;j<(*ad).numberOfParticles;j++) {
                    if (typeCmp( (char *)&(*ad).particles[j].type, (char *)&(*thisPd).type[i])) {
                        // Add the item to the list
                        atomCob->addItem( QString( (char *)&(*thisPd).type[i]));
                        break;
                    }
                }
            }
            atomCob->setMinimumSize( atomCob->sizeHint() );
        }
    }

    // Sort the entries alphabetically, at least approximately
    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        for (int j=0;j<atomCob->count()-1;j++) {
            if (QString::compare(atomCob->itemText(j), atomCob->itemText(j+1)) > 0) {
                tag tmp;
                typeCopy( qPrintable(atomCob->itemText(j+1)), (char *)&tmp );
                atomCob->insertItem(j, QString( (char *)&tmp));
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
void PolymerBoard::setPolymerAtom()
{
    tag thisAtom;
    thisPolymerAtomIndex = -1;

    // Find out what combination of atoms is set
    typeCopy( qPrintable(atomCob->currentText()), (char *)&thisAtom);

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisAtom)) {
            // Found the correct entry
            thisPolymerAtomIndex = i;

            // Adjust the color board
            this->setColors();

            // Adjust the toggles
            bool thisShowParticle = (*thisPd).showParticle[thisPolymerAtomIndex];
            showPolymerAtomCb->setChecked( thisShowParticle );

            if( thisShowParticle) {
                if (thisPolymerAtomIndex >= 0) {
                    modeL->setDisabled(false);
                    colorMode->setDisabled(false);
                }
                else {
                    modeL->setDisabled(true);
                    colorMode->setDisabled(true);
                }
            } else {
                sizeBox->setDisabled(true);
                modeL->setDisabled(true);
                colorMode->setDisabled(true);
            }

            // Adjust the toggles
            if (polymerAtomRenderStyle != PADOT && polymerAtomRenderStyle != PALINE && thisShowParticle) {
                sizeBox->setDisabled(false);
            }
            else {
                sizeBox->setDisabled(true);
            }

            // Adjust size settings
            sizeBox->setParticle( thisPd, thisPolymerAtomIndex);

            colorCriterion colorCrit = (*thisPd).colorCrit[thisPolymerAtomIndex];
            switch (colorCrit) {
            case TYPE:
                this->buildLayout( TYPE );
                colorMode0->setChecked(true);
                colorButton->setDisabled(false);
                break;
            case POSITION:
                this->buildLayout( POSITION );
                colorMode1->setChecked(true);
                colorButton->setDisabled(false);
                break;
            case PROPERTY:
                this->buildLayout( PROPERTY );
                colorMode2->setChecked(true);
                colorButton->setDisabled(false);
                break;
            case COLORCODE:
                this->buildLayout( COLORCODE );
                colorMode3->setChecked(true);
                colorButton->setDisabled(true);
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
void PolymerBoard::adjustPolymer()
{
    // Read the toggle switch values and
    // update the particle data entry
    if (thisPolymerAtomIndex >= 0)
        (*thisPd).showParticle[thisPolymerAtomIndex] = showPolymerAtomCb->isChecked();

    if ( showPolymerAtomCb->isChecked() ) {
        if (thisPolymerAtomIndex >= 0) {
            this->buildLayout( (*thisPd).colorCrit[thisPolymerAtomIndex] );
        }
        else {
            this->buildLayout( TYPE );
            colorButton->setDisabled(false);
        }
    } else {
        this->buildLayout( TYPE );
        colorButton->setDisabled(true);
    }
}



// Adjust the board after change of color criterion
void PolymerBoard::adjustCriterion()
{
    this->readToggles();
    this->setPolymerAtom();
}


// Read the toggle switches
void PolymerBoard::readToggles()
{
    if (thisPolymerAtomIndex >= 0) {
        // Read the settings and
        // update the particle data entry
        sizeBox->readToggles( thisPd, thisPolymerAtomIndex);

        if (colorMode0->isChecked()) {
            (*thisPd).colorCrit[thisPolymerAtomIndex] = TYPE;
        }
        if (colorMode1->isChecked()) {
            (*thisPd).colorCrit[thisPolymerAtomIndex] = POSITION;
        }
        if (colorMode2->isChecked()) {
            (*thisPd).colorCrit[thisPolymerAtomIndex] = PROPERTY;
        }
        if (colorMode3->isChecked()) {
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
    showColorBoard = true;

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

                cb = new ColorBoard(this);

                if (cb) {
                    // This is an ugly hack, intended to
                    // propagate the address of the calling class
                    cb->setPolymerBoardAddress(this);
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

                cb = new ColorBoard(this);

                if (cb) {
                    // This is an ugly hack, intended to
                    // propagate the address of the calling class
                    cb->setPolymerBoardAddress(this);
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
void PolymerBoard::setDotStyle()
{	
    polymerAtomRenderStyle = PADOT;
    sizeBox->setDisabled(true);
}


// Adjust the board
void PolymerBoard::setLineStyle()
{	
    polymerAtomRenderStyle = PALINE;
    sizeBox->setDisabled(true);
}


// Adjust the board
void PolymerBoard::setCubeStyle()
{	
    polymerAtomRenderStyle = PACUBE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void PolymerBoard::setCylinderStyle()
{	
    polymerAtomRenderStyle = PACYLINDER;
    sizeBox->setDisabled(false);
}


// Adjust the board
void PolymerBoard::setConeStyle()
{	
    polymerAtomRenderStyle = PACONE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void PolymerBoard::setSphereStyle()
{	
    polymerAtomRenderStyle = PASPHERE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void PolymerBoard::setLowQuality()
{
    renderQuality = LOW;
}


// Adjust the board
void PolymerBoard::setHighQuality()
{
    renderQuality = HIGH;
}


// Adjust the board
void PolymerBoard::setFinalQuality()
{
    renderQuality = FINAL;
}


// Set a flag when the color board is closed
void PolymerBoard::closeColorBoard()
{
    showColorBoard = false;

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
