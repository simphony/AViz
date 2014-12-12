//
// Implementation of main menu 
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

#include "aviz.h"

#include <Q3PopupMenu>

// Constructor and deconstructor of main widget
AViz::AViz() 
    : Q3MainWindow( NULL, "aviz", Qt::WDestructiveClose )
{

	// Make a cascade menu to read files
	openfile = new Q3PopupMenu( this );
	openfile->insertItem( "Open XYZ File...", this, SLOT(openXYZ()) );
	openfile->insertItem( "Open File List...", this, SLOT(openList()) );
	openfile->insertItem( "Open ViewParam File...", this, SLOT(openViewParam()) );

	// Make a general file menu 
	file = new Q3PopupMenu( this );
	file->insertItem( "&Open", openfile);
	file->insertItem( "Save ViewParam File...", this, SLOT(saveViewParam()) );
	file->insertSeparator();
	file->insertItem( "File List...", this, SLOT(launchFileList()) );
	file->insertItem( "Animation...", this, SLOT(launchAnimation()) );
	file->insertSeparator();
	inOutWatchModeId = file->insertItem( "Watch XYZ File", this, SLOT(watchFile()) );
	file->insertSeparator();
	file->insertItem( "Snap PNG File...", this, SLOT(savePNGFile()) );
	file->insertSeparator();
	file->insertItem( "Set Default View Param", this, SLOT(setDefaultView()));
	file->insertSeparator();
	file->insertItem( "E&xit",  qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q );

	// Make a cascade menu to set standard view points
	viewpoint = new Q3PopupMenu( this );
	viewpoint->insertItem( "Explicit...", this, SLOT(launchExplicit()) );
	viewpoint->insertSeparator();
	viewpoint->insertItem( "Default View", this, SLOT(setDefaults()) );
	viewpoint->insertSeparator();
	viewpoint->insertItem( "View XY +", this, SLOT(viewXYPlus()) );
	viewpoint->insertItem( "View XY -", this, SLOT(viewXYMinus()) );
	viewpoint->insertSeparator();
	viewpoint->insertItem( "View XZ +", this, SLOT(viewXZPlus()) );
	viewpoint->insertItem( "View XZ -", this, SLOT(viewXZMinus()) );
	viewpoint->insertSeparator();
	viewpoint->insertItem( "View YZ +", this, SLOT(viewYZPlus()) );
	viewpoint->insertItem( "View YZ -", this, SLOT(viewYZMinus()) );

	// Make a submenu for atom specifications 
	elementsAtoms = new Q3PopupMenu( this );
	elementsAtoms->insertItem( "Atoms/Molecules...", this, SLOT(launchAtoms()) );
	elementsAtoms->insertItem( "Bonds...", this, SLOT(launchBonds()) );

	// Make a submenu for polymer specifications 
	elementsPolymers = new Q3PopupMenu( this );
	elementsPolymers->insertItem( "Polymers...", this, SLOT(launchPolymers()) );
	elementsPolymers->insertItem( "Bonds...", this, SLOT(launchBonds()) );

	// Make a general elements menu
	elements = new Q3PopupMenu( this );
	atomsId = elements->insertItem( "Atoms...", elementsAtoms );
	spinsId = elements->insertItem( "Spins...", this, SLOT(launchSpins()) );
	liquidCrystalsId = elements->insertItem( "Liquid Crystals...", this, SLOT(launchLiquidCrystals()) );
	polymersId = elements->insertItem( "Polymers...", elementsPolymers);
	poresId = elements->insertItem( "Pores ...", this, SLOT(launchPores()) );
	elements->insertItem( "Lights...", this, SLOT(launchLights()) );
	elements->insertItem( "Annotation...", this, SLOT(launchAnnotation()) );

	// Make a general view menu 
	view = new Q3PopupMenu( this );
	view->insertItem( "Set &Viewpoint", viewpoint );
	view->insertItem( "Clipping...", this, SLOT(launchClip()) );
	view->insertItem( "Slicing...", this, SLOT(launchSlice()) );

	// Make a general settings menu 
	settings = new Q3PopupMenu( this );
	showHideAxesId = settings->insertItem( "Hide Axes", this, SLOT(showHideAxesCB()) );
	showHideContourId = settings->insertItem( "Hide Contour", this, SLOT(showHideContourCB()) );
	onlyContourId = settings->insertItem( "Only Contour", this, SLOT(onlyContourCB()) );

	// Make a general data menu 
	data = new Q3PopupMenu( this );
	data->insertItem( "&Translate...", this, SLOT(launchTranslation()) );
	data->insertSeparator();
	data->insertItem( "Swap XY", this, SLOT(swapXY()) );
	data->insertItem( "Swap XZ", this, SLOT(swapXZ()) );
	data->insertItem( "Swap YZ", this, SLOT(swapYZ()) );
	data->insertSeparator();
	data->insertItem( "Mirror X", this, SLOT(mirrorX()) );
	data->insertItem( "Mirror Y", this, SLOT(mirrorY()) );
	data->insertItem( "Mirror Z", this, SLOT(mirrorZ()) );
	data->insertSeparator();
	data->insertItem( "Stretch XYZ...", this, SLOT(launchStretch()) );

	// Make a general help menu 
	help = new Q3PopupMenu( this );
	help->insertItem( "&About", this, SLOT(about()), Qt::CTRL+Qt::Key_H );
	help->insertItem( "License", this, SLOT(license()) );
	help->insertItem( "Distribution", this, SLOT(distribute()) );

	menuBar()->insertItem( "&File", file );
	menuBar()->insertItem( "&Elements", elements);
	menuBar()->insertItem( "&View", view);
	menuBar()->insertItem( "&Settings", settings);
	menuBar()->insertItem( "&Data", data);
	menuBar()->insertSeparator();
	menuBar()->insertItem( "&Help", help );
	menuBar()->setSeparator( QMenuBar::InWindowsStyle );

    // Now construct the main form
    // (Note having aviz as paramter for MainForm ctor
    // is "This is an ugly hack, intended to propagate the
    // address of the calling class")
    mf = new MainForm(this/*parent*/, this /*aviz*/);
    setCentralWidget(mf);

    // Construct a timer
    watchTimer = new QTimer( this, "watchTimer" );

    // Set initial settings
    this->setAtomMenus();
}


