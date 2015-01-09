//
// Implementation of main form
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

#include "mainForm.h"

#include <QLabel>
#include <QGridLayout>
#include <QSizePolicy>

#include "aviz.h"
#include "parameterLimits.h"
#include "fileFunctions.h"
#include "annotationBoard.h"
#include "atomBoard.h"
#include "bondBoard.h"
#include "clipBoard.h"
#include "colorBoard.h"
#include "explicitBoard.h"
#include "fileListBoard.h"
#include "lcBoard.h"
#include "lightBoard.h"
#include "polymerBoard.h"
#include "poreBoard.h"
#include "sliceBoard.h"
#include "spinBoard.h"
#include "stretchBoard.h"
#include "trackBoard.h"
#include "translationBoard.h"
#include "glCanvasFrame.h"
#include "renderStyleBox.h"
#include "liveBox.h"

// 
// Main form: implements drawing control elements
// 
MainForm::MainForm(QWidget *parent, AViz *aviz)
    : QWidget(parent), m_aviz(aviz),
      ab(NULL), m_anb(NULL), bb(NULL),
      clb(NULL), eb(NULL), flb(NULL), lcb(NULL), lb(NULL), plb(NULL),
      pb(NULL), sb(NULL), slb(NULL), stb(NULL), tb(NULL), trab(NULL),
      m_keepViewObject(false) {

    // Make a grid layout that will hold all elements
    QGridLayout *mainBox = new QGridLayout(this);

    // Create the OpenGL widget
    glCanvasFrame = new GLCanvasFrame(this);
    glCanvasFrame->setFormAddress(this);

    // Make a box that will hold
    // a row of elements to define render quality
    renderBox = new RenderBox(this /*mainForm*/, this /*parent*/);
    renderBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainBox->addWidget(renderBox, 1, 0);

    // Make a box that will hold
    // two buttons to take snapshots
    m_liveBox = new LiveBox(this /*mainForm*/, this);
    mainBox->addWidget(m_liveBox, 2, 0);

    // Add a label that pretends to be a status bar
    m_status = new QLabel(this);
    m_status->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_status->setFixedHeight(m_status->sizeHint().height());
    m_status->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    mainBox->addWidget(m_status, 3, 0);

    // Create some of the more important boards
    ab = new AtomBoard(this/*mainForm*/, this /*parent*/);

    sb = new SpinBoard(this);
    sb->setMainFormAddress(this);

    lcb = new LcBoard(this);
    lcb->setMainFormAddress(this);

    plb = new PolymerBoard(this);
    plb->setMainFormAddress(this);

    pb = new PoreBoard(this);
    pb->setMainFormAddress(this);

    // Fix the geometry of this panel so that
    // the above widgets are all kept small
    mainBox->setColumnStretch(1, 1/*stretch*/ );
    mainBox->setRowStretch(4, 1/*stretch*/ );
}

// Set the file type to be used during reading or writing files
void MainForm::setFileType(fileType thisFt) {
    ft = thisFt;
}


// Get the file type to be used during reading or writing files
fileType MainForm::isFileType() {
    return ft;
}


// Set the view freeze control used to prevent updating of the 
// view object
void MainForm::setKeepViewObject( bool thisKeepViewObject ) {
    m_keepViewObject = thisKeepViewObject;
}


// Get the  view freeze control used to prevent updating of the 
// view object
bool MainForm::isKeepViewObject() {
    return m_keepViewObject;
}


// Read a file
void MainForm::readFile( const QString &fn ) {
    aggregateData * ad = getAggregateData();;
    viewParam thisVp;

    if ( !fn.isEmpty() ) {
        // Write a message to the status bar
        statusMessage( "Loading", fn );

        switch (isFileType()) {
        case XYZ_FILE:

            // Read these coordinates
            if (openCoordinateFunction( qPrintable(fn), ad )) {
                // Write a message to the status bar
                statusMessage("Loaded", fn);

                // Let this be the current aggregate
                updateView();

                // Pass on information to the
                // various boards
                ab->setData();
                lcb->setData();
                plb->setData();
                pb->setData();
                sb->setData();
                if (bb)
                    bb->setData();
                if (trab)
                    trab->setData();
            }
            else {
                // Write a message to the status bar
                statusMessage( "Could not load ", fn );
            }
            break;
        case ANIMATION:
            if (openFileListFunction( qPrintable(fn), &fl )) {
                // Write a message to the status bar
                statusMessage( "Loaded ", fn );
            }
            else {
                // Write a message to the status bar
                statusMessage( "Could not load ", fn );
            }
            break;
        case VP_FILE:

            // Read these view parameters
            if (openViewParamFunction( qPrintable(fn), &thisVp )) {
                // Write a message to the status bar
                statusMessage( "Loaded", fn );

                // Let this be the current view
                // parameters
                setViewParam( thisVp );
            }
            else {
                // Write a message to the status bar
                statusMessage( "Could not load ", fn);
            }
            break;
        case PNG_FILE: case UNKNOWN:
            statusMessage("??? Could not load ", fn);
            break;
        }
    }
    else {
        statusMessage("Loading aborted");
    }
}


