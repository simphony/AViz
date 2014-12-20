//
// Implementation of stretch board dialog widget class
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

#include "stretchBoard.h"

#include <cmath>

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "floatSpin.h"
#include "mainForm.h"
#include "parameterLimits.h"
#include "widgets/doneapplycancelwidget.h"

// Make a popup dialog box 
StretchBoard::StretchBoard(MainForm *mainForm, QWidget * parent)
    : QDialog(parent, Qt::WType_TopLevel),
      m_mainForm(mainForm), stretchChange(false) {
    setWindowTitle( "AViz: Set Data Stretching" );

    // Insert a grid that will hold control buttons
    QGridLayout *grid = new QGridLayout(this);
    grid->setHorizontalSpacing(SPACE);
    grid->setVerticalSpacing(SPACE);

    // Create labels and spin boxes
    grid->addWidget(new QLabel(" Stretch X: "), 0, 0);
    stretchSBX = new QFSpinBox( this );
    stretchSBX->setMaximum( MAXSTRETCH );
    stretchSBX->setMinimum( MINSTRETCH );
    grid->addWidget( stretchSBX, 0, 1);

    grid->addWidget(new QLabel(" Stretch Y: "), 1, 0);
    stretchSBY = new QFSpinBox( this );
    stretchSBY->setMaximum( MAXSTRETCH );
    stretchSBY->setMinimum( MINSTRETCH );
    grid->addWidget( stretchSBY, 1, 1);

    grid->addWidget( new QLabel(" Stretch Z: "), 2, 0);
    stretchSBZ = new QFSpinBox( this );
    stretchSBZ->setMaximum( MAXSTRETCH );
    stretchSBZ->setMinimum( MINSTRETCH );
    grid->addWidget(stretchSBZ, 2, 1);

    // Define a callback for the spin boxes
    connect( stretchSBX, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );
    connect( stretchSBY, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );
    connect( stretchSBZ, SIGNAL(valueChanged(int)), this, SLOT(stretchChanged()) );

    // Create pushbuttons
    QPushButton * resetx = new QPushButton("Reset Stretch X");
    QPushButton * resety = new QPushButton("Reset Stretch Y");
    QPushButton * resetz = new QPushButton("Reset Stretch Z");
    grid->addWidget( resetx, 0, 2);
    grid->addWidget( resety, 1, 2);
    grid->addWidget( resetz, 2, 2);

    // connect the pushbuttons
    connect( resetx, SIGNAL(clicked()), SLOT(doResetX()) );
    connect( resety, SIGNAL(clicked()), SLOT(doResetY()) );
    connect( resetz, SIGNAL(clicked()), SLOT(doResetZ()) );

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));
    grid->addWidget(doneApplyCancel, 3 /*fromRow*/, 0 /*fromCol*/, 3/*rowSpan*/, 3/*colSpan*/);
}

// Receive the current view parameters
void StretchBoard::setData( viewParam vp )
{
    stretchSBX->setValue( (int)floor(10.0*vp.stretchX+0.5) );
    stretchSBY->setValue( (int)floor(10.0*vp.stretchY+0.5) );
    stretchSBZ->setValue( (int)floor(10.0*vp.stretchZ+0.5) );
}


// Set a flag if the stretching parameters change so that
// the view object must be re-defined
void StretchBoard::stretchChanged()
{
    stretchChange = true;
}


// Read the current settings
void StretchBoard::registerSettings( void ) {
    // Get the current settings
    viewParam * vp = m_mainForm->getViewParam( );

    // Update the values
    (*vp).stretchX = (float)stretchSBX->value()/10.0;
    (*vp).stretchY = (float)stretchSBY->value()/10.0;
    (*vp).stretchZ = (float)stretchSBZ->value()/10.0;
}


// Reset the translations
void StretchBoard::doResetX() {
    viewParam * vp = m_mainForm->getViewParam( );

    (*vp).stretchX = 1.0;
    stretchChange = true;
    this->setData( (*vp) );
}


// Reset the translations
void StretchBoard::doResetY() {
    viewParam * vp = m_mainForm->getViewParam( );

    (*vp).stretchY = 1.0;
    stretchChange = true;
    this->setData( (*vp) );
}


// Reset the translations
void StretchBoard::doResetZ() {
    viewParam * vp = m_mainForm->getViewParam( );

    (*vp).stretchZ = 1.0;
    stretchChange = true;
    this->setData( (*vp) );
}


// Complete the setting: save the view parameter and hide the board
void StretchBoard::bdone() {
    registerSettings();

    // Re-do the graphics, using the new parameters
    if (stretchChange) {
        // Re-define the view object and re-draw
        m_mainForm->updateView();
        m_mainForm->updateRendering();

        // Reset flag
        stretchChange = false;
    }
    else {
        // Merely re-draw the slice planes
        m_mainForm->updateRendering();
    }

    // Hide now
    hide();
}


// Accept the setting: save the view parameter
void StretchBoard::bapply() {
    registerSettings();

    // Re-do the graphics, using the new parameters

    if (stretchChange) {
        // Re-define the view object and re-draw
        m_mainForm->updateView();
        m_mainForm->updateRendering();

        // Reset flag
        stretchChange = false;
    }
    else {
        // Merely re-draw the slice planes
        m_mainForm->updateRendering();
    }
}


// Cancel the setting: hide the board
void StretchBoard::bcancel() {
    // Hide now
    hide();
}
