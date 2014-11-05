//
// Implementation of customized file dialog class
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

#include "customFileDialog.h"

// Define a file dialog with a generate file list button
CustomFileDialog::CustomFileDialog( )
    : QFileDialog( 0, 0, TRUE )
{
	QPushButton * genFileListPb = new QPushButton( this );
	genFileListPb->setText( "Generate File List" );

	addToolButton( genFileListPb, FALSE );

	// Add a callback function
	connect( genFileListPb, SIGNAL(clicked()), SLOT(generateFileList()) );
}


// Deconstructor
CustomFileDialog::~CustomFileDialog()
{
	// Do nothing
}

// Callback function to generate a file list
void CustomFileDialog::generateFileList( )
{
	// Get the current directory
	QString currentDir = this->dirPath();

	// Generate a file list
	char * command = (char *)malloc(BUFSIZ);
	sprintf(command, "cd %s; ls *xyz* |sort > filelist.dat", (const char *)currentDir );
	system(command);
	free(command);

	// Re-read the directory
	QString filter = "General (*)";
	this->setFilters( filter );
	this->rereadDir();
}
