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

#include <sys/stat.h>

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QTimer>

#include "animationBoard.h"
#include "customFileDialog.h"
#include "defaultParticles.h"
#include "license.h"
#include "mainForm.h"
#include "messageBox.h"
#include "version.h"

AViz::AViz() 
    : QMainWindow()
{
    // Make menus in menubar
    QMenu *file = menuBar()->addMenu("&File");
    QMenu *elements = menuBar()->addMenu( "&Elements");
    QMenu *view = menuBar()->addMenu( "&View");
    QMenu *settings = menuBar()->addMenu( "&Settings");
    QMenu *data = menuBar()->addMenu("&Data");
    menuBar()->addSeparator();
    QMenu *help = menuBar()->addMenu("&Help");

    // Make a cascade menu to read files
    QMenu *openfile = file->addMenu("&Open");
    openfile->addAction("Open XYZ File...", this, SLOT(openXYZ()));
    openfile->addAction("Open File List...", this, SLOT(openList()));
    openfile->addAction("Open ViewParam File...", this, SLOT(openViewParam()));



    file->addAction( "Save ViewParam File...", this, SLOT(saveViewParam()) );
    file->addSeparator();
    file->addAction( "File List...", this, SLOT(launchFileList()) );
    file->addAction( "Animation...", this, SLOT(launchAnimation()) );
    file->addSeparator();
    m_inOutWatchModelAction = file->addAction( "Watch XYZ File", this, SLOT(watchFile()) );
    file->addSeparator();
    file->addAction( "Snap PNG File...", this, SLOT(savePNGFile()) );
    file->addSeparator();
    file->addAction( "Set Default View Param", this, SLOT(setDefaultView()));
    file->addSeparator();
    file->addAction( "E&xit", qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q );

    // Make a general view menu
    QMenu *viewpoint = view->addMenu("Set &Viewpoint");
    view->addAction( "Clipping...", this, SLOT(launchClip()) );
    view->addAction( "Slicing...", this, SLOT(launchSlice()) );

    // Make a cascade menu to set standard view points
    viewpoint->addAction( "Explicit...", this, SLOT(launchExplicit()) );
    viewpoint->addSeparator();
    viewpoint->addAction( "Default View", this, SLOT(setDefaults()) );
    viewpoint->addSeparator();
    viewpoint->addAction( "View XY +", this, SLOT(viewXYPlus()) );
    viewpoint->addAction( "View XY -", this, SLOT(viewXYMinus()) );
    viewpoint->addSeparator();
    viewpoint->addAction( "View XZ +", this, SLOT(viewXZPlus()) );
    viewpoint->addAction( "View XZ -", this, SLOT(viewXZMinus()) );
    viewpoint->addSeparator();
    viewpoint->addAction( "View YZ +", this, SLOT(viewYZPlus()) );
    viewpoint->addAction( "View YZ -", this, SLOT(viewYZMinus()) );


    // Fill a general elements menu
    m_atomsMenu = elements->addMenu("Atoms...");

    // Make a submenu for atom specifications
    m_atomsMenu->addAction( "Atoms/Molecules...", this, SLOT(launchAtoms()) );
    m_atomsMenu->addAction( "Bonds...", this, SLOT(launchBonds()) );

    m_spinsAction = elements->addAction( "Spins...", this, SLOT(launchSpins()) );
    m_liquidCrystalsAction = elements->addAction( "Liquid Crystals...", this, SLOT(launchLiquidCrystals()) );
    m_polymersMenu = elements->addMenu( "Polymers...");
    m_poresAction = elements->addAction( "Pores ...", this, SLOT(launchPores()) );
    elements->addAction( "Lights...", this, SLOT(launchLights()) );
    elements->addAction( "Annotation...", this, SLOT(launchAnnotation()) );

    // fill submenu for polymer specifications
    m_polymersMenu->addAction( "Polymers...", this, SLOT(launchPolymers()) );
    m_polymersMenu->addAction( "Bonds...", this, SLOT(launchBonds()) );


    // fill a general settings menu
    m_showHideAxesAction = settings->addAction( "Hide Axes", this, SLOT(showHideAxesCB()) );
    m_showHideContourAction = settings->addAction( "Hide Contour", this, SLOT(showHideContourCB()) );
    m_onlyContourAction = settings->addAction( "Only Contour", this, SLOT(onlyContourCB()) );

    // fill a general data menu
    data->addAction( "&Translate...", this, SLOT(launchTranslation()) );
    data->addSeparator();
    data->addAction( "Swap XY", this, SLOT(swapXY()) );
    data->addAction( "Swap XZ", this, SLOT(swapXZ()) );
    data->addAction( "Swap YZ", this, SLOT(swapYZ()) );
    data->addSeparator();
    data->addAction( "Mirror X", this, SLOT(mirrorX()) );
    data->addAction( "Mirror Y", this, SLOT(mirrorY()) );
    data->addAction( "Mirror Z", this, SLOT(mirrorZ()) );
    data->addSeparator();
    data->addAction( "Stretch XYZ...", this, SLOT(launchStretch()) );

    // fill a general help menu
    help->addAction( "&About", this, SLOT(about()), Qt::CTRL+Qt::Key_H );
    help->addAction( "License", this, SLOT(license()) );
    help->addAction( "Distribution", this, SLOT(distribute()) );

    // Now construct the main form
    // (Note having aviz as paramter for MainForm ctor
    // is "This is an ugly hack, intended to propagate the
    // address of the calling class")
    m_mainForm = new MainForm(this/*parent*/, this /*aviz*/);
    setCentralWidget(m_mainForm);

    // Construct a timer
    m_watchTimer = new QTimer(this);

    // Set initial settings
    setAtomMenus();
}


