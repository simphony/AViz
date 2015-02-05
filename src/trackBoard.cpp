//
// Implementation of track board dialog widget class
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

#include "trackBoard.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include "mainForm.h"
#include "fileFunctions.h"
#include "widgets/doneapplycancelwidget.h"

// Make a popup dialog box 
TrackBoard::TrackBoard(MainForm* mainForm, QWidget * parent)
    : QDialog(parent), mainForm(mainForm)
{
    setWindowTitle( "AViz: Track Rendering Control" );

    // Insert a grid that will hold control buttons
    QGridLayout *trackBox = new QGridLayout(this);
    trackBox->setHorizontalSpacing(SPACE);
    trackBox->setVerticalSpacing(SPACE);

    // Create a label
    trackBox->addWidget(new QLabel(" Type: "), 0, 0);

    // Create a combo box that will go into the
    // second column; entries in the combo box will
    // be made later
    typeCob = new QComboBox(this);
    trackBox->addWidget( typeCob, 0, 1);

    // Define a callback for this combo box
    connect( typeCob, SIGNAL(activated(int)), SLOT(setType()) );

    // Add a checkbox button
    showTypeTrackCb = new QCheckBox("Show Tracks");
    showTypeTrackCb->setChecked(true);
    trackBox->addWidget( showTypeTrackCb, 0, 3);

    // Add a label
    trackBox->addWidget(new QLabel("   Tracking Mode: "), 1, 0);


    // Create radio buttons to choose track render mode
    {
        QGroupBox *stages = new QGroupBox(this);
        trackBox->addWidget(stages, 1/*fromRow*/, 1 /*fromCol*/, 1 /*rowSpan*/, 1/*colSpan*/);

        QRadioButton *allStagesRb = new QRadioButton("All Stages" );
        QRadioButton *upToStagesRb = new QRadioButton("Up To Current Stage" );

        QHBoxLayout *hbox = new QHBoxLayout(stages);
        hbox->addWidget(allStagesRb);
        hbox->addWidget(upToStagesRb);

        QButtonGroup *stagesButtonGroup = new QButtonGroup(stages);
        stagesButtonGroup->addButton(allStagesRb, 0);
        stagesButtonGroup->addButton(upToStagesRb, 1);

        // Define a callback for these radio buttons
        connect( stagesButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(bstages(int)) );

        allStagesRb->setChecked(true);
    }

    // Add a checkbox button
    periodicCb = new QCheckBox("Periodic BC");
    periodicCb->setChecked(true);
    trackBox->addWidget(periodicCb, 1, 3);

    // Create buttons
    showTracksPb = new QPushButton("Generate Tracks");
    showTracksPb->setCheckable(true);
    trackBox->addWidget(showTracksPb, 2, 0);

    // Define a callback for this pushbutton
    connect(showTracksPb, SIGNAL(clicked()), SLOT(showTracks()));

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()));
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(bapply()));
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(bcancel()));

    trackBox->addWidget(doneApplyCancel, 2/*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, -1/*colSpan*/);
}

// Get the current particle data from main form, register
// it using a local copy,  and prepare a list of particle
// types; this function is called each time the board is launched
void TrackBoard::setData() {
    tag tmp;

    // Get the current settings and register
    // it using a local particle data structure
    thisPd = mainForm->getParticleData();

    // Get a list of particles that are currently rendered
    aggregateData * ad = mainForm->getAggregateData();

    // Make entries in the combo box -- use only particle
    // types that are really needed; otherwise the list
    // gets too long
    if (typeCob) {
        typeCob->clear();
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
                typeCob->addItem(QString( (char *)&(*thisPd).type[i]));
            }
        }
        typeCob->setMinimumSize( typeCob->sizeHint() );
    }

    // Sort the entries alphabetically, at least approximately
    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        for (int j=0;j<typeCob->count()-1;j++) {
            if (QString::compare(typeCob->itemText(j), typeCob->itemText(j+1)) > 0) {
                typeCopy( qPrintable(typeCob->itemText(j+1)), (char *)&tmp );
                typeCob->insertItem(j, QString( (char *)&tmp));
                typeCob->removeItem(j+2);
            }
        }
    }

    // Set the first particle type as the current type
    if ((*ad).numberOfParticles > 0) {
        thisTypeIndex = 0;
        this->setType();
    }
    else {
        thisTypeIndex = -1;
    }
}


