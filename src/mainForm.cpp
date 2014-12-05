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


// Private pointers are defined in mainForm.h, but the corresponding
// header files cannot be included there -- it causes problems regarding 
// the mutual inclusion of  header files.  Here the pointers are of 
// the general class QWiget and must be cast into the specific 
// class 

// 
// Main form: implements drawing control elements
// 
MainForm::MainForm( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
	// Make a grid layout that will hold all elements
	numCols = 1;
	numRows = 4;
	mainBox = new Q3GridLayout( this, numCols, numRows, 0 );

	// Create the OpenGL widget
	glCanvasFrame = new GLCanvasFrame( this, "glcanvas");
	if (glCanvasFrame)
		((GLCanvasFrame *)glCanvasFrame)->setFormAddress( this );

	// Make a box that will hold 
	// a row of elements to define render quality
	renderBox = new RenderBox( this, "renderBox" );
	if (renderBox) { 
		((RenderBox *)renderBox)->setFormAddress( this );
		mainBox->addWidget( renderBox, 1, 0 );
	}

	// Make a box that will hold 
	// two buttons to take snapshots
	liveBox = new LiveBox( this, "liveBox" );
	if (liveBox) {
		((LiveBox *)liveBox)->setFormAddress( this );
		mainBox->addWidget( liveBox, 2, 0 );
	}

	// Add a label that pretends to be a status bar
	status = new QLabel( this );
	if (status) {
		status->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
		status->setFixedHeight( status->sizeHint().height() );
		status->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
		mainBox->addMultiCellWidget( status, numRows-1, numRows-1, 0, numCols-1 );
	}

    // Clear pointers
    aviz = NULL;
    ab = NULL;
    anb = NULL;
    bb = NULL;
    clb = NULL;
    eb = NULL;
    flb = NULL;
    lb = NULL;
    lcb = NULL;
    pb = NULL;
    plb = NULL;
    sb = NULL;
    slb = NULL;
    stb = NULL;
    tb = NULL;
    trab = NULL;

	// Create some of the more important boards
        if (!ab) {
                ab = new AtomBoard( this, "atomBoard" );
                if (ab)
                        ((AtomBoard *)ab)->setMainFormAddress( this );
		
        }
	if (!sb) {
                sb = new SpinBoard( this, "spinBoard" );
                if (sb)
                        ((SpinBoard *)sb)->setMainFormAddress( this );
        }

        if (!lcb) {
                lcb = new LcBoard( this, "lcBoard" );
                if (lcb)
                        ((LcBoard *)lcb)->setMainFormAddress( this );
        }

        if (!plb) {
                plb = new PolymerBoard( this, "polymerBoard" );
                if (plb)
                        ((PolymerBoard *)plb)->setMainFormAddress( this );
        }

        if (!pb) {
                pb = new PoreBoard( this, "poreBoard" );
                if (pb)
                        ((PoreBoard *)pb)->setMainFormAddress( this );
        }

	// Fix the geometry of this panel
	this->setFixedHeight( renderBox->sizeHint().height() + liveBox->sizeHint().height() + status->sizeHint().height() );
	this->setFixedWidth( renderBox->sizeHint().width() );
}


// Set a pointer to the calling class
void MainForm::setAVizAddress( char * thisAViz )
{
	aviz =  (AViz *)thisAViz;
}


// Set the file type to be used during reading or writing files
void MainForm::setFileType( fileType thisFt )
{
	ft = thisFt;
}


// Get the file type to be used during reading or writing files
fileType MainForm::isFileType( void )
{
	return ft;
}


// Set the view freeze control used to prevent updating of the 
// view object
void MainForm::setKeepViewObject( bool thisKeepViewObject )
{
	keepViewObject = thisKeepViewObject;
}


// Get the  view freeze control used to prevent updating of the 
// view object
bool MainForm::isKeepViewObject( void )
{
	return keepViewObject;
}


