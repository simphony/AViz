//
// Implementation of file list board dialog widget class
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

#include "fileListBoard.h"

#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QTimer>
#include <QLabel>

#include "mainForm.h"

// Make a popup dialog box 
FileListBoard::FileListBoard(MainForm * mainForm, QWidget * parent)
    : QDialog( parent, Qt::WType_TopLevel ), mainForm(mainForm),
      thisDirection(FORWARD), cycleMode(false), haveFileList(false)
{
    setWindowTitle( "AViz: File List Control" );

    // Insert a grid that will hold control buttons
    QGridLayout * fileListBox = new QGridLayout(this);
    fileListBox->setHorizontalSpacing(SPACE);
    fileListBox->setVerticalSpacing(SPACE);

    // Create buttons
    QPushButton * singleStepPb = new QPushButton("SingleStep");
    fileListBox->addWidget( singleStepPb, 0, 0);

    QPushButton * fastStepPb = new QPushButton("FastStep");
    fileListBox->addWidget( fastStepPb, 0, 1);

    QPushButton * cyclePb = new QPushButton("Cycle");
    cyclePb->setCheckable(true);
    fileListBox->addWidget( cyclePb, 0, 2);

    // Construct the cycle timer
    cycleTimer = new QTimer( this );

    // Define a callback for these pushbuttons
    connect( singleStepPb, SIGNAL(clicked()), SLOT(bsingleStep()) );
    connect( fastStepPb, SIGNAL(clicked()), SLOT(bfastStep()) );
    connect( cyclePb, SIGNAL(clicked()), SLOT(bcycle()) );

    // Create radio buttons to choose direction
    QGroupBox *direction = new QGroupBox();
    QHBoxLayout *directionLayout = new QHBoxLayout(direction);
    fileListBox->addWidget(direction, 0 /*fromRow*/, 3 /*fromCol*/, 1 /*rowSpan*/, 1/*colSpan*/);
    forwardRb = new QRadioButton("Forward" , direction);
    directionLayout->addWidget(forwardRb);
    QRadioButton *backwardRb = new QRadioButton("Backward", direction);
    directionLayout->addWidget(backwardRb);

    // Define a callback for these radio buttons
    QButtonGroup *directionButtonGroup = new QButtonGroup(this);
    directionButtonGroup->addButton(forwardRb);
    directionButtonGroup->addButton(backwardRb);
    connect(directionButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(bdirection(int)) );

    // Create pushbuttons to jump to start
    QPushButton *startPb = new QPushButton("Jump To Start");
    fileListBox->addWidget(startPb, 1, 0);

    // Define a callback for that button
    QObject::connect( startPb, SIGNAL(clicked()), this, SLOT(start()) );

    // Create pushbuttons to jump to end
    QPushButton * endPb = new QPushButton("Jump To End");
    fileListBox->addWidget( endPb, 1, 1);

    // Define a callback for that button
    QObject::connect( endPb, SIGNAL(clicked()), this, SLOT(end()) );

    // Create a checkbox
    keepViewScaleCb = new QCheckBox("Keep ViewScale");

    fileListBox->addWidget( keepViewScaleCb, 1, 4);
    this->resetKeepViewScale();

    // Define a callback for that check box
    QObject::connect( keepViewScaleCb, SIGNAL(clicked()), this, SLOT(keepViewScaleChanged()) );

    // Create a label
    QLabel * lineL = new QLabel(" File List: ");
    fileListBox->addWidget( lineL, 2, 0 );

    // Create a text window
    fileLine = new QLineEdit("--");
    fileLine->setReadOnly(true);
    fileListBox->addWidget(fileLine, 2 /*fromRow*/, 1 /*fromCol*/, 1 /*rowSpan*/, 3/*colSpan*/);

    // Create a pushbutton
    QPushButton * tracksPb = new QPushButton("Tracks...");
    fileListBox->addWidget( tracksPb, 3, 0);

    // Define a callback for that button
    connect( tracksPb, SIGNAL(clicked()), this, SLOT(launchTrack()) );

    // Create more pushbuttons that will go into the lowest row
    QPushButton * done = new QPushButton("Done");
    fileListBox->addWidget( done, 3, 4);

    // Define a callback for that button
    connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

    forwardRb->setChecked (thisDirection == FORWARD);
}

// Receive the current file list parameters 
void FileListBoard::setFileListParam( int thisCurrentFile, 
                                      int thisNumberOfFiles, const QString &fn )
{
    numberOfFiles = thisNumberOfFiles;
    currentFile = thisCurrentFile;
    fileListName = fn;

    // Adjust the caption of the board
    QString caption =  "AViz: File List Control ";
    caption.append(fn);
    setWindowTitle( caption );

    // Show the current file
    this->showCurrentFile();

    // Set a flag
    haveFileList = true;
}


// Increment the current counter by one and show the 
// next file
void FileListBoard::bsingleStep()
{
    switch (this->getDirection()) {
    case FORWARD:
        currentFile++;
        break;
    case BACKWARD:
        currentFile--;
        break;
    }

    if (currentFile >= numberOfFiles)
        currentFile = 0;

    if (currentFile < 0)
        currentFile = numberOfFiles-1;

    // Show this file
    this->showCurrentFile();
}


// Increment the current counter by ten and show the 
// next file
void FileListBoard::bfastStep()
{
    switch (this->getDirection()) {
    case FORWARD:
        currentFile+=10;
        break;
    case BACKWARD:
        currentFile-=10;
        break;
    }

    if (currentFile >= numberOfFiles)
        currentFile = 0;

    if (currentFile < 0)
        currentFile = numberOfFiles-1;

    // Show this file
    this->showCurrentFile();
}


// Start or stop the cycling
void FileListBoard::bcycle()
{
    if (cycleMode) {
        // End the cycling
        cycleTimer->stop();
        cycleMode = false;
    } else {
        // Start the timer
        connect(cycleTimer, SIGNAL(timeout()), this, SLOT(bsingleStep()) );
        cycleTimer->start( 40 );
        cycleMode = true;
    }
}


// Jump to start of cycle
void FileListBoard::start()
{
    currentFile = 0;

    // Show this file
    this->showCurrentFile();
}


// Jump to end of cycle
void FileListBoard::end()
{
    currentFile = numberOfFiles-1;

    // Show this file
    this->showCurrentFile();
}


// Adjust the direction
void FileListBoard::bdirection( int radio )
{
    switch (radio) {
    case 0:
        thisDirection = FORWARD;
        break;
    case 1:
        thisDirection = BACKWARD;
        break;
    }
}


// Return the current direction 
FileListBoard::listDirection FileListBoard::getDirection( )
{
    return thisDirection;
}


// Reset the view freezing -- by default, it should be off 
// (when on, the result can be that a new data set is not 
// visible at all!)
void FileListBoard::resetKeepViewScale( )
{
    keepViewScaleCb->setChecked( false );
}


// Callback function to launch track board
void FileListBoard::launchTrack( )
{
    mainForm->launchTrack( fileListName );
}


// Callback function to control view freezing
void FileListBoard::keepViewScaleChanged( )
{
    // Do nothing
}


// Show the current file in the file list
void FileListBoard::showCurrentFile( )
{
    if (haveFileList) {

        // Read the data; control the freezing of the view object data
        QString file = mainForm->readFileFromList( currentFile, keepViewScaleCb->isChecked() );

        // Update the graphics, with or without track display
        mainForm->updateRendering();

        // Update text window
        fileLine->setText( file );
    }
}


// Hide the board
void FileListBoard::bdone()
{
    // Hide now
    hide();

    // Also hide the track board
    mainForm->hideTrack();
}