// Write a file
void MainForm::writeFile( const QString &fn ) {
    // Read the current view parameters
    if ( !fn.isEmpty() ) {

        switch (isFileType()) {
        case VP_FILE:
            // Save these view parameters
            // Make sure the suffix is correct
            checkSuffix( qPrintable(fn), "vpm" );

            // Write a message to the status bar
            statusMessage( "Saving ", fn );

            if (saveViewParamFunction( qPrintable(fn), getViewParam() )) {
                // Write a message to the status bar
                statusMessage( "Saved ", fn );
            }
            else {
                // Write a message to the status bar
                statusMessage("Could not save ", fn );
            }
            break;
        case XYZ_FILE:
        case ANIMATION:
        case PNG_FILE:
        case UNKNOWN:
            statusMessage("?? Could not save ", fn);
            break;
        }
    }
    else {
        statusMessage( "Saving aborted" );
    }
}


// Read a file from the file list, specified by the index, 
// upon request from the file list board; return the 
// filename
QString MainForm::readFileFromList( int current, bool thisKeepViewObject ) {
    // Register index of file in file list
    fl.currentFile = current;

    // Prepare reading of file
    setFileType( XYZ_FILE );
    if (fl.currentFile >= 0 && fl.currentFile < fl.numberOfFiles) {
        QString file = QString( fl.filename[fl.currentFile] );

        // Control freezing of the view object data
        setKeepViewObject( thisKeepViewObject );

        // Set limits for track rendering, if necessary
        if (getViewParam()->showTracks) {
            getViewParam()->trackRenderFromStage = 0;
            switch (getViewParam()->tRenderMode) {
            case ALL_STAGES:
                getViewParam()->trackRenderToStage = fl.numberOfFiles-1;
                break;
            case UP_TO_CURRENT:
                getViewParam()->trackRenderToStage = fl.currentFile;
                break;
            }
        }

        // Read the file
        readFile( file );

        // Set freezing of the view object data back to default
        setKeepViewObject( FALSE );

        return file;
    }
    else
        return QString( "--" );
}