// Destructor
AViz::~AViz() {
    
}


// Set the file type to be used during reading or writing files
void AViz::setFileType(fileType thisFt) {
    m_mainForm->setFileType( thisFt );
}


// Get the file type to be used during reading or writing files
fileType AViz::isFileType() {
    return (m_mainForm)->isFileType();
}


// Open a XYZ coordinate file
void AViz::openXYZ() {
    // Prepare reading of a coordinate file
    setFileType( XYZ_FILE );

    // Launch a customized file selector (with generate file list button)
    CustomFileDialog * fd = new CustomFileDialog();
    fd->setModal(true);
    connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
    fd->show();
}


// Open a list of coordinate files
void AViz::openList() {
    // Prepare reading of a list of coordinate files
    setFileType( ANIMATION);

    // Launch a standard file selector
    QFileDialog *fd = new QFileDialog();
    fd->setModal(true);
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setNameFilter("General (*)");
    connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
    fd->show();
}



// Read the parameters defining view parameters
void AViz::openViewParam() {
    // Prepare reading of a view parameters file
    setFileType( VP_FILE );

    // Launch standard a file selector
    QFileDialog *fd = new QFileDialog();
    fd->setModal(true);
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setNameFilter("AViz viewpoint files (*.vpm)");
    connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(readFile(const QString&)) );
    fd->show();
}


// Save the parameters defining view parameters
void AViz::saveViewParam() {
    // Prepare writing of a view parameters files
    setFileType( VP_FILE );

    // Launch a standard file selector
    QFileDialog *fd = new QFileDialog();
    fd->setModal(true);
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setNameFilter("AViz viewpoint files (*.vpm)");

    connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(writeFile(const QString&)) );
    fd->show();
}


// Save the parameters defining default view parameters
void AViz::setDefaultView() {
    char * filename = (char *)malloc( BUFSIZ );

    // Prepare writing of a view parameter file
    setFileType( VP_FILE );

    // Write the file now in the home directory
    sprintf( filename, "%s/.aviz/%s", getenv("HOME"), defaultViewParamFile);
    writeFile( filename );

    free(filename);
}