// Read a file
void MainForm::readFile( const QString &fn )
{
	aggregateData * ad = this->getAggregateData();;
	viewParam thisVp;

	if ( !fn.isEmpty() ) {
		// Write a message to the status bar
		this->statusMessage( "Loading", (const char *)fn );
		
		switch (this->isFileType()) {
			case XYZ_FILE:

				// Read these coordinates
				if (openCoordinateFunction( (const char *)fn, ad )) {
					// Write a message to the status bar
					this->statusMessage( "Loaded", (const char *)fn );

					// Let this be the current aggregate
					this->updateView();

					// Pass on information to the 
					// various boards
        				if (ab) 
				                ((AtomBoard *)ab)->setData();
        				if (bb) 
				                ((BondBoard *)bb)->setData();
        				if (lcb) 
				                ((LcBoard *)lcb)->setData();
        				if (plb) 
				                ((PolymerBoard *)plb)->setData();
        				if (pb) 
				                ((PoreBoard *)pb)->setData();
        				if (sb) 
				                ((SpinBoard *)sb)->setData();
        				if (trab) 
				                ((TrackBoard *)trab)->setData();
				}
				else {
					// Write a message to the status bar
					this->statusMessage( "Could not load", (const char *)fn );
				}
			break;
			case ANIMATION:
				if (openFileListFunction( (const char *)fn, &fl )) {
					// Write a message to the status bar
					this->statusMessage( "Loaded", (const char *)fn );
				}
				else {
					// Write a message to the status bar
					this->statusMessage( "Could not load", (const char *)fn );
					printf( "Could not load %s\n", (const char *)fn );
				}
			break;
			case VP_FILE:

				// Read these view parameters 
				if (openViewParamFunction( (const char *)fn, &thisVp )) {
					// Write a message to the status bar
					this->statusMessage( "Loaded", (const char *)fn );

					// Let this be the current view 
					// parameters 
					this->setViewParam( thisVp );
				}
				else {
					// Write a message to the status bar
					this->statusMessage( "Could not load", (const char *)fn );
					printf( "Could not load %s\n", (const char *)fn );
				}
			break;
			case UNKNOWN:
					printf( "??? Could not load %s\n", (const char *)fn );
			break;
		}
	}
	else {
		this->statusMessage( "Loading aborted" );
	}
}


// Write a file
void MainForm::writeFile( const QString &fn )
{
	// Read the current view parameters
	viewParam * thisVp = this->getViewParam();				

	if ( !fn.isEmpty() ) {

		switch (this->isFileType()) {
			case VP_FILE:
				// Save these view parameters
				// Make sure the suffix is correct
				checkSuffix( (const char *)fn, "vpm" );

				// Write a message to the status bar
				this->statusMessage( "Saving", (const char *)fn );

				if (saveViewParamFunction( (const char *)fn, thisVp )) {
					// Write a message to the status bar
					this->statusMessage( "Saved", (const char *)fn );
				}
				else {
					// Write a message to the status bar
					this->statusMessage( "Could not save", (const char *)fn );
				}
			break;
			case XYZ_FILE:
				printf( "?? Could not save %s\n", (const char *)fn );
			break;
			case ANIMATION:
				printf( "?? Could not save %s\n", (const char *)fn );
			break;
			case UNKNOWN:
				printf( "?? Could not save %s\n", (const char *)fn );
			break;
		}
	}
	else {
		this->statusMessage( "Saving aborted" );
	}
}


// Read a file from the file list, specified by the index, 
// upon request from the file list board; return the 
// filename
QString MainForm::readFileFromList( int current, bool thisKeepViewObject )
{
	// Register index of file in file list
	fl.currentFile = current;

	// Prepare reading of file
	this->setFileType( XYZ_FILE );
	if (fl.currentFile >= 0 && fl.currentFile < fl.numberOfFiles) {
		QString file = QString( fl.filename[fl.currentFile] );

		// Control freezing of the view object data
		this->setKeepViewObject( thisKeepViewObject );
	
		// Set limits for track rendering, if necessary
		if ((*this->getViewParam()).showTracks) {
			(*this->getViewParam()).trackRenderFromStage = 0;
			switch ((*this->getViewParam()).tRenderMode) {
				case ALL_STAGES:
					(*this->getViewParam()).trackRenderToStage = fl.numberOfFiles-1;
				break;
				case UP_TO_CURRENT:
					(*this->getViewParam()).trackRenderToStage = fl.currentFile;
				break;
			}
		}
		
		// Read the file 
		this->readFile( file );

		// Set freezing of the view object data back to default
		this->setKeepViewObject( FALSE );

		return file;
	}
	else  
		return QString( "--" );
}