// Set factory default values for the view parameters
// and settings (overloaded function) 
void MainForm::setDefaults()
{
    viewParam vp;

    // Write a message
    statusMessage( "Setting factory defaults");

    // Assume auto clip and set clip and slice
    // parameter values
    vp.autoClipNear = TRUE;
    vp.autoClipFar = TRUE;
    vp.clipNear = 0.0;
    vp.clipFar = 0.0;
    vp.slicing = FALSE;
    vp.slicingSet = FALSE;
    vp.showSlicePlaneX = FALSE;
    vp.showSlicePlaneY = FALSE;
    vp.showSlicePlaneZ = FALSE;
    vp.showTracks = FALSE;
    vp.sliceXMin = 0.0;
    vp.sliceXMax = 0.0;
    vp.sliceYMin = 0.0;
    vp.sliceYMin = 0.0;
    vp.sliceZMax = 0.0;
    vp.sliceZMax = 0.0;

    // No stretching
    vp.stretchX = 1.0;
    vp.stretchY = 1.0;
    vp.stretchZ = 1.0;

    // Set more parameter values
    vp.phi = PHI;
    vp.theta = THETA;
    vp.chi = CHI;
    vp.dolly = DOLLY;
    vp.fovy = FOVY;
    vp.phiHome = vp.phi;
    vp.thetaHome = vp.theta;
    vp.chiHome = vp.chi;
    vp.fovyHome = vp.fovy;

    vp.panX = vp.panY = vp.panZ = 0.0;
    vp.panRight = vp.panTop = vp.panForward = 0.0;

    // No auto motion to start with
    vp.autoRot1 = FALSE;
    vp.autoRot2 = FALSE;
    vp.autoTilt1 = FALSE;
    vp.autoTilt2 = FALSE;
    vp.autoSpin1 = FALSE;
    vp.autoSpin2 = FALSE;
    vp.autoZoom1 = FALSE;
    vp.autoZoom2 = FALSE;

    // Set render parameters
    vp.renderMode = ATOMS;
    vp.renderSize = SIZE;
    vp.renderQuality = LOW;
    vp.showAxes = TRUE;
    vp.showContour = TRUE;
    vp.onlyContour = FALSE;
    vp.atomRenderStyle = ADOT;
    vp.spinRenderStyle = SLINE;
    vp.lcRenderStyle = LLINE;
    vp.poreRenderStyle = PLINE;

    // Set annotation parameters
    vp.showAnnotation = TRUE;
    vp.annotationSize = 1;
    vp.annotationCoordX = 100;
    vp.annotationCoordY = 100;
    sprintf(vp.annotationText, " " );

    // Set stereo vision patemeters
    vp.stereoVisionEn = FALSE;
    vp.eyeSeparation = 0.4;

    // Set more settings
    vp.background = BGBLACK;
    vp.viewMode = PERSPECTIVE;

    for (int i=0;i<8;i++) {
        vp.light[i] = FALSE;
        vp.lightDepth[i] = 0.4;
    }
    vp.light[5] = TRUE;
    vp.ambientLight = 0.2;
    vp.shininess = 100.0;

    // Prevent immediate updating
    vp.enableUpdate = FALSE;

    renderBox->setValue( vp );

    glCanvasFrame->setAuto( vp );

    // Enable updating
    vp.enableUpdate = TRUE;

    // Refresh the drawing now
    setViewParam( vp );
    updateRendering();

    // Always update the view object data
    setKeepViewObject( FALSE );
}


// Set factory default values for the view parameters
// (overloaded function) 
void MainForm::setDefaults( viewParam vp, const char * filename )
{
    // Write a message
    statusMessage( "Setting defaults in file ", filename );

    // Prevent immediate updating
    vp.enableUpdate = false;

    renderBox->setValue( vp );
    glCanvasFrame->setAuto( vp );

    // Adjust the menus
    switch (vp.renderMode) {
    case ATOMS:
        m_aviz->setAtomMenus();
        break;
    case SPINS:
        m_aviz->setSpinMenus();
        break;
    case LIQUID_CRYSTALS:
        m_aviz->setLcMenus();
        break;
    case POLYMERS:
        m_aviz->setPolymerMenus();
        break;
    case PORES:
        m_aviz->setPoreMenus();
        break;
    default:
        m_aviz->setAtomMenus();
        break;
    }

    // Enable updating
    vp.enableUpdate = TRUE;

    // Refresh the drawing now
    setViewParam( vp );
    updateRendering();

    // Always update the view object data
    setKeepViewObject( FALSE );
}


// Accept new data sent from the main panel 
void MainForm::updateView()
{
    switch (isKeepViewObject()) {
    case FALSE:
        // Update the view object
        glCanvasFrame->updateView();
        break;
    case TRUE:
        // Update the view object
        glCanvasFrame->updateViewWithoutViewObjectChange();
        break;
    }
}


// Return a pointer to the current data to the main panel
aggregateData * MainForm::getAggregateData()
{
    return glCanvasFrame->getAggregateData();
}


// Accept new view parameters and settings sent from the main panel 
void MainForm::setViewParam( viewParam vp )
{
    // Refresh the drawing now
        glCanvasFrame->setViewParam( vp );
}