// Destructor
AViz::~AViz()
{
    
}


// Set the file type to be used during reading or writing files
void AViz::setFileType( fileType thisFt )
{
	
	if (mf) 
		mf->setFileType( thisFt );
}


// Get the file type to be used during reading or writing files
fileType AViz::isFileType( void )
{
	if (mf) 
		return (mf)->isFileType();
	else
		return UNKNOWN;
}


// Open a XYZ coordinate file
void AViz::openXYZ( void )
{
	// Prepare reading of a coordinate file
	this->setFileType( XYZ_FILE );

	// Launch a customized file selector (with generate file list button)
	Q3FileDialog * fd = new CustomFileDialog();
	fd->setMode( Q3FileDialog::ExistingFile );
	QString filter = "XYZ coordinate files (*.xyz)";
	fd->setFilters( filter );

	connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
	fd->show();
}


// Open a list of coordinate files
void AViz::openList( void )
{
        // Prepare reading of a list of coordinate files
        this->setFileType( ANIMATION);

        // Launch a standard file selector
        Q3FileDialog * fd = new Q3FileDialog();
        fd->setMode( Q3FileDialog::ExistingFile );
        QString filter = "General (*)";
        fd->setFilters( filter );

        connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
        fd->show();
}



// Read the parameters defining view parameters
void AViz::openViewParam( void )
{
	// Prepare reading of a view parameters file
	this->setFileType( VP_FILE );

	// Launch standard a file selector
	Q3FileDialog * fd = new Q3FileDialog();
	fd->setMode( Q3FileDialog::ExistingFile );
	QString filter = "AViz viewpoint files (*.vpm)";
	fd->setFilters( filter );

	connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
	fd->show();
}


// Save the parameters defining view parameters
void AViz::saveViewParam( void )
{
	// Prepare writing of a view parameters files
	this->setFileType( VP_FILE );

	// Launch a standard file selector
	Q3FileDialog * fd = new Q3FileDialog();
	fd->setMode( Q3FileDialog::AnyFile );
	QString filter = "AViz viewpoint files (*.vpm)";
	fd->setFilters( filter );

	connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(writeFile(const QString&)) );
	fd->show();
}


