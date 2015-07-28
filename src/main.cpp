// Main -- open the display window and start to run

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

#include "aviz.h"
#include "fileFunctions.h"
#include "version.h"
#include "license.h"
#include "data.h"

#include <qapplication.h>
//
#include <qgl.h>
#include <qstring.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Print usage instructions
void printUsage()
{
	printf("Usage: aviz [ command line arguments ] [ Qt-options ] [X11-options] [XYZfile] \n");
	printf("Command line argumets:\n");
	printf("-ar 	Start auto rotation\n");
	printf("-as 	Start auto spinning\n");
	printf("-at 	Start auto tilting\n");
	printf("-az 	Start auto zooming\n");
	printf("-asm 	Start in auto snap mode\n");
	printf("-fl [list_file] Use file list \n");
	printf("-rm [atom|spin|liquidcrystal|polymer|pore]	Set render mode\n");
	printf("-rs [dots|lines|cubes|cones|cylinders|spheres|]	Set render style\n");
	printf("-rq [low|high|final]	Set render quality\n");
	printf("-si [percentage]	Set render size in percentage\n");
	printf("-annot [string]		Set the annotation string\n");
	printf("-snap 		Take a screen shot of the rendering at startup\n");
	printf("-snapq 		Take a screen shot of the rendering at startup and exit\n");
	printf("-v(ersion)	Print version number and exit\n");
	printf("-vpm [viewParamfile]	Use view parameters in specified file at startup\n");
	printf("-watch [XYZfile]	Update rendering upon changes in data set\n");
	printf("-license 	Print GNU license applying to this software\n");
	printf("-distribution 	Print GNU regulations under applying to this software\n");
	printf("-usage|-h(elp)	Print this message and exit\n");
	printf("dan-test\n");
}