// Set the particle type under consideration: adjust the toggle switch
void TrackBoard::setType() {
    tag thisType;
    thisTypeIndex = -1;

    // Find out what combination of atoms is set
    typeCopy( qPrintable(typeCob->currentText()), (char *)&thisType);

    for (int i=0;i<(*thisPd).numberOfParticleTypes;i++) {
        if (typeCmp( (char *)&(*thisPd).type[i], (char *)&thisType) == TRUE) {
            // Found the correct entry
            thisTypeIndex = i;

            // Adjust the toggles
            bool thisShowTrack = (*thisPd).showTrack[thisTypeIndex];
            showTypeTrackCb->setChecked( thisShowTrack );
        }
    }
}


// Receive the current file list parameters 
void TrackBoard::setFileListParam( const QString &fn ) {
    // Adjust the caption of the board
    QString caption =  "AViz: Track Rendering Control ";
    caption.append(fn);
    setWindowTitle( caption );
}


// Activate track rendering
void TrackBoard::showTracks( void )
{
    if (showTracksPb->isChecked()) {
        // Change labeling of the button
        showTracksPb->setText( "Show Tracks" );

        // Update the view parameters
        viewParam * vp = mainForm->getViewParam();
        (*vp).showTracks = TRUE;

        // Read the current settings (the button is similar
        // to the apply button)
        this->readToggles();

        // Generate track data
        mainForm->generateTracks();

        // Update the rendering
        mainForm->updateRendering();

    }
    else {
        // Change labeling of the button
        showTracksPb->setText( "Generate Tracks" );

        // Update the view parameters
        viewParam * vp = mainForm->getViewParam();
        (*vp).showTracks = FALSE;

        // Update the rendering
        mainForm->updateRendering();
    }
}


// Adjust the track render mode
void TrackBoard::bstages( int radio )
{
    switch (radio) {
    case 0:
        // Update the view parameters
    {
        viewParam * vp = mainForm->getViewParam();
        (*vp).tRenderMode = ALL_STAGES;
    }
        break;
    case 1:
        // Update the view parameters
    {
        viewParam * vp = mainForm->getViewParam();
        (*vp).tRenderMode = UP_TO_CURRENT;
    }
        break;
    }
}


// Read the toggle switches
void TrackBoard::readToggles( void )
{
    if (thisTypeIndex >= 0) {
        (*thisPd).showTrack[thisTypeIndex] = showTypeTrackCb->isChecked();
    }

    viewParam * vp = mainForm->getViewParam();
    (*vp).periodicBCTracks = periodicCb->isChecked();
}


// Complete the particle switch setting: save the particle data and hide
// the board
void TrackBoard::bdone()
{
    // Read the toggle switches
    this->readToggles();

    // Save the particle data
    char * filename = (char *)malloc(BUFSIZ);
    sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
    if (saveParticleDataFunction( filename, thisPd ) ) {
        mainForm->statusMessage( "Saved particle data in ", filename );
    }
    free(filename);

    // Generate track data
    mainForm->generateTracks();

    // Re-do the graphics, using the new particle data
    // Hide now
    hide();
}


// Accept the particle switch setting: save the particle data
void TrackBoard::bapply()
{
    // Read the toggle switches
    this->readToggles();

    // Save the particle data
    char * filename = (char *)malloc(BUFSIZ);
    sprintf(filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile);
    if (saveParticleDataFunction( filename, thisPd ) ) {
        mainForm->statusMessage( "Saved particle data in ", filename );
    }
    free(filename);

    // Generate track data
    mainForm->generateTracks();

    // Re-do the graphics, using the new particle data
    mainForm->updateRendering();
}


// Cancel the setting: hide the board
void TrackBoard::bcancel()
{
    // Hide now
    hide();
}