// Save the parameters defining default view parameters
void AViz::setDefaultView( void )
{
	char * filename = (char *)malloc( BUFSIZ );

        // Prepare writing of a view parameter file
        this->setFileType( VP_FILE );
	
	// Write the file now in the home directory
	sprintf( filename, "%s/.aviz/%s", getenv("HOME"), defaultViewParamFile);
	writeFile( filename );

        free(filename);
}


// Set factory defaults (overloaded function)
void AViz::setDefaults( void )
{
	char * filename = (char *)malloc( BUFSIZ );
        struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
	particleData * thisPd;
	viewParam thisVp;
        bool atomExistFlag = FALSE;
	bool defaultExistFlag = FALSE;

	// Get a pointer to the particle data
	if (mf) 
		thisPd = mf->getParticleData();
	else {
		printf ("Error: cannot get pointer to particle data.  Abort.\n");
		exit(0);
	}

        // Always start by setting default values
        useDefaultParticleData( thisPd );

        // Is there a particle data file in the home directory?
        sprintf( filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile );
        if (!stat( (const char *)filename, buf )) {
                // A atom data file exists in the home directory --
                // read it
		atomExistFlag = TRUE;
                if (!openParticleDataFunction( filename, thisPd )) {
                        // Read failed: use built-in defaults
			if (mf)
				mf->statusMessage( "Could not read %s -- using defaults.", filename);
                        printf("Could not load %s -- using defaults.\n", filename);
		}
        }
	
	// If no atom data file exists, use built-in defaults
        if (atomExistFlag == FALSE) {
		if (mf)
			mf->statusMessage( "Could not stat %s -- using defaults.", filename);
		printf("Could not stat %s -- using defaults.\n", filename);
	}

	// Is there a default view param file in the home directory?
	sprintf( filename, "%s/.aviz/%s", getenv("HOME"), defaultViewParamFile);
        if (!stat( (const char *)filename, buf )) {
		// A default view param file exists in the home directory --
		// read it
		defaultExistFlag = TRUE;
		if (openViewParamFunction( (const char *)filename, &thisVp )) {
			// If successful, send the parameters to the main form
			if (mf) 
				mf->setDefaults( thisVp, (const char *)filename );
		}
		else {
			if (mf) 
				mf->statusMessage( "Could not read %s -- using defaults.", filename);
			printf("Could not load %s -- using defaults.\n", filename);
			// Use factory defaults if this failed
			if (mf) 
				mf->setDefaults();
		}
	}

	// If now default view param file exists, use factory defaults
	if (defaultExistFlag == FALSE) {
		if (mf) 
			mf->statusMessage( "Could not stat %s -- using defaults.", filename);
		printf("Could not stat %s -- using defaults.\n", filename);

		// Use factory defaults if this failed
		if (mf) 
			mf->setDefaults();
	}

        free(filename);
        free(buf);
}


// Set factory defaults (overloaded function)
void AViz::setDefaults( char * filename0 )
{
	char * filename1 = (char *)malloc( BUFSIZ );
        struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
	particleData * thisPd;
	viewParam thisVp;
        bool atomExistFlag = FALSE;

	// Get a pointer to the particle data
	if (mf) 
		thisPd = mf->getParticleData();
	else {
		printf ("Error: cannot get pointer to particle data.  Abort.\n");
		exit(0);
	}

        // Always start by allocating memory and setting default values
        useDefaultParticleData( thisPd );

        // Is there a particle data file in the home directory?
        sprintf( filename1, "%s/.aviz/%s", getenv("HOME"), particleDataFile );
        if (!stat( (const char *)filename1, buf )) {
                // A atom data file exists in the home directory --
                // read it
		atomExistFlag = TRUE;
                if (!openParticleDataFunction( filename1, thisPd )) {
                        // Read failed: use built-in defaults
			if (mf)
				mf->statusMessage( "Could not read %s -- using defaults.", filename1 );
                        printf("Could not read %s -- using defaults.\n", filename1);
		}
        }
	
	// If no atom/spin data file exists, use built-in defaults
        if (atomExistFlag == FALSE) {
		if (mf)
			mf->statusMessage( "Could not stat %s -- using defaults.", filename1);
		printf("Could not stat %s -- using defaults.\n", filename1);
	}

	// Read the specified view parameters 
	if (openViewParamFunction( (const char *)filename0, &thisVp )) {

		// If successful, send them on to the main form
		if (mf) 
			mf->setDefaults( thisVp, (const char *)filename0 );
	}
	else {
		if (mf) 
			mf->statusMessage( "Could not load", (const char *)filename0 );
		printf( "Could not load %s\n", (const char *)filename0 );

		// Use factory defaults if this failed
		if (mf) 
			mf->setDefaults();
	}

        free(filename1);
        free(buf);
}


