//
// Implementation of clip board dialog widget class
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

#include "clipBoard.h"

#include <cmath>

#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

#include "floatSpin.h"
#include "mainForm.h"
#include "parameterLimits.h" // for MAXCLIP
#include "widgets/doneapplycancelwidget.h"

// Make a popup dialog box 
ClipBoard::ClipBoard(MainForm *mainForm, QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel ), mainForm(mainForm){
    setWindowTitle( "AViz: Set Clipping" );

    QGridLayout * grid = new QGridLayout(this);
    grid->setHorizontalSpacing(SPACE);
    grid->setVerticalSpacing(SPACE);

    // Create a label and a spin box
    clipNearL = new QLabel(" Near Clip Dist: ");
    grid->addWidget( clipNearL, 0, 0 );

    clipNearSb = new QFSpinBox( this );
    clipNearSb->setMaximum( MAXCLIP );
    grid->addWidget( clipNearSb, 0, 1 );

    // Create a label and a spin box
    clipFarL = new QLabel(" Far Clip Dist: ");
    grid->addWidget( clipFarL, 1, 0 );

    clipFarSb = new QFSpinBox( this );
    clipFarSb->setMaximum( MAXCLIP );
    grid->addWidget( clipFarSb, 1, 1 );

    // Create check boxes to control auto clipping
    autoNearCb = new QCheckBox(" Auto Clip ");
    grid->addWidget( autoNearCb, 0, 2 );

    autoFarCb = new QCheckBox(" Auto Clip ");
    grid->addWidget( autoFarCb, 1, 2 );

    // Define callbacks for these checkboxes
    QObject::connect( autoNearCb, SIGNAL(clicked()), this, SLOT(autoClip()) );
    QObject::connect( autoFarCb, SIGNAL(clicked()), this, SLOT(autoClip()) );

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(hide()));
    grid->addWidget(doneApplyCancel, 2 /*fromRow*/, 0 /*fromCol*/, 1/*rowSpan*/, 3/*colSpan*/);
}

// Get the current settings from the main form
void ClipBoard::setClip( viewParam vp ) {
    // Update the controls
    autoNearCb->setChecked( vp.autoClipNear );
    autoFarCb->setChecked( vp.autoClipFar );

    clipNearSb->setValue( (int)floor(10.0*vp.clipNear+0.5) );
    clipFarSb->setValue( (int)floor(10.0*vp.clipFar+0.5) );

    // Activate or disactivate the controls
    this->autoClip();
}


// Callback function to respond to auto clipping
void ClipBoard::autoClip()
{
    switch( autoNearCb->isChecked() ) {
    case TRUE:
        clipNearL->setDisabled( TRUE );
        clipNearSb->setDisabled( TRUE );
        break;
    case FALSE:
        clipNearL->setDisabled( FALSE );
        clipNearSb->setDisabled( FALSE );
        break;
    }

    switch( autoFarCb->isChecked() ) {
    case TRUE:
        clipFarL->setDisabled( TRUE );
        clipFarSb->setDisabled( TRUE );
        break;
    case FALSE:
        clipFarL->setDisabled( FALSE );
        clipFarSb->setDisabled( FALSE );
        break;
    }
}


// Read the current clip settings
void ClipBoard::registerSettings() {
    // Get the current settings
        viewParam * vp = mainForm->getViewParam( );

        (*vp).clipNear = (float)clipNearSb->value()/10.0;
        (*vp).clipFar = (float)clipFarSb->value()/10.0;

        (*vp).autoClipNear = autoNearCb->isChecked();
        (*vp).autoClipFar = autoFarCb->isChecked();
}


// Accept the setting: update the rendering and hide the board
void ClipBoard::bdone() {
    this->registerSettings();

    // Re-do the graphics, using the new view settings
        mainForm->updateRendering();

    // Hide now
    hide();
}


// Accept the setting: update the rendering 
void ClipBoard::bapply() {
    this->registerSettings();

    // Re-do the graphics, using the new view settings
        mainForm->updateRendering();
}


// Cancel the setting: hide the board
void ClipBoard::bcancel() {
    // Hide now
    hide();
}
