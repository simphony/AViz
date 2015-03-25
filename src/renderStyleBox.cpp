//
// Implementation of render style box class
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

#include "renderStyleBox.h"

#include <cmath>

#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>

#include "defaults.h"
#include "mainForm.h"


// Make a hbox that will hold the render quality elements
RenderBox::RenderBox(MainForm * mainForm, QWidget * parent)
    : QWidget(parent), mainForm(mainForm)
{
    // Create a horizontal box
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setMargin( SPACE );

    // Add labels and combo boxes
    hbox->addWidget(new QLabel(" Mode: "));

    // Add a selector to choose rendering style
    modeCob = new QComboBox();
    modeCob->addItem( "Atom" );
    modeCob->addItem( "Spin" );
    modeCob->addItem( "LiquidCrystal" );
    modeCob->addItem( "Polymer" );
    modeCob->addItem( "Pore" );
    hbox->addWidget(modeCob);

    // Define a callback for that combo box
    connect( modeCob, SIGNAL(activated(int)), this, SLOT(renderModeCb()) );

    hbox->addWidget(new QLabel(" Style: "));


    // Add a selector to choose rendering style
    styleCob = new QComboBox();
    hbox->addWidget(styleCob);
    styleCob->addItem( "Dots" );
    styleCob->addItem( "Lines" );
    styleCob->addItem( "Cubes" );
    styleCob->addItem( "Cylinders" );
    styleCob->addItem( "Cones" );
    styleCob->addItem( "Spheres" );

    // Define two callbacks for that combo box
    connect( styleCob, SIGNAL(activated(int)), this, SLOT(renderStyleCb()) );
    connect( styleCob, SIGNAL(activated(int)), this, SLOT(updateRendering()) );

    // Add a label and a spin box to set the size
    hbox->addWidget(new QLabel(" Size: "));

    renderSSb = new QSpinBox();
    renderSSb->setMinimum(RENDERS_MIN);
    renderSSb->setMaximum(RENDERS_MAX);
    renderSSb->setSingleStep(RENDERS_STEP);
    renderSSb->setSuffix( "%" );
    renderSSb->setValue( RENDERS0 );
    hbox->addWidget(renderSSb);

    // Define a callback for that spin box
    connect( renderSSb, SIGNAL(valueChanged(int)), this, SLOT(updateRendering()) );

    // Add a label and a selector to choose rendering quality
    hbox->addWidget(new QLabel(" Quality: "));

    qualityCob = new QComboBox();
    qualityCob->addItem( "Low" );
    qualityCob->addItem( "High" );
    qualityCob->addItem( "Final" );
    qualityCob->setMinimumSize( qualityCob->sizeHint() );
    hbox->addWidget(qualityCob);

    // Define two callbacks for that combo box
    connect( qualityCob, SIGNAL(activated(int)), this, SLOT(renderQualityCb()) );
    connect( qualityCob, SIGNAL(activated(int)), this, SLOT(updateRendering()) );

    // Reset a flag
    setValueFlag = false;
}

