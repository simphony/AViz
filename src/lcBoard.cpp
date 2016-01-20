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

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>

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
#include "widgets/doneapplycancelwidget.h"
#include "aggregateData.h"


// Make a popup dialog box 
LcBoard::LcBoard(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle( "AViz: Set Liquid Crystals" );

    {
        // Create a hboxlayout that will fill the first row
        hb1 = new QWidget(this);

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        lcCob = new QComboBox();
        connect( lcCob, SIGNAL(activated(int)), SLOT(setLc()) );

        // Add a check box button
        showLcCb = new QCheckBox("Show Liquid Crystals");
        showLcCb->setChecked(true);
        connect( showLcCb, SIGNAL(clicked()), this, SLOT(adjustLc()) );

        QHBoxLayout *hbox = new QHBoxLayout(hb1);
        hbox->addWidget(new QLabel(" Type: "));
        hbox->addWidget(lcCob);
        hbox->addStretch(1);
        hbox->addWidget(showLcCb);
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
    typeColorNumberBox = new TypeColorNumberBox( this, 3 );

    // Create a box that will fill the next row
    lineTypeBox = new LineTypeBox( this );

    {
        hb5 = new DoneApplyCancelWidget(this);
        connect(hb5, SIGNAL(done()), this, SLOT(bdone()) );
        connect(hb5, SIGNAL(applied()), this, SLOT(bapply()) );
        connect(hb5, SIGNAL(canceled()), this, SLOT(bcancel()));
    }

    // Clear the board pointer
    lcBox = NULL;

    // Clear the color board pointer
    cb = NULL;

    // Clear internal variables
    colors = 1;
    colorPosX = colorPosY = 0;
    showColorBoard = false;
    lcRenderStyle = LDOT;
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
void LcBoard::setMainFormAddress( MainForm * thisMF )
{
    mainForm = thisMF;
}


// Build the layout
void LcBoard::buildLayout( colorCriterion crit ) {
    // Destroy existing layout
    if (lcBox) {
        lcBox->~QGridLayout();
    }

    // Insert a grid that will hold control buttons
    lcBox = new QGridLayout(this);
    lcBox->setHorizontalSpacing(SPACE);
    lcBox->setVerticalSpacing(SPACE);

    // Add components that are always needed
    lcBox->addWidget(hb1, 0 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    lcBox->addWidget(hb2, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    lcBox->addWidget(sizeBox, 2 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    lcBox->addWidget(hb4, 3 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    lcBox->addWidget(lineTypeBox, 5 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    lcBox->addWidget(hb5, 6 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);


    // Add additional components
    switch (crit) {
    case TYPE:
        positionBox->hide();
        propertyBox->hide();
        codeBox->hide();
        typeColorNumberBox->show();
        lcBox->addWidget(typeColorNumberBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case POSITION:
        positionBox->show();
        propertyBox->hide();
        codeBox->hide();
        typeColorNumberBox->hide();
        lcBox->addWidget(positionBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case PROPERTY:
        positionBox->hide();
        propertyBox->show();
        codeBox->hide();
        typeColorNumberBox->hide();
        lcBox->addWidget(propertyBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case COLORCODE:
        positionBox->hide();
        propertyBox->hide();
        codeBox->show();
        typeColorNumberBox->hide();
        lcBox->addWidget(codeBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    }

    this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void LcBoard::setData()
{
    tag tmp;

    // Get a list of particles that are currently rendered
    auto ad = mainForm->getAggregateData();

    if (mainForm) {
        // Get the current switch settings and register
        // it using a local particle data structure
        thisPd = mainForm->getParticleData();

        // Make entries in the combo box -- use only atoms
        // types that are really needed; otherwise the list
        // gets too long
        if (lcCob) {
            lcCob->clear();
            for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
                // Check: is this particle type really needed?
                for (int j=0;j<(*ad).numberOfParticles;j++) {
                    if (typeCmp( (char *)&(*ad).particles[j].type, (char *)&(*thisPd).type[i])) {
                        // Add the item to the list
                        lcCob->addItem( QString( (char *)&(*thisPd).type[i]));
                        break;
                    }
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
void LcBoard::setLc()
{
    tag thisLc;
    thisLcIndex = -1;

    // Find out what combination of atoms is set
    typeCopy( qPrintable(lcCob->currentText()), (char *)&thisLc );

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisLc )) {
            // Found the correct entry
            thisLcIndex = i;

            // Adjust the color board
            this->setColors();

            // Adjust the toggles
            bool thisShowParticle = (*thisPd).showParticle[thisLcIndex];
            showLcCb->setChecked( thisShowParticle);

            if( thisShowParticle ) {
                if (thisLcIndex >= 0) {
                    modeL->setDisabled(false);
                    colorMode->setDisabled(false);
                }
                else {
                    modeL->setDisabled(true);
                    colorMode->setDisabled(true);
                }
            } else {
                modeL->setDisabled(true);
                colorMode->setDisabled(true);
            }

            // Adjust the toggles
            if (lcRenderStyle != LDOT && lcRenderStyle != LLINE
                    && thisShowParticle) {
                sizeBox->setDisabled(false);
            }
            else {
                sizeBox->setDisabled(true);
            }

            colorCriterion colorCrit = (*thisPd).colorCrit[thisLcIndex];
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
void LcBoard::adjustLc()
{
    // Read the toggle switch values and
    // update the atom data entry
    (*thisPd).showParticle[thisLcIndex] = showLcCb->isChecked();

    // Activate or deactivate the remaining radio buttons
    if ( showLcCb->isChecked() ) {
        if (thisLcIndex >= 0) {
            this->buildLayout( (*thisPd).colorCrit[thisLcIndex] );
        }
        else {
            this->buildLayout( TYPE );
            colorButton->setDisabled(false);
        }
    } else {
        this->buildLayout( TYPE );
        colorButton->setDisabled(false);
    }
}


// Adjust the atom switch settings, using the local
// copy of the atom data structure
void LcBoard::adjustCriterion()
{
    this->readToggles();
    this->setLc();
}


// Read the toggle switches and the emission text window
void LcBoard::readToggles()
{
    if (thisLcIndex >= 0) {
        // Read the settings and
        // update the particle data entry
        sizeBox->readToggles( thisPd, thisLcIndex );

        if (colorMode0->isChecked()) {
            (*thisPd).colorCrit[thisLcIndex] = TYPE;
        }
        if (colorMode1->isChecked()) {
            (*thisPd).colorCrit[thisLcIndex] = POSITION;
        }
        if (colorMode2->isChecked()) {
            (*thisPd).colorCrit[thisLcIndex] = PROPERTY;
        }
        if (colorMode3->isChecked()) {
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
    showColorBoard = true;

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
void LcBoard::setDotStyle()
{
    lcRenderStyle = LDOT;
    sizeBox->setDisabled(true);
    lineTypeBox->setDisabled(true );
}


// Adjust the board
void LcBoard::setLineStyle()
{
    lcRenderStyle = LLINE;
    sizeBox->setDisabled(true);
    lineTypeBox->setDisabled(false,true);
}


// Adjust the board
void LcBoard::setCubeStyle()
{
    lcRenderStyle = LCUBE;
    sizeBox->setDisabled(false);
    lineTypeBox->setDisabled(false);
}


// Adjust the board
void LcBoard::setCylinderStyle()
{
    lcRenderStyle = LCYLINDER;
    sizeBox->setDisabled(false);
    lineTypeBox->setDisabled(false);
}


// Adjust the board
void LcBoard::setConeStyle()
{
    lcRenderStyle = LCONE;
    sizeBox->setDisabled(false);
    lineTypeBox->setDisabled(false);
}


// Adjust the board
void LcBoard::setSphereStyle()
{
    lcRenderStyle = LSPHERE;
    sizeBox->setDisabled(false);
    lineTypeBox->setDisabled(false);
}


// Adjust the board
void LcBoard::setLowQuality()
{
    renderQuality = LOW;
}


// Adjust the board
void LcBoard::setHighQuality()
{
    renderQuality = HIGH;
}


// Adjust the board
void LcBoard::setFinalQuality()
{
    renderQuality = FINAL;
}


// Set a flag when the color board is closed
void LcBoard::closeColorBoard()
{
    showColorBoard = false;

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