// Set the auto buttons according to settings sent from 
// the main panel and trigger auto rot/tilt/spin
void MainForm::setAutoAndStart( viewParam vp ) {
    if (vp.autoRot1 == TRUE) {
        glCanvasFrame->startAutoRot1( );
    }
    if (vp.autoRot2 == TRUE) {
        glCanvasFrame->startAutoRot2( );
    }
    if (vp.autoTilt1 == TRUE) {
        glCanvasFrame->startAutoTilt1( );
    }
    if (vp.autoTilt2 == TRUE) {
        glCanvasFrame->startAutoTilt2( );
    }
    if (vp.autoSpin1 == TRUE) {
        glCanvasFrame->startAutoSpin1( );
    }
    if (vp.autoSpin2 == TRUE) {
        glCanvasFrame->startAutoSpin2( );
    }
    if (vp.autoZoom1 == TRUE) {
        glCanvasFrame->startAutoZoom1( );
    }
    if (vp.autoZoom2 == TRUE) {
        glCanvasFrame->startAutoZoom2( );
    }

    glCanvasFrame->setAuto( vp );

    if (vp.autoSnap == TRUE) {
        ((LiveBox *)m_liveBox)->startAutoSnap( );
    }

    // Save those settings
    glCanvasFrame->setViewParam( vp );
}


// Update the rendering of the data, without recompiling
void MainForm::updateRendering() {
    glCanvasFrame->updateRendering();
}


// Update the settings of the view parameter board
void MainForm::updateExplicitBoard() {
    if (eb)
        eb->setExplicit(*getViewParam());
}


// Create an image file from the current rendering
// (overloaded function)
void MainForm::snapRendering() {
    glCanvasFrame->updateRendering();
    glCanvasFrame->snapRendering();
}


// Create an image file from the current rendering
// (overloaded function)
void MainForm::snapRendering( const QString &fn )
{
    glCanvasFrame->updateRendering();
    glCanvasFrame->snapRendering(qPrintable(fn));
}


// Adjust the menus
void MainForm::setAtomMenus() {
    m_aviz->setAtomMenus();
}


// Adjust the menus
void MainForm::setSpinMenus() {
    m_aviz->setSpinMenus();
}


// Adjust the menus
void MainForm::setLcMenus() {
    m_aviz->setLcMenus();
}


// Adjust the menus
void MainForm::setPolymerMenus() {
    m_aviz->setPolymerMenus();
}

// Adjust the menus
void MainForm::setPoreMenus() {
    m_aviz->setPoreMenus();
}

// Adjust the boards
void MainForm::broadcastDotStyle() {
    ab->setDotStyle();
    sb->setDotStyle();
    lcb->setDotStyle();
    plb->setDotStyle();
    pb->setDotStyle();
}

// Adjust the boards
void MainForm::broadcastLineStyle() {
    ab->setLineStyle();
    sb->setLineStyle();
    lcb->setLineStyle();
    plb->setLineStyle();
    pb->setLineStyle();
}


// Adjust the boards
void MainForm::broadcastCubeStyle() {
    ab->setCubeStyle();
    sb->setCubeStyle();
    lcb->setCubeStyle();
    plb->setCubeStyle();
    pb->setCubeStyle();
}


// Adjust the boards
void MainForm::broadcastCylinderStyle() {
    ab->setCylinderStyle();
    sb->setCylinderStyle();
    lcb->setCylinderStyle();
    plb->setCylinderStyle();
    pb->setCylinderStyle();
}


// Adjust the boards
void MainForm::broadcastConeStyle() {
    ab->setConeStyle();
    sb->setConeStyle();
    lcb->setConeStyle();
    plb->setConeStyle();
    pb->setConeStyle();
}


// Adjust the boards
void MainForm::broadcastSphereStyle() {
    ab->setSphereStyle();
    sb->setSphereStyle();
    lcb->setSphereStyle();
    plb->setSphereStyle();
    pb->setSphereStyle();
}


// Adjust the boards
void MainForm::broadcastLowQuality() {
    ab->setLowQuality();
    sb->setLowQuality();
    lcb->setLowQuality();
    plb->setLowQuality();
    pb->setLowQuality();
}


// Adjust the boards
void MainForm::broadcastHighQuality() {
    ab->setHighQuality();
    sb->setHighQuality();
    lcb->setHighQuality();
    plb->setHighQuality();
    pb->setHighQuality();
}


// Adjust the boards
void MainForm::broadcastFinalQuality() {
    ab->setFinalQuality();
    sb->setFinalQuality();
    lcb->setFinalQuality();
    plb->setFinalQuality();
    pb->setFinalQuality();
}