// Read a file
void AViz::readFile( const QString &fn )
{
	// Check if the file type was set correctly, based on 
	// file name 
	if ( strstr((const char *)fn, ".xyz")) 
		this->setFileType( XYZ_FILE );
	if ( strstr((const char *)fn, "filelist.dat") || !strstr((const char *)fn, ".")) 
		this->setFileType( ANIMATION );
	if ( strstr((const char *)fn, ".vpm")) 
		this->setFileType( VP_FILE );

	if (mf) {
		mf->readFile( fn );

		// Launch the file list board upon reading 
		// a list of files
		if (this->isFileType() == ANIMATION) {
			mf->launchFileList( fn );
		}
	}
}


// Write a file
void AViz::writeFile( const QString &fn )
{
	if (mf) 
		mf->writeFile( fn );
}


// Go in and out of watch mode
void AViz::watchFile( void )
{
	switch (inWatchMode) {
		case FALSE:
			// Start the timer, using the command-line callback
			this->setWatchMode();

			// Write a message	
			if (mf) {	
				aggregateData * ad = mf->getAggregateData();
				mf->statusMessage( "Start watching file", (*ad).filename );
			}

			// Adjust the menu entry
			file->changeItem( inOutWatchModeId, "Stop Watching" );

			// Set a flag
			inWatchMode = TRUE;
		break;	
		case TRUE:	
			// Stop the timer
        		watchTimer->stop();

			// Write a message	
			if (mf) {	
				aggregateData * ad = mf->getAggregateData();
				mf->statusMessage( "Stop watching file", (*ad).filename );
			}

			// Adjust the menu entry
			file->changeItem( inOutWatchModeId, "Watch XYZ File..." );
			// Reset a flag
			inWatchMode = FALSE;
		break;
	}
}


// Show the file list panel
void AViz::launchFileList( void )
{
	if (mf) 
		mf->launchFileList();
}


// Show the animation panel -- a convenience function
void AViz::launchAnimation( void )
{
	AnimationBoard * ab = new AnimationBoard();
	ab->show();
}


// Help callback function
void AViz::about( void )
{
	// Launch a message box
	MessageBox * mb = new MessageBox( this, "mb" );
	mb->setText( VERSION_STRING );
	mb->show();
}


// Help callback function
void AViz::license( void )
{
        // Launch a message box
        MessageBox * mb = new MessageBox( this, "mb" );
        mb->setText( SHORT_LICENSE_STRING );
        mb->show();

	// Print the full license conditions
	printf("%s\n", LICENSE_STRING );
	printf("%s\n", DISTRIBUTE_STRING );
}


// Help callback function
void AViz::distribute( void )
{
        // Launch a message box
        MessageBox * mb = new MessageBox( this, "mb" );
        mb->setText( SHORT_DISTRIBUTE_STRING );
        mb->show();

	// Print the full distribution conditions
	printf("%s\n", DISTRIBUTE_STRING );
}


// Callback function to set fixed view parameters
void AViz::viewXYPlus( void )
{
	if (mf) 
		mf->setViewXYPlus();
}


// Callback function to set fixed view parameters
void AViz::viewXYMinus( void )
{
	if (mf) 
		mf->setViewXYMinus();
}


// Callback function to set fixed view parameters
void AViz::viewXZPlus( void )
{
	if (mf) 
		mf->setViewXZPlus();
}


// Callback function to set fixed view parameters
void AViz::viewXZMinus( void )
{
	if (mf) 
		mf->setViewXZMinus();
}