// Set factory default values for the view parameters
// and settings (overloaded function) 
void MainForm::setDefaults( void )
{
	viewParam vp;

	// Write a message
	this->statusMessage( "Setting factory defaults");

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

	if (renderBox) 
		((RenderBox *)renderBox)->setValue( vp );

	if (glCanvasFrame)
		((GLCanvasFrame *)glCanvasFrame)->setAuto( vp );

        // Enable updating
        vp.enableUpdate = TRUE;

        // Refresh the drawing now 
	this->setViewParam( vp );
	this->updateRendering();

	// Always update the view object data
	this->setKeepViewObject( FALSE );
}


// Set factory default values for the view parameters
// (overloaded function) 
void MainForm::setDefaults( viewParam vp, const char * filename )
{
	// Write a message
	this->statusMessage( "Setting defaults in file ", filename );

	// Prevent immediate updating
        vp.enableUpdate = FALSE;

	if (renderBox) 
		((RenderBox *)renderBox)->setValue( vp );

	if (glCanvasFrame)
		((GLCanvasFrame *)glCanvasFrame)->setAuto( vp );

	// Adjust the menus
	switch (vp.renderMode) {
		case ATOMS:
			if (aviz) 
				((AViz * )aviz)->setAtomMenus();	
		break;
		case SPINS:
			if (aviz) 
				((AViz *)aviz)->setSpinMenus();	
		break;
		case LIQUID_CRYSTALS:
			if (aviz) 
				((AViz *)aviz)->setLcMenus();	
		break;
		case POLYMERS:
			if (aviz) 
				((AViz *)aviz)->setPolymerMenus();	
		break;
		case PORES:
			if (aviz) 
				((AViz *)aviz)->setPoreMenus();	
		break;
		default:
			if (aviz) 
				((AViz * )aviz)->setAtomMenus();	
		break;
	}

        // Enable updating
        vp.enableUpdate = TRUE;

        // Refresh the drawing now 
	this->setViewParam( vp );
	this->updateRendering();

	// Always update the view object data
	this->setKeepViewObject( FALSE );
}


// Accept new data sent from the main panel 
void MainForm::updateView( void )
{
	if (glCanvasFrame) {
		switch (this->isKeepViewObject()) {
			case FALSE:
				// Update the view object
				((GLCanvasFrame *)glCanvasFrame)->updateView();
			break;
			case TRUE:
				// Update the view object
				((GLCanvasFrame *)glCanvasFrame)->updateViewWithoutViewObjectChange();
			break;
		}
	}
}


// Return a pointer to the current data to the main panel
aggregateData * MainForm::getAggregateData( void )
{
	if (glCanvasFrame) 
		return ((GLCanvasFrame *)glCanvasFrame)->getAggregateData();
	else 
		return NULL;

}


// Accept new view parameters and settings sent from the main panel 
void MainForm::setViewParam( viewParam vp )
{
	// Refresh the drawing now
	if (glCanvasFrame) 
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( vp );
}


// Set the auto buttons according to settings sent from 
// the main panel and trigger auto rot/tilt/spin
void MainForm::setAutoAndStart( viewParam vp )
{
	if (glCanvasFrame) {
		if (vp.autoRot1 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoRot1( );
		}
		if (vp.autoRot2 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoRot2( );
		}
		if (vp.autoTilt1 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoTilt1( );
		}
		if (vp.autoTilt2 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoTilt2( );
		}
		if (vp.autoSpin1 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoSpin1( );
		}
		if (vp.autoSpin2 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoSpin2( );
		}
		if (vp.autoZoom1 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoZoom1( );
		}
		if (vp.autoZoom2 == TRUE) {
			((GLCanvasFrame *)glCanvasFrame)->startAutoZoom2( );
		}

		((GLCanvasFrame *)glCanvasFrame)->setAuto( vp );
	}

	if (liveBox) {
		if (vp.autoSnap == TRUE) {
			((LiveBox *)liveBox)->startAutoSnap( );
		}
	}

	// Save those settings
	if (glCanvasFrame)
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( vp );
}


// Update the rendering of the data, without recompiling
void MainForm::updateRendering( void )
{
	if (glCanvasFrame)
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();
}


// Update the settings of the view parameter board
void MainForm::updateExplicitBoard( void )
{
	if (eb)
                ((ExplicitBoard *)eb)->setExplicit( (*this->getViewParam()) );
}


