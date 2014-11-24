//
// Implementation of animation board dialog widget class
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

#include "animationBoard.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Q3GridLayout>
#include <QLabel>

#define MIN_FILE_LINE_WIDTH 500
#define MIN_FILE_LINE_HEIGHT 100

// Make a popup dialog box 
AnimationBoard::AnimationBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	this->setCaption( "AViz: Animation Generator" );

       // Insert a grid that will hold control buttons
	const int numCols = 1;
	const int numRows = 3;
	Q3GridLayout * animationBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "animationBox" );
	

	// Create and horizontal boxes
        Q3HBox * hb0 = new Q3HBox( this, "hb0" );
        hb0->setSpacing( SPACE );
        animationBox->addWidget( hb0, 0, 0 );

        Q3HBox * hb1 = new Q3HBox( this, "hb1" );
        hb1->setSpacing( SPACE );
        animationBox->addWidget( hb1, 1, 0 );

        Q3HBox * hb2 = new Q3HBox( this, "hb1" );
        hb1->setSpacing( SPACE );
        animationBox->addWidget( hb2, 2, 0 );

	// Create a label
	QLabel * targetL = new QLabel( hb0, "targetL" );
	targetL->setText( " Target Directory: ");

	// Create a text window
	fileLine = new QLineEdit( hb0, "fileLine" );
	fileLine->setText("--");

	// Create a pushbutton
	QPushButton * browsePb = new QPushButton( hb0, "browsePb" );
	browsePb->setText( "Browse..." );

        // Define a callback for that button
	QObject::connect( browsePb, SIGNAL(clicked()), this, SLOT(browseCB()) );

	// Create a label
	numberL= new QLabel( hb1, "numberL" );
	numberL->setText( " Note: Image files in target directory must be named abc.0001.png, abc.0002.png, etc");

	// Create a placeholder 
	QLabel * emptyL1 = new QLabel( hb1, "emptyL2" );
	
	// Create more pushbuttons that will go into the lowest row
	animGIF = new QPushButton( hb2, "animGIF" );
	animGIF->setText( "Create AnimatedGIF" ); 

	 // Define a callback for that button
        QObject::connect( animGIF, SIGNAL(clicked()), this, SLOT(createAGIF()) );

	// Create a placeholder 
	QLabel * emptyL2 = new QLabel( hb2, "emptyL2" );

	// Create more pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( hb2, "done" );
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

	hb0->setStretchFactor( fileLine, 10 );
	hb2->setStretchFactor( emptyL1, 10 );
	hb2->setStretchFactor( emptyL2, 10 );
	this->setMinimumWidth( MIN_FILE_LINE_WIDTH  );

	// Set a flag
	haveTarget = false;
	animGIF->setEnabled( FALSE );
}


// Create animatedGIF
void AnimationBoard::createAGIF()
{
	if (haveTarget) {

		// Create an animated GIF
		char * command = (char *)malloc(BUFSIZ);
	
		sprintf(command, "cd %s; convert *.png* anim.gif", (const char *)targetDir);
		printf("Executing command %s\n", command);
		system(command);

		free(command);	
	}
}


// Launch a file browser
void AnimationBoard::browseCB()
{
	DIR *dirp;
	struct dirent *direntp;
	char * filename = (char *)malloc(BUFSIZ);
	char * buffer = (char *)malloc(BUFSIZ);

	QString fn (Q3FileDialog::getExistingDirectory( QString::null, this, "./") );
	// Register the selection
	if ( !fn.isEmpty() ) {
		targetDir = fn;
		fileLine->setText( targetDir );

		// Set a flag	
		haveTarget = TRUE;
		animGIF->setEnabled( TRUE );
	}

	// Count number of PNG files
	int fileNumber = 0;
	dirp = opendir( (const char *)targetDir);
	while ( (direntp = readdir( dirp )) != NULL ) {
		sprintf(filename, "%s", direntp->d_name );
		if (strstr(filename, ".png")) 
			fileNumber++;
	}

	closedir( dirp );

	// Update the label
	sprintf(buffer, "Number of PNG files in target directory: %d", fileNumber);
	numberL->setText( buffer );	


	free(filename);
	free(buffer);

}


// Hide the board
void AnimationBoard::bdone()
{
	// Hide now
        hide();
}


// Define size policy
QSizePolicy AnimationBoard::sizePolicy() const
{
	return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

