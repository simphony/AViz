//
// Implementation of translation board dialog widget class
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

#include "translationBoard.h"

#include <cmath>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "floatSpin.h"
#include "mainForm.h"
#include "parameterLimits.h"
#include "widgets/doneapplycancelwidget.h"

TranslationBoard::TranslationBoard(MainForm *mainForm, QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel), mainForm(mainForm) {
    setWindowTitle( "AViz: Set Data Translation" );

    // Insert a grid that will hold control buttons
    QGridLayout *grid = new QGridLayout(this);
    grid->setHorizontalSpacing(SPACE);
    grid->setVerticalSpacing(SPACE);

    // Create labels and spin boxes
    QLabel * labelx = new QLabel(" Translate X: ");
    grid->addWidget( labelx, 0, 0);
    panSBX = new QFSpinBox( this );
    panSBX->setMaximum( MAXPAN );
    panSBX->setMinimum( MINPAN );
    grid->addWidget( panSBX, 0, 1);

    QLabel * labely = new QLabel(" Translate Y: ");
    grid->addWidget( labely, 1, 0);
    panSBY = new QFSpinBox( this );
    panSBY->setMaximum( MAXPAN );
    panSBY->setMinimum( MINPAN );
    grid->addWidget( panSBY, 1, 1);

    QLabel * labelz = new QLabel(" Translate Z: ");
    grid->addWidget( labelz, 2, 0);
    panSBZ = new QFSpinBox( this );
    panSBZ->setMaximum( MAXPAN );
    panSBZ->setMinimum( MINPAN );
    grid->addWidget( panSBZ, 2, 1);

    // Create labels and spin boxes
    QLabel * labelright = new QLabel(" Translate Right: ");
    grid->addWidget( labelright, 0, 2);
    panSBRight = new QFSpinBox( this );
    panSBRight->setMaximum( MAXPAN );
    panSBRight->setMinimum( MINPAN );
    grid->addWidget( panSBRight, 0, 3);

    QLabel * labeltop = new QLabel(" Translate Top: ");
    grid->addWidget( labeltop, 1, 2);
    panSBTop = new QFSpinBox( this );
    panSBTop->setMaximum( MAXPAN );
    panSBTop->setMinimum( MINPAN );
    grid->addWidget( panSBTop, 1, 3);

    QLabel * labelforward = new QLabel(" Translate Forward: ");
    grid->addWidget( labelforward, 2, 2);
    panSBForward = new QFSpinBox( this );
    panSBForward->setMaximum( MAXPAN );
    panSBForward->setMinimum( MINPAN );
    grid->addWidget( panSBForward, 2, 3);

    // Create a pushbutton
    QPushButton *reset = new QPushButton("Reset Translation");
    connect( reset, SIGNAL(clicked()), SLOT(doReset()) );
    grid->addWidget(reset, 3, 0);

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(hide()));
    grid->addWidget(doneApplyCancel, 3 /*fromRow*/, 1 /*fromCol*/, 1/*rowSpan*/, 3/*colSpan*/);
}

// Receive the current view parameters
void TranslationBoard::setData( viewParam vp ) {
    panSBX->setValue( (int)floor(10.0*vp.panX+0.5) );
    panSBY->setValue( (int)floor(10.0*vp.panY+0.5) );
    panSBZ->setValue( (int)floor(10.0*vp.panZ+0.5) );
    panSBRight->setValue( (int)floor(10.0*vp.panRight+0.5) );
    panSBTop->setValue( (int)floor(10.0*vp.panTop+0.5) );
    panSBForward->setValue( (int)floor(10.0*vp.panForward+0.5) );
}


// Read the current settings
void TranslationBoard::registerSettings( void ) {
    // Get the current settings
    viewParam * vp = mainForm->getViewParam( );

    // Update the values
    (*vp).panX = (float)panSBX->value()/10.0;
    (*vp).panY = (float)panSBY->value()/10.0;
    (*vp).panZ = (float)panSBZ->value()/10.0;
    (*vp).panRight = (float)panSBRight->value()/10.0;
    (*vp).panTop = (float)panSBTop->value()/10.0;
    (*vp).panForward = (float)panSBForward->value()/10.0;
}


// Reset the translations
void TranslationBoard::doReset( void ) {
    viewParam thisVp;

    thisVp.panX = thisVp.panY = thisVp.panZ = 0.0;
    thisVp.panRight = thisVp.panTop = thisVp.panForward = 0.0;

    this->setData( thisVp );
}


// Complete the setting: save the view parameter and hide the board
void TranslationBoard::bdone() {
    registerSettings();

    // Re-do the graphics, using the new parameters
    mainForm->updateRendering();

    // Hide now
    hide();
}


// Accept the setting: save the view parameter
void TranslationBoard::bapply() {
    registerSettings();

    // Re-do the graphics, using the new parameters
    mainForm->updateRendering();
}