// Create an image file from the current rendering
// (overloaded function)
void MainForm::snapRendering( void )
{
        if (glCanvasFrame) {
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();
		((GLCanvasFrame *)glCanvasFrame)->snapRendering();
	}
}


// Create an image file from the current rendering
// (overloaded function)
void MainForm::snapRendering( const QString &fn )
{
        if (glCanvasFrame) {
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();
		((GLCanvasFrame *)glCanvasFrame)->snapRendering( (const char *)fn );
	}
}


// Adjust the menus
void MainForm::setAtomMenus( void )
{
	if (aviz) {
		((AViz *)aviz)->setAtomMenus();
	}
}


// Adjust the menus
void MainForm::setSpinMenus( void )
{
	if (aviz) {
		((AViz *)aviz)->setSpinMenus();
	}
}


// Adjust the menus
void MainForm::setLcMenus( void )
{
	if (aviz) {
		((AViz *)aviz)->setLcMenus();
	}
}


// Adjust the menus
void MainForm::setPolymerMenus( void )
{
	if (aviz) {
		((AViz *)aviz)->setPolymerMenus();
	}
}


// Adjust the menus
void MainForm::setPoreMenus( void )
{
	if (aviz) {
		((AViz *)aviz)->setPoreMenus();
	}
}


// Adjust the boards
void MainForm::broadcastDotStyle( void )
{
        if (ab)
                ((AtomBoard *)ab)->setDotStyle();
        if (sb)
                ((SpinBoard *)sb)->setDotStyle();
        if (lcb)
                ((LcBoard *)lcb)->setDotStyle();
        if (plb)
                ((PolymerBoard *)plb)->setDotStyle();
        if (pb)
                ((PoreBoard *)pb)->setDotStyle();

}


// Adjust the boards
void MainForm::broadcastLineStyle( void )
{
        if (ab)
                ((AtomBoard *)ab)->setLineStyle();
        if (sb)
                ((SpinBoard *)sb)->setLineStyle();
        if (lcb)
                ((LcBoard *)lcb)->setLineStyle();
        if (plb)
                ((PolymerBoard *)plb)->setLineStyle();
        if (pb)
                ((PoreBoard *)pb)->setLineStyle();

}


// Adjust the boards
void MainForm::broadcastCubeStyle( void )
{
       if (ab)
                ((AtomBoard *)ab)->setCubeStyle();
        if (sb)
                ((SpinBoard *)sb)->setCubeStyle();
        if (lcb)
                ((LcBoard *)lcb)->setCubeStyle();
        if (plb)
                ((PolymerBoard *)plb)->setCubeStyle();
        if (pb)
                ((PoreBoard *)pb)->setCubeStyle();

}


// Adjust the boards
void MainForm::broadcastCylinderStyle( void )
{
       if (ab)
                ((AtomBoard *)ab)->setCylinderStyle();
        if (sb)
                ((SpinBoard *)sb)->setCylinderStyle();
        if (lcb)
                ((LcBoard *)lcb)->setCylinderStyle();
        if (plb)
                ((PolymerBoard *)plb)->setCylinderStyle();
        if (pb)
                ((PoreBoard *)pb)->setCylinderStyle();

}


// Adjust the boards
void MainForm::broadcastConeStyle( void )
{
        if (ab)
                ((AtomBoard *)ab)->setConeStyle();
        if (sb)
                ((SpinBoard *)sb)->setConeStyle();
        if (lcb)
                ((LcBoard *)lcb)->setConeStyle();
        if (plb)
                ((PolymerBoard *)plb)->setConeStyle();
        if (pb)
                ((PoreBoard *)pb)->setConeStyle();
}


// Adjust the boards
void MainForm::broadcastSphereStyle( void )
{
       if (ab)
                ((AtomBoard *)ab)->setSphereStyle();
        if (sb)
                ((SpinBoard *)sb)->setSphereStyle();
        if (lcb)
                ((LcBoard *)lcb)->setSphereStyle();
        if (plb)
                ((PolymerBoard *)plb)->setSphereStyle();
        if (pb)
                ((PoreBoard *)pb)->setSphereStyle();
}