// Set the render parameters
void RenderBox::setValue( viewParam vp )
{
    // Set a flag to prevent closing of
    // boards
    setValueFlag = true;

    // Set the render mode
    switch (vp.renderMode) {
    case ATOMS:
        modeCob->setCurrentIndex( 0 );

        // Update style combo box
        this->renderModeCb();
        switch (vp.atomRenderStyle) {
        case ADOT:
            styleCob->setCurrentIndex( 0 );
            break;
        case ACUBE:
            styleCob->setCurrentIndex( 1 );
            break;
        case ASPHERE:
            styleCob->setCurrentIndex( 2 );
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            break;
        }
        break;
    case SPINS:
        modeCob->setCurrentIndex( 1 );

        // Update style combo box
        this->renderModeCb();
        switch (vp.spinRenderStyle) {
        case SLINE:
            styleCob->setCurrentIndex( 0 );
            break;
        case SCONE:
            styleCob->setCurrentIndex( 1 );
            break;
        case SSPHERE:
            styleCob->setCurrentIndex( 2 );
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            break;
        }
        break;
    case LIQUID_CRYSTALS:
        modeCob->setCurrentIndex( 2 );

        // Update style combo box
        this->renderModeCb();
        switch (vp.lcRenderStyle) {
        case LDOT:
            styleCob->setCurrentIndex( 0 );
            break;
        case LLINE:
            styleCob->setCurrentIndex( 1 );
            break;
        case LCONE:
            styleCob->setCurrentIndex( 2 );
            break;
        case LCYLINDER:
            styleCob->setCurrentIndex( 3 );
            break;
        default:
            styleCob->setCurrentIndex( 1 );
            break;
        }
        break;
    case POLYMERS:
        modeCob->setCurrentIndex( 3 );

        // Update style combo box
        this->renderModeCb();
        switch (vp.polymerAtomRenderStyle) {
        case PADOT:
            styleCob->setCurrentIndex( 0 );
            break;
        case PACUBE:
            styleCob->setCurrentIndex( 1 );
            break;
        case PASPHERE:
            styleCob->setCurrentIndex( 2 );
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            break;
        }
        break;
    case PORES:
        modeCob->setCurrentIndex( 4 );

        // Update style combo box
        this->renderModeCb();
        switch (vp.poreRenderStyle) {
        case PDOT:
            styleCob->setCurrentIndex( 0 );
            break;
        case PLINE:
            styleCob->setCurrentIndex( 1 );
            break;
        case PCYLINDER:
            styleCob->setCurrentIndex( 2 );
            break;
        default:
            styleCob->setCurrentIndex( 1 );
            break;
        }
        break;
    }

    // Update remaining controls
    this->renderStyleCb();

    // Set the render size
    renderSSb->setValue( (int)floor( 100.0*vp.renderSize) );

    // Set the render quality
    switch (vp.renderQuality) {
    case LOW:
        qualityCob->setCurrentIndex( 0 );
        break;
    case HIGH:
        qualityCob->setCurrentIndex( 1 );
        break;
    case FINAL:
        qualityCob->setCurrentIndex( 2 );
        break;
    }

    // Reset the flag
    setValueFlag = false;
}


// Return the render mode
void RenderBox::getRenderMode( viewParam * vp )
{
    switch (modeCob->currentIndex()) {
    case 0:
        (*vp).renderMode = ATOMS;
        break;
    case 1:
        (*vp).renderMode = SPINS;
        break;
    case 2:
        (*vp).renderMode = LIQUID_CRYSTALS;
        break;
    case 3:
        (*vp).renderMode = POLYMERS;
        break;
    case 4:
        (*vp).renderMode = PORES;
        break;
    }
}


// Return the render style
void RenderBox::getRenderStyle( viewParam * vp )
{
    const int item = styleCob->currentIndex();

    if ((*vp).renderMode == ATOMS) {
        switch (item) {
        case 0:
            (*vp).atomRenderStyle = ADOT;
            break;
        case 1:
            (*vp).atomRenderStyle = ACUBE;
            break;
        case 2:
            (*vp).atomRenderStyle = ASPHERE;
            break;
        default:
            (*vp).atomRenderStyle = ADOT;
            break;
        }
    }

    if ((*vp).renderMode == SPINS) {
        switch (item) {
        case 0:
            (*vp).spinRenderStyle = SLINE;
            break;
        case 1:
            (*vp).spinRenderStyle = SCONE;
            break;
        case 2:
            (*vp).spinRenderStyle = SSPHERE;
            break;
        default:
            (*vp).spinRenderStyle = SLINE;
            break;
        }
    }

    if ((*vp).renderMode == LIQUID_CRYSTALS) {
        switch (item) {
        case 0:
            (*vp).lcRenderStyle = LDOT;
            break;
        case 1:
            (*vp).lcRenderStyle = LLINE;
            break;
        case 2:
            (*vp).lcRenderStyle = LCONE;
            break;
        case 3:
            (*vp).lcRenderStyle = LCYLINDER;
            break;
        default:
            (*vp).lcRenderStyle = LLINE;
            break;
        }
    }

    if ((*vp).renderMode == POLYMERS) {
        switch (item) {
        case 0:
            (*vp).polymerAtomRenderStyle = PADOT;
            break;
        case 1:
            (*vp).polymerAtomRenderStyle = PACUBE;
            break;
        case 2:
            (*vp).polymerAtomRenderStyle = PASPHERE;
            break;
        default:
            (*vp).polymerAtomRenderStyle = PADOT;
            break;
        }
    }

    if ((*vp).renderMode == PORES) {
        switch (item) {
        case 0:
            (*vp).poreRenderStyle = PDOT;
            break;
        case 1:
            (*vp).poreRenderStyle = PLINE;
            break;
        case 2:
            (*vp).poreRenderStyle = PCYLINDER;
            break;
        default:
            (*vp).poreRenderStyle = PDOT;
            break;
        }
    }
}