// Set factory defaults (overloaded function)
void AViz::setDefaults() {
    char * filename = (char *)malloc( BUFSIZ );
    struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
    particleData * thisPd;
    viewParam thisVp;
    bool atomExistFlag =false;
    bool defaultExistFlag =false;

    // Get a pointer to the particle data

    thisPd = m_mainForm->getParticleData();

    // Always start by setting default values
    useDefaultParticleData( thisPd );

    // Is there a particle data file in the home directory?
    sprintf( filename, "%s/.aviz/%s", getenv("HOME"), particleDataFile );
    if (!stat( (const char *)filename, buf )) {
        // A atom data file exists in the home directory --
        // read it
        atomExistFlag = true;
        if (!openParticleDataFunction( filename, thisPd )) {
            // Read failed: use built-in defaults

            m_mainForm->statusMessage( "Could not read %s -- using defaults.", filename);
            printf("Could not load %s -- using defaults.\n", filename);
        }
    }

    // If no atom data file exists, use built-in defaults
    if (!atomExistFlag) {
        m_mainForm->statusMessage( "Could not stat %s -- using defaults.", filename);
        printf("Could not stat %s -- using defaults.\n", filename);
    }

    // Is there a default view param file in the home directory?
    sprintf( filename, "%s/.aviz/%s", getenv("HOME"), defaultViewParamFile);
    if (!stat( (const char *)filename, buf )) {
        // A default view param file exists in the home directory --
        // read it
        defaultExistFlag = true;
        if (openViewParamFunction( (const char *)filename, &thisVp )) {
            // If successful, send the parameters to the main form

            m_mainForm->setDefaults( thisVp, (const char *)filename );
        }
        else {
            m_mainForm->statusMessage( "Could not read %s -- using defaults.", filename);
            printf("Could not load %s -- using defaults.\n", filename);
            // Use factory defaults if this failed
            m_mainForm->setDefaults();
        }
    }

    // If now default view param file exists, use factory defaults
    if (!defaultExistFlag) {
        m_mainForm->statusMessage( "Could not stat %s -- using defaults.", filename);
        printf("Could not stat %s -- using defaults.\n", filename);

        // Use factory defaults if this failed
        m_mainForm->setDefaults();
    }

    free(filename);
    free(buf);
}


// Set factory defaults (overloaded function)
void AViz::setDefaults(char * filename0) {
    char * filename1 = (char *)malloc( BUFSIZ );
    struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
    particleData * thisPd;
    viewParam thisVp;
    bool atomExistFlag =false;

    // Get a pointer to the particle data
    thisPd = m_mainForm->getParticleData();

    // Always start by allocating memory and setting default values
    useDefaultParticleData( thisPd );

    // Is there a particle data file in the home directory?
    sprintf( filename1, "%s/.aviz/%s", getenv("HOME"), particleDataFile );
    if (!stat( (const char *)filename1, buf )) {
        // A atom data file exists in the home directory --
        // read it
        atomExistFlag = true;
        if (!openParticleDataFunction( filename1, thisPd )) {
            // Read failed: use built-in defaults
            m_mainForm->statusMessage( "Could not read %s -- using defaults.", filename1 );
            printf("Could not read %s -- using defaults.\n", filename1);
        }
    }

    // If no atom/spin data file exists, use built-in defaults
    if (!atomExistFlag) {
        m_mainForm->statusMessage( "Could not stat %s -- using defaults.", filename1);
        printf("Could not stat %s -- using defaults.\n", filename1);
    }

    // Read the specified view parameters
    if (openViewParamFunction( (const char *)filename0, &thisVp )) {

        // If successful, send them on to the main form
        m_mainForm->setDefaults( thisVp, (const char *)filename0 );
    }
    else {
        m_mainForm->statusMessage( "Could not load", (const char *)filename0 );
        printf( "Could not load %s\n", (const char *)filename0 );

        // Use factory defaults if this failed
        m_mainForm->setDefaults();
    }

    free(filename1);
    free(buf);
}


// Read a file
void AViz::readFile( const QString &fn )
{
    // Check if the file type was set correctly, based on
    // file name
    if ( strstr(qPrintable(fn), ".xyz"))
        setFileType( XYZ_FILE );
    if ( strstr(qPrintable(fn), "filelist.dat") || !strstr(qPrintable(fn), "."))
        setFileType( ANIMATION );
    if ( strstr(qPrintable(fn), ".vpm"))
        setFileType( VP_FILE );

    m_mainForm->readFile( fn );

    m_mainForm->updateRendering();

    // Launch the file list board upon reading
    // a list of files
    if (isFileType() == ANIMATION) {
        m_mainForm->launchFileList( fn );
    }

}


// Write a file
void AViz::writeFile( const QString &fn )
{
    m_mainForm->writeFile( fn );
}


// toggle watch mode
void AViz::watchFile() {
    if (!m_inWatchMode) {
        // Start the timer, using the command-line callback
        setWatchMode();

        // Write a message
        aggregateData * ad = m_mainForm->getAggregateData();
        m_mainForm->statusMessage( "Start watching file", (*ad).filename );

        // Adjust the menu entry
        m_inOutWatchModelAction->setText("Stop Watching");
    }  else {
        // Stop the timer
        m_watchTimer->stop();

        // Write a message
        aggregateData * ad = m_mainForm->getAggregateData();
        m_mainForm->statusMessage( "Stop watching file", (*ad).filename );

        // Adjust the menu entry
        m_inOutWatchModelAction->setText("Watch XYZ File..." );
    }

    m_inWatchMode = !m_inWatchMode;
}


