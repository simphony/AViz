//
// Implementation of atom board dialog widget class
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

#include "atomBoard.h"

#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>


#include "defaultParticles.h" // typeCopy, typeCmp
#include "mainForm.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "codeBox.h"
#include "fileFunctions.h"
#include "sizeBox.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "widgets/doneapplycancelwidget.h"

namespace {
/// creates color label and adds to @param layout
ColorLabel* createColorLabel(QHBoxLayout* layout, QWidget *parent) {
    ColorLabel *cLabel = new ColorLabel(parent);
    cLabel->setFixedHeight( LABEL_HEIGHT );
    layout->addWidget(cLabel);
    return cLabel;
}
}

// Make a popup dialog box 
AtomBoard::AtomBoard(MainForm *mainForm, QWidget * parent )
    : QDialog( parent ),
      mainForm(mainForm)
{
    setWindowTitle( "AViz: Set Atom Types" );

    // Create a widget for basic atom configuration
    {
        m_atomWidget = new QWidget( this);

        // Create a combo box that will go into the
        // second column; entries in the combo box will
        // be made later
        atomCob = new QComboBox(m_atomWidget);
        connect( atomCob, SIGNAL(activated(int)), SLOT(setAtom()) );

        // Add a check box button
        showAtomCb = new QCheckBox("Show Atoms", m_atomWidget);
        showAtomCb->setChecked(true);

        QHBoxLayout *hbox = new QHBoxLayout(m_atomWidget);
        hbox->addWidget(new QLabel(" Type: "));
        hbox->addWidget(atomCob);
        hbox->addStretch(1);
        hbox->addWidget(showAtomCb);

        // Define a callback for this toggle switch
        connect( showAtomCb, SIGNAL(clicked()), this, SLOT(adjustAtom()) );
    }

    // Create a widget for colors that fill the next row
    {
        m_colorWidget = new QWidget(this);

        QHBoxLayout *hbox = new QHBoxLayout(m_colorWidget);
        hbox->setSpacing(0);

        // Add a label
        hbox->addWidget(new QLabel(" Color: "));

        // 6 color labels to display the various color
        // schemes
        colorLabel0 = createColorLabel(hbox, m_colorWidget);
        colorLabel1 = createColorLabel(hbox, m_colorWidget);
        colorLabel2 = createColorLabel(hbox, m_colorWidget);
        colorLabel3 = createColorLabel(hbox, m_colorWidget);
        colorLabel4 = createColorLabel(hbox, m_colorWidget);
        colorLabel5 = createColorLabel(hbox, m_colorWidget);

        // Add a push button to edit the color
        colorButton = new QPushButton("Set Color...", m_colorWidget);
        hbox->addWidget(colorButton);
        connect( colorButton, SIGNAL(clicked()), this, SLOT(setColorCb()) );
    }

    // Create a sizebox widget that will
    // be added to next row in @see buildLayout()
    sizeBox = new SizeBox( this );

    // Create a widget for the next row
    {
        m_colorCriterionWidget = new QWidget(this);

        // Add radiobuttons and a label
        QLabel *modeL = new QLabel(" Color Criterion: ");

        QGroupBox *colorMode = new QGroupBox();
        QHBoxLayout *colorModeLayout = new QHBoxLayout(colorMode);

        colorMode0 = new QRadioButton("Type");
        colorMode1 = new QRadioButton("Position");
        colorMode2 = new QRadioButton("Property");
        colorMode3 = new QRadioButton("ColorCode");
        colorModeLayout->addWidget(colorMode0);
        colorModeLayout->addWidget(colorMode1);
        colorModeLayout->addWidget(colorMode2);
        colorModeLayout->addWidget(colorMode3);

        // Define a callback for these radio buttons
        QButtonGroup *colorModeButtonGroup = new QButtonGroup(this);
        colorModeButtonGroup->addButton(colorMode0);
        colorModeButtonGroup->addButton(colorMode1);
        colorModeButtonGroup->addButton(colorMode2);
        colorModeButtonGroup->addButton(colorMode3);
        connect(colorModeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(adjustCriterion()) );

        QHBoxLayout *hbox = new QHBoxLayout(m_colorCriterionWidget);
        hbox->addWidget(modeL);
        hbox->addWidget(colorMode);
    }


    // Create hboxlayouts that will fill the next row; these
    // are shown only when appropriate
    positionBox = new PositionBox(this);
    propertyBox = new PropertyBox(this);
    codeBox = new CodeBox(this);


    // Create widget (of buttons) for the lowest row
    {
        m_BondsDoneApplyWidget = new QWidget(this);

        // Create pushbuttons that will go into the lowest row
        QPushButton * bonds = new QPushButton("Bonds...");
        connect( bonds, SIGNAL(clicked()), this, SLOT(bbonds()) );

        DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
        connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
        connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
        connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));

        QHBoxLayout *hbox = new QHBoxLayout(m_BondsDoneApplyWidget);
        hbox->addWidget(bonds);
        hbox->addStretch(1);
        hbox->addWidget(doneApplyCancel);
    }

    // Clear the board pointer
    atomBox = NULL;

    // Clear the color board pointer
    cb = NULL;

    // Clear internal variables
    colors = 1;
    colorPosX = colorPosY = 0;
    showColorBoard = FALSE;
    atomRenderStyle = ADOT;
    renderQuality = LOW;

    // Set defaults appropriate for startup without data
    colorButton->setDisabled(true);
    sizeBox->setDisabled(true);
    m_colorCriterionWidget->setDisabled(true);

    // Build default layout
    this->buildLayout( TYPE );
}