// Return the render size
float RenderBox::getRenderSize() const {
    return (float)renderSSb->value()/100.0;
}


// Return the render quality 
void RenderBox::getRenderQuality( viewParam * vp )
{
    const int item = qualityCob->currentIndex();
    switch (item) {
    case 0:
        (*vp).renderQuality = LOW;
        break;
    case 1:
        (*vp).renderQuality = HIGH;
        break;
    case 2:
        (*vp).renderQuality = FINAL;
        break;
    }
}


// Depending on the setting of the mode combo box, activate 
// or deactivate some style combo box entries
void RenderBox::renderModeCb() {
    // Read the current parameter settings

    viewParam * vp = mainForm->getViewParam();

    const int item = modeCob->currentIndex();
    switch (item) {
    case 0:
        // Atom style chosen
        mainForm->setAtomMenus();

        // Re-build the style combo box
        styleCob->clear();
        styleCob->addItem( "Dots" );
        styleCob->addItem( "Cubes" );
        styleCob->addItem( "Spheres" );

        // Set the style value
        switch ((*vp).atomRenderStyle) {
        case ADOT:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastDotStyle();
            break;
        case ACUBE:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastCubeStyle();
            break;
        case ASPHERE:
            styleCob->setCurrentIndex( 2 );
            mainForm->broadcastSphereStyle();
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastDotStyle();
            break;
        }
        break;
    case 1:
        // Spin style chosen
        mainForm->setSpinMenus();

        // Re-build the style combo box
        styleCob->clear();
        styleCob->addItem( "Lines" );
        styleCob->addItem( "Cones" );
        styleCob->addItem( "Pins" );		// These are really spheres

        // Set the style value
        switch ((*vp).spinRenderStyle) {
        case SLINE:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastLineStyle();
            break;
        case SCONE:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastConeStyle();
            break;
        case SSPHERE:
            styleCob->setCurrentIndex( 2 );
            mainForm->broadcastSphereStyle();
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastLineStyle();
            break;
        }
        break;
    case 2:
        // Liquid crystal style chosen
        mainForm->setLcMenus();

        // Re-build the style combo box
        styleCob->clear();
        styleCob->addItem( "Dots" );
        styleCob->addItem( "Lines" );
        styleCob->addItem( "Cones" );
        styleCob->addItem( "Cylinders" );

        // Set the style value
        switch ((*vp).lcRenderStyle) {
        case LDOT:
            styleCob->setCurrentIndex( 0 );
            break;
        case LLINE:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastLineStyle();
            break;
        case LCONE:
            styleCob->setCurrentIndex( 2 );
            mainForm->broadcastConeStyle();
            break;
        case LCYLINDER:
            styleCob->setCurrentIndex( 3 );
            mainForm->broadcastCylinderStyle();
            break;
        default:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastLineStyle();
            break;
        }
        break;
    case 3:
        // Polymer style chosen
        mainForm->setPolymerMenus();


        // Re-build the style combo box
        styleCob->clear();
        styleCob->addItem( "Dots" );
        styleCob->addItem( "Cubes" );
        styleCob->addItem( "Spheres" );

        // Set the style value
        switch ((*vp).polymerAtomRenderStyle) {
        case PADOT:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastDotStyle();
            break;
        case PACUBE:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastCubeStyle();
            break;
        case PASPHERE:
            styleCob->setCurrentIndex( 2 );
            mainForm->broadcastSphereStyle();
            break;
        default:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastDotStyle();
            break;
        }
        break;
    case 4:
        // Pore style chosen
        mainForm->setPoreMenus();

        // Re-build the style combo box
        styleCob->clear();
        styleCob->addItem( "Dots" );
        styleCob->addItem( "Lines" );
        styleCob->addItem( "Cylinders" );

        // Set the style value
        switch ((*vp).poreRenderStyle) {
        case PDOT:
            styleCob->setCurrentIndex( 0 );
            mainForm->broadcastDotStyle();
            break;
        case PLINE:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastLineStyle();
            break;
        case PCYLINDER:
            styleCob->setCurrentIndex( 2 );
            mainForm->broadcastCylinderStyle();
            break;
        default:
            styleCob->setCurrentIndex( 1 );
            mainForm->broadcastLineStyle();
            break;
        }
        break;
    }

    // Update also other controls
    this->renderStyleCb();

    // Update the rendering -- no need to do this when
    // the settings have been sent by the main board
    if (!setValueFlag)
        this->updateRendering();

    // Close any open boards that refer to other render modes
    if (!setValueFlag) {
        mainForm->closeModeBoards();
    }
}