// Close open boards after change of render mode
void MainForm::closeModeBoards()
{
    if (ab)
        ab->hide();
    if (m_anb)
        m_anb->hide();
    if (bb)
        bb->hide();
    if (clb)
        clb->hide();
    if (eb)
        eb->hide();
    if (flb)
        flb->hide();
    if (lb)
        lb->hide();
    if (lcb)
        lcb->hide();
    if (pb)
        pb->hide();
    if (plb)
        plb->hide();
    if (sb)
        sb->hide();
    if (slb)
        slb->hide();
    if (stb)
        stb->hide();
    if (tb)
        tb->hide();
}


// Return a pointer to the current view parameters
viewParam * MainForm::getViewParam() {
    return glCanvasFrame->getViewParam();
}


// Return a pointer to the current view object structure 
viewObject * MainForm::getViewObject() {
    return glCanvasFrame->getViewObject();
}


// Read the current particle data structure and return
particleData * MainForm::getParticleData() {
    return glCanvasFrame->getParticleData();
}


// Set fixed view parameters
void MainForm::setViewXYPlus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = 0.01;
    (*vp).theta = 270.0;
    (*vp).chi = 0.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting XY+ viewpoint");
}


// Set fixed view parameters
void MainForm::setViewXYMinus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = 180.01;
    (*vp).theta = 270.0;
    (*vp).chi = 90.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting XY- viewpoint");
}


// Set fixed view parameters
void MainForm::setViewXZPlus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = 90.01;
    (*vp).theta = -90.0;
    (*vp).chi = 90.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting XZ+ viewpoint");
}


// Set fixed view parameters
void MainForm::setViewXZMinus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = 0.01;
    (*vp).theta = 0.0;
    (*vp).chi = 0.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting XZ- viewpoint");
}


// Set fixed view parameters
void MainForm::setViewYZPlus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = 0.01;
    (*vp).theta = 0.0;
    (*vp).chi = -90.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting YZ+ viewpoint");
}


// Set fixed view parameters
void MainForm::setViewYZMinus() {
    // Read the current view parameters
    viewParam * vp = glCanvasFrame->getViewParam();

    // Set the new parameter values
    (*vp).phi = -90.01;
    (*vp).theta = 90.0;
    (*vp).chi = 180.0;

    // Refresh the drawing now
    glCanvasFrame->setViewParam( (*vp) );
    glCanvasFrame->updateRendering();

    // Write a message
    statusMessage("Setting YZ- viewpoint");
}


// Upon request from the drawing area, pass on data to the 
// frame -- this is used to handle mouse-controlled rotations 
// and spins
void MainForm::computeTransformation( double delta, double w1, double w2, double w3, double * deltaPhi, double * deltaTheta, double * deltaChi ) {
    glCanvasFrame->computeTransformation( delta, w1, w2, w3, deltaPhi, deltaTheta, deltaChi );
}


// Write a status bar message
void MainForm::statusMessage(const QString& mess) {
    m_status->setText( mess );
}


// Write a status bar message (overloaded function) 
void MainForm::statusMessage(const QString& mess1, const QString& mess2) {
    m_status->setText(mess1 + mess2);
}


// Callback function to launch the annotation settings panel
void MainForm::launchAnnotation() {
    if (!m_anb) {
        m_anb = new AnnotationBoard(this /*parent*/, this /*mainForm*/);
    }

    // Adjust the controls and show
    m_anb->setAnnotation(*getViewParam());
    m_anb->show();
}


// Launch the atom control board
void MainForm::launchAtoms() {
    // Get the current particle data and show
    // the panel
    ab->setData();
    ab->show();
}


// Callback function to launch the bond settings panel
void MainForm::launchBonds() {
    if (!bb) {
        bb = new BondBoard(this);
        bb->setMainFormAddress( this );
    }

    // Adjust the controls and show
    // the panel
    bb->setData();
    bb->show();
}


// Callback function to launch the clip settings panel
void MainForm::launchClip() {
    if (!clb) {
        clb = new ClipBoard(this /*mainForm*/, this /*parent*/);
    }

    // Get the current settings and show
    // the panel
    clb->setClip(*getViewParam());
    clb->show();
}


// Callback function to launch the explicit view point settings panel
void MainForm::launchExplicit() {
    if (!eb) {
        eb = new ExplicitBoard(this /*mainForm*/, this /*parent*/);
    }

    // Get the current settings and show
    // the panel
    eb->setExplicit(*getViewParam());
    eb->show();
}