// Adjust the boards
void MainForm::broadcastLowQuality( void )
{
        if (ab)
                ((AtomBoard *)ab)->setLowQuality();
        if (sb)
                ((SpinBoard *)sb)->setLowQuality();
        if (lcb)
                ((LcBoard *)lcb)->setLowQuality();
        if (plb)
                ((PolymerBoard *)plb)->setLowQuality();
        if (pb)
                ((PoreBoard *)pb)->setLowQuality();
}


// Adjust the boards
void MainForm::broadcastHighQuality( void )
{
        if (ab)
                ((AtomBoard *)ab)->setHighQuality();
        if (sb)
                ((SpinBoard *)sb)->setHighQuality();
        if (lcb)
                ((LcBoard *)lcb)->setHighQuality();
        if (plb)
                ((PolymerBoard *)plb)->setHighQuality();
        if (pb)
                ((PoreBoard *)pb)->setHighQuality();
}


// Adjust the boards
void MainForm::broadcastFinalQuality( void )
{
        if (ab)
                ((AtomBoard *)ab)->setFinalQuality();
        if (sb)
                ((SpinBoard *)sb)->setFinalQuality();
        if (lcb)
                ((LcBoard *)lcb)->setFinalQuality();
        if (plb)
                ((PolymerBoard *)plb)->setFinalQuality();
        if (pb)
                ((PoreBoard *)pb)->setFinalQuality();
}


// Close open boards after change of render mode
void MainForm::closeModeBoards( void )
{
        if (ab)
                ab->hide();
        if (anb)
                anb->hide();
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
viewParam * MainForm::getViewParam( void )
{
	if (glCanvasFrame)
		return ((GLCanvasFrame *)glCanvasFrame)->getViewParam();
	else 
		return NULL;
}


// Return a pointer to the current view object structure 
viewObject * MainForm::getViewObject( void )
{
        // Read the current view object
        // and return
        if (glCanvasFrame)
                return ((GLCanvasFrame *)glCanvasFrame)->getViewObject();
	else 
		return NULL;
}


// Read the current particle data structure and return
particleData * MainForm::getParticleData( void )
{
        // Read the current particle type data
        // and return
        if (glCanvasFrame)
                return ((GLCanvasFrame *)glCanvasFrame)->getParticleData();
	else 
		return NULL;
}


// Set fixed view parameters
void MainForm::setViewXYPlus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = 0.01;
	        (*vp).theta = 270.0;
		(*vp).chi = 0.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting XY+ viewpoint");
	}
}


// Set fixed view parameters
void MainForm::setViewXYMinus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = 180.01;
	        (*vp).theta = 270.0;
		(*vp).chi = 90.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting XY- viewpoint");
	}
}


// Set fixed view parameters
void MainForm::setViewXZPlus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = 90.01;
	        (*vp).theta = -90.0;
		(*vp).chi = 90.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting XZ+ viewpoint");
	}
}


// Set fixed view parameters
void MainForm::setViewXZMinus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = 0.01;
	        (*vp).theta = 0.0;
		(*vp).chi = 0.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting XZ- viewpoint");
	}
}


// Set fixed view parameters
void MainForm::setViewYZPlus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = 0.01;
	        (*vp).theta = 0.0;
		(*vp).chi = -90.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting YZ+ viewpoint");
	}
}


// Set fixed view parameters
void MainForm::setViewYZMinus( void )
{
	if (glCanvasFrame) {
        	// Read the current view parameters
        	viewParam * vp = ((GLCanvasFrame *)glCanvasFrame)->getViewParam();

        	// Set the new parameter values
	        (*vp).phi = -90.01;
	        (*vp).theta = 90.0;
		(*vp).chi = 180.0;

		// Refresh the drawing now
		((GLCanvasFrame *)glCanvasFrame)->setViewParam( (*vp) );
		((GLCanvasFrame *)glCanvasFrame)->updateRendering();

		// Write a message
		this->statusMessage("Setting YZ- viewpoint");
	}
}


// Upon request from the drawing area, pass on data to the 
// frame -- this is used to handle mouse-controlled rotations 
// and spins
void MainForm::computeTransformation( double delta, double w1, double w2, double w3, double * deltaPhi, double * deltaTheta, double * deltaChi )
{
	if (glCanvasFrame) 
		((GLCanvasFrame *)glCanvasFrame)->computeTransformation( delta, w1, w2, w3, deltaPhi, deltaTheta, deltaChi );
}