// Build the layout
void AtomBoard::buildLayout( colorCriterion crit )  {
    // Destroy existing layout
    if (atomBox) {
        atomBox->~QGridLayout();
    }

    // Insert a grid that will hold control buttons
    atomBox = new QGridLayout(this);
    atomBox->setHorizontalSpacing(SPACE);
    atomBox->setVerticalSpacing(SPACE);

    // Add components that are always needed
    atomBox->addWidget(m_atomWidget, 0 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    atomBox->addWidget(m_colorWidget, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    atomBox->addWidget(sizeBox, 2 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    atomBox->addWidget(m_colorCriterionWidget, 3 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
    atomBox->addWidget(m_BondsDoneApplyWidget, 5 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);


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
        atomBox->addWidget(positionBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case PROPERTY:
        positionBox->hide();
        propertyBox->show();
        codeBox->hide();
        atomBox->addWidget(propertyBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    case COLORCODE:
        positionBox->hide();
        propertyBox->hide();
        codeBox->show();
        atomBox->addWidget(codeBox, 4 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
        break;
    }

    this->adjustSize();
}


// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void AtomBoard::setData( void )
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
                    atomCob->addItem(QString( (char *)&(*thisPd).type[i]));
                }
            }
            atomCob->setMinimumSize( atomCob->sizeHint() );
        }
    }

    // Sort the entries alphabetically, at least approximately
    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        for (int j=0;j<atomCob->count()-1;j++) {
            if (QString::compare(atomCob->itemText(j), atomCob->itemText(j+1)) > 0) {
                typeCopy( qPrintable(atomCob->itemText(j+1)), (char *)&tmp );
                atomCob->insertItem(j, QString( (char *)&tmp));
                atomCob->removeItem(j+2);
            }
        }
    }

    // Set the first particle type as the current type
    if ((*ad).numberOfParticles > 0) {
        thisAtomIndex = 0;
        this->setAtom();
    }
    else {
        thisAtomIndex = -1;
    }
}