// Callback function to set fixed view parameters
void AViz::viewYZPlus( void )
{
	if (mf) 
		mf->setViewYZPlus();
}


// Callback function to set fixed view parameters
void AViz::viewYZMinus( void )
{
	if (mf) 
		mf->setViewYZMinus();
}


// Command function to start auto rot/tilt/spin
void AViz::startAutoRot( void )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();
		(*thisVp).autoRot1 = TRUE;
		mf->setAutoAndStart( (*thisVp) );
	}
}


// Command function to start auto rot/tilt/spin
void AViz::startAutoTilt( void )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();
		(*thisVp).autoTilt1 = TRUE;
		mf->setAutoAndStart( (*thisVp) );
	}
}


// Command function to start auto rot/tilt/spin
void AViz::startAutoSpin( void )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();
		(*thisVp).autoSpin1 = TRUE;
		mf->setAutoAndStart( (*thisVp) );
	}
}


// Command line function to start auto zooming
void AViz::startAutoZoom( void )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();
		(*thisVp).autoZoom1 = TRUE;
		mf->setAutoAndStart( (*thisVp) );
	}
}


// Command line function to start auto snapping
void AViz::startAutoSnap( void )
{
        if (mf) {
                // Read the current view parameters
                viewParam * thisVp = mf->getViewParam();
                (*thisVp).autoSnap = TRUE;
                mf->setAutoAndStart( (*thisVp) );
        }
}


// Command line function to read a file list
void AViz::useFileList( char * ptr )
{
        // Prepare reading of a list of coordinate files
        this->setFileType( ANIMATION);

	// Read the list and produce the file list board
	QString fl = ptr;
	this->readFile( fl );
}


// Command line function to set rendering mode
void AViz::setRenderMode( char * ptr )
{
	if (mf) {
		// Read the current view parameters
		viewParam * thisVp = mf->getViewParam();				
	        // Set a new render mode and refresh drawing
		if (strcmp( ptr, "atom" ) == 0) {
			(*thisVp).renderMode = ATOMS;
			this->setAtomMenus();
		}
		if (strcmp( ptr, "spin" ) == 0) {
			(*thisVp).renderMode = SPINS;
			this->setSpinMenus();
		}
		if (strcmp( ptr, "liquidcrystal" ) == 0) {
			(*thisVp).renderMode = LIQUID_CRYSTALS;
			this->setLcMenus();
		}
		if (strcmp( ptr, "polymer" ) == 0) {
			(*thisVp).renderMode = POLYMERS;
			this->setPolymerMenus();
		}
		if (strcmp( ptr, "pore" ) == 0) {
			(*thisVp).renderMode = PORES;
			this->setPoreMenus();
		}

                mf->setViewParam( (*thisVp) );
		mf->updateView();
                mf->updateRendering();
        }
}


// Command line function to set rendering style
void AViz::setRenderStyle( char * ptr )
{
	if (mf) {
		// Read the current view parameters
		viewParam * thisVp = mf->getViewParam();				
	        // Set a new render style and refresh drawing.
		if (strcmp( ptr, "dots" ) == 0) {
			(*thisVp).atomRenderStyle = ADOT;
			(*thisVp).spinRenderStyle = SDOT;
			(*thisVp).lcRenderStyle = LDOT;
			(*thisVp).poreRenderStyle = PDOT;
		}
		if (strcmp( ptr, "lines" ) == 0) {
			(*thisVp).atomRenderStyle = ALINE;
			(*thisVp).spinRenderStyle = SLINE;
			(*thisVp).lcRenderStyle = LLINE;
			(*thisVp).poreRenderStyle = PLINE;
		}
		if (strcmp( ptr, "cubes" ) == 0) {
			(*thisVp).atomRenderStyle = ACUBE;
			(*thisVp).spinRenderStyle = SCUBE;
			(*thisVp).lcRenderStyle = LCUBE;
			(*thisVp).poreRenderStyle = PCUBE;
		}
		if (strcmp( ptr, "cones" ) == 0) {
			(*thisVp).atomRenderStyle = ACONE;
			(*thisVp).spinRenderStyle = SCONE;
			(*thisVp).lcRenderStyle = LCONE;
			(*thisVp).poreRenderStyle = PCONE;
		}
		if (strcmp( ptr, "cylinders" ) == 0) {
			(*thisVp).atomRenderStyle = ACYLINDER;
			(*thisVp).spinRenderStyle = SCYLINDER;
			(*thisVp).lcRenderStyle = LCYLINDER;
			(*thisVp).poreRenderStyle = PCYLINDER;
		}
		if (strcmp( ptr, "spheres" ) == 0) {
			(*thisVp).atomRenderStyle = ASPHERE;
			(*thisVp).spinRenderStyle = SSPHERE;
			(*thisVp).lcRenderStyle = LSPHERE;
			(*thisVp).poreRenderStyle = PSPHERE;
		}

                mf->setViewParam( (*thisVp) );
		mf->updateView();
                mf->updateRendering();
	}
}