// Write a status bar message
void MainForm::statusMessage( const char * mess )
{
	if (status)
		status->setText( mess );
}


// Write a status bar message (overloaded function) 
void MainForm::statusMessage( const char * mess1, const char * mess2 )
{
	char * buffer = (char *)malloc(BUFSIZ);
	sprintf(buffer, "%s %s", mess1, mess2);
	if (status)
		status->setText( buffer );
	free(buffer);
}


// Callback function to launch the annotation settings panel
void MainForm::launchAnnotation( void )
{
        if (!anb) {
                anb = new AnnotationBoard( this, "annotationBoard" );
                if (anb)
                        ((AnnotationBoard *)anb)->setMainFormAddress( this );
        }

        // Adjust the controls and show
        // the panel
        if (anb) {
                ((AnnotationBoard *)anb)->setAnnotation( (*this->getViewParam()) );
                anb->show();
        }
}


// Launch the atom control board
void MainForm::launchAtoms( void )
{
        // Get the current particle data and show
        // the panel
        if (ab) {
                ((AtomBoard *)ab)->setData();
                ab->show();
        }

}


// Callback function to launch the bond settings panel
void MainForm::launchBonds( void )
{
        if (!bb) {
                bb = new BondBoard( this, "bondBoard" );
                if (bb)
                        ((BondBoard *)bb)->setMainFormAddress( this );
        }

        // Adjust the controls and show
        // the panel
        if (bb) {
                ((BondBoard *)bb)->setData();
                bb->show();
        }
}


// Callback function to launch the clip settings panel
void MainForm::launchClip( void )
{
        if (!clb) {
                clb = new ClipBoard( this, "clipBoard" );
                if (clb)
                        ((ClipBoard *)clb)->setMainFormAddress( this );
        }

        // Get the current settings and show
        // the panel
        if (clb) {
                ((ClipBoard *)clb)->setClip( (*this->getViewParam()) );
                clb->show();
        }
}


// Callback function to launch the explicit view point settings panel
void MainForm::launchExplicit( void )
{
        if (!eb) {
                eb = new ExplicitBoard( this, "explicitBoard" );
                if (eb)
                        ((ExplicitBoard *)eb)->setMainFormAddress( this );
        }

        // Get the current settings and show
        // the panel
        if (eb) {
                ((ExplicitBoard *)eb)->setExplicit( (*this->getViewParam()) );
                eb->show();
        }
}


// Callback function to launch the file list panel (overloaded function)
void MainForm::launchFileList( void )
{
        if (!flb) {
                flb = new FileListBoard( this, "fileListBoard" );
                if (flb)
                        ((FileListBoard *)flb)->setMainFormAddress( this );
        }

        // Show the file list now (resetting the view freeze control)
	if (flb) {
		((FileListBoard *)flb)->resetKeepViewScale();
		flb->show();
	}
}


// Callback function to launch the file list panel (overloaded function)
void MainForm::launchFileList( const QString &fn )
{
        if (!flb) {
                flb = new FileListBoard( this, "fileListBoard" );
                if (flb)
                        ((FileListBoard *)flb)->setMainFormAddress( this );
        }

	// Reset counter and prepare cycling
	if (flb) {
		((FileListBoard *)flb)->setFileListParam( 0, fl.numberOfFiles, fn );
	}

        // Show the file list now (resetting the view freeze control)
	if (flb) {
		((FileListBoard *)flb)->resetKeepViewScale();
		flb->show();
	}
}


// Callback function to launch the lights settings panel
void MainForm::launchLights( void )
{
        if (!lb) {
                lb = new LightsBoard( this, "lightsBoard" );
                if (lb)
                        ((LightsBoard *)lb)->setMainFormAddress( this );
        }

        // Adjust the controls and show
        // the panel
        if (lb) {
                ((LightsBoard *)lb)->setLights( (*this->getViewParam()) );
                lb->show();
        }
}


// Callback function to launch the liquid crystal settings panel
void MainForm::launchLiquidCrystals( void )
{
        // Get the current liquid crystal data and show
        // the panel
        if (lcb) {
                ((LcBoard *)lcb)->setData();
                lcb->show();
        }
}


// Callback function to launch the pore settings panel
void MainForm::launchPores( void )
{
        // Get the current pore data and show
        // the panel
        if (pb) {
                ((PoreBoard *)pb)->setData();
                pb->show();
        }
}


