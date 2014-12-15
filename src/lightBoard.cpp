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

#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "widgets/doneapplycancelwidget.h"

#include <cmath>

LightsBoard::LightsBoard(MainForm * mainForm, QWidget * parent)
    : QDialog( parent, Qt::WType_TopLevel ), m_mainForm(mainForm)
{
    setWindowTitle( "AViz: Set Lights" );

    // Layout the sliders, labels and
    // check boxes, plus a row of control buttons
    // in a vertical layout
    QVBoxLayout * lightsBox = new QVBoxLayout(this);
    {
        QWidget *lightCheckBoxes = new QWidget();
        QGridLayout *lightGrid = new QGridLayout(lightCheckBoxes);
        lightGrid->setSpacing( SPACE );
        lightGrid->setMargin( SPACE );

        light0 = new QCheckBox("Left");
        light1 = new QCheckBox("Right");
        light2 = new QCheckBox("Top");
        light3 = new QCheckBox("Bottom");
        light4 = new QCheckBox("Top Left");
        light5 = new QCheckBox("Top Right");
        light6 = new QCheckBox("Bottom Left");
        light7 = new QCheckBox("Bottom Right");

        lightGrid->addWidget(light0, 0, 0);
        lightGrid->addWidget(light1, 0, 1);
        lightGrid->addWidget(light2, 0, 2);
        lightGrid->addWidget(light3, 0, 3);
        lightGrid->addWidget(light4, 1, 0);
        lightGrid->addWidget(light5, 1, 1);
        lightGrid->addWidget(light6, 1, 2);
        lightGrid->addWidget(light7, 1, 3);

        // Create another widget below the checkboxes
        // that will have the light positions
        QWidget *lightPositions = new QWidget();
        lightGrid->addWidget(lightPositions,
                             2 /*fromRow*/, 0 /*fromColumn*/,
                             1 /*rowSpan*/, 4/*columnSpan*/);
        {
            QHBoxLayout * hBox = new QHBoxLayout(lightPositions);

            // Create a combo box to set light position depth
            lightCob = new QComboBox();
            lightCob->addItem( "Left" );
            lightCob->addItem( "Right" );
            lightCob->addItem( "Top" );
            lightCob->addItem( "Bottom" );
            lightCob->addItem( "Top Left" );
            lightCob->addItem( "Top Right" );
            lightCob->addItem( "Bottom Left" );
            lightCob->addItem( "Bottom Right" );

            connect( lightCob, SIGNAL(activated(int)), SLOT(setLightDepth(int)) );

            depthS = new QSlider( Qt::Horizontal);
            depthS->setMinimum( 0 );
            depthS->setMaximum( 100 );
            depthS->setTickInterval( 10 );
            depthS->setTickPosition( QSlider::TicksAbove );
            depthS->setFixedHeight( SLIDER_HEIGHT );

            hBox->addWidget(lightCob);
            hBox->addWidget(new QLabel(" Position Depth:  Near  "));
            hBox->addWidget(depthS);
            hBox->addWidget(new QLabel("  Far  "));
        }


        QWidget *w = new QWidget();
        QHBoxLayout* hBox = new QHBoxLayout(w);
        hBox->addWidget(new QLabel(" Spotlights:  "));
        hBox->addWidget(lightCheckBoxes, 1/*stretch*/);

        lightsBox->addWidget(w);
    }

    {
        // Create a widget to hold two sliders and labels
        QFrame *ambientShine = new QFrame(this);
        ambientShine->setFrameStyle(QFrame::Box | QFrame::Sunken);

        QGridLayout *grid1 = new QGridLayout(ambientShine);
        grid1->setSpacing(SPACE);
        grid1->setMargin(SPACE);
        lightsBox->addWidget(ambientShine);

        ambientS = new QSlider(Qt::Horizontal);
        ambientS->setMinimum( LIGHT_MIN );
        ambientS->setMaximum( LIGHT_MAX );
        ambientS->setTickInterval( 10 );
        ambientS->setTickmarks( QSlider::TicksAbove );
        ambientS->setFixedHeight( SLIDER_HEIGHT );

        shininessS = new QSlider( Qt::Horizontal);
        shininessS->setMinimum( SHINE_MIN );
        shininessS->setMaximum( SHINE_MAX );
        shininessS->setTickInterval( 10 );
        shininessS->setTickmarks( QSlider::TicksAbove );
        shininessS->setFixedHeight( SLIDER_HEIGHT );

        grid1->addWidget(new QLabel(" Global Ambient Light:  "), 0, 0);
        grid1->addWidget(ambientS, 0, 1);
        grid1->addWidget(new QLabel(" Surface Shininess:  "), 1, 0);
        grid1->addWidget(shininessS, 1, 1);
    }

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));
    lightsBox->addWidget(doneApplyCancel);
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
    lightCob->setCurrentIndex(0);
    this->setLightDepth(0);
}


// Read the current settings
void LightsBoard::registerSettings( void )
{
    // Get the current settings
    viewParam * vp = m_mainForm->getViewParam( );

    // Update the values
    vp->light[0] = light0->isChecked();
    vp->light[1] = light1->isChecked();
    vp->light[2] = light2->isChecked();
    vp->light[3] = light3->isChecked();
    vp->light[4] = light4->isChecked();
    vp->light[5] = light5->isChecked();
    vp->light[6] = light6->isChecked();
    vp->light[7] = light7->isChecked();

    // Set the depth position as well
    const int thisLight = lightCob->currentIndex();
    if (thisLight >= 0 && thisLight < LIGHTS_MAX) {
        vp->lightDepth[thisLight] = (float)depthS->value()/100.0;
    }

    vp->ambientLight = (float)ambientS->value()/100.0;

    vp->shininess = (float)shininessS->value();
}


// Update the setting of the depth slider
void LightsBoard::setLightDepth(int thisLight) {
    // Get the current settings
    viewParam * vp = m_mainForm->getViewParam( );

    // Update the values
    if (thisLight >= 0 && thisLight < LIGHTS_MAX)
        depthS->setValue( (int)floor(100.0*vp->lightDepth[thisLight] + 0.5) );
}


// Complete the lights setting
void LightsBoard::bdone() {
    this->registerSettings();

    // Re-do the graphics, using the new view settings
    m_mainForm->updateRendering();

    // Hide now
    hide();
}


// Accept the lights setting
void LightsBoard::bapply() {
    this->registerSettings();

    // Re-do the graphics, using the new view settings
    m_mainForm->updateRendering();
}


// Cancel the lights setting: hide the board
void LightsBoard::bcancel() {
    // Hide now
    hide();
}