// Command line function to set rendering mode
void AViz::setRenderQuality( char * ptr )
{
	if (mf) {
        	// Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();

	        // Set a new render mode and refresh drawing
	        if (strcmp( ptr, "low" ) == 0) {
			(*thisVp).renderQuality = LOW;
		}
	        if (strcmp( ptr, "high" ) == 0) {
	                (*thisVp).renderQuality = HIGH;
	        }
	        if (strcmp( ptr, "final" ) == 0) {
	                (*thisVp).renderQuality = FINAL;
	        }
	
                mf->setViewParam( (*thisVp) );
		mf->updateView();
                mf->updateRendering();
        }
}


// Command line function to set rendering size 
void AViz::setRenderSize( int size )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();
	
		(*thisVp).renderSize = (double)size/100.0;

                mf->setViewParam( (*thisVp) );
		mf->updateView();
                mf->updateRendering();
        }
}

// Command line function to set the annotation string
void AViz::setAnnotationString( char* annotationString )
{
        if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();

		strcpy((*thisVp).annotationText, annotationString);

                mf->setViewParam( (*thisVp) );
		mf->updateView();
                mf->updateRendering();
        }
}

// Save a PNG screenshot file
void AViz::savePNGFile( void )
{
	// Prepare reading of a coordinate file
	this->setFileType( PNG_FILE );

	// Launch a standard file selector
	Q3FileDialog * fd = new Q3FileDialog();
	fd->setMode( Q3FileDialog::AnyFile );
	QString filter = "PNG image files (*.png)";
	fd->setFilters( filter );

	connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(snapFile(const QString&)) );
	fd->show();
}


// Snap a PNG screenshot file
void AViz::snapFile( const QString &fn )
{
        if (mf) {
                mf->updateRendering();
                mf->snapRendering( fn );
        }
}


// Command line function to trigger screen shot
void AViz::setSnap( void )
{
	QTimer * snapTimer = new QTimer( this, "snapTimer" );
	connect(snapTimer, SIGNAL(timeout()), this, SLOT(setSnapNow()) );

	// Wait 1 s to have the rendering completed and 
	// then take a screen shot
       	snapTimer->start( 1000, TRUE );
}


// Command line function to trigger screen shot and exit
void AViz::setSnapq( void )
{
        QTimer * snapTimer = new QTimer( this, "snapTimer" );
        connect(snapTimer, SIGNAL(timeout()), this, SLOT(setSnapqNow()) );

	// Wait 1 s to have the rendering completed;
	// then take a screen shot and exit
        snapTimer->start( 1000, TRUE );
}


// Used to trigger screen shot
void AViz::setSnapNow( void )
{
        if (mf) {
                mf->updateRendering();
                mf->snapRendering();
        }
}


// Used to trigger screen shot and exit
void AViz::setSnapqNow( void )
{
        if (mf) {
                mf->updateRendering();
                mf->snapRendering();
        }

	// Exit now
	exit(0);
}


// Command line function to set watch file mode, with 
// automatic update of the rendering upon changes
void AViz::setWatchMode( void )
{
        connect(watchTimer, SIGNAL(timeout()), this, SLOT(keepWatchNow()) );

        // Wait 10 s and check repeatedly if the file has been modified
        watchTimer->start( 10000, FALSE );

	// Set a flag
	inWatchMode = TRUE;
}