// Depending on the setting of the style combo box, activate 
// or deactivate some menu entries
void RenderBox::renderStyleCb() {
    const int itemM = modeCob->currentIndex();
    const int itemS = styleCob->currentIndex();

    // Should the quality and size controls be disactivated?
    switch (itemS) {
    case 0:
        switch (itemM) {
        case 0:
            // Atom mode is chosen
            // -> dot style is chosen
            renderSSb->setDisabled( true );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();

            mainForm->broadcastDotStyle();
            break;
        case 1:
            // Spin mode is chosen
            // -> line style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastLineStyle();
            break;
        case 2:
            // Liquid crystal mode is chosen
            // -> dot style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastDotStyle();
            break;
        case 3:
            // Polymer mode is chosen
            // -> dot style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastDotStyle();
            break;
        case 4:
            // Pore mode is chosen ->
            // dot style is chosen
            renderSSb->setDisabled( true );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastDotStyle();
            break;
        }
        break;
    case 1:
        switch (itemM) {
        case 0:
            // Atom mode is chosen
            // -> cube style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastCubeStyle();
            break;
        case 1:
            // Spin mode is chosen
            // -> cone style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastConeStyle();
            break;
        case 2:
            // Liquid crystal mode is chosen
            // -> line style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastLineStyle();
            break;
        case 3:
            // Polymer mode is chosen
            // -> cube style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastCubeStyle();
            break;
        case 4:
            // Pore mode is chosen ->
            // line style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( true );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastLineStyle();
            break;
        }
        break;
    case 2:
        switch (itemM) {
        case 0:
            // Atom mode is chosen ->
            // sphere style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastSphereStyle();
            break;
        case 1:
            // Spin mode is chosen ->
            // sphere style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastSphereStyle();
            break;
        case 2:
            // Liquid crystal mode is chosen ->
            // cone style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastConeStyle();
            break;
        case 3:
            // Polymer mode is chosen ->
            // sphere style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastSphereStyle();
            break;
        case 4:
            // Pore mode is chosen ->
            // cylinder style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastCylinderStyle();
            break;
        }
        break;
    case 3:
        switch (itemM) {
        case 2:
            // Liquid crystal mode is chosen
            // -> cylinder style is chosen
            renderSSb->setDisabled( false );
            qualityCob->setDisabled( false );
            qualityCob->setCurrentIndex( 0 );
            this->renderQualityCb();
            mainForm->broadcastCylinderStyle();
            break;
        }
        break;
    }
}


// Depending on the setting of the style combo box, activate 
// or deactivate some menu entries
void RenderBox::renderQualityCb() {
    const int item = qualityCob->currentIndex();

    // Pass the setting on to some of the boards
    switch (item) {
    case 0:
        mainForm->broadcastFinalQuality();
        break;
    case 1:
        mainForm->broadcastHighQuality();
        break;
    case 2:
        mainForm->broadcastLowQuality();
        break;
    }
}


// Read the current settings and trigger an
// update of the rendering with recompiling -- 
// this function is called whenever values change
void RenderBox::updateRendering() {
    // Read the current view parameters
    viewParam * vp = mainForm->getViewParam();

    // Update the view parameters
    (*vp).renderSize = getRenderSize();
    getRenderMode( vp );
    getRenderStyle( vp );
    getRenderQuality( vp );

    // Send the new parameters back
    // without requesting update
    mainForm->setViewParam( (*vp) );

    if ((*vp).enableUpdate) {

        // Re-compile the drawing lists
        mainForm->updateView();

        // Re-draw the scene
        mainForm->updateRendering();
    }
}


// Define size policy
QSizePolicy RenderBox::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}