// Set the atom type under consideration: adjust the toggle switch
void AtomBoard::setAtom( void )
{
    tag thisAtom;
    thisAtomIndex = -1;

    // Find out what combination of atoms is set
    typeCopy( qPrintable(atomCob->currentText()), (char *)&thisAtom);

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisAtom) == TRUE) {
            // Found the correct entry
            thisAtomIndex = i;

            // Adjust the color board
            this->setColors();

            // Adjust the toggles
            bool thisShowParticle = (*thisPd).showParticle[thisAtomIndex];
            showAtomCb->setChecked( thisShowParticle );

            switch( thisShowParticle) {
            case TRUE:
                if (thisAtomIndex >= 0) {
                    m_colorCriterionWidget->setDisabled(false);
                }
                else {
                    m_colorCriterionWidget->setDisabled(true);
                }
                break;
            case FALSE:
                sizeBox->setDisabled(true);
                m_colorCriterionWidget->setDisabled(true);
                break;
            }

            // Adjust the toggles
            if (atomRenderStyle != ADOT && atomRenderStyle != ALINE && thisShowParticle) {
                sizeBox->setDisabled(false);
            }
            else {
                sizeBox->setDisabled(true);
            }

            // Adjust size settings
            sizeBox->setParticle( thisPd, thisAtomIndex );

            colorCriterion colorCrit = (*thisPd).colorCrit[thisAtomIndex];

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

            positionBox->setParticle( thisPd, thisAtomIndex );
            propertyBox->setParticle( thisPd, thisAtomIndex );
            codeBox->setParticle( thisPd, thisAtomIndex );
        }
    }
}


// Adjust the settings, using the local
// copy of the particle data structure 
void AtomBoard::adjustAtom() {
    // Read the toggle switch values and
    // update the particle data entry
    if (thisAtomIndex >= 0)
        (*thisPd).showParticle[thisAtomIndex] = showAtomCb->isChecked();

    switch ( showAtomCb->isChecked() ) {
    case TRUE:
        if (thisAtomIndex >= 0) {
            this->buildLayout( (*thisPd).colorCrit[thisAtomIndex] );
            colorButton->setDisabled(false);
        }
        else {
            this->buildLayout( TYPE );
            colorButton->setDisabled(true);
        }
        break;
    case FALSE:
        this->buildLayout( TYPE );
        colorButton->setDisabled(true);
        break;
    }
}



// Adjust the board after change of color criterion
void AtomBoard::adjustCriterion( void )
{
    this->readToggles();
    this->setAtom();
}


// Read the toggle switches
void AtomBoard::readToggles( void )
{
    if (thisAtomIndex >= 0) {
        // Read the settings and
        // update the particle data entry
        sizeBox->readToggles( thisPd, thisAtomIndex );

        if (colorMode0->isChecked() == TRUE) {
            (*thisPd).colorCrit[thisAtomIndex] = TYPE;
        }
        if (colorMode1->isChecked() == TRUE) {
            (*thisPd).colorCrit[thisAtomIndex] = POSITION;
        }
        if (colorMode2->isChecked() == TRUE) {
            (*thisPd).colorCrit[thisAtomIndex] = PROPERTY;
        }
        if (colorMode3->isChecked() == TRUE) {
            (*thisPd).colorCrit[thisAtomIndex] = COLORCODE;
        }
    }

    if (thisAtomIndex >= 0) {
        positionBox->readToggles( thisPd, thisAtomIndex );
        propertyBox->readToggles( thisPd, thisAtomIndex );
        codeBox->readToggles( thisPd, thisAtomIndex );
    }
}


// Launch the color board to adjust colors
void AtomBoard::setColorCb()
{
    // Set a flag
    showColorBoard = TRUE;

    // Adjust the color board
    this->setColors();
}