// Adjust the menu entries
void AViz::setAtomMenus( void )
{
	elements->setItemEnabled( atomsId, TRUE );
	elements->setItemEnabled( spinsId, FALSE );
	elements->setItemEnabled( liquidCrystalsId, FALSE );
	elements->setItemEnabled( polymersId, FALSE );
	elements->setItemEnabled( poresId, FALSE );
}


// Adjust the menu entries
void AViz::setSpinMenus( void )
{
	elements->setItemEnabled( atomsId, FALSE );
	elements->setItemEnabled( spinsId, TRUE );
	elements->setItemEnabled( liquidCrystalsId, FALSE );
	elements->setItemEnabled( polymersId, FALSE );
	elements->setItemEnabled( poresId, FALSE );
}


// Adjust the menu entries
void AViz::setLcMenus( void )
{
	elements->setItemEnabled( atomsId, FALSE );
	elements->setItemEnabled( spinsId, FALSE );
	elements->setItemEnabled( liquidCrystalsId, TRUE );
	elements->setItemEnabled( polymersId, FALSE );
	elements->setItemEnabled( poresId, FALSE );
}

// Adjust the menu entries
void AViz::setPolymerMenus( void )
{
	elements->setItemEnabled( atomsId, FALSE );
	elements->setItemEnabled( spinsId, FALSE );
	elements->setItemEnabled( liquidCrystalsId, FALSE );
	elements->setItemEnabled( polymersId, TRUE );
	elements->setItemEnabled( poresId, FALSE );
}


// Adjust the menu entries
void AViz::setPoreMenus( void )
{
	elements->setItemEnabled( atomsId, FALSE );
	elements->setItemEnabled( spinsId, FALSE );
	elements->setItemEnabled( liquidCrystalsId, FALSE );
	elements->setItemEnabled( polymersId, FALSE );
	elements->setItemEnabled( poresId, TRUE );
}


// Used to trigger updates of the rendering upon change of 
// current XYZ file
void AViz::keepWatchNow( void )
{
	char * filename = (char *)malloc( BUFSIZ );
        struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
//	time_t last_access;
//	time_t last_modification;
	time_t last_change;
	time_t now = time( NULL );

	// Get the current data set and retrieve filename
        if (mf) {
		aggregateData * ad = mf->getAggregateData();

        	sprintf( filename, "./%s", (*ad).filename);
	        if (!stat( (const char *)filename, buf ) ) {
//			last_access = (*buf).st_atime;
//			last_modification = (*buf).st_mtime;
			last_change = (*buf).st_ctime;

			// Was the file was modified during the last cycle?
			if (last_change > now-10) {
				// Yes: update rendering
				this->readFile( (*ad).filename );
				mf->updateRendering();

				mf->statusMessage( "Updating rendering...");
			}	
			else {
				// No: do nothing
				mf->statusMessage( "Watching XYZ file", (*ad).filename );
			}
		}
	}	

	free(filename);
        free(buf);
}


// Callback function to launch the atoms settings panel
void AViz::launchAtoms( void )
{
	if (mf) 
		mf->launchAtoms();
}


// Callback function to launch the annotation settings panel
void AViz::launchAnnotation( void )
{
	if (mf) 
		mf->launchAnnotation();
}


// Callback function to launch the bond settings panel
void AViz::launchBonds( void )
{
	if (mf) 
		mf->launchBonds();
}


// Callback function to launch the clip settings panel
void AViz::launchClip( void )
{
	if (mf) 
		mf->launchClip();
}


// Callback function to launch the explicit view point settings panel
void AViz::launchExplicit( void )
{
	if (mf) 
		mf->launchExplicit();
}


// Callback function to launch the lights settings panel
void AViz::launchLights( void )
{
	if (mf) 
		mf->launchLights();
}


// Callback function to launch the liquid crystal settings panel
void AViz::launchLiquidCrystals( void )
{
	if (mf) 
		mf->launchLiquidCrystals();
}


// Callback function to launch the slicing settings panel
void AViz::launchSlice( void )
{
	if (mf) 
		mf->launchSlice();
}


// Callback function to launch the spin settings panel
void AViz::launchSpins( void )
{
	if (mf) 
		mf->launchSpins();
}


// Callback function to launch the stretch settings panel
// (to manipulate data)
void AViz::launchStretch( void )
{
	if (mf) 
		mf->launchStretch();
}