// Show the file list panel
void AViz::launchFileList() {
    m_mainForm->launchFileList();
}


// Show the animation panel -- a convenience function
void AViz::launchAnimation()
{
    AnimationBoard * ab = new AnimationBoard();
    ab->show();
}


// Help callback function
void AViz::about() {
    // Launch a message box
    MessageBox *mb = new MessageBox(VERSION_INFO_STRING, this);
    mb->show();
}


// Help callback function
void AViz::license() {
    // Launch a message box
    MessageBox *mb = new MessageBox(SHORT_LICENSE_STRING, this);
    mb->show();

    // Print the full license conditions
    printf("%s\n", LICENSE_STRING );
    printf("%s\n", DISTRIBUTE_STRING );
}


// Help callback function
void AViz::distribute()
{
    // Launch a message box
    MessageBox * mb = new MessageBox(SHORT_DISTRIBUTE_STRING, this);
    mb->show();

    // Print the full distribution conditions
    printf("%s\n", DISTRIBUTE_STRING );
}


// Callback function to set fixed view parameters
void AViz::viewXYPlus() {
    m_mainForm->setViewXYPlus();
}


// Callback function to set fixed view parameters
void AViz::viewXYMinus() {
    m_mainForm->setViewXYMinus();
}


// Callback function to set fixed view parameters
void AViz::viewXZPlus() {
    m_mainForm->setViewXZPlus();
}


// Callback function to set fixed view parameters
void AViz::viewXZMinus() {
    m_mainForm->setViewXZMinus();
}


// Callback function to set fixed view parameters
void AViz::viewYZPlus() {
    m_mainForm->setViewYZPlus();
}


// Callback function to set fixed view parameters
void AViz::viewYZMinus() {
    m_mainForm->setViewYZMinus();
}


// Command function to start auto rot/tilt/spin
void AViz::startAutoRot() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    thisVp->autoRot1 = true;
    m_mainForm->setAutoAndStart( (*thisVp) );

}


// Command function to start auto rot/tilt/spin
void AViz::startAutoTilt() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    (*thisVp).autoTilt1 = true;
    m_mainForm->setAutoAndStart( (*thisVp) );
}


// Command function to start auto rot/tilt/spin
void AViz::startAutoSpin() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    (*thisVp).autoSpin1 = true;
    m_mainForm->setAutoAndStart( (*thisVp) );

}


// Command line function to start auto zooming
void AViz::startAutoZoom() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    (*thisVp).autoZoom1 = true;
    m_mainForm->setAutoAndStart( (*thisVp) );
}


// Command line function to start auto snapping
void AViz::startAutoSnap() {

    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    (*thisVp).autoSnap = true;
    m_mainForm->setAutoAndStart( (*thisVp) );

}


// Command line function to read a file list
void AViz::useFileList(char * ptr) {
    // Prepare reading of a list of coordinate files
    setFileType( ANIMATION);

    // Read the list and produce the file list board
    QString fl = ptr;
    readFile( fl );
}