// Adjust the colors of the color board
void AtomBoard::setColors()
{
    float red, green, blue;
    float redMin, redMax;
    float greenMin, greenMax;
    float blueMin, blueMax;
    red = green = blue = 0.0;
    redMin = redMax = 0.0;
    greenMin = greenMax = 0.0;
    blueMin = blueMax = 0.0;

    if (thisAtomIndex >= 0) {

        // Get the current particle data and show the
        // color panel
        switch ((*thisPd).colorCrit[thisAtomIndex]) {
        case TYPE:
            // Only one color is needed
            red = (*thisPd).particleColors[thisAtomIndex].red;
            green = (*thisPd).particleColors[thisAtomIndex].green;
            blue = (*thisPd).particleColors[thisAtomIndex].blue;
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
                    cb->setAtomBoardAddress(this);
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
            redMin = (*thisPd).particleColors[thisAtomIndex].redMin;
            greenMin = (*thisPd).particleColors[thisAtomIndex].greenMin;
            blueMin = (*thisPd).particleColors[thisAtomIndex].blueMin;
            redMax = (*thisPd).particleColors[thisAtomIndex].redMax;
            greenMax = (*thisPd).particleColors[thisAtomIndex].greenMax;
            blueMax = (*thisPd).particleColors[thisAtomIndex].blueMax;
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
                    cb->setAtomBoardAddress(this);
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
void AtomBoard::getColors( float r0, float g0, float b0, float r1, float g1, float b1 )
{
    if (thisAtomIndex >= 0) {
        if ((*thisPd).colorCrit[thisAtomIndex] == TYPE) {
            // Only one color are sent
            (*thisPd).particleColors[thisAtomIndex].red = r0;
            (*thisPd).particleColors[thisAtomIndex].green = g0;
            (*thisPd).particleColors[thisAtomIndex].blue = b0;
            colorLabel0->setColor( r0, g0, b0 );
            colorLabel1->setColor( r0, g0, b0 );
            colorLabel2->setColor( r0, g0, b0 );
            colorLabel3->setColor( r0, g0, b0 );
            colorLabel4->setColor( r0, g0, b0 );
            colorLabel5->setColor( r0, g0, b0 );
        }
        else {
            // A spectrum of colors is sent
            (*thisPd).particleColors[thisAtomIndex].redMin = r0;
            (*thisPd).particleColors[thisAtomIndex].greenMin = g0;
            (*thisPd).particleColors[thisAtomIndex].blueMin = b0;
            (*thisPd).particleColors[thisAtomIndex].redMax = r1;
            (*thisPd).particleColors[thisAtomIndex].greenMax = g1;
            (*thisPd).particleColors[thisAtomIndex].blueMax = b1;
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
void AtomBoard::setDotStyle( void )
{	
    atomRenderStyle = ADOT;
    sizeBox->setDisabled(true);
}


// Adjust the board
void AtomBoard::setLineStyle( void )
{	
    atomRenderStyle = ALINE;
    sizeBox->setDisabled(true);
}


// Adjust the board
void AtomBoard::setCubeStyle( void )
{	
    atomRenderStyle = ACUBE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void AtomBoard::setCylinderStyle( void )
{	
    atomRenderStyle = ACYLINDER;
    sizeBox->setDisabled(false);
}


// Adjust the board
void AtomBoard::setConeStyle( void )
{	
    atomRenderStyle = ACONE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void AtomBoard::setSphereStyle( void )
{	
    atomRenderStyle = ASPHERE;
    sizeBox->setDisabled(false);
}


// Adjust the board
void AtomBoard::setLowQuality( void )
{
    renderQuality = LOW;
}


// Adjust the board
void AtomBoard::setHighQuality( void )
{
    renderQuality = HIGH;
}


// Adjust the board
void AtomBoard::setFinalQuality( void )
{
    renderQuality = FINAL;
}


// Set a flag when the color board is closed
void AtomBoard::closeColorBoard( void )
{
    showColorBoard = FALSE;

    if (cb)
        cb->~ColorBoard();

    cb = NULL;
    colors = 0;
}


// Remember the board position when the color board is closed
void AtomBoard::getColorBoardPos( int posX, int posY )
{
    colorPosX = posX;
    colorPosY = posY;
}


// Launch the bond board
void AtomBoard::bbonds()
{
    if (mainForm)
        mainForm->launchBonds();
}


// Complete the particle switch setting: save the particle data and hide 
// the board
void AtomBoard::bdone()
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
void AtomBoard::bapply()
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
void AtomBoard::bcancel()
{
    // Set a flag
    this->closeColorBoard();

    // Hide now
    hide();
}