// Callback function to launch the polymers settings panel
void AViz::launchPolymers( void )
{
	if (mf) 
		mf->launchPolymers();
}


// Callback function to launch the pore settings panel
void AViz::launchPores( void )
{
	if (mf) 
		mf->launchPores();
}


// Callback function to launch the translation settings panel
void AViz::launchTranslation( void )
{
	if (mf) 
		mf->launchTranslation();
}


// Show or hide axes
void AViz::showHideAxesCB( void )
{
	if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();

		// Toggle the axes flag
		if ((*thisVp).showAxes == TRUE) {
			(*thisVp).showAxes = FALSE;
			settings->changeItem( showHideAxesId, "Show Axes" );
		}
		else {
			(*thisVp).showAxes = TRUE;
			settings->changeItem( showHideAxesId, "Hide Axes" );
		}

		// Update the rendering
		mf->setViewParam( (*thisVp) );
		mf->updateView();
		mf->updateRendering();
	}
}


// Show or hide the contour
void AViz::showHideContourCB( void )
{
	if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();

		// Toggle the show contour flag
		if ((*thisVp).showContour == TRUE) {
			(*thisVp).showContour = FALSE;
			settings->changeItem( showHideContourId, "Show Contour" );
			settings->setItemEnabled( onlyContourId, FALSE );
		}
		else {
			(*thisVp).showContour = TRUE;
			settings->changeItem( showHideContourId, "Hide Contour" );
			settings->setItemEnabled( onlyContourId, TRUE );
		}

		// Update the rendering
		mf->setViewParam( (*thisVp) );
		mf->updateView();
		mf->updateRendering();
	}
}


// Draw only contour or draw all the scene
void AViz::onlyContourCB( void )
{
	if (mf) {
	        // Read the current view parameters
	        viewParam * thisVp = mf->getViewParam();

		// Toggle the show contour flag
		if ((*thisVp).onlyContour == TRUE) {
			(*thisVp).onlyContour = FALSE;
			settings->changeItem( onlyContourId, "Only Contour" );
			settings->setItemEnabled( showHideContourId, TRUE );
		}
		else {
			(*thisVp).onlyContour = TRUE;
			settings->changeItem( onlyContourId, "Contour And Particles" );
			settings->setItemEnabled( showHideContourId, FALSE );
		}

		// Update the rendering
		mf->setViewParam( (*thisVp) );
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::swapXY( void )
{
	if (mf) {
		// Write a message
		mf->statusMessage( "Swapping X <-> Y" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Swap now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			float tmp = (*ad).particles[i].x;
			(*ad).particles[i].x = (*ad).particles[i].y;
			(*ad).particles[i].y = tmp;
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::swapXZ( void )
{
	if (mf) {
		// Write a message
		mf->statusMessage( "Swapping X <-> Z" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Swap now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			float tmp = (*ad).particles[i].x;
			(*ad).particles[i].x = (*ad).particles[i].z;
			(*ad).particles[i].z = tmp;
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::swapYZ( void )
{
	if (mf) {
		// Write a message
		mf->statusMessage( "Swapping Y <-> Z" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Swap now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			float tmp = (*ad).particles[i].y;
			(*ad).particles[i].y = (*ad).particles[i].z;
			(*ad).particles[i].z = tmp;
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::mirrorX( void )
{
	if (mf) {
		// Write a message
		mf->statusMessage( "Mirroring X -> -X" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Mirror now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			(*ad).particles[i].x *= (-1.0);
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::mirrorY( void )
{
	if (mf) {
		// Write a message
		mf->statusMessage( "Mirroring Y -> -Y" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Mirror now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			(*ad).particles[i].y *= (-1.0);
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
	}
}


// Callback function to manipulate data
void AViz::mirrorZ( void )
{
    if (mf) {
        // Write a message
		mf->statusMessage( "Mirroring Z -> -Z" );

		// Get local copy of the current data set
		aggregateData * ad = mf->getAggregateData();

		// Mirror now
		for (int i=0;i<(*ad).numberOfParticles;i++) {
			(*ad).particles[i].z *= (-1.0);
		}

		// Let this be the current aggregate and render
		mf->updateView();
		mf->updateRendering();
    }
}