// Main function
int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	
	char ** ptr;

	if ( !QGLFormat::hasOpenGL() ) {
		qWarning( "This system has no OpenGL support. Exiting." );
		return -1;
	}

	// Create a .aviz directory in the user's home directory, if
	// it does not exist
	char * filename = (char *)malloc(BUFSIZ);
	sprintf(filename, "%s/.aviz", getenv( "HOME" ));
	struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
	if (stat( (const char *)filename, buf) != 0) {
		printf("Creating directory %s (only first time)...\n", filename);

		char * command = (char *)malloc(BUFSIZ);
		sprintf(command, "mkdir -p %s", filename);
		system(command);
		free(command);

		printf("done.\n");
	}
	free(filename);
	free(buf);

	// Check for command line options, 1st pass
	for (ptr = argv+1; *ptr != NULL; ++ptr) {
		if (strcmp( *ptr, "-help" ) == 0  || strcmp( *ptr, "-h" ) == 0 || strcmp( *ptr, "-usage" ) == 0) {
			printUsage();
			return 0;
	        }
		else if (strcmp( *ptr, "-version" ) == 0 || strcmp( *ptr, "-v" ) == 0) {
            printf( "AViz version %s \n", qPrintable(aviz::version_info_string));
			return 0;
        	}
		else if (strcmp( *ptr, "-license" ) == 0) {
			printf( "%s \n", LICENSE_STRING );
			printf( "%s \n", DISTRIBUTE_STRING );
			return 0;
		}
		else if (strcmp( *ptr, "-distribution" ) == 0) {
			printf( "%s \n", DISTRIBUTE_STRING );
			return 0;
		}
	}

	// Construct the main panel
	AViz * av = new AViz();

	// Set defaults
	av->setDefaults();

	// Check for command line options, 2nd pass
	for (ptr = argv+1; *ptr != NULL; ++ptr) {
		// Check for valid command line options
		if (strcmp( *ptr, "-ar" ) == 0) {
			av->startAutoRot( );
		}
		else if (strcmp( *ptr, "-as" ) == 0) {
			av->startAutoSpin( );
		}
		else if (strcmp( *ptr, "-at" ) == 0) {
			av->startAutoTilt( );
		}
		else if (strcmp( *ptr, "-az" ) == 0) {
			av->startAutoZoom( );
		}
		else if (strcmp( *ptr, "-asm" ) == 0) {
			av->startAutoSnap( );
		}
		else if (strcmp( *ptr, "-fl" ) == 0) {
			ptr++;
			if ( *ptr && fileExists( *ptr) ) {
				av->useFileList( *ptr );
			}
			else {
				printUsage();
				return 0;
			}
		}
		else if (strcmp( *ptr, "-rm" ) == 0) {
			ptr++;
			if ( *ptr ) {
				if (strcmp( *ptr, "atom" ) == 0) {
					av->setRenderMode( *ptr );
				}
				if (strcmp( *ptr, "spin" ) == 0) {
					av->setRenderMode( *ptr );
				}
				if (strcmp( *ptr, "liquidcrystal" ) == 0) {
					av->setRenderMode( *ptr );
				}
				if (strcmp( *ptr, "polymer" ) == 0) {
					av->setRenderMode( *ptr );
				}
				if (strcmp( *ptr, "pore" ) == 0) {
					av->setRenderMode( *ptr );
				}
			}
		}
		else if (strcmp( *ptr, "-rs" ) == 0) {
			ptr++;
			if ( *ptr ) {
				if (strcmp( *ptr, "dots" ) == 0) {
					av->setRenderStyle( *ptr );
				}
				if (strcmp( *ptr, "lines" ) == 0) {
					av->setRenderStyle( *ptr );
				}
				if (strcmp( *ptr, "cubes" ) == 0) {
					av->setRenderStyle( *ptr );
				}
				if (strcmp( *ptr, "cones" ) == 0) {
					av->setRenderStyle( *ptr );
				}
				if (strcmp( *ptr, "cylinders" ) == 0) {
					av->setRenderStyle( *ptr );
				}
				if (strcmp( *ptr, "spheres" ) == 0) {
					av->setRenderStyle( *ptr );
				}
			}
		}
		else if (strcmp( *ptr, "-rq" ) == 0) {
			ptr++;
			if ( *ptr ) {
				if (strcmp( *ptr, "draft" ) == 0) {
					av->setRenderQuality( *ptr );
				}
				if (strcmp( *ptr, "low" ) == 0) {
					av->setRenderQuality( *ptr );
				}
				if (strcmp( *ptr, "high" ) == 0) {
					av->setRenderQuality( *ptr );
				}
				if (strcmp( *ptr, "final" ) == 0) {
					av->setRenderQuality( *ptr );
				}
			}
		}
		else if (strcmp( *ptr, "-si" ) == 0) {
			ptr++;
			if ( *ptr ) {
				int size = atoi( *ptr );
				av->setRenderSize( size );
			}
		}
		else if (strcmp( *ptr, "-vpm" ) == 0) {
			ptr++;
			if ( *ptr && fileExists( *ptr) ) {
				av->setDefaults( *ptr );
			}
			else {
				printUsage();
				return 0;
			}
		}
		else if (strcmp( *ptr, "-annot" ) == 0) {
			ptr++;
			if ( *ptr ) {
				av->setAnnotationString( *ptr );
			}

		}
		else if (strcmp( *ptr, "-watch" ) == 0) {
			av->setWatchMode( );
		}
		// Accept all sorts of data file names
		else if (argc > 1) {
			// Do not confuse data file arguments with
			// the command itself, but do not exclude
			// filenames that contain a dash
			if (strstr( *ptr, "-" ) == NULL || strstr( *ptr, "xyz") != NULL || strstr( *ptr, "XYZ") != NULL) {
				QString file = QString( *ptr );
				av->setFileType( XYZ_FILE );
				av->readFile( file );
        		}
        	}
        }


	// Print disclaimer
    printf( "%s \n", qPrintable(aviz::start_string) );

	// Manage the panel 
	av->setGeometry(0, FRAME_SIZE+FRAME_WIDTH+3, av->sizeHint().width(), av->sizeHint().height()); 
	av->show();

	// Check for command line options, 3rd pass
	for (ptr = argv+1; *ptr != NULL; ++ptr) {
		// Check for valid command line options
		if (strcmp( *ptr, "-snap" ) == 0) {
        		av->setSnap();
		}
		else if (strcmp( *ptr, "-snapq" ) == 0) {
        		av->setSnapq();
		}
	}

	return a.exec();
}	
