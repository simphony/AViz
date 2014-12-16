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

#include <Q3GridLayout>
#include <QPushButton>
#include <Q3ButtonGroup>
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
	const int numCols = 5;
        const int numRows = 4;
        Q3GridLayout * fileListBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "fileListBox" );

	// Create buttons
	QPushButton * singleStepPb = new QPushButton( this, "singleStep" );
	singleStepPb->setText( "SingleStep" );
	fileListBox->addWidget( singleStepPb, 0, 0);

	QPushButton * fastStepPb = new QPushButton( this, "fastStep" );
	fastStepPb->setText( "FastStep" );
	fileListBox->addWidget( fastStepPb, 0, 1);

	QPushButton * cyclePb = new QPushButton( this, "cycle" );
	cyclePb->setText( "Cycle" );
    cyclePb->setToggleButton(true);
	fileListBox->addWidget( cyclePb, 0, 2);

	// Construct the cycle timer
	cycleTimer = new QTimer( this );

	// Define a callback for these pushbuttons
        connect( singleStepPb, SIGNAL(clicked()), SLOT(bsingleStep()) );
        connect( fastStepPb, SIGNAL(clicked()), SLOT(bfastStep()) );
        connect( cyclePb, SIGNAL(clicked()), SLOT(bcycle()) );

	// Create radio buttons to choose direction
    Q3ButtonGroup *direction = new Q3ButtonGroup( 2, Qt::Horizontal, this, "direction" );
	fileListBox->addMultiCellWidget( direction, 0, 0, 3, 4 );
	forwardRb = new QRadioButton( direction, "forward" );
	forwardRb->setText( "Forward" );
    QRadioButton *backwardRb = new QRadioButton( direction, "backward" );
	backwardRb->setText( "Backward" );

	// Define a callback for these radio buttons
        connect( direction, SIGNAL(clicked(int)), this, SLOT(bdirection(int)) );

	// Create pushbuttons to jump to start 
	QPushButton * startPb = new QPushButton( this, "start" );
	startPb->setText( "Jump To Start" );
	fileListBox->addWidget( startPb, 1, 0);

        // Define a callback for that button
	QObject::connect( startPb, SIGNAL(clicked()), this, SLOT(start()) );

	// Create pushbuttons to jump to end
	QPushButton * endPb = new QPushButton( this, "end" );
	endPb->setText( "Jump To End" );
	fileListBox->addWidget( endPb, 1, 1);

        // Define a callback for that button
	QObject::connect( endPb, SIGNAL(clicked()), this, SLOT(end()) );

	// Create a checkbox
	keepViewScaleCb = new QCheckBox( this, "keepViewScale" );
	fileListBox->addWidget( keepViewScaleCb, 1, 4);
	keepViewScaleCb->setText( "Keep ViewScale" );
	this->resetKeepViewScale();

        // Define a callback for that check box 
	QObject::connect( keepViewScaleCb, SIGNAL(clicked()), this, SLOT(keepViewScaleChanged()) );

	// Create a label
	QLabel * lineL = new QLabel( this, "lineL ");
	lineL->setText( " File List: " );
	fileListBox->addWidget( lineL, 2, 0 );

	// Create a text window
	fileLine = new QLineEdit( this, "fileLine" );
	fileLine->setText("--");
    fileLine->setReadOnly(true);
	fileListBox->addMultiCellWidget( fileLine, 2, 2, 1, 4 );

	// Create a pushbutton
	QPushButton * tracksPb = new QPushButton( this, "tracks" );
	tracksPb->setText( "Tracks..." );
	fileListBox->addWidget( tracksPb, 3, 0);

        // Define a callback for that button
	QObject::connect( tracksPb, SIGNAL(clicked()), this, SLOT(launchTrack()) );

	// Create more pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( this, "done" );
	fileListBox->addWidget( done, 3, 4);
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

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
	if (mainForm)
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
	if (mainForm && haveFileList) {

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
	if (mainForm)
		mainForm->hideTrack();
}