// Callback function to launch the file list panel (overloaded function)
void MainForm::launchFileList() {
    if (!flb) {
        flb = new FileListBoard(this/*mainForm*/, this/*parent*/);
    }

    // Show the file list now (resetting the view freeze control)
    flb->resetKeepViewScale();
    flb->show();
}


// Callback function to launch the file list panel (overloaded function)
void MainForm::launchFileList( const QString &fn ) {
    if (!flb) {
        flb = new FileListBoard(this/*mainForm*/, this/*parent*/);
    }

    // Reset counter and prepare cycling
    flb->setFileListParam( 0, fl.numberOfFiles, fn );

    // Show the file list now (resetting the view freeze control)
    flb->resetKeepViewScale();
    flb->show();
}


// Callback function to launch the lights settings panel
void MainForm::launchLights() {
    if (!lb) {
        lb = new LightsBoard(this /*mainForm*/, this /*parent*/);
    }

    // Adjust the controls and show
    // the panel
    lb->setLights(*getViewParam());
    lb->show();
}


// Callback function to launch the liquid crystal settings panel
void MainForm::launchLiquidCrystals() {
    // Get the current liquid crystal data and show
    // the panel
    lcb->setData();
    lcb->show();
}


// Callback function to launch the pore settings panel
void MainForm::launchPores() {
    // Get the current pore data and show
    // the panel
    pb->setData();
    pb->show();
}


// Callback function to launch the polymers settings panel
void MainForm::launchPolymers()
{
    // Get the current polymer data and show
    // the panel
    plb->setData();
    plb->show();
}


// Callback function to launch the slicing settings panel
void MainForm::launchSlice()
{
    if (!slb) {
        slb = new SliceBoard(this);
        if (slb)
            slb->setMainFormAddress( this );
    }

    // Get the current settings and show
    // the panel
    if (slb) {
        slb->setSlice(*getViewParam());

        // Use the object boundaries as defaults
        if (!getViewParam()->slicingSet)
            slb->autoSlice();

        slb->show();
    }
}


// Callback function to launch the spin settings panel
void MainForm::launchSpins()
{
    // Get the current spin data and show
    // the panel
    sb->setData();
    sb->show();
}


// Callback function to launch the stretch settings panel
// (to manipulate data)
void MainForm::launchStretch() {
    if (!stb) {
        stb = new StretchBoard(this /*MainForm*/, this);
    }

    // Adjust the controls and show
    // the panel
    stb->setData(*getViewParam());
    stb->show();
}


// Callback function to launch the track board panel
void MainForm::launchTrack(const QString &fn) {
    if (!trab) {
        trab = new TrackBoard(this);
        trab->setMainFormAddress(this);
    }

    // Get the current particle data and show
    // the panel
    trab->setFileListParam( fn );
    trab->setData();
    trab->show();
}


// Callback function to launch the translation settings panel
void MainForm::launchTranslation() {
    if (!tb) {
        tb = new TranslationBoard(this /*mainForm*/, this);
    }

    // Adjust the controls and show
    // the panel
    tb->setData(*getViewParam());
    tb->show();
}


// Callback function to hide the track board panel
void MainForm::hideTrack() {
    if (trab)
        trab->hide();
}


// Generate track data file based on current file list
void MainForm::generateTracks() {
    // Write a message to the m_status bar
    statusMessage("Generating tracks...");

    // Attempt to generate tracks now, using a pointer to
    // the current aggregated structure (it will be necessary
    // to read in aggregated data over and over again) and
    // a pointer to the current track data structure
    if (generateTrackDataFunction( &fl, getAggregateData(), glCanvasFrame->getTrackData() )) {

        // Write a message to the m_status bar
        statusMessage( "Completed generation of tracks" );

        // Set limits for first track rendering
        getViewParam()->trackRenderFromStage = 0;

        switch (getViewParam()->tRenderMode) {
        case ALL_STAGES:
            getViewParam()->trackRenderToStage = fl.numberOfFiles-1;
            break;
        case UP_TO_CURRENT:
            getViewParam()->trackRenderToStage = fl.currentFile;
            break;
        }

        // Cause a recompile of the drawing lists
        updateView();
    }
    else {
        // Write a message to the m_status bar
        statusMessage( "Could not generate tracks" );
    }
}
