//
// Implementation of explicit view point settings board dialog widget class
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

#include "explicitBoard.h"

#include <cmath>

#include <QPixmap>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "parameterLimits.h"
#include "floatSpin.h"
#include "mainForm.h"
#include "widgets/doneapplycancelwidget.h"

#include "./pixmaps/phi.xpm"
#include "./pixmaps/theta.xpm"
#include "./pixmaps/chi.xpm"
#include "./pixmaps/dolly.xpm"
#include "./pixmaps/fovy.xpm"


// Make a popup dialog box
ExplicitBoard::ExplicitBoard(MainForm *mainForm, QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel), mainForm(mainForm)
{
    setWindowTitle( "AViz: Set Viewpoint" );

    // Insert a grid that will hold spin boxes
    // plus a row of control buttons
    QGridLayout * grid = new QGridLayout(this);
    grid->setHorizontalSpacing(SPACE);
    grid->setVerticalSpacing(SPACE);

    // Create a label and a spin box - PHI
    QLabel *phiL = new QLabel(this);
    phiL->setPixmap( QPixmap( phi ) );
    grid->addWidget( phiL, 0, 0 );

    phiSb = new QFSpinBox( this );
    phiSb->setMinimum( (int)floor(10.0*PHI_MIN+0.5) );
    phiSb->setMaximum( (int)floor(10.0*PHI_MAX+0.5) );
    phiSb->setWrapping( true );
    grid->addWidget( phiSb, 0, 1 );

    QString strPhiRange = QString("Range: [%1, %2]").arg((int)PHI_MIN).arg((int)PHI_MAX);
    grid->addWidget( new QLabel(strPhiRange), 0, 2 );

    // Create a label and a spin box - THETA
    thetaL = new QLabel(this);
    QPixmap thetaPm = QPixmap( theta );
    thetaL->setPixmap(  thetaPm );
    grid->addWidget( thetaL, 1, 0 );

    thetaSb = new QFSpinBox( this );
    thetaSb->setMinimum( (int)floor(10.0*THETA_MIN+0.5) );
    thetaSb->setMaximum( (int)floor(10.0*THETA_MAX+0.5) );
    thetaSb->setWrapping( true );
    grid->addWidget( thetaSb, 1, 1 );

    QString strThetaRange = QString("Range: [%1, %2]").arg((int)THETA_MIN).arg((int)THETA_MAX);
    grid->addWidget(new QLabel(strThetaRange), 1, 2 );

    // Create a label and a spin box - CHI
    chiL = new QLabel(this);
    chiL->setPixmap( QPixmap(chi) );
    grid->addWidget( chiL, 2, 0 );

    chiSb = new QFSpinBox( this );
    chiSb->setMinimum( (int)floor(10.0*CHI_MIN+0.5) );
    chiSb->setMaximum( (int)floor(10.0*CHI_MAX+0.5) );
    chiSb->setWrapping( true );
    grid->addWidget( chiSb, 2, 1 );

    QString strChiRange = QString("Range: [%1, %2]").arg((int)CHI_MIN).arg((int)CHI_MAX);
    grid->addWidget(new QLabel(strChiRange), 2, 2 );

    // Create a label and a spin box - DOLLY
    dollyL = new QLabel(this);
    dollyL->setPixmap( QPixmap(dolly) );
    grid->addWidget( dollyL, 3, 0 );

    dollySb = new QFSpinBox(this);
    dollySb->setMinimum( (int)floor(10.0*DOLLY_MIN+0.5) );
    dollySb->setMaximum( (int)floor(10.0*DOLLY_MAX+0.5) );
    grid->addWidget( dollySb, 3, 1 );

    QString strDollyRange = QString("Range: [%1, %2]").arg((int)DOLLY_MIN).arg((int)DOLLY_MAX);
    grid->addWidget(new QLabel(strDollyRange), 3, 2 );

    // Create a label and a spin box - FOVY
    QLabel *fovyL = new QLabel(this);
    fovyL->setPixmap( QPixmap(fovy) );
    grid->addWidget( fovyL, 4, 0 );

    fovySb = new QFSpinBox( this );
    fovySb->setMinimum( (int)floor(10.0*FOVY_MIN+0.5) );
    fovySb->setMaximum( (int)floor(10.0*FOVY_MAX+0.5) );
    grid->addWidget( fovySb, 4, 1 );

    QString strFovyRange = QString("Range: [%1, %2]").arg((int)FOVY_MIN).arg((int)FOVY_MAX);
    grid->addWidget( new QLabel(strFovyRange), 4, 2 );

    // Define common callbacks
    connect( phiSb, SIGNAL(valueChanged(int)), SLOT(registerSettings()) );
    connect( thetaSb, SIGNAL(valueChanged(int)), SLOT(registerSettings()) );
    connect( chiSb, SIGNAL(valueChanged(int)), SLOT(registerSettings()) );
    connect( dollySb, SIGNAL(valueChanged(int)), SLOT(registerSettings()) );
    connect( fovySb, SIGNAL(valueChanged(int)), SLOT(registerSettings()) );

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), SLOT(bcancel()));

    grid->addWidget(doneApplyCancel, 5 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
}


// Get the current settings from the main form
void ExplicitBoard::setExplicit( viewParam vp )
{
    while (vp.phi > PHI_MAX) {
        vp.phi -= 360.0;
    }
    while (vp.phi < PHI_MIN) {
        vp.phi += 360.0;
    }
    phiSb->setValue( (int)floor(10.0*vp.phi+0.5) );

    while (vp.theta > THETA_MAX) {
        vp.theta -= 360.0;
    }
    while (vp.theta < THETA_MIN) {
        vp.theta += 360.0;
    }
    thetaSb->setValue( (int)floor(10.0*vp.theta+0.5) );

    while (vp.chi > CHI_MAX) {
        vp.chi -= 360.0;
    }
    while (vp.chi < CHI_MIN) {
        vp.chi += 360.0;
    }
    chiSb->setValue( (int)floor(10.0*vp.chi+0.5) );

    dollySb->setValue( (int)floor(10.0*vp.dolly+0.5) );

    while (vp.fovy > FOVY_MAX) {
        vp.fovy -= 360.0;
    }
    while (vp.fovy < FOVY_MIN) {
        vp.fovy += 360.0;
    }
    fovySb->setValue( (int)floor(10.0*vp.fovy+0.5) );
}


// Read the current clip settings
void ExplicitBoard::registerSettings()
{
    // Get the current settings
    viewParam * vp = mainForm->getViewParam( );

    (*vp).phi = (float)phiSb->value()/10.0;
    (*vp).theta = (float)thetaSb->value()/10.0;
    (*vp).chi = (float)chiSb->value()/10.0;
    (*vp).dolly = (float)dollySb->value()/10.0;
    (*vp).fovy = ((float)fovySb->value()/10.0 > 0.001)? (float)fovySb->value()/10.0 : 0.001; // Avoid a zero fovy
}


// Accept the setting: update the rendering and hide the board
void ExplicitBoard::bdone()
{
    this->registerSettings();

    // Re-do the graphics, using the new view settings
    mainForm->updateRendering();

    // Hide now
    hide();
}


// Accept the setting: update the rendering
void ExplicitBoard::bapply()
{
    this->registerSettings();

    // Re-do the graphics, using the new view settings
    mainForm->updateRendering();
}


// Cancel the setting: hide the board
void ExplicitBoard::bcancel()
{
    // Hide now
    hide();
}