// Callback function to launch the polymers settings panel
void MainForm::launchPolymers( void )
{
        // Get the current polymer data and show
        // the panel
        if (plb) {
                ((PolymerBoard *)plb)->setData();
                plb->show();
        }
}


// Callback function to launch the slicing settings panel
void MainForm::launchSlice( void )
{
        if (!slb) {
                slb = new SliceBoard( this, "sliceBoard" );
                if (slb)
                        ((SliceBoard *)slb)->setMainFormAddress( this );
        }

        // Get the current settings and show
        // the panel
        if (slb) {
                ((SliceBoard *)slb)->setSlice( (*this->getViewParam()) );

                // Use the object boundaries as defaults
                if ( (*this->getViewParam()).slicingSet == FALSE)
                        ((SliceBoard *)slb)->autoSlice();

                slb->show();
        }
}


// Callback function to launch the spin settings panel
void MainForm::launchSpins( void )
{
        // Get the current spin data and show
        // the panel
        if (sb) {
                ((SpinBoard *)sb)->setData();
                sb->show();
        }
}


// Callback function to launch the stretch settings panel
// (to manipulate data)
void MainForm::launchStretch( void )
{
        if (!stb) {
                stb = new StretchBoard( this, "stretchBoard" );
                if (stb)
                        ((StretchBoard *)stb)->setMainFormAddress( this );
        }

        // Adjust the controls and show
        // the panel
        if (stb) {
                ((StretchBoard *)stb)->setData( (*this->getViewParam()) );
                stb->show();
        }
}


// Callback function to launch the track board panel
void MainForm::launchTrack( const QString &fn )
{
        if (!trab) {
                trab = new TrackBoard( this, "trackBoard" );
                if (trab)
                        ((TrackBoard *)trab)->setMainFormAddress( this );
        }

        // Get the current particle data and show
        // the panel
        if (trab) {
                ((TrackBoard *)trab)->setFileListParam( fn );
                ((TrackBoard *)trab)->setData();
                trab->show();
        }
}


// Callback function to launch the translation settings panel
void MainForm::launchTranslation( void )
{
        if (!tb) {
                tb = new TranslationBoard( this, "transBoard" );
                if (tb)
                        ((TranslationBoard *)tb)->setMainFormAddress( this );
        }

        // Adjust the controls and show
        // the panel
        if (tb) {
                ((TranslationBoard *)tb)->setData( (*this->getViewParam()) );
                tb->show();
        }
}


// Callback function to hide the track board panel
void MainForm::hideTrack()
{
	if (trab)
		trab->hide();
}


// Generate track data file based on current file list
void MainForm::generateTracks()
{	
	aggregateData * ad = this->getAggregateData();;
	trackData * td =  NULL;

	// Get a pointer to the track data structure
	if (glCanvasFrame)
		td = ((GLCanvasFrame *)glCanvasFrame)->getTrackData();

	// Write a message to the status bar
	this->statusMessage( "Generating tracks..." );

	// Attempt to generate tracks now, using a pointer to 
	// the current aggregated structure (it will be necessary
	// to read in aggregated data over and over again) and
	// a pointer to the current track data structure
	if (generateTrackDataFunction( &fl, ad, td )) {

		// Write a message to the status bar
		this->statusMessage( "Completed generation of tracks" );

		// Set limits for first track rendering
		(*this->getViewParam()).trackRenderFromStage = 0;

		switch ((*this->getViewParam()).tRenderMode) {
			case ALL_STAGES:
				(*this->getViewParam()).trackRenderToStage = fl.numberOfFiles-1;
			break;
			case UP_TO_CURRENT:
				(*this->getViewParam()).trackRenderToStage = fl.currentFile;
			break;
		}

		// Cause a recompile of the drawing lists	
		this->updateView();
	}
	else {
		// Write a message to the status bar
		this->statusMessage( "Could not generate tracks" );
	}
}


// Give size hints and define size policy
QSize MainForm::sizeHint() const
{
	if (renderBox && status)
		return QSize( renderBox->width(), renderBox->height() + status->height() );
	else 
		return QSize( MIN_SIZE, MIN_SIZE );
}


// Give size hints and define size policy
QSizePolicy MainForm::sizePolicy() const
{
	return QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}