// Command line function to set rendering mode
void AViz::setRenderMode(char * ptr) {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
    // Set a new render mode and refresh drawing
    if (strcmp( ptr, "atom" ) == 0) {
        (*thisVp).renderMode = ATOMS;
        setAtomMenus();
    }
    if (strcmp( ptr, "spin" ) == 0) {
        (*thisVp).renderMode = SPINS;
        setSpinMenus();
    }
    if (strcmp( ptr, "liquidcrystal" ) == 0) {
        (*thisVp).renderMode = LIQUID_CRYSTALS;
        setLcMenus();
    }
    if (strcmp( ptr, "polymer" ) == 0) {
        (*thisVp).renderMode = POLYMERS;
        setPolymerMenus();
    }
    if (strcmp( ptr, "pore" ) == 0) {
        (*thisVp).renderMode = PORES;
        setPoreMenus();
    }

    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Command line function to set rendering style
void AViz::setRenderStyle(char * ptr) {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();
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

    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Command line function to set rendering mode
void AViz::setRenderQuality(char * ptr) {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

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

    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Command line function to set rendering size
void AViz::setRenderSize(int size) {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

    (*thisVp).renderSize = (double)size/100.0;

    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}

// Command line function to set the annotation string
void AViz::setAnnotationString(char* annotationString) {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

    strcpy((*thisVp).annotationText, annotationString);

    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();

}

// Save a PNG screenshot file
void AViz::savePNGFile() {
    setFileType( PNG_FILE );

    // Launch a standard file selector
    QFileDialog * fd = new QFileDialog();
    fd->setModal(true);
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setNameFilter("PNG image files (*.png)");
    connect( fd, SIGNAL(fileSelected(const QString&)), this, SLOT(snapFile(const QString&)) );
    fd->show();
}

// Snap a PNG screenshot file
void AViz::snapFile( const QString &fn ) {
    m_mainForm->updateRendering();
    m_mainForm->snapRendering( fn );
}

// Command line function to trigger screen shot
void AViz::setSnap() {
    QTimer * snapTimer = new QTimer(this);
    connect(snapTimer, SIGNAL(timeout()), this, SLOT(setSnapNow()) );

    // Wait 1 s to have the rendering completed and
    // then take a screen shot
    snapTimer->setSingleShot(true);
    snapTimer->start( 1000);
}


// Command line function to trigger screen shot and exit
void AViz::setSnapq() {
    QTimer * snapTimer = new QTimer(this);
    connect(snapTimer, SIGNAL(timeout()), this, SLOT(setSnapqNow()) );

    // Wait 1 s to have the rendering completed;
    // then take a screen shot and exit
    snapTimer->setSingleShot(true);
    snapTimer->start(1000);
}


// Used to trigger screen shot
void AViz::setSnapNow() {
    m_mainForm->updateRendering();
    m_mainForm->snapRendering();

}


// Used to trigger screen shot and exit
void AViz::setSnapqNow() {
    m_mainForm->updateRendering();
    m_mainForm->snapRendering();

    // Exit now
    exit(0);
}


// Command line function to set watch file mode, with
// automatic update of the rendering upon changes
void AViz::setWatchMode() {
    connect(m_watchTimer, SIGNAL(timeout()), this, SLOT(keepWatchNow()) );

    // Wait 10 s and check repeatedly if the file has been modified
    m_watchTimer->setSingleShot(false);
    m_watchTimer->start(10000);

    // Set a flag
    m_inWatchMode = true;
}

void AViz::enableElement(ElementType type) {
    m_atomsMenu->setEnabled(type == AViz::ET_ATOM);
    m_spinsAction->setEnabled(type == AViz::ET_SPIN);
    m_liquidCrystalsAction->setEnabled(type == AViz::ET_LIQUID_CRYSTALS);
    m_polymersMenu->setEnabled(type == AViz::ET_POLYMERS);
    m_poresAction->setEnabled(type == AViz::ET_PORES);
}

void AViz::setAtomMenus() {
    enableElement(AViz::ET_ATOM);
}

void AViz::setSpinMenus() {
    enableElement(AViz::ET_SPIN);
}

void AViz::setLcMenus() {
    enableElement(AViz::ET_LIQUID_CRYSTALS);
}

void AViz::setPolymerMenus() {
    enableElement(AViz::ET_POLYMERS);
}

void AViz::setPoreMenus() {
    enableElement(AViz::ET_PORES);
}


// Used to trigger updates of the rendering upon change of
// current XYZ file
void AViz::keepWatchNow() {
    char * filename = (char *)malloc( BUFSIZ );
    struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
    //	time_t last_access;
    //	time_t last_modification;
    time_t last_change;
    time_t now = time( NULL );

    // Get the current data set and retrieve filename
    aggregateData * ad = m_mainForm->getAggregateData();

    sprintf( filename, "./%s", (*ad).filename);
    if (!stat( (const char *)filename, buf ) ) {
        //			last_access = (*buf).st_atime;
        //			last_modification = (*buf).st_mtime;
        last_change = (*buf).st_ctime;

        // Was the file was modified during the last cycle?
        if (last_change > now-10) {
            // Yes: update rendering
            readFile( (*ad).filename );
            m_mainForm->updateRendering();

            m_mainForm->statusMessage( "Updating rendering...");
        }
        else {
            // No: do nothing
            m_mainForm->statusMessage( "Watching XYZ file", (*ad).filename );
        }
    }

    free(filename);
    free(buf);
}


// Callback function to launch the atoms settings panel
void AViz::launchAtoms() {
    m_mainForm->launchAtoms();
}


// Callback function to launch the annotation settings panel
void AViz::launchAnnotation() {
    m_mainForm->launchAnnotation();
}


// Callback function to launch the bond settings panel
void AViz::launchBonds() {
    m_mainForm->launchBonds();
}


// Callback function to launch the clip settings panel
void AViz::launchClip() {
    m_mainForm->launchClip();
}


// Callback function to launch the explicit view point settings panel
void AViz::launchExplicit() {
    m_mainForm->launchExplicit();
}


// Callback function to launch the lights settings panel
void AViz::launchLights() {
    m_mainForm->launchLights();
}


// Callback function to launch the liquid crystal settings panel
void AViz::launchLiquidCrystals() {
    m_mainForm->launchLiquidCrystals();
}


// Callback function to launch the slicing settings panel
void AViz::launchSlice() {
    m_mainForm->launchSlice();
}


// Callback function to launch the spin settings panel
void AViz::launchSpins() {
    m_mainForm->launchSpins();
}


// Callback function to launch the stretch settings panel
// (to manipulate data)
void AViz::launchStretch() {
    m_mainForm->launchStretch();
}


// Callback function to launch the polymers settings panel
void AViz::launchPolymers() {
    m_mainForm->launchPolymers();
}


// Callback function to launch the pore settings panel
void AViz::launchPores() {
    m_mainForm->launchPores();
}


// Callback function to launch the translation settings panel
void AViz::launchTranslation() {
    m_mainForm->launchTranslation();
}


// Show or hide axes
void AViz::showHideAxesCB() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

    // Toggle the axes flag
    if ((*thisVp).showAxes) {
        (*thisVp).showAxes =false;
        m_showHideAxesAction->setText("Show Axes");
    }
    else {
        (*thisVp).showAxes = true;
        m_showHideAxesAction->setText("Hide Axes");
    }

    // Update the rendering
    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Show or hide the contour
void AViz::showHideContourCB() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

    // Toggle the show contour flag and update realted menu actions
    m_showHideContourAction->setText(thisVp->showContour ? "Show Contour" : "Hide Contour");
    m_onlyContourAction->setEnabled(thisVp->showContour);
    thisVp->showContour = !thisVp->showContour;

    // Update the rendering
    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Draw only contour or draw all the scene
void AViz::onlyContourCB() {
    // Read the current view parameters
    viewParam * thisVp = m_mainForm->getViewParam();

    // Toggle the onlycontour flag and update related menu actions
    m_onlyContourAction->setText(thisVp->onlyContour ? "Only Contour" : "Contour And Particles");
    m_showHideContourAction->setEnabled(thisVp->onlyContour);
    thisVp->onlyContour = !thisVp->onlyContour;

    // Update the rendering
    m_mainForm->setViewParam( (*thisVp) );
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Callback function to manipulate data
void AViz::swapXY() {
    // Write a message
    m_mainForm->statusMessage( "Swapping X <-> Y" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Swap now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        float tmp = (*ad).particles[i].x;
        (*ad).particles[i].x = (*ad).particles[i].y;
        (*ad).particles[i].y = tmp;
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Callback function to manipulate data
void AViz::swapXZ() {
    // Write a message
    m_mainForm->statusMessage( "Swapping X <-> Z" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Swap now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        float tmp = (*ad).particles[i].x;
        (*ad).particles[i].x = (*ad).particles[i].z;
        (*ad).particles[i].z = tmp;
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Callback function to manipulate data
void AViz::swapYZ() {
    // Write a message
    m_mainForm->statusMessage( "Swapping Y <-> Z" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Swap now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        float tmp = (*ad).particles[i].y;
        (*ad).particles[i].y = (*ad).particles[i].z;
        (*ad).particles[i].z = tmp;
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Callback function to manipulate data
void AViz::mirrorX() {
    // Write a message
    m_mainForm->statusMessage( "Mirroring X -> -X" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Mirror now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        (*ad).particles[i].x *= (-1.0);
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}


// Callback function to manipulate data
void AViz::mirrorY() {
    // Write a message
    m_mainForm->statusMessage( "Mirroring Y -> -Y" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Mirror now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        (*ad).particles[i].y *= (-1.0);
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();

}


// Callback function to manipulate data
void AViz::mirrorZ() {
    // Write a message
    m_mainForm->statusMessage( "Mirroring Z -> -Z" );

    // Get local copy of the current data set
    aggregateData * ad = m_mainForm->getAggregateData();

    // Mirror now
    for (int i=0;i<(*ad).numberOfParticles;i++) {
        (*ad).particles[i].z *= (-1.0);
    }

    // Let this be the current aggregate and render
    m_mainForm->updateView();
    m_mainForm->updateRendering();
}
