//
// Implementation of the OpenGL drawing canvas area
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
                 Technion. 32000 Haifa Israel gery@tx.technion.ac.il
***********************************************************************/

#include "glCanvasArea.h"
#include "memoryFunctions.h"
#include "./bitmaps/hand2.xbm"
#include "./bitmaps/hand2Shift.xbm"
#include "./bitmaps/hand2Zoom.xbm"
#include "./bitmaps/hand2Mask.xbm"
#include "./bitmaps/hand2ShiftMask.xbm"
#include "./bitmaps/hand2ZoomMask.xbm"

static GLfloat black_mat[] = { black_color, 1.0 };
static GLfloat slice_mat[] = { white_color, 0.95 };
static GLfloat opaque_mat[] = { white_color, 1.0 };

// OpenGL Drawing area widget constructor 
GLCanvasArea::GLCanvasArea( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
{
	// Init the data structures
	allocateParticleData( &pd );

	ad.haveMemoryAllocated = FALSE;
	ad.numberOfParticles = 0;

	td.haveMemoryAllocated = FALSE;
	td.numberOfTracks = 0;
	td.numberOfStages = 0;

	singleAtoms = atomBonds = spins = lcs = pores = polymerAtoms = polymerBonds = 0;

	// Reset the main form pointer 
	mainForm = NULL;

	// Set defaults to have well-defined starting point
	vo.centerx = vo.centery = vo.centerz = 0.0;
	vo.xmin = vo.ymin = vo.zmin = -0.5;
	vo.xmax = vo.ymax = vo.zmax = 0.5;
	vo.xyzmin = -0.5;
	vo.xyzmax = 0.5;
	vo.maxExtension = 1.0;
	vp.eyex = vp.eyey = 0.0;
	vp.eyez = -1.0;
	vp.fovy = FOVY;
	vp.phi = PHI;
	vp.theta = THETA;
	vp.chi = CHI;
	vp.dolly = DOLLY;
	vp.panX = vp.panY = vp.panZ = 0.0;
	vp.renderQuality = LOW;
	vp.renderMode = ATOMS;
	vp.atomRenderStyle = ADOT;
	vp.spinRenderStyle = SLINE;
	vp.lcRenderStyle = LLINE;
	vp.poreRenderStyle = PLINE;

	// Reset a flag
	mouseOn = FALSE;
}


GLCanvasArea::~GLCanvasArea()
{
}


// Set a pointer to the main form
void GLCanvasArea::setFormAddress( MainForm * thisForm )
{
        mainForm = thisForm;
}


// Accept new data for the drawing and prepare 
// drawing lists 
void GLCanvasArea::setViewObjectAndRecompile( viewObject thisVo )
{
	// Register the new view object data
	vo = thisVo;

	// Make sure that colors are defined for all atom
	// types
	this->completeParticleData();

	// Compile the drawing lists
	this->recompileObjects();
}


// Recompile the drawing lists after object parameters have 
// been changed
void GLCanvasArea::recompileObjects( void )
{
	// Clear all lists 
	if (glIsList(singleAtoms)) 
		glDeleteLists( singleAtoms, 1 );
	if (glIsList(atomBonds)) 
		glDeleteLists( atomBonds, 1 );

	if (glIsList(spins)) 
		glDeleteLists( spins, 1 );
	if (glIsList(lcs)) 
		glDeleteLists( lcs, 1 );
	if (glIsList(polymerAtoms)) 
		glDeleteLists( polymerAtoms, 1 );
	if (glIsList(polymerBonds)) 
		glDeleteLists( polymerBonds, 1 );
	if (glIsList(pores)) 
		glDeleteLists( pores, 1 );
	if (glIsList(tracks)) 
		glDeleteLists( tracks, 1 );

	// Should drawing lists be compiled? 
	if (ad.numberOfParticles) {
		// Compile the drawing procedures: first the bonds...
		if (vp.renderMode == ATOMS) 	
			atomBonds = this->makeBondsObject( FALSE );

		if (vp.renderMode == POLYMERS) 	
			polymerBonds = this->makeBondsObject( TRUE );

		// Do the particles
		switch (vp.renderMode) {
			case ATOMS:
				switch (vp.atomRenderStyle) {
					case ADOT:
						singleAtoms = this->makeDotsObject();
					break;
					case ACUBE:
						singleAtoms = this->makeCubesObject();
					break;
					case ASPHERE:
						singleAtoms = this->makeSpheresObject();
					break;
					default:
						singleAtoms = this->makeDotsObject();
					break;
				}
			break;
			case SPINS:	
				switch (vp.spinRenderStyle) {
					case SLINE:
						spins = this->makeLinesObject( TRUE );
					break;
					case SCONE:
						spins = this->makeConesObject( TRUE );
					break;
					case SSPHERE:
						spins = this->makePinsObject();
					break;
					default:
						spins = this->makeLinesObject( TRUE );
					break;
				}
				break;
			case LIQUID_CRYSTALS:	
				switch (vp.lcRenderStyle) {
					case LDOT:
						lcs = this->makeDotsObject();
					break;
					case LLINE:
						lcs = this->makeLinesObject( FALSE );
					break;
					case LCYLINDER:
						lcs = this->makeCylindersObject( FALSE );
					break;
					case LCONE:
						lcs = this->makeConesObject( FALSE );
					break;
					default:
						lcs = this->makeLinesObject( FALSE );
					break;
				}
			break;
			case POLYMERS:	
				switch (vp.polymerAtomRenderStyle) {
					case PADOT:
						polymerAtoms = this->makeDotsObject();
					break;
					case PACUBE:
						polymerAtoms = this->makeCubesObject();
					break;
					case PASPHERE:
						polymerAtoms = this->makeSpheresObject();
					break;
					default:
						polymerAtoms = this->makeDotsObject();
					break;
				}
			break;
			case PORES:	
				switch (vp.poreRenderStyle) {
					case PDOT:
						pores = this->makeDotsObject();
					break;
					case PLINE:
						pores = this->makeLinesObject( FALSE );
					break;
					case PCYLINDER:
						pores = this->makeCylindersObject( FALSE );
					break;
					default:
						pores = this->makeLinesObject( FALSE );
					break;
				}
			break;
		}
	}

	// Also compile drawing lists for particle tracks, if needed
	if (vp.showTracks == TRUE) 
		tracks = this->makeTracksObject();
}


// Create an image file from the current rendering
// (overloaded function)
void GLCanvasArea::snapRendering( void )
{
	char * filename = (char *)malloc( BUFSIZ );

	// Create the image filename
	getImageFilename( ad.filename, filename );

	// Modify filename to avoid overwriting of existing files
	modifyImageFilename( filename );

	this->snapRendering( filename );	

	free(filename);
}


// Create an image file from the current rendering
// (overloaded function)
void GLCanvasArea::snapRendering( const char * filename )
{
	int width = this->width();
	int height = this->height();
	char * imageRed;
	char * imageGreen;
	char * imageBlue;

	// Allocate memory for the image
	imageRed = (char *)malloc( width*height*sizeof(char));
	imageGreen = (char *)malloc( width*height*sizeof(char));
	imageBlue = (char *)malloc( width*height*sizeof(char));

	// Read the pixels off the screen
	glPixelStorei( GL_PACK_ALIGNMENT, 1);
	glReadPixels( 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, imageRed );
	glReadPixels( 0, 0, width, height, GL_GREEN, GL_UNSIGNED_BYTE, imageGreen );
	glReadPixels( 0, 0, width, height, GL_BLUE, GL_UNSIGNED_BYTE, imageBlue );
	if (writePNGImage( filename, ad.IDstring, width, height, imageRed, imageGreen, imageBlue ) == TRUE) {
		if (mainForm) 
			mainForm->statusMessage( "Created PNG image file ", filename);
	}
	else {
		if (mainForm) 
			mainForm->statusMessage( "Could not create PNG image file");
			printf("Could not create PNG image file\n");
	}

	// Free memory
	free(imageRed);
	free(imageGreen);
	free(imageBlue);
}


// Accept new parameters and settings for the drawing 
// (overloaded function) 
void GLCanvasArea::setViewParam( viewParam thisVp )
{
	vp = thisVp;
}


// Return a pointer to the molecule data currently used in the rendering
aggregateData * GLCanvasArea::getAggregateData( void ) 
{
	return &ad;
}


// Return a pointer to the view parameters currently used in the rendering
viewParam * GLCanvasArea::getViewParam( void ) 
{
	return &vp;
}


// Return a pointer to the view object structure currently used 
// in the rendering
viewObject * GLCanvasArea::getViewObject( void ) 
{
	return &vo;
}


// Return a pointer to the particle data structure currently 
// used in the rendering
particleData * GLCanvasArea::getParticleData( void ) 
{
	return &pd;
}


// Complete the current color definitions 
void GLCanvasArea::readParticleData( void )
{
	// Make sure that colors are defined for all atom
	// types
	this->completeParticleData();
}


// Make sure that the current color definitions are adequate
// for the current set of particles
void GLCanvasArea::completeParticleData( void )
{
	// Go through the entire set of particles and compare
	// types with known color types
	for (int i=0;i<ad.numberOfParticles;i++) {
		int j = 0;
		while (typeCmp( (char *)&ad.particles[i].type, (char *)&pd.type[j] ) != TRUE && j < pd.numberOfParticleTypes-1) {	
			j++;
		}

		if (j >= pd.numberOfParticleTypes-1) {
			// Found an unknown particle type --
			// need to enlarge the list of atom colors

			char * buffer = (char *)malloc( BUFSIZ );
			sprintf(buffer, "%s %s", (char *)&ad.particles[i].type, unknown_color);
			// The new color is the second-last color; 
			// the last one is reserved if later identification
			// fails for some reason despite the present
			// effort
			int nc = pd.numberOfParticleTypes;
			setColorAndDefaults( &pd, buffer, nc-1 );

			// Restore the emergency color 
			setUnidentColor( &pd, nc );

			free(buffer);
		}
	}
}


// Respond to mouse events
void GLCanvasArea::mousePressEvent( QMouseEvent * qme )
{
	// Create bitmaps to set cursor icon
	QBitmap handIcon( hand2_width, hand2_height, hand2_bits, TRUE );
	QBitmap handIconMask( hand2Mask_width, hand2Mask_height, hand2Mask_bits, TRUE );
	QBitmap handShiftIcon( hand2Shift_width, hand2Shift_height, hand2Shift_bits, TRUE );
	QBitmap handShiftIconMask( hand2ShiftMask_width, hand2ShiftMask_height, hand2ShiftMask_bits, TRUE );
	QBitmap handZoomIcon( hand2Zoom_width, hand2Zoom_height, hand2Zoom_bits, TRUE );
	QBitmap handZoomIconMask( hand2ZoomMask_width, hand2ZoomMask_height, hand2ZoomMask_bits, TRUE );

	// Create cursor icons
	QCursor handCursor( handIcon, handIconMask );
	QCursor handShiftCursor( handShiftIcon, handShiftIconMask );
	QCursor handZoomCursor( handZoomIcon, handZoomIconMask );

	// Register the current mouse position
	mouseXPre = qme->x(); 
	mouseYPre = qme->y();
	mouseX = qme->x(); 
	mouseY = qme->y();

	// Register the current button state
	mouseButton = (int)qme->button();

	// Assign a hand cursor
	switch (mouseButton) {
        case Qt::LeftButton:
			// Show standard hand icon and rotate/tilt
			// if the left or right mouse button is pressed
		        grabMouse( handCursor );
		break;
        case Qt::MidButton:
			// Show pan hand icon and pan
			// if the middle mouse button is pressed
		        grabMouse( handShiftCursor );
		break;	
        case Qt::RightButton:
			// Show zoom hand icon and zoom
			// if the middle mouse button is pressed
		        grabMouse( handZoomCursor );
		break;	
	}
}


// Respond to mouse events
void GLCanvasArea::mouseReleaseEvent( QMouseEvent * qme )
{
        releaseMouse();

	// Register the current mouse position
	mouseX = qme->x(); 
	mouseY = qme->y();

	// Reset a flag
	mouseOn = FALSE;
}


// Respond to mouse events
void GLCanvasArea::mouseMoveEvent( QMouseEvent * qme )
{      
	GLdouble mvmatrix[16], projmatrix[16];
	GLint viewport[4];
	GLdouble wx, wy, wz, wxpre, wypre, wzpre;
	double vx, vy, vz, handlex, handley, handlez; 
	double axisx, axisy, axisz, norm2 = 0.0;
	double ox, oy, oz, oax, oay, oaz; 
	double dirx = 0.0; 
	double diry = 0.0; 
	double dirz = 0.0;
	double delta = 0.0; 
	double deltaPhi, deltaTheta, deltaChi;
	int dx = 0; 
	int dy = 0;

	// Register the current mouse position
	mouseX = qme->x(); 
	mouseY = this->height() - qme->y();

	// Find the world position 
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	// Find the world coordinates of a point on the ray covered 	
	// by the current mouse view port position 
        gluUnProject( mouseX, mouseY, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

	// Similarly, find the previous world coordinates (can't use
	// values from previous cycle since coordinate system
	// has been transformed in the mean time)
        gluUnProject( mouseXPre, mouseYPre, 0.0, mvmatrix, projmatrix, viewport, &wxpre, &wypre, &wzpre);

	if (mouseOn) {
		// Find the direction of the 'handle' formed by this point 
		// and the origin
		handlex = wx - vo.centerx;
		handley = wy - vo.centery;
		handlez = wz - vo.centerz;

		// Translate the motion of the mouse
		// into changes of the view parameters 
		// for panning operations
		dx = mouseX-mouseXPre;
		dy = mouseY-mouseYPre;

		// Find the 'velocity vector', given by the cursor motion
		vx = wx - wxpre;
		vy = wy - wypre;
		vz = wz - wzpre;

		// Find the magnitude of velocity -- this controls 
		// extent of transformation
		norm2 = sqrt( vx*vx + vy*vy + vz*vz );
		delta = norm2*MOUSE_TRANSFORMATION_STEP;

		// Find axis that is perpendicular to both velocity and 
		// 'handle' vector from origin to world coordinate point,
		// and which intersects the origin -- this is given by
		// the vector product of velocity and 'handle'
		axisx = vy*handlez - vz*handley;
		axisy = -vx*handlez + vz*handlex;
		axisz = vx*handley - vy*handlex;

		// Normalize the axis
		norm2 = sqrt( axisx*axisx + axisy*axisy + axisz*axisz );
		if (norm2 > 0.0) {
			axisx /= norm2;
			axisy /= norm2;
			axisz /= norm2;
		}

		// Now this axis must be expressed in terms of the fixed 
		// space axis coordinate system
		gluProject( vo.centerx, vo.centery, vo.centerz, mvmatrix, projmatrix, viewport, &ox, &oy, &oz);
		gluProject( vo.centerx+axisx, vo.centery+axisy, vo.centerz+axisz, mvmatrix, projmatrix, viewport, &oax, &oay, &oaz);

		dirx = oax - ox;
		diry = oay - oy;
		dirz = oaz - oz;

		// Normalize the fixed-space axis
		norm2 = sqrt((dirx*dirx + diry*diry + dirz*dirz));
		if (norm2 > 0.0) {
			dirx /= norm2;
			diry /= norm2;
			dirz /= norm2;
		}
	}

	// Register the new current mouse position and its projection
	mouseXPre = mouseX;	
	mouseYPre = mouseY;	

	// Depending on the mouse button state, this
	// is either rotating/tilting or panning or zooming
	if (mouseOn) {
		switch (mouseButton) {
            case Qt::LeftButton:
				// Rotate or tilt here (components must be 
				// swapped and signs changed)
				if (mainForm && norm2 > 0.0) {
					mainForm->computeTransformation( delta, dirx, dirz, -diry, &deltaPhi, &deltaTheta, &deltaChi );

					vp.phi += deltaPhi;
					vp.theta += deltaTheta;
					vp.chi += deltaChi;
				
					// Update the viewpoint parameter 
					// settings board	
					mainForm->updateExplicitBoard();
				}

			break;
            case Qt::MidButton:
				// Pan here
				if (dx > 0) {
					for (int ix=0;ix<dx;ix++) {
						vp.panRight += MOUSE_PAN_STEP;
					}
				}
				if (dx < 0) {
					for (int ix=0;ix>dx;ix--) {
						vp.panRight -= MOUSE_PAN_STEP;
					}
				}

				if (dy > 0) {
					for (int iy=0;iy<dy;iy++) {
						vp.panTop -= MOUSE_PAN_STEP;
					}
				}
				if (dy < 0) {
					for (int iy=0;iy>dy;iy--) {
						vp.panTop += MOUSE_PAN_STEP;
					}
				}
			break;
            case Qt::RightButton:
				// Zoom here (it's really panning into the 
				// image)
				if (dy > 0) {
					for (int iy=0;iy<dy;iy++) {
						vp.panForward += MOUSE_PAN_STEP;
					}
				}
				if (dy < 0) {
					for (int iy=0;iy>dy;iy--) {
						vp.panForward -= MOUSE_PAN_STEP;
					}
				}
			break;
		}
	}
	else {
		// Set a flag to enable mouse-controlled transformations --
		// this can be done only after the mouse button have been
		// pressed and the mouse moved a bit
 		mouseOn = TRUE;
	}
	
	// Redraw
	this->updateGL();
}


// Respond to exposing: draw now
void GLCanvasArea::paintGL( void )
{
	
	double scaleFact = (double)this->width() / (double)this->height();

	// Always enable depth testing for hidden surface removal 
	glEnable( GL_DEPTH_TEST );

	// Enable lighting
	glEnable( GL_LIGHTING );

	// Decide on a shading model, depending on the complexity of the 
	// scene
	if (!simpleRendering() ) {
		glShadeModel( GL_SMOOTH );
	}
	else {
		glShadeModel( GL_FLAT );
	}

	// Set the background color to black or white
	switch (vp.background) {
		case BGBLACK:
			glClearColor( black_color, 1.0 ); 	// black
		break;
		case BGWHITE:
			glClearColor( white_color, 1.0 );	// white
		break;
	}

	// Clearing the screen  (color buffer)
	glClear(GL_COLOR_BUFFER_BIT);

	double eyeShift[2] = {0, 0};
	int eyeNum = 1 + (int)vp.stereoVisionEn;
	if (vp.stereoVisionEn) {
		eyeShift[0] = -vp.eyeSeparation;
		eyeShift[1] = vp.eyeSeparation;
	}

	// This loop will iterate twice (for both eyes) if Stereo Vision is enabled
	// and once if it is diabled
	for (int i = 0; i < eyeNum; i++) {
		// Clearing the screen  (depth buffer)
		glClear(GL_DEPTH_BUFFER_BIT);
		// Right and left color masking for stereo vision
		if (vp.stereoVisionEn && i == 0) // Right eye color mask
			glColorMask(false, true, true, false);
		if (vp.stereoVisionEn && i == 1) // Left eye color mask
			glColorMask(true, false, false, false);

		// Set up a coordinate system 
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		// Rotate the coordinate system about the Euler axes. 
		// Calculate the eye position; make sure everything is 
		// visible under the chosen fovy angle

		vp.eyex = vo.centerx;
		vp.eyey = vo.centery;
		vp.eyez = vo.xyzmin - (vo.maxExtension*0.75) / tan(vp.fovy*PI/180.*0.5);
	

		// Move eye position according to dolly value
		GLdouble eyez = vp.eyez;

		switch (vp.viewMode) {
			case PERSPECTIVE:
				eyez += (GLdouble)vp.dolly/100.0*vo.maxExtension;
			break;
			case ORTHO:
				eyez += (GLdouble)vp.dolly/10.0*vo.maxExtension;
			break;
		}

		// Prepare the panning
		GLfloat panx = (GLfloat)vp.panX;
		GLfloat pany = (GLfloat)vp.panY;
		GLfloat panz = (GLfloat)vp.panZ;
		GLfloat panright = (GLfloat)vp.panRight;
		GLfloat pantop = (GLfloat)vp.panTop;
		GLfloat panforward = (GLfloat)vp.panForward;

		// Suggest the max and min distance defining the scene
		vp.minDist = fabs(eyez + panz - vo.centerz) - (vo.maxExtension)*SAFETY_DIST - 1.0;
		if (vp.minDist < 5.0 && vp.minDist > 0.0) {
			do {
				vp.minDist -= 0.1;
			} while (vp.minDist > 0.1);
		}
		if (vp.minDist < 0.0) {
				vp.minDist = 0.1;
		}

		vp.maxDist =  vp.minDist + 2.0*(vo.maxExtension)*SAFETY_DIST + 10.0 - panforward*SAFETY_DIST;

		// Define perspective and view point, suitable for a plain XY-view
		// under vp.viewAngle deg.  If necessary, use user-defined clip plane 
		// settings
		GLfloat minDist = vp.minDist;
		GLfloat maxDist = vp.maxDist;
		if (vp.autoClipNear == FALSE) {
			minDist = vp.clipNear;
		}
		else {
			vp.clipNear = minDist;
		}
		if (vp.autoClipFar == FALSE) {
			maxDist = vp.clipFar;
		}
		else {
			vp.clipFar = maxDist;
		}


		switch (vp.viewMode) {
			case PERSPECTIVE:
				gluPerspective( vp.fovy, (GLfloat)scaleFact, minDist, maxDist); 
				gluLookAt( vp.eyex + eyeShift[i], vp.eyey, eyez, vo.centerx, vo.centery, vo.centerz, 0.0, 1.0, 0.0 );
			break;
			case ORTHO:
				GLdouble dollyFact =  (GLdouble)vp.dolly/100.0*vo.maxExtension;
				glOrtho( -vo.maxXYExtension*scaleFact/dollyFact, vo.maxXYExtension*scaleFact/dollyFact, -vo.maxXYExtension/dollyFact, vo.maxXYExtension/dollyFact, minDist, maxDist );
				gluLookAt( vp.eyex + eyeShift[i], vp.eyey, eyez, vo.centerx, vo.centery, vo.centerz, 0.0, 1.0, 0.0 );
			break;
		}
	
		// Do the 1st part of the panning transition 
		glTranslatef( -panright, pantop, -panforward );

		// Prepare rotations
		glTranslatef( vo.centerx, vo.centery, vo.centerz );
		
		// To end up with the z-axis pointing upwards, subtact
		// 90 deg from theta; to end up with the x-axis 
		// pointing out of the screen, add 90 deg to phi 
		// Rotate the object about original z-axis of the object
		glRotated( (GLdouble)vp.phi, 0.0, 1.0, 0.0 ); 
		glRotated( (GLdouble)90.0, 0.0, 0.0, 1.0 ); 

		// Rotate about y-axis of the object (theta)
		glRotated( -(GLdouble)vp.theta+90.0, 0.0, 1.0, 0.0 ); 

		// Rotate again about new z-axis of the object (chi)
		glRotated( (GLdouble)vp.chi+90.0, 0.0, 0.0, 1.0 );
		
		// Complete rotations
		glTranslatef( -vo.centerx, -vo.centery, -vo.centerz);

		// Do the 2nd part of the panning transition 
		glTranslatef( -panx, pany, panz );
		
		// Create the light(s) that are at a fixed position in space
		makeLights();

		// First draw the slicing planes, if needed 
		if (vp.slicing == TRUE) 
			drawSlicingPlanes();

		// Set common material properties -- here shininess and 
		// specular intensity are coupled
		GLfloat thisShininess[] = { vp.shininess };
		GLfloat thisSpecular[] = {  (GLfloat)(vp.shininess)/128.0,  (GLfloat)(vp.shininess)/128.0,  (GLfloat)(vp.shininess)/128.0, 1.0 };
		glMaterialfv( GL_FRONT, GL_SHININESS, thisShininess );
		glMaterialfv( GL_FRONT, GL_SPECULAR, thisSpecular );
		glMaterialfv( GL_FRONT, GL_EMISSION, black_mat );

		// Render now
		glPushMatrix();
		if (vp.onlyContour == FALSE) {

			// Begin by rendering the tracks	
			if (vp.showTracks == TRUE && glIsList(tracks)) {
				glCallList( tracks );
			}

			// Continuer by rendering the bonds
			if (vp.renderMode == ATOMS && glIsList(atomBonds)) {
				glCallList( atomBonds );
			}
			if (vp.renderMode == POLYMERS && glIsList(polymerBonds)) {
				glCallList( polymerBonds );
			}
	
			// Render now the particles
			switch (vp.renderMode) {
				case ATOMS:	
					if (glIsList(singleAtoms)) 
						glCallList( singleAtoms );
				break;
				case SPINS:
					if (glIsList(spins))
						glCallList( spins );
				break;
				case LIQUID_CRYSTALS:	
					if (glIsList(lcs)) 
						glCallList( lcs );
				break;
				case POLYMERS:	
					if (glIsList(polymerAtoms)) 
						glCallList( polymerAtoms );
				break;
				case PORES:
					if (glIsList(pores)) 
						glCallList( pores );
				break;
			}
		}
		glPopMatrix();

		// Switch on antialiasing 
		this->antiAliasing( TRUE );

		// Switch off lighting for the remaining part
		glDisable( GL_LIGHTING );	

		// Draw contours 
		if (vp.showContour == TRUE)
			drawContourBox();

		// Draw axes if needed 
		if (vp.showAxes == TRUE) {
			drawAxes();
			// Clear the matrix stack while drawing the labels
			drawAxesAnnotation();
		}

		// Draw annotation text if needed
		if (vp.showAnnotation == TRUE) {
			drawAnnotation();
		}

		// Switch off antialiasing 
		this->antiAliasing( FALSE );
	} // Both eyes iterations

	// Restore (disable) color mask
	if (vp.stereoVisionEn)
		glColorMask(true, true, true, true);

	// Automatically take a snapshot, if desired
	if (vp.autoSnap == TRUE) {
		this->snapRendering();
	}
}


// Init the canvas
void GLCanvasArea::initializeGL( void )
{

    if (this->isValid()) {
		// Double-buffering is usually set already by default...
		// this is just to be sure
		QGLFormat format = this->format();
		format.setDoubleBuffer( TRUE );
		this->setFormat(format);

		if (!this->isValid()) {
			printf("Double-buffering not supported: Falling back on single buffer\n");
			format.setDoubleBuffer( FALSE );
			this->setFormat(format);
		}
	}
}


// Respond to resizing
void GLCanvasArea::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
}


// Position the light(s) 
void GLCanvasArea::makeLights( void )
{
	GLdouble mvmatrix[16], projmatrix[16];
	GLint viewport[4];
	GLdouble wx, wy, wz;
	GLfloat light[] = { white_color, 1.0 };
	GLfloat distx = this->width();
	GLfloat disty = this->height();

	// Set global light parameters -- by default use 
	// 100% ambient light in line/dot rendering mode
	if (!simpleRendering() ) {
		GLfloat lmodel_ambient[] = { (GLfloat)vp.ambientLight, (GLfloat)vp.ambientLight, (GLfloat)vp.ambientLight, 1.0 };
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lmodel_ambient );
	}
	else {
		GLfloat lmodel_ambient[] = { white_color, 1.0 };
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lmodel_ambient );
	}

	// Find the world position 
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	// Position a light that points in left direction
	if (vp.light[0]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[0]);
		gluUnProject( -thisDistx, 0.0, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light0_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT0, GL_POSITION, light0_dir );
			glLightfv( GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light );
			glLightfv( GL_LIGHT0, GL_EMISSION, light );
			glEnable( GL_LIGHT0 );
		}
	}
	else {
		glDisable( GL_LIGHT0 );
	}

	// Position a light that points in right direction
	if (vp.light[1]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[1]);
		gluUnProject( thisDistx, 0.0, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light1_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT1, GL_POSITION, light1_dir );
			glLightfv( GL_LIGHT1, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT1, GL_EMISSION, light );
			glLightfv( GL_LIGHT1, GL_SPECULAR, light );
			glEnable( GL_LIGHT1 );
		}
	}
	else {
		glDisable( GL_LIGHT1 );
	}

	// Position a light that points in top direction
	if (vp.light[2]) {
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[2]);
		gluUnProject( 0.0, thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light2_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT2, GL_POSITION, light2_dir );
			glLightfv( GL_LIGHT2, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT2, GL_EMISSION, light );
			glLightfv( GL_LIGHT2, GL_SPECULAR, light );
			glEnable( GL_LIGHT2 );
		}
	}
	else {
		glDisable( GL_LIGHT2 );
	}
	
	// Position a light that points in bottom direction
	if (vp.light[3]) {
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[3]);
		gluUnProject( 0.0, -thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light3_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT3, GL_POSITION, light3_dir );
			glLightfv( GL_LIGHT3, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT3, GL_EMISSION, light );
			glLightfv( GL_LIGHT3, GL_SPECULAR, light );
			glEnable( GL_LIGHT3 );
		}
	}
	else {
		glDisable( GL_LIGHT3 );
	}

	// Position a light that points in top left direction
	if (vp.light[4]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[4]);
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[4]);
		gluUnProject( -thisDistx, thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light4_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT4, GL_POSITION, light4_dir );
			glLightfv( GL_LIGHT4, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT4, GL_EMISSION, light );
			glLightfv( GL_LIGHT4, GL_SPECULAR, light );
			glEnable( GL_LIGHT4 );
		}
	}
	else {
		glDisable( GL_LIGHT4 );
	}

	// Position a light that points in top right direction
	if (vp.light[5]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[5]);
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[5]);
		gluUnProject( thisDistx, thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light5_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT5, GL_POSITION, light5_dir );
			glLightfv( GL_LIGHT5, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT5, GL_EMISSION, light );
			glLightfv( GL_LIGHT5, GL_SPECULAR, light );
			glEnable( GL_LIGHT5 );
		}
	}
	else {
		glDisable( GL_LIGHT5 );
	}

	// Position a light that points in bottom left direction
	if (vp.light[6]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[6]);
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[6]);
		gluUnProject( -thisDistx, -thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light6_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT6, GL_POSITION, light6_dir );
			glLightfv( GL_LIGHT6, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT6, GL_EMISSION, light );
			glLightfv( GL_LIGHT6, GL_SPECULAR, light );
			glEnable( GL_LIGHT6 );
		}
	}
	else {
		glDisable( GL_LIGHT6 );
	}

	// Position a light that points in bottom right direction
	if (vp.light[7]) {
		GLfloat thisDistx = distx*(1.0+vp.lightDepth[7]);
		GLfloat thisDisty = disty*(1.0+vp.lightDepth[7]);
		gluUnProject( thisDistx, -thisDisty, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
		GLfloat norm = sqrt(wx*wx + wy*wy + wz*wz);
		if (norm > 0.0) {
			GLfloat light7_dir[] = { (GLfloat)wx/norm, (GLfloat)wy/norm, (GLfloat)wz/norm, 0.0};
			glLightfv( GL_LIGHT7, GL_POSITION, light7_dir );
			glLightfv( GL_LIGHT7, GL_DIFFUSE, light );
			glLightfv( GL_LIGHT7, GL_EMISSION, light );
			glLightfv( GL_LIGHT7, GL_SPECULAR, light );
			glEnable( GL_LIGHT7 );
		}
	}
	else {
		glDisable( GL_LIGHT7 );
	}
}


// Switch on or off antialiasing
void GLCanvasArea::antiAliasing( bool antiAlias )
{
	switch (antiAlias) {
		case TRUE:
			glEnable( GL_LINE_SMOOTH );
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
		break;
		case FALSE:
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_BLEND );
		break;
	}
}


// Draw a box indicating the contours of the data set
void GLCanvasArea::drawContourBox( void )
{
	// Set line width
	glLineWidth( 2.0 );

	// Choose a grey outline -- this is visible
	// on black and white backgrounds
	glColor3f(0.5f, 0.5f, 0.5f); 

	// Draw the outline now
	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymin, vo.zmin );
	glVertex3f( vo.xmax, vo.ymin, vo.zmin );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymin, vo.zmin );
	glVertex3f( vo.xmax, vo.ymax, vo.zmin );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymax, vo.zmin );
	glVertex3f( vo.xmin, vo.ymax, vo.zmin );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymax, vo.zmin );
	glVertex3f( vo.xmin, vo.ymin, vo.zmin );
	glEnd();

	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymin, vo.zmax );
	glVertex3f( vo.xmax, vo.ymin, vo.zmax );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymin, vo.zmax );
	glVertex3f( vo.xmax, vo.ymax, vo.zmax );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymax, vo.zmax );
	glVertex3f( vo.xmin, vo.ymax, vo.zmax );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymax, vo.zmax );
	glVertex3f( vo.xmin, vo.ymin, vo.zmax );
	glEnd();

    	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymin, vo.zmin );
	glVertex3f( vo.xmin, vo.ymin, vo.zmax );
	glEnd();
    	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymin, vo.zmin );
	glVertex3f( vo.xmax, vo.ymin, vo.zmax );
	glEnd();
    	glBegin( GL_LINES );
	glVertex3f( vo.xmax, vo.ymax, vo.zmin );
	glVertex3f( vo.xmax, vo.ymax, vo.zmax );
	glEnd();
    	glBegin( GL_LINES );
	glVertex3f( vo.xmin, vo.ymax, vo.zmin );
	glVertex3f( vo.xmin, vo.ymax, vo.zmax );
	glEnd();
}


// Draw three lines indicating the xyz axes
void GLCanvasArea::drawAxes( void )
{
	GLdouble mvmatrix[16], projmatrix[16];
	GLint viewport[4];
	GLdouble wx, wy, wz;
	GLdouble wx2, wy2, wz2;
	GLdouble dist;

	// Choose a color that is visible
	// on all backgrounds
	switch (vp.background) {
		case BGBLACK:
			glColor3f( yellow_color );
		break;
		case BGWHITE:
			glColor3f( darkyellow_color );
		break;
	}

	// Find the world position 
        glGetIntegerv( GL_VIEWPORT, viewport );
        glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
        glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

	// Draw the axes now in the lower left corner (40/40 pixels away)
	gluUnProject( (GLdouble)40, (double)40, 0.9, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

	// Find a second point to set the axes length
	gluUnProject( (GLdouble)70, (double)70, 0.9, mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);
	dist = sqrt( (wx-wx2)*(wx-wx2)+(wy-wy2)*(wy-wy2)+(wz-wz2)*(wz-wz2) );

	// Draw the axes now
	glLineWidth( 2.0 );
	glBegin( GL_LINES );
	glVertex3f( wx, wy, wz );
	glVertex3f( wx+dist, wy, wz );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( wx, wy, wz );
	glVertex3f( wx, wy+dist, wz );
	glEnd();
	glBegin( GL_LINES );
	glVertex3f( wx, wy, wz );
	glVertex3f( wx, wy, wz+dist );
	glEnd();
}


// Draw three labels indicating the xyz axes
void GLCanvasArea::drawAxesAnnotation( void )
{
	GLdouble mvmatrix[16], projmatrix[16];
	GLint viewport[4];
	GLdouble wx, wy, wz;
	GLdouble wx2, wy2, wz2;
	GLdouble oxX, oyX, ozX;
	GLdouble oxY, oyY, ozY;
	GLdouble oxZ, oyZ, ozZ;
	GLdouble dist, dist5;
	static GLuint myAxesFont = 0;

	// Compile a font if needed
	if (myAxesFont == 0) 
		myAxesFont = makeRasterFont( 0 );

	// Choose a color that is visible
	// on all backgrounds
	switch (vp.background) {
		case BGBLACK:
			glColor3f( yellow_color );
		break;
		case BGWHITE:
			glColor3f( darkyellow_color );
		break;
	}

	// Find the world position 
        glGetIntegerv( GL_VIEWPORT, viewport );
        glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
        glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

	// Find again the axis tips
	gluUnProject( (GLdouble)40, (double)40, 0.9, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
	// Find a second point to set the axes length
	gluUnProject( (GLdouble)70, (double)70, 0.9, mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);

	dist = sqrt( (wx-wx2)*(wx-wx2)+(wy-wy2)*(wy-wy2)+(wz-wz2)*(wz-wz2) );
	dist5 = dist/5.0;

	// Find the window coordinates of the axis tip 
	gluProject( wx+dist-dist5, wy, wz, mvmatrix, projmatrix, viewport, &oxX, &oyX, &ozX);
	gluProject( wx, wy+dist+dist5, wz, mvmatrix, projmatrix, viewport, &oxY, &oyY, &ozY);
	gluProject( wx, wy, wz+dist+dist5, mvmatrix, projmatrix, viewport, &oxZ, &oyZ, &ozZ);

	// Save original model matrix
	glPushMatrix();

	// Set up a coordinate system 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Set up a simple orthogonal coordinate system 
        gluOrtho2D( 0.0, (GLdouble)this->width(), 0.0, (GLdouble)this->height() );
	// Draw annotation for the coordinate system
	if (myAxesFont) {
		glRasterPos2d( oxX, oyX );
		printString("x", myAxesFont);
		glRasterPos2d( oxY, oyY );
		printString("y", myAxesFont);
		glRasterPos2d( oxZ, oyZ );
		printString("z", myAxesFont);
	}

	// Retrieve original model matrix
	glPopMatrix();
}


// Draw the annotation
void GLCanvasArea::drawAnnotation( void )
{
	static GLuint myAnnotationFont1 = 0;
	static GLuint myAnnotationFont2 = 0;
	static GLuint myAnnotationFont3 = 0;
	static GLuint myAnnotationFont4 = 0;
	GLuint myFont = 0;

	// Compile fonts if needed
	if (myAnnotationFont1 == 0) 
		myAnnotationFont1 = makeRasterFont( 1 );
	if (myAnnotationFont2 == 0) 
		myAnnotationFont2 = makeRasterFont( 2 );
	if (myAnnotationFont3 == 0) 
		myAnnotationFont3 = makeRasterFont( 3 );
	if (myAnnotationFont4 == 0) 
		myAnnotationFont4 = makeRasterFont( 4 );

	// Choose a color
	switch (vp.background) {
		case BGBLACK:
			glColor3f( white_color );
		break;
		case BGWHITE:
			glColor3f( black_color );
		break;
	}

	switch ( vp.annotationSize) {
		case 1: 
			myFont = myAnnotationFont1;
		break;
		case 2: 
			myFont = myAnnotationFont2;
		break;
		case 3: 
			myFont = myAnnotationFont3;
		break;
		case 4: 
			myFont = myAnnotationFont4;
		break;
		default:
			myFont = myAnnotationFont1;
		break;
	}
		
	// Save original model matrix
	glPushMatrix();

	// Set up a coordinate system 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Set up a simple orthogonal coordinate system 
        gluOrtho2D( 0.0, (GLdouble)this->width(), 0.0, (GLdouble)this->height() );
	// Print now
	glRasterPos2d( vp.annotationCoordX, vp.annotationCoordY );
	printString( vp.annotationText, myFont);

	// Retrieve original model matrix
	glPopMatrix();
}


// Draw a string on the drawable 
void GLCanvasArea::printString(char *string, GLuint thisFont )
{
        glListBase( thisFont );
	glPushAttrib( GL_LIST_BIT );
        glCallLists( strlen(string), GL_UNSIGNED_BYTE, (GLubyte *)string );
        glPopAttrib();
}


// Draw slicing planes
void GLCanvasArea::drawSlicingPlanes( void )
{
	float xmin = vo.xmin;
	float xmax = vo.xmax;
	float ymin = vo.ymin;
	float ymax = vo.ymax;
	float zmin = vo.zmin;
	float zmax = vo.zmax;

	// Choose white transparent color
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, slice_mat );

	if (fabs(xmax-xmin) < 1.0) {
		xmin -= 0.5;
		xmax += 0.5;
	}
	if (fabs(ymax-ymin) < 1.0) {
		ymin -= 0.5;
		ymax += 0.5;
	}
	if (fabs(zmax-zmin) < 1.0) {
		zmin -= 0.5;
		zmax += 0.5;
	}

	// Must the x-planes be rendered?
	if (vp.showSlicePlaneX) {
		// Yes
		glBegin( GL_POLYGON );
		glNormal3f( -1.0, 0.0, 0.0 );
		glVertex3f( vp.sliceXMin, ymin, zmin );
		glVertex3f( vp.sliceXMin, ymax, zmin );
		glVertex3f( vp.sliceXMin, ymax, zmax );
		glVertex3f( vp.sliceXMin, ymin, zmax );
		glEnd();
		glBegin( GL_POLYGON );
		glNormal3f( 1.0, 0.0, 0.0 );
		glVertex3f( vp.sliceXMax, ymin, zmin );
		glVertex3f( vp.sliceXMax, ymax, zmin );
		glVertex3f( vp.sliceXMax, ymax, zmax );
		glVertex3f( vp.sliceXMax, ymin, zmax );
		glEnd();
	}

	// Must the y-planes be rendered?
	if (vp.showSlicePlaneY) {
		// Yes
		glBegin( GL_POLYGON );
		glNormal3f( 0.0, -1.0, 0.0 );
		glVertex3f( xmin, vp.sliceYMin, zmin );
		glVertex3f( xmax, vp.sliceYMin, zmin );
		glVertex3f( xmax, vp.sliceYMin, zmax );
		glVertex3f( xmin, vp.sliceYMin, zmax );
		glEnd();
		glBegin( GL_POLYGON );
		glNormal3f( 0.0, 1.0, 0.0 );
		glVertex3f( xmin, vp.sliceYMax, zmin );
		glVertex3f( xmax, vp.sliceYMax, zmin );
		glVertex3f( xmax, vp.sliceYMax, zmax );
		glVertex3f( xmin, vp.sliceYMax, zmax );
		glEnd();
	}

	// Must the z-planes be rendered?
	if (vp.showSlicePlaneZ) {
		// Yes	
		glBegin( GL_POLYGON );
		glNormal3f( 0.0, 0.0, -1.0 );
		glVertex3f( xmin, ymin, vp.sliceZMin );
		glVertex3f( xmax, ymin, vp.sliceZMin );
		glVertex3f( xmax, ymax, vp.sliceZMin );
		glVertex3f( xmin, ymax, vp.sliceZMin );
		glEnd();
		glBegin( GL_POLYGON );
		glNormal3f( 0.0, 0.0, 1.0 );
		glVertex3f( xmin, ymin, vp.sliceZMax );
		glVertex3f( xmax, ymin, vp.sliceZMax );
		glVertex3f( xmax, ymax, vp.sliceZMax );
		glVertex3f( xmin, ymax, vp.sliceZMax );
		glEnd();
	}

	// Use opaque material from now
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, opaque_mat );
	glDisable( GL_BLEND );
}


// Find out if the scene is simple, involving only lines and dots, 
// or complex
bool GLCanvasArea::simpleRendering( void )
{
	bool simple = FALSE;

	if (vp.renderMode == ATOMS && vp.atomRenderStyle == ADOT)
		simple = TRUE;
	if (vp.renderMode == SPINS && vp.spinRenderStyle == SLINE)
		simple = TRUE;
	if (vp.renderMode == SPINS && vp.lcRenderStyle == LLINE)
		simple = TRUE;
	if (vp.renderMode == LIQUID_CRYSTALS && vp.lcRenderStyle == LDOT)
		simple = TRUE;
	if (vp.renderMode == LIQUID_CRYSTALS && vp.lcRenderStyle == LLINE)
		simple = TRUE;
	if (vp.renderMode == PORES && vp.poreRenderStyle == PLINE)
		simple = TRUE;
	if (vp.renderMode == PORES && vp.poreRenderStyle == PDOT)
		simple = TRUE;

	return simple;
}


// This is used to render text on the drawable
GLuint GLCanvasArea::makeRasterFont( int size )
{
        XFontStruct *fontInfo;
        Font id;
        unsigned int first, last;
        Display *xdisplay = QPaintDevice::x11AppDisplay();
	GLuint thisFont = 0;

	switch (size) { 
		case 0:
			fontInfo = XLoadQueryFont(xdisplay, "7x14");
		break;
		case 1:
			fontInfo = XLoadQueryFont(xdisplay, "6x10");
		break;
		case 2:
			fontInfo = XLoadQueryFont(xdisplay, "6x13");
		break;
		case 3:
			fontInfo = XLoadQueryFont(xdisplay, "8x13");
		break;
		case 4:
			fontInfo = XLoadQueryFont(xdisplay, "9x15");
		break;
		default:
			fontInfo = XLoadQueryFont(xdisplay, "6x13"); // DanP
		break;
	}

	if (fontInfo == NULL) {
        	printf ("Error: No font found\n");
	}
	else { 
	        id = fontInfo->fid;
	        first = fontInfo->min_char_or_byte2;
	        last = fontInfo->max_char_or_byte2;

	        thisFont = glGenLists((GLuint) last+1);

	        glXUseXFont(id, first, last-first+1, thisFont+first);
	        XFreeFont(xdisplay, fontInfo);
	}

        return thisFont;
}


// Generate an OpenGL display list for the object to be shown 
// (here a set of bonds)
GLuint GLCanvasArea::makeBondsObject( bool onlyWithinMolecule )
{
	double dCurrentBondLength;
	bool isInList,isSeqBond;
	int i, combCount = 0;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	tag * fParticleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*BONDS_MAX*sizeof(tag));
	tag * tParticleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*BONDS_MAX*sizeof(tag));
	tag neighborParticleType;
	float minLength, maxLength; 
	fixedColor color;
	fixedThickness thickness;
	GLfloat emission;
	GLdouble radius = (GLdouble)(0.05*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble relRadius, thisRadius; 
	GLint qual;
	GLUquadricObj * qobj;

	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Prepare rendering of cylinders
	qobj = gluNewQuadric( );
	gluQuadricNormals( qobj, (GLenum)GLU_SMOOTH );
	gluQuadricDrawStyle (qobj, (GLenum)GLU_FILL);

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j = numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and find the
	// bonding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Look at all neighbor types
		for (int j=0;j<pd.particleBonds[pdIndex].numberOfNeighborBonds;j++) {
			typeCopy( (char *)&pd.particleBonds[pdIndex].neighborParticleType[j], (char *)&neighborParticleType);

			// Is this a new combination of bonds?  And 
			// if yes, should the bond be rendered?
			if (bondCombinationNew( fParticleTypes, tParticleTypes, &combCount, (char *)&particleTypes[i], (char *)&neighborParticleType) == TRUE && pd.particleBonds[pdIndex].bShowBond[j] == TRUE) {
				emission = (GLfloat)pd.particleBonds[pdIndex].bEmission[j];
				minLength = pd.particleBonds[pdIndex].bMinLength[j];
				maxLength = pd.particleBonds[pdIndex].bMaxLength[j];
				isSeqBond = pd.particleBonds[pdIndex].bSeqBond[j];
				color = pd.particleBonds[pdIndex].bColor[j];
				thickness = pd.particleBonds[pdIndex].bThickness[j];
				if (pd.particleBonds[pdIndex].bAntiAlias[j]) {
					this->antiAliasing( TRUE );
				}
				else {
					this->antiAliasing( FALSE );
				}

				// Go through all particle types and look at 
				// bonding neighbors; draw the corresponding 
				// bonds.  In case of polymers, there is 
				// an extra constraint: both bonding partners
				// must belong to the same atom
				for (int k=0;k<ad.numberOfParticles;k++) {
	                		for (int l=k+1;l<ad.numberOfParticles;l++) {
						if (this->particleIsInSlice(k) && this->particleIsInSlice(l)) {
							// Draw bonds from 
							// target to 
							// destination
							// and from 
							// destination to 
							// target
		                                	if ( (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[k].type ) == TRUE && typeCmp( (char *)&neighborParticleType, (char *)&ad.particles[l].type ) == TRUE) || (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[l].type ) == TRUE && typeCmp( (char *)&neighborParticleType, (char *)&ad.particles[k].type ) == TRUE) ) {
								if (this->isBond(k, l, minLength, maxLength, onlyWithinMolecule,isSeqBond, &dCurrentBondLength) == TRUE) {

									// Set 
									// color, 
									// quality 
									// and 
									// thickness 
									// for 
									// this 
									// bond
									this->setBondProperty( color, thickness, emission, &relRadius, &qual );	

									double dLengthPercent = (dCurrentBondLength - minLength) / (maxLength - minLength);
									double minBondDrawingRadius = 0;
									double maxBondDrawingRadius = 3;

									// If the bond type is VARYING, vary the bond's width (radius) according to the bonds' length
									if (thickness == VARYING_LINEAR)
									{
										relRadius = minBondDrawingRadius + (1 - dLengthPercent) * maxBondDrawingRadius;
									}
									if (thickness == VARYING_QUADRATIC)
									{
										relRadius = minBondDrawingRadius + ((dLengthPercent - 1) * (dLengthPercent - 1)) * maxBondDrawingRadius;
									}
									thisRadius = radius*relRadius;

									// Render the bond now as a cylinder (which
									// becomes a line for low quality)
									this->renderBondCylinder( k, l, qobj, qual, thisRadius, thickness );
								}
							}
						}
					}
				}
			}
		}		
	}		

	// Activate lighting in any case
	glEnable( GL_LIGHTING );

	// Switch off antialiasing
	this->antiAliasing( FALSE );

	// No more glowing from now on
	glMaterialfv( GL_FRONT, GL_EMISSION, black_mat );

        gluDeleteQuadric(qobj);

	glEndList();
	
	free(particleTypes);
	free(fParticleTypes);
	free(tParticleTypes);

	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here dots indicating positions in the data set)
GLuint GLCanvasArea::makeDotsObject( void )
{
	bool isInList;
	int i; 
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	
	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j=numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {
		
		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of atom colors
		this->setTypeColor( pdIndex );
	
		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {
				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right 
					// type: render it
					this->renderDot(j, pdIndex);
				}
			}
		}				
	}				
	
	glEndList();

	free(particleTypes);

	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here spheres indicating positions in the data set)
GLuint GLCanvasArea::makeSpheresObject( void )
{
	bool isInList;
	int i;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	relativeSize relSize = REGULAR;
	GLint qual = 3; 
	GLUquadricObj * qobj;
	
	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Prepare rendering of spheres
	qobj = gluNewQuadric( );
	gluQuadricNormals( qobj, (GLenum)GLU_SMOOTH );
	gluQuadricDrawStyle (qobj, (GLenum)GLU_FILL);

	// Set the render quality
	switch (vp.renderQuality) {
		case FINAL:
			qual = 20;
			if (ad.numberOfParticles > MANY_ATOMS) 
				qual = 15;
			if (ad.numberOfParticles > VERY_MANY_ATOMS) 
				qual = 12;
			if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS) 
				qual = 10;
		break;
		case HIGH:
			qual = 10;
			if (ad.numberOfParticles > MANY_ATOMS) 
				qual = 9;
			if (ad.numberOfParticles > VERY_MANY_ATOMS) 
				qual = 6;
			if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS) 
				qual = 5;
		break;
		case LOW:
			qual = 3;
		break;
	}

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j=numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of atom colors
		this->setTypeColor( pdIndex );
	
		// Choose the relative size from the list of rel sizes
		this->setTypeRelSize( pdIndex, &relSize );

		// Draw the particles that are of the given type. 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {

				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: 
					// render it
					this->renderSphere( j, pdIndex, qobj, qual, relSize );
				}	
			}
		}
	}

	gluDeleteQuadric(qobj);

	glEndList();

	free(particleTypes);

	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here cubes indicating the atom positions in the data set)
GLuint GLCanvasArea::makeCubesObject( void )
{
	bool isInList;
	int i;
	relativeSize relSize = REGULAR;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	
	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j=numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of atom colors
		this->setTypeColor( pdIndex );
	
		// Choose the relative size from the list of rel sizes
		this->setTypeRelSize( pdIndex, &relSize );

		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {

				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: render it
					this->renderCube( j, pdIndex, relSize );
				}
			}
		}				
	}				
	
	glEndList();

	free(particleTypes);

	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here pins indicating positions and directions in the data set)
GLuint GLCanvasArea::makePinsObject( void )
{
	bool isInList;
	int i;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	relativeSize relSize = REGULAR;
	fixedThickness thickness;
	GLfloat posx, posy, posz; 
	GLfloat p1, p2, p3, p4, p5, p6, p7, p8;
	GLfloat n, n1, n2, n3;
	GLfloat emission;
	GLdouble radius = (GLdouble)(0.05*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble length = (GLdouble)(0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble relRadius, thisRadius, thisLen;
	GLint qual = 3;
	GLUquadricObj * qobj;

	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Prepare rendering of spheres
	qobj = gluNewQuadric( );
	gluQuadricNormals( qobj, (GLenum)GLU_SMOOTH );
	gluQuadricDrawStyle (qobj, (GLenum)GLU_FILL);

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j = numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );
		
		// Choose a color from the list of colors
		this->setTypeColor( pdIndex );

		// Choose the relative size from the list of rel sizes
		this->setTypeRelSize( pdIndex, &relSize );

		// Set parameters for the rendering of the spin foot
		emission = (GLfloat)pd.line[pdIndex].fEmission;
		thickness = pd.line[pdIndex].fThickness;
		if (pd.line[pdIndex].fAntiAlias) {
			this->antiAliasing( TRUE );
		}
		else {
			this->antiAliasing( FALSE );
		}

		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {

				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: render it
					posx = ad.particles[j].x*vp.stretchX;
					posy = ad.particles[j].y*vp.stretchY;
					posz = ad.particles[j].z*vp.stretchZ;
					p1 = ad.particles[j].prop1;
					p2 = ad.particles[j].prop2;
					p3 = ad.particles[j].prop3;
					p4 = ad.particles[j].prop4;
					p5 = ad.particles[j].prop5;
					p6 = ad.particles[j].prop6;
					p7 = ad.particles[j].prop7;
					p8 = ad.particles[j].prop8;

					// Set a different color if coloring 
					// is not according to type.
					if (pd.colorCrit[pdIndex] == POSITION) 
						this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchZ, posz/vp.stretchZ, pdIndex); 
					if (pd.colorCrit[pdIndex] == PROPERTY) 
						this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
					if (pd.colorCrit[pdIndex] == COLORCODE) 
						this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

					// Set quality and thickness
					// for this spin foot
					this->setLineFootProperty( thickness, emission, &relRadius, &qual );	
					thisRadius = radius*relRadius/2.0;

					// The spin vector is given by the 
					// first 3 property entries
					n = (GLfloat)sqrt( p1*p1 + p2*p2 + p3*p3 );
	
					// Rather than using a unit vector
					// length, use the spin data to 
					// modify the length (assuming that
					// the spins are already properly
					// normalized)
					thisLen = length*n;

					if (fabs(n) > 0.0) {
						n1 = p1/n;
						n2 = p2/n;
						n3 = p3/n;
					}
					else {
						n1 = 1.0;
						n2 = 0.0;
						n3 = 0.0;
					}

					// Render the body first
					this->renderLineCylinder( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE, FALSE );

					// Switch on lighting, in case it
					// was switched off while rendering
					// lines 
					glEnable( GL_LIGHTING );	

					// No more glowing for the tip
					GLfloat foot_mat[] = { 0.0, 0.0, 0.0, 1.0 };
					glMaterialfv( GL_FRONT, GL_EMISSION, foot_mat );

					// Now the tip: either a line or a 
					// sphere or a cone.  Modify size of spin tip 
					// if required
					switch (relSize) {
						case REGULAR:
							thisRadius = radius;
						break;
						case TINY:
							thisRadius = radius/RELSIZE_FACTOR/RELSIZE_FACTOR;
						break;
						case SMALL:
							thisRadius = radius/RELSIZE_FACTOR;
						break;
						case LARGE:
							thisRadius = radius*RELSIZE_FACTOR;
						break;
						case HHUGE:
							thisRadius = radius*RELSIZE_FACTOR*RELSIZE_FACTOR;
						break;
						default:
							thisRadius = radius;
						break;
					}

					this->renderSpinTip( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius );
				}
			}
		}	
	}

        gluDeleteQuadric(qobj);

	glEndList();

	free(particleTypes);
				
	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here lines indicating positions and directions in the data set)
GLuint GLCanvasArea::makeLinesObject( bool adjustLength )
{
	bool isInList;
	int i;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	fixedThickness thickness = LINE;
	GLfloat red, green, blue = 0.0;
	GLfloat redTwo, greenTwo, blueTwo = 0.0;
	GLfloat redThree, greenThree, blueThree = 0.0;
	GLfloat posx, posy, posz; 
	GLfloat p1, p2, p3, p4, p5, p6, p7, p8;
	GLfloat n, n1, n2, n3;
	GLfloat emission;
	GLdouble radius = (GLdouble)(0.05*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble length = (GLdouble)(0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble relRadius, thisRadius, thisLen = 1.0;
	GLint qual = 3;

	GLuint list = glGenLists( 1 );

	glNewList( list, GL_COMPILE );

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j = numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}
	
	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of multi-colors
		this->setTypeColor( &red, &green, &blue, &redTwo, &greenTwo, &blueTwo, &redThree, &greenThree, &blueThree, pdIndex );

		// Set parameters for the rendering of the foot
		emission = (GLfloat)pd.line[pdIndex].fEmission;
		thickness = pd.line[pdIndex].fThickness;
		if (pd.line[pdIndex].fAntiAlias) {
			this->antiAliasing( TRUE );
		}
		else {
			this->antiAliasing( FALSE );
		}

		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {
				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: render it
					posx = ad.particles[j].x*vp.stretchX;
					posy = ad.particles[j].y*vp.stretchY;
					posz = ad.particles[j].z*vp.stretchZ;
					p1 = ad.particles[j].prop1;
					p2 = ad.particles[j].prop2;
					p3 = ad.particles[j].prop3;
					p4 = ad.particles[j].prop4;
					p5 = ad.particles[j].prop5;
					p6 = ad.particles[j].prop6;
					p7 = ad.particles[j].prop7;
					p8 = ad.particles[j].prop8;

					// Set a different color if coloring 
					// is not according to type.
					if (pd.colorCrit[pdIndex] == POSITION) 
						this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchZ, posz/vp.stretchZ, pdIndex); 
					if (pd.colorCrit[pdIndex] == PROPERTY) 
						this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
					if (pd.colorCrit[pdIndex] == COLORCODE) 
						this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

					// Set quality and thickness
					// for this liquid crystal foot
					this->setLineFootProperty( thickness, emission, &relRadius, &qual );	
					thisRadius = radius*relRadius/2.0;

					// The liquid crystal vector is given by the 
					// first 3 property entries
					n = (GLfloat)sqrt( p1*p1 + p2*p2 + p3*p3 );
	
					switch (adjustLength) {
						case FALSE:
							thisLen = n;
						break;
						case TRUE:
							// Rather than using a unit vector
							// length, use the spin data to 
							// modify the length (assuming that
							// the spins are already properly
							// normalized)
							thisLen = length*n;
						break;
					}

					if (fabs(n) > 0.0) {
						n1 = p1/n;
						n2 = p2/n;
						n3 = p3/n;
					}
					else {
						n1 = 1.0;
						n2 = 0.0;
						n3 = 0.0;
					}

					// Here, render only the line foot.
					// Divide it into several small bites 
					// when neccessary
					if (pd.colorCrit[pdIndex] != TYPE) {
						this->renderLineCylinder( posx, posy, posz, n1, n2, n3, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
					}
					else {
						// Coloring according to types: 
						// Use one, two, or three colors
						switch (pd.line[pdIndex].typeColorNumber) {	
							case 1:
								this->renderLineCylinder( posx, posy, posz, n1, n2, n3, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
							break;
							case 2:
								glColor3f( red, green, blue);
								this->renderLineCylinder( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
								posx += n1*thisLen/2.0; 
								posy += n2*thisLen/2.0; 
								posz += n3*thisLen/2.0;
	
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderLineCylinder( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
							break;
							case 3:
								glColor3f( red, green, blue);
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redThree, greenThree, blueThree );
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, NULL, qual, thisRadius, REGULAR, LINE, FALSE, FALSE );
							break;
						}
					}							
				}	
			}
		}	
	}

	glEndList();

	free(particleTypes);
				
	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here cones indicating the liquid crystal positions in the data set)
GLuint GLCanvasArea::makeConesObject( bool adjustLength )
{
	bool isInList;
	int i;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	relativeSize relSize = REGULAR;
	fixedThickness thickness;
	GLfloat red, green, blue;
	GLfloat redTwo, greenTwo, blueTwo;
	GLfloat redThree, greenThree, blueThree;
	GLfloat posx, posy, posz; 
	GLfloat p1, p2, p3, p4, p5, p6, p7, p8;
	GLfloat n, n1, n2, n3;
	GLfloat emission;
	GLdouble radius = (GLdouble)(0.05*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble length = (GLdouble)(0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble relRadius, thisRadius, thisLen = 1.0;
	GLint qual = 3;
	GLUquadricObj * qobj;

	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Prepare rendering of spheres
	qobj = gluNewQuadric( );
	gluQuadricNormals( qobj, (GLenum)GLU_SMOOTH );
	gluQuadricDrawStyle (qobj, (GLenum)GLU_FILL);

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j = numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of multi-colors
		this->setTypeColor( &red, &green, &blue, &redTwo, &greenTwo, &blueTwo, &redThree, &greenThree, &blueThree, pdIndex );

		// Choose the relative size from the list of rel sizes
		this->setTypeRelSize( pdIndex, &relSize );

		// Set parameters for the rendering of the liquid crystal foot
		emission = (GLfloat)pd.line[pdIndex].fEmission;
		thickness = pd.line[pdIndex].fThickness;
		if (pd.line[pdIndex].fAntiAlias) {
			this->antiAliasing( TRUE );
		}
		else {
			this->antiAliasing( FALSE );
		}

		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {
				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: render it
					posx = ad.particles[j].x*vp.stretchX;
					posy = ad.particles[j].y*vp.stretchY;
					posz = ad.particles[j].z*vp.stretchZ;
					p1 = ad.particles[j].prop1;
					p2 = ad.particles[j].prop2;
					p3 = ad.particles[j].prop3;
					p4 = ad.particles[j].prop4;
					p5 = ad.particles[j].prop5;
					p6 = ad.particles[j].prop6;
					p7 = ad.particles[j].prop7;
					p8 = ad.particles[j].prop8;

					// Set a different color if coloring 
					// is not according to type.
					if (pd.colorCrit[pdIndex] == POSITION) 
						this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchZ, posz/vp.stretchZ, pdIndex); 
					if (pd.colorCrit[pdIndex] == PROPERTY) 
						this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
					if (pd.colorCrit[pdIndex] == COLORCODE) 
						this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

					// Set thickness and quality
					// for this liquid crystal foot
					this->setLineFootProperty( thickness, emission, &relRadius, &qual );	
					thisRadius = radius*relRadius/2.0;

					// The liquid crystal vector 
					// is given by the 
					// first 3 property entries
					n = (GLfloat)sqrt( p1*p1 + p2*p2 + p3*p3 );
					switch (adjustLength) {
						case FALSE:
							thisLen = n;
						break;
						case TRUE:
							// Rather than using a unit vector
							// length, use the spin data to 
							// modify the length (assuming that
							// the spins are already properly
							// normalized)
							thisLen = length*n;
						break;
					}

					if (fabs(n) > 0.0) {
						n1 = p1/n;
						n2 = p2/n;
						n3 = p3/n;
					}
					else {
						n1 = 1.0;
						n2 = 0.0;
						n3 = 0.0;
					}

					// Render only a cone
					// Divide it into several small bites when
					// neccessary
					if (pd.colorCrit[pdIndex] != TYPE) {
						this->renderCone( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE );
					}
					else {
						// Coloring according to types: 
						// Use one, two, or three colors
						switch (pd.line[pdIndex].typeColorNumber) {	
							case 1:
								this->renderCone( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE );
							break;
							case 2:
								glColor3f( red, green, blue);
								this->renderCone( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, qobj, qual, thisRadius, thisRadius/2.0, relSize, thickness, TRUE );
								posx += n1*thisLen/2.0; 
								posy += n2*thisLen/2.0; 
								posz += n3*thisLen/2.0;
	
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderCone( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, qobj, qual, thisRadius/2.0, relSize, thickness, FALSE );
							break;
							case 3:
								glColor3f( red, green, blue);
								this->renderCone( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius, thisRadius*2.0/3.0, relSize, thickness, TRUE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderCone( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius*2.0/3.0, thisRadius/3.0, relSize, thickness, FALSE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redThree, greenThree, blueThree );
								this->renderCone( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius/3.0, relSize, thickness, FALSE );
							break;
						}
					}
				}
			}
		}	
	}

        gluDeleteQuadric(qobj);

	glEndList();

	free(particleTypes);
				
	return list;
}


// Generate an OpenGL display list for the object to be shown 
// (here cylinders indicating positions and directions in the data set)
GLuint GLCanvasArea::makeCylindersObject( bool adjustLength )
{
	bool isInList;
	int i;
	tag * particleTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	relativeSize relSize = REGULAR;
	fixedThickness thickness;
	GLfloat red, green, blue;
	GLfloat redTwo, greenTwo, blueTwo;
	GLfloat redThree, greenThree, blueThree;
	GLfloat posx, posy, posz; 
	GLfloat p1, p2, p3, p4, p5, p6, p7, p8;
	GLfloat n, n1, n2, n3;
	GLfloat emission;
	GLdouble radius = (GLdouble)(0.05*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble length = (GLdouble)(0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble relRadius, thisRadius, thisLen = 1.0;
	GLint qual = 3;
	GLUquadricObj * qobj;

	GLuint list = glGenLists( 1 );

    	glNewList( list, GL_COMPILE );

	// Prepare rendering of spheres
	qobj = gluNewQuadric( );
	gluQuadricNormals( qobj, (GLenum)GLU_SMOOTH );
	gluQuadricDrawStyle (qobj, (GLenum)GLU_FILL);

	// Go through all the particles and make a list of
	// known types
	int numberOfParticleTypes = 0;
	for (i=0;i<ad.numberOfParticles;i++) {
		if (this->particleIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfParticleTypes;j++) {
				if (typeCmp( (char *)&particleTypes[j], (char *)&ad.particles[i].type ) == TRUE) {
					isInList = TRUE;
					j = numberOfParticleTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new particle -- add it to the list
				if (numberOfParticleTypes < MAX_PARTICLE_TYPES) {
					typeCopy( (char *)&ad.particles[i].type, (char *)&particleTypes[numberOfParticleTypes]);
					numberOfParticleTypes++;
				}
			}
		}
	}

	// Go through all the known particle types and draw the
	// corresponding particles
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	for (i=0;i<numberOfParticleTypes;i++) {

		// Find the correct entry in the list of particle types
		int pdIndex = findPdListIndex( (char *)&particleTypes[i] );

		// Choose a color from the list of multi-colors
		this->setTypeColor( &red, &green, &blue, &redTwo, &greenTwo, &blueTwo, &redThree, &greenThree, &blueThree, pdIndex );

		// Choose the relative size from the list of rel sizes
		this->setTypeRelSize( pdIndex, &relSize );

		// Set parameters for the rendering of the liquid crystal foot
		emission = (GLfloat)pd.line[pdIndex].fEmission;
		thickness = pd.line[pdIndex].fThickness;
		if (pd.line[pdIndex].fAntiAlias) {
			this->antiAliasing( TRUE );
		}
		else {
			this->antiAliasing( FALSE );
		}

		// Draw the particles that are of the given type 
		for (int j=0;j<ad.numberOfParticles;j++) {
			if (this->particleIsInSlice(j) && pd.showParticle[pdIndex]) {
				if (typeCmp( (char *)&particleTypes[i], (char *)&ad.particles[j].type ) == TRUE) {
					// This one is of the right type: 
					// render it
					posx = ad.particles[j].x*vp.stretchX;
					posy = ad.particles[j].y*vp.stretchY;
					posz = ad.particles[j].z*vp.stretchZ;
					p1 = ad.particles[j].prop1;
					p2 = ad.particles[j].prop2;
					p3 = ad.particles[j].prop3;
					p4 = ad.particles[j].prop4;
					p5 = ad.particles[j].prop5;
					p6 = ad.particles[j].prop6;
					p7 = ad.particles[j].prop7;
					p8 = ad.particles[j].prop8;

					// Set a different color if coloring 
					// is not according to type.
					if (pd.colorCrit[pdIndex] == POSITION) 
						this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchZ, posz/vp.stretchZ, pdIndex); 
					if (pd.colorCrit[pdIndex] == PROPERTY) 
						this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
					if (pd.colorCrit[pdIndex] == COLORCODE) 
						this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

					// Set thickness and quality
					// for this liquid crystal foot
					this->setLineFootProperty( thickness, emission, &relRadius, &qual );	
					thisRadius = radius*relRadius/2.0;

					// The liquid crystal vector is 
					// given by the 
					// first 3 property entries
					n = (GLfloat)sqrt( p1*p1 + p2*p2 + p3*p3 );
					switch (adjustLength) {
						case FALSE:
							thisLen = n;
						break;
						case TRUE:
							// Rather than using a unit vector
							// length, use the spin data to 
							// modify the length (assuming that
							// the spins are already properly
							// normalized)
							thisLen = length*n;
						break;
					}

					if (fabs(n) > 0.0) {
						n1 = p1/n;
						n2 = p2/n;
						n3 = p3/n;
					}
					else {
						n1 = 1.0;
						n2 = 0.0;
						n3 = 0.0;
					}

					// Render only a cylinder
					// Divide it into several small bites 
					// when neccessary
					if (pd.colorCrit[pdIndex] != TYPE) {
						this->renderLineCylinder( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE, TRUE );
					}
					else {
						// Coloring according to types: 
						// Use one, two, or three colors
						switch (pd.line[pdIndex].typeColorNumber) {	
							case 1:
								this->renderLineCylinder( posx, posy, posz, n1, n2, n3, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE, TRUE );
							break;
							case 2:
								glColor3f( red, green, blue);
								this->renderLineCylinder( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE, FALSE );
								posx += n1*thisLen/2.0; 
								posy += n2*thisLen/2.0; 
								posz += n3*thisLen/2.0;
	
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderLineCylinder( posx, posy, posz, n1/2.0, n2/2.0, n3/2.0, thisLen, qobj, qual, thisRadius, relSize, thickness, FALSE, TRUE );
							break;
							case 3:
								glColor3f( red, green, blue);
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius, relSize, thickness, TRUE, FALSE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redTwo, greenTwo, blueTwo );
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius, relSize, thickness, FALSE, FALSE );
								posx += n1*thisLen/3.0; 
								posy += n2*thisLen/3.0; 
								posz += n3*thisLen/3.0;
								glColor3f( redThree, greenThree, blueThree );
								this->renderLineCylinder( posx, posy, posz, n1/3.0, n2/3.0, n3/3.0, thisLen, qobj, qual, thisRadius, relSize, thickness, FALSE, TRUE );
							break;
						}
					}
				}
			}
		}	
	}

        gluDeleteQuadric(qobj);

	glEndList();

	free(particleTypes);
				
	return list;
}


// Return a pointer to the track data structure currently used in the
// rendering
trackData * GLCanvasArea::getTrackData( void )
{
	return &td;
}


// Generate an OpenGL display list for the object to be shown 
// (here lines indicating particle tracks the data set)
GLuint GLCanvasArea::makeTracksObject( void )
{
	bool isInList;
	int i;
	tag * trackTypes = (tag *)malloc(MAX_PARTICLE_TYPES*sizeof(tag));
	GLuint list = glGenLists( 1 );

	glNewList( list, GL_COMPILE );

	// Go through all the tracks and make a list of
	// known types
	int numberOfTrackTypes = 0;
	for (i=0;i<td.numberOfTracks;i++) {
		if (this->trackIsInSlice(i)) {
			isInList = FALSE;
			for (int j=0;j<numberOfTrackTypes;j++) {
				if (typeCmp( (char *)&trackTypes[j], (char *)&td.type[i] ) == TRUE) {
					isInList = TRUE;
					j = numberOfTrackTypes;
				}
			}
			if (isInList == FALSE) {
				// Found a new track -- add it to the list
				if (numberOfTrackTypes < MAX_TRACK_TYPES) {
					typeCopy( (char *)&td.type[i], (char *)&trackTypes[numberOfTrackTypes]);
					numberOfTrackTypes++;
				}
			}
		}
	}

	// Set a color
	glColor3f( track_color );

	// Switch off lighting 
	glDisable( GL_LIGHTING );	

	// Go through all the known track types and draw the
	// corresponding tracks
	for (i=0;i<numberOfTrackTypes;i++) {

		// Draw the tracks that are of the given type 
		for (int j=0;j<td.numberOfTracks;j++) {
			if (typeCmp( (char *)&trackTypes[i], (char *)&td.type[j]) == TRUE) {
				// This one is of the right type: 
				// render it if required
				int pdIndex = findPdListIndex( (char *)&trackTypes[i] );
				if (this->trackIsInSlice(j) && pd.showTrack[pdIndex]) {
					glBegin( GL_LINES );
					for (int stage=vp.trackRenderFromStage;stage<=vp.trackRenderToStage;stage++) {

						GLfloat posx = td.x[j][stage];
						GLfloat posy = td.y[j][stage];
						GLfloat posz = td.z[j][stage];

						// Take periodic boundary 
						// conditions into account
						if (vp.periodicBCTracks && stage > vp.trackRenderFromStage) {
							int stage0 = vp.trackRenderFromStage;

							if (posx-td.x[j][stage0] > (vo.xmax-vo.xmin)/2.0) 
								posx -= (vo.xmax-vo.xmin);
							if (posx-td.x[j][stage0] < -(vo.xmax-vo.xmin)/2.0) 
								posx += (vo.xmax-vo.xmin);
							if (posy-td.y[j][stage0] > (vo.ymax-vo.ymin)/2.0)
								posy -= (vo.ymax-vo.ymin);
							if (posy-td.y[j][stage0] < -(vo.ymax-vo.ymin)/2.0) 
								posy += (vo.ymax-vo.ymin);
							if (posz-td.z[j][stage0] > (vo.zmax-vo.zmin)/2.0) 
								posz -= (vo.zmax-vo.zmin);
							if (posz-td.z[j][stage0] < -(vo.zmax-vo.zmin)/2.0)
								posz += (vo.zmax-vo.zmin);
						}
						glVertex3f( posx, posy, posz );
					}
					glEnd();
				}
			}
		}
	}

	glEndList();

	free(trackTypes);

       	return list;
}


// Render a dot 
void GLCanvasArea::renderDot( int j, int pdIndex )
{
	float posx = ad.particles[j].x*vp.stretchX;
	float posy = ad.particles[j].y*vp.stretchY;
	float posz = ad.particles[j].z*vp.stretchZ;
	float p1 = ad.particles[j].prop1;
	float p2 = ad.particles[j].prop2;
	float p3 = ad.particles[j].prop3;
	float p4 = ad.particles[j].prop4;
	float p5 = ad.particles[j].prop5;
	float p6 = ad.particles[j].prop6;
	float p7 = ad.particles[j].prop7;
	float p8 = ad.particles[j].prop8;

	// Set a different color if coloring 
	// is not according to type.
	if (pd.colorCrit[pdIndex] == POSITION) 
		this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchY, posz/vp.stretchZ, pdIndex ); 
	if (pd.colorCrit[pdIndex] == PROPERTY) 
		this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
	if (pd.colorCrit[pdIndex] == COLORCODE) 
		this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

	// Here a dot is rendered
	glBegin( GL_POINTS );
		glVertex3f( posx, posy, posz );
	glEnd();
}


// Render a sphere
void GLCanvasArea::renderSphere( int j, int pdIndex, GLUquadricObj * qobj, 
					GLint qual, relativeSize relSize )
{
	float posx = ad.particles[j].x*vp.stretchX;
	float posy = ad.particles[j].y*vp.stretchY;
	float posz = ad.particles[j].z*vp.stretchZ;
	float p1 = ad.particles[j].prop1;
	float p2 = ad.particles[j].prop2;
	float p3 = ad.particles[j].prop3;
	float p4 = ad.particles[j].prop4;
	float p5 = ad.particles[j].prop5;
	float p6 = ad.particles[j].prop6;
	float p7 = ad.particles[j].prop7;
	float p8 = ad.particles[j].prop8;
	GLdouble radius = (GLdouble)(0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33));
	GLdouble thisRadius;

	// Set a different color if coloring 
	// is not according to type.
	if (pd.colorCrit[pdIndex] == POSITION) 
		this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchY, posz/vp.stretchZ, pdIndex ); 
	if (pd.colorCrit[pdIndex] == PROPERTY) 
		this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
	if (pd.colorCrit[pdIndex] == COLORCODE) 
		this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

	// Here a sphere is rendered
	glPopMatrix();
	glPushMatrix();
	glTranslatef( posx, posy, posz );

	// Modify size if required
	switch (relSize) {
		case REGULAR:
			thisRadius = radius;
		break;
		case TINY:
			thisRadius = radius/RELSIZE_FACTOR/RELSIZE_FACTOR;
		break;
		case SMALL:
			thisRadius = radius/RELSIZE_FACTOR;
		break;
		case LARGE:
			thisRadius = radius*RELSIZE_FACTOR;
		break;
		case HHUGE:
			thisRadius = radius*RELSIZE_FACTOR*RELSIZE_FACTOR;
		break;
		default:
			thisRadius = radius;
		break;
	}

	// Render now
	gluSphere( qobj, (GLdouble)thisRadius, qual, qual );
}


// Render a cube 
void GLCanvasArea::renderCube( int j, int pdIndex, relativeSize relSize )
{
	float posx = ad.particles[j].x*vp.stretchX;
	float posy = ad.particles[j].y*vp.stretchY;
	float posz = ad.particles[j].z*vp.stretchZ;
	float p1 = ad.particles[j].prop1;
	float p2 = ad.particles[j].prop2;
	float p3 = ad.particles[j].prop3;
	float p4 = ad.particles[j].prop4;
	float p5 = ad.particles[j].prop5;
	float p6 = ad.particles[j].prop6;
	float p7 = ad.particles[j].prop7;
	float p8 = ad.particles[j].prop8;
	GLfloat size = 0.25*vp.renderSize*vo.maxExtension/pow((double)ad.numberOfParticles, 0.33);
	GLfloat thisSize;

	// Set a different color if coloring 
	// is not according to type.
	if (pd.colorCrit[pdIndex] == POSITION) 
		this->setRampColorPos( posx/vp.stretchX, posy/vp.stretchY, posz/vp.stretchZ, pdIndex ); 
	if (pd.colorCrit[pdIndex] == PROPERTY) 
		this->setRampColorProp( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 
	if (pd.colorCrit[pdIndex] == COLORCODE) 
		this->setRampColorCode( p1, p2, p3, p4, p5, p6, p7, p8, pdIndex ); 

	// Modify size if required
	switch (relSize) {
		case REGULAR:
			thisSize = size;
		break;
		case TINY:
			thisSize = size/RELSIZE_FACTOR/RELSIZE_FACTOR;
		break;
		case SMALL:
			thisSize = size/RELSIZE_FACTOR;
		break;
		case LARGE:
			thisSize = size*RELSIZE_FACTOR;
		break;
		case HHUGE:
			thisSize = size*RELSIZE_FACTOR*RELSIZE_FACTOR;
		break;
		default:
			thisSize = size;
		break;
	}

	switch (vp.renderQuality) {
		case LOW:
			// Here only one square is rendered
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 0.0, 1.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz );
			glVertex3f( posx+thisSize, posy-thisSize, posz );
			glVertex3f( posx+thisSize, posy+thisSize, posz );
			glVertex3f( posx-thisSize, posy+thisSize, posz );
			glEnd();
		break;
		case HIGH:
			// Here three intersecting squares are rendered
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 0.0, 1.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz );
			glVertex3f( posx+thisSize, posy-thisSize, posz );
			glVertex3f( posx+thisSize, posy+thisSize, posz );
			glVertex3f( posx-thisSize, posy+thisSize, posz );
			glEnd();

			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 1.0, 0.0 );
			glVertex3f( posx-thisSize, posy, posz-thisSize );
			glVertex3f( posx+thisSize, posy, posz-thisSize );
			glVertex3f( posx+thisSize, posy, posz+thisSize );
			glVertex3f( posx-thisSize, posy, posz+thisSize );
			glEnd();

			glBegin( GL_POLYGON );
			glNormal3f( 1.0, 0.0, 0.0 );
			glVertex3f( posx, posy-thisSize, posz-thisSize );
			glVertex3f( posx, posy+thisSize, posz-thisSize );
			glVertex3f( posx, posy+thisSize, posz+thisSize );
			glVertex3f( posx, posy-thisSize, posz+thisSize );
			glEnd();
		break;
		case FINAL:
			// Here a full cube is rendered: one face..
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 0.0, -1.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz-thisSize );
			glVertex3f( posx-thisSize, posy+thisSize, posz-thisSize );
			glEnd();

			// ..next face..
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 0.0, 1.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz+thisSize );
			glVertex3f( posx+thisSize, posy-thisSize, posz+thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz+thisSize );
			glVertex3f( posx-thisSize, posy+thisSize, posz+thisSize );
			glEnd();

			// ..next face..
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, -1.0, 0.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy-thisSize, posz+thisSize );
			glVertex3f( posx-thisSize, posy-thisSize, posz+thisSize );
			glEnd();

			// ..next face..
			glBegin( GL_POLYGON );
			glNormal3f( 0.0, 1.0, 0.0 );
			glVertex3f( posx-thisSize, posy+thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz+thisSize );
			glVertex3f( posx-thisSize, posy+thisSize, posz+thisSize );
			glEnd();

			// ..next face..
			glBegin( GL_POLYGON );
			glNormal3f( 1.0, 0.0, 0.0 );
			glVertex3f( posx+thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz-thisSize );
			glVertex3f( posx+thisSize, posy+thisSize, posz+thisSize );
			glVertex3f( posx+thisSize, posy-thisSize, posz+thisSize );
			glEnd();

			// ..last face
			glBegin( GL_POLYGON );
			glNormal3f( -1.0, 0.0, 0.0 );
			glVertex3f( posx-thisSize, posy-thisSize, posz-thisSize );
			glVertex3f( posx-thisSize, posy+thisSize, posz-thisSize );
			glVertex3f( posx-thisSize, posy+thisSize, posz+thisSize );
			glVertex3f( posx-thisSize, posy-thisSize, posz+thisSize );
			glEnd();
		break;
	}
}


// Render a bond cylinder -- depending on the quality, it is just a 
// line or a full cylinder
void GLCanvasArea::renderBondCylinder( int k, int l, GLUquadricObj * qobj, GLint qual, GLdouble thisRadius, fixedThickness thickness )
{
	GLdouble dist, alpha, axex, axey;
	bool lighting = TRUE;
	bool drawFlag = FALSE;

	if (vp.renderQuality == LOW || thickness == LINE) {
		// Switch off lighting 
		if (lighting == TRUE) {
			glDisable( GL_LIGHTING );	
			lighting = FALSE;
		}

		glBegin( GL_LINES );
			glVertex3f( ad.particles[k].x, ad.particles[k].y, ad.particles[k].z );
			glVertex3f( ad.particles[l].x, ad.particles[l].y, ad.particles[l].z );
		glEnd();
		drawFlag = TRUE;
	}
	if ((vp.renderQuality == HIGH || vp.renderQuality == FINAL) && drawFlag == FALSE) {
		// Switch on lighting 
		if (lighting == FALSE) {
			glEnable( GL_LIGHTING );	
			lighting = TRUE;
		}

		dist = findBondDistanceAngle( k, l, &alpha, &axex, &axey);
		glPushMatrix();

		// Translate to the first endpoint
		glTranslatef( ad.particles[k].x*vp.stretchX, ad.particles[k].y*vp.stretchY, ad.particles[k].z*vp.stretchZ );

		// The cylinder is by default rendered along the z-axis.
		// Rotate about a suitable axe by a suitable 
		// angle to obtain a cylinder in the
		// direction of the bond
		glRotated( alpha, axex, axey, 0.0 );
		gluCylinder( qobj, (GLdouble)thisRadius, (GLdouble)thisRadius, dist, qual, qual );
		glPopMatrix();
	}
}


// Render a cylinder -- depending on the quality, it is just a 
// line or a full cylinder
void GLCanvasArea::renderLineCylinder( GLdouble posx, GLdouble posy, GLdouble posz, GLdouble n1, GLdouble n2, GLdouble n3, GLdouble thisLen, GLUquadricObj * qobj, GLint qual, GLdouble radius, relativeSize relSize, fixedThickness thickness, bool bottom, bool top)
{
	GLdouble dist, alpha, axex, axey, thisRadius;
	bool lighting = TRUE;
	bool drawFlag = FALSE;

	// Modify size if required
	switch (relSize) {
		case REGULAR:
			thisRadius = radius;
		break;
		case TINY:
			thisRadius = radius/RELSIZE_FACTOR/RELSIZE_FACTOR;
		break;
		case SMALL:
			thisRadius = radius/RELSIZE_FACTOR;
		break;
		case LARGE:
			thisRadius = radius*RELSIZE_FACTOR;
		break;
		case HHUGE:
			thisRadius = radius*RELSIZE_FACTOR*RELSIZE_FACTOR;
		break;
		default:
			thisRadius = radius;
		break;
	}

	if (thickness == LINE) {
		// Switch off lighting
		if (lighting == TRUE) {
			glDisable( GL_LIGHTING );
			lighting = FALSE;
		}
		glBegin( GL_LINES );
			glVertex3f( posx, posy, posz );
			glVertex3f( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
		glEnd();
		drawFlag = TRUE;
	}
	if (drawFlag == FALSE) {
		// Switch on lighting 
		if (lighting == FALSE) {
			glEnable( GL_LIGHTING );	
			lighting = TRUE;
		}

		dist = findSpinDistanceAngle( posx, posy, posz, posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen, &alpha, &axex, &axey);
		glPushMatrix();

		if (bottom) {
			// Draw a disk to close the cylinder bottom
			glPushMatrix();

			// Translate into the foot position
			glTranslatef( posx, posy, posz);

			// The cylinder is by default rendered 
			// along the z-axis.  Rotate about a
			// suitable axis by a suitable angle
			// to obtain a cylinder in the direction of the spin / 
			// liquid crystal / pore
			glRotated( alpha, axex, axey, 0.0 );

			// Draw a bottom
			gluDisk( qobj, 0.0, (GLdouble)thisRadius, qual, qual );
			glPopMatrix();
		}
	
		// Translate into the tip position
		glTranslatef( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );

		// The cylinder is by default rendered 
		// along the z-axis.  Rotate about a
		// suitable axis by a suitable angle
		// to obtain a cylinder in the direction of the spin / 
		// liquid crystal / pore
		glRotated( alpha, axex, axey, 0.0 );
		gluCylinder( qobj, (GLdouble)thisRadius, (GLdouble)thisRadius, dist, qual, qual );

		if (top) {
			// Draw a disk to close the top
			gluDisk( qobj, 0.0, (GLdouble)thisRadius, qual, qual );
		}
		
		glPopMatrix();
	}
}


// Render a cone -- depending on the quality, it is just a 
// line or a full cone (overloaded function)
void GLCanvasArea::renderCone( GLdouble posx, GLdouble posy, GLdouble posz, GLdouble n1, GLdouble n2, GLdouble n3, GLdouble thisLen, GLUquadricObj * qobj, GLint qual, GLdouble radius, relativeSize relSize, fixedThickness thickness, bool bottom )
{
	GLdouble dist, alpha, axex, axey, thisRadius;
	bool lighting = TRUE;
	bool drawFlag = FALSE;
	
        // Modify size if required
        switch (relSize) {
                case REGULAR:
                        thisRadius = radius;
                break;
                case TINY:
                        thisRadius = radius/RELSIZE_FACTOR/RELSIZE_FACTOR;
                break;
                case SMALL:
                        thisRadius = radius/RELSIZE_FACTOR;
                break;
                case LARGE:
                        thisRadius = radius*RELSIZE_FACTOR;
                break;
                case HHUGE:
                        thisRadius = radius*RELSIZE_FACTOR*RELSIZE_FACTOR;
                break;
                default:
                        thisRadius = radius;
                break;
        }

	if (thickness == LINE) {
		// Switch off lighting
		if (lighting == TRUE) {
			glDisable( GL_LIGHTING );
			lighting = FALSE;
		}
		glBegin( GL_LINES );
			glVertex3f( posx, posy, posz );
			glVertex3f( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
		glEnd();
		drawFlag = TRUE;
	}
	if (drawFlag == FALSE) {
		// Switch on lighting 
		if (lighting == FALSE) {
			glEnable( GL_LIGHTING );	
			lighting = TRUE;
		}

		dist = findSpinDistanceAngle( posx, posy, posz, posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen, &alpha, &axex, &axey);
		glPushMatrix();

		if (bottom) {
			// Draw a disk to close the cone bottom
			glPushMatrix();

			// Translate into the foot position
			glTranslatef( posx, posy, posz);

			// The cylinder is by default rendered 
			// along the z-axis.  Rotate about a
			// suitable axis by a suitable angle
			// to obtain a cylinder in the direction of the spin / 
			// liquid crystal / pore
			glRotated( alpha, axex, axey, 0.0 );

			// Draw a bottom
			gluDisk( qobj, 0.0, (GLdouble)thisRadius, qual, qual );
			glPopMatrix();
		}
	
		// Translate into the tip position
		glTranslatef( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );

		// The cone (a cylinder with one of the radii set to zero) 
		//  is by default rendered along the z-axis.  Rotate about a
		// suitable axis by a suitable angle
		// to obtain a cylinder in the direction of the spin /
		// liquid crystal / pore
		glRotated( alpha, axex, axey, 0.0 );
		gluCylinder( qobj, 0.0, (GLdouble)thisRadius, dist, qual, qual );
		glPopMatrix();
	}
}


// Render a cone -- depending on the quality, it is just a 
// line or a full cone (overloaded function)
void GLCanvasArea::renderCone( GLdouble posx, GLdouble posy, GLdouble posz, GLdouble n1, GLdouble n2, GLdouble n3, GLdouble thisLen, GLUquadricObj * qobj, GLint qual, GLdouble radius, GLdouble radius2, relativeSize relSize, fixedThickness thickness, bool bottom )
{
	GLdouble dist, alpha, axex, axey, thisRadius, thisRadius2;
	bool lighting = TRUE;
	bool drawFlag = FALSE;

        // Modify size if required
        switch (relSize) {
                case REGULAR:
                        thisRadius = radius;
                        thisRadius2 = radius2;
                break;
                case TINY:
                        thisRadius = radius/RELSIZE_FACTOR/RELSIZE_FACTOR;
                        thisRadius2 = radius2/RELSIZE_FACTOR/RELSIZE_FACTOR;
                break;
                case SMALL:
                        thisRadius = radius/RELSIZE_FACTOR;
                        thisRadius2 = radius2/RELSIZE_FACTOR;
                break;
                case LARGE:
                        thisRadius = radius*RELSIZE_FACTOR;
                        thisRadius2 = radius2*RELSIZE_FACTOR;
                break;
                case HHUGE:
                        thisRadius = radius*RELSIZE_FACTOR*RELSIZE_FACTOR;
                        thisRadius2 = radius2*RELSIZE_FACTOR*RELSIZE_FACTOR;
                break;
                default:
                        thisRadius = radius;
                        thisRadius2 = radius2;
                break;
        }

	if (thickness == LINE) {
		// Switch off lighting
		if (lighting == TRUE) {
			glDisable( GL_LIGHTING );
			lighting = FALSE;
		}
		glBegin( GL_LINES );
			glVertex3f( posx, posy, posz );
			glVertex3f( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
		glEnd();
		drawFlag = TRUE;
	}
	if (drawFlag == FALSE) {
		// Switch on lighting 
		if (lighting == FALSE) {
			glEnable( GL_LIGHTING );	
			lighting = TRUE;
		}

		dist = findSpinDistanceAngle( posx, posy, posz, posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen, &alpha, &axex, &axey);
		glPushMatrix();

		if (bottom) {
			// Draw a disk to close the cone bottom
			glPushMatrix();

			// Translate into the foot position
			glTranslatef( posx, posy, posz);

			// The cylinder is by default rendered 
			// along the z-axis.  Rotate about a
			// suitable axis by a suitable angle
			// to obtain a cylinder in the direction of the spin / 
			// liquid crystal / pore
			glRotated( alpha, axex, axey, 0.0 );

			// Draw a bottom
			gluDisk( qobj, 0.0, (GLdouble)thisRadius, qual, qual );
			glPopMatrix();
		}
	
		// Translate into the tip position
		glTranslatef( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );

		// The cone (a cylinder with one of the radii set to zero) 
		//  is by default rendered along the z-axis.  Rotate about a
		// suitable axis by a suitable angle
		// to obtain a cylinder in the direction of the spin /
		// liquid crystal / pore
		glRotated( alpha, axex, axey, 0.0 );
		gluCylinder( qobj, (GLdouble)thisRadius2, (GLdouble)thisRadius, dist, qual, qual );
		glPopMatrix();
	}
}


// Render a spin tip -- either a line or a sphere 
// or a cone
void GLCanvasArea::renderSpinTip( GLdouble posx, GLdouble posy, GLdouble posz, GLdouble n1, GLdouble n2, GLdouble n3, GLdouble thisLen, GLUquadricObj * qobj, GLint qual, GLdouble thisRadius )
{
	bool lighting = TRUE;
	
	switch (vp.renderQuality) {
		case LOW:
			// Switch off lighting 
			if (lighting == TRUE) {
				glDisable( GL_LIGHTING );	
				lighting = FALSE;
			}
			// Set line width
			glLineWidth(3.0);
			glBegin( GL_LINES );
			glVertex3f( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
			glVertex3f( posx+n1*(thisLen-thisRadius), posy+n2*(thisLen-thisRadius), posz+n3*(thisLen-thisRadius) );
			glEnd();
			// Reset line width
			glLineWidth(1.0);
		break;
		case HIGH:
			// Switch on lighting 
			if (lighting == FALSE) {
				glEnable( GL_LIGHTING );	
				lighting = TRUE;
			}
			glTranslatef( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
			gluSphere( qobj, (GLdouble)(thisRadius/2.0), qual, qual );
			glTranslatef( -(posx+n1*thisLen), -(posy+n2*thisLen), -(posz+n3*thisLen) );
		break;
		case FINAL:
			// Switch on lighting 
			if (lighting == FALSE) {
				glEnable( GL_LIGHTING );	
				lighting = TRUE;
			}
			glTranslatef( posx+n1*thisLen, posy+n2*thisLen, posz+n3*thisLen );
			gluSphere( qobj, (GLdouble)(thisRadius/2.0), qual, qual );
			glTranslatef( -(posx+n1*thisLen), -(posy+n2*thisLen), -(posz+n3*thisLen) );
		break;
	}
}


// Find out if a given particle is in the specified slicing area
bool GLCanvasArea::particleIsInSlice( int i )
{
	float posx = ad.particles[i].x;
	float posy = ad.particles[i].y;
	float posz = ad.particles[i].z;

	// Skip the test is slicing is not required
	if (vp.slicing == FALSE)
		return TRUE;

 	if ( (vp.showSlicePlaneX == TRUE) || (posx >= vp.sliceXMin && posx <= vp.sliceXMax) ) {
 		if ( (vp.showSlicePlaneY == TRUE) || (posy >= vp.sliceYMin && posy <= vp.sliceYMax) ) {
 			if ( (vp.showSlicePlaneZ == TRUE) || (posz >= vp.sliceZMin && posz <= vp.sliceZMax) ) {
				return TRUE;
			}
		}
	}

	return FALSE;
}


// Find out if a given track is in the specified slicing area (simply by
// looking at first entry)
bool GLCanvasArea::trackIsInSlice( int i )
{
	float posx = td.x[i][0];
	float posy = td.y[i][0];
	float posz = td.z[i][0];

	// Skip the test is slicing is not required
	if (vp.slicing == FALSE)
		return TRUE;

 	if ( (vp.showSlicePlaneX == TRUE) || (posx >= vp.sliceXMin && posx <= vp.sliceXMax) ) {
 		if ( (vp.showSlicePlaneY == TRUE) || (posy >= vp.sliceYMin && posy <= vp.sliceYMax) ) {
 			if ( (vp.showSlicePlaneZ == TRUE) || (posz >= vp.sliceZMin && posz <= vp.sliceZMax) ) {
				return TRUE;
			}
		}
	}

	return FALSE;
}


// Find out if two given atoms form a bond within a minimum and maximum
// length, and also belong to the same molecule (indicated by the first
// property value)
bool GLCanvasArea::isBond( int i, int j, float minLength, float maxLength, 
						bool onlyWithinMolecule, bool isSeqBond, double *pBondLength)
{
        float posxi = ad.particles[i].x;
        float posyi = ad.particles[i].y;
        float poszi = ad.particles[i].z;
        float posxj = ad.particles[j].x;
        float posyj = ad.particles[j].y;
        float poszj = ad.particles[j].z;
	int pi = (int)ad.particles[i].prop1;
	int pj = (int)ad.particles[j].prop1;

	// No bond between an atom and itself
	if (i == j) 
		return FALSE;
	
	// In polymer mode, no bond between atoms belonging to 
	// different molecules (indicated by the first property 
	// value)

	if (onlyWithinMolecule && (pi != pj))
		return FALSE;

	// If "sequential Bonding" is checked, bonds can only be created
	// between atoms that are adjacent (inc. first<->last) in the coordinate file
	if (isSeqBond && !(j==i+1) && !(i==0 && j==ad.numberOfParticles-1))
		return FALSE;

	// Check distancjes
	double distx = fabs((double)(posxi-posxj));
	if (distx > maxLength) 
		return FALSE;
	double disty = fabs((double)(posyi-posyj));
	if (disty > maxLength) 
		return FALSE;
	double distz = fabs((double)(poszi-poszj));
	if (distz > maxLength) 
		return FALSE;

	double dist2 = distx*distx + disty*disty + distz*distz;
	if (dist2 < minLength*minLength)
		return FALSE;
	if (dist2 > maxLength*maxLength)
		return FALSE;

	*pBondLength = sqrt(dist2);

	return TRUE;
}


// Return the distance between two atoms forming a bond, and 
// find the appropriate rotation angles
GLdouble GLCanvasArea::findBondDistanceAngle( int i, int j, GLdouble * alpha, GLdouble * axex, GLdouble * axey)
{
	double normx, normy, normz;

        float posxi = ad.particles[i].x*vp.stretchX;
        float posyi = ad.particles[i].y*vp.stretchY;
        float poszi = ad.particles[i].z*vp.stretchZ;
        float posxj = ad.particles[j].x*vp.stretchX;
        float posyj = ad.particles[j].y*vp.stretchY;
        float poszj = ad.particles[j].z*vp.stretchZ;

	double distx = (double)(posxi-posxj);
	double disty = (double)(posyi-posyj);
	double distz = (double)(poszi-poszj);

        double dist2 = distx*distx+ disty*disty + distz*distz;
	double dist = (GLdouble)sqrt( dist2 );

	// Find the angle between the z-axis and the
	// bond.  This is given by the arccos of the 
	// scalar product of the appropriate unit vectors
	if (dist > 0.0) {
		normx = distx/dist;
		normy = disty/dist;
		normz = distz/dist;
		(*alpha) = 180.0-180.0/PI*acos( normz );
	}
	else {
		normx = normy = normz = 1.0;
		(*alpha) = 0.0;
	}

	// Find the x- and y-components of the rotation axis	
	if (dist > 0.0) {
		if (fabs(normx) > 0.0 && fabs(normy) > 0.0) {
			// Standard case: rotate about an axis with x- and 
			// y-components
			if (normy > 0.0) {
				(*axex) = 1.0;
				(*axey) = -normx/normy;
			}
			if (normy < 0.0) {
				(*axex) = -1.0;
				(*axey) = normx/normy;
			}
		}
		else {
			// Pathologic case: rotate about one of the coordinate
			// axis
			if (fabs(normx) > 0.0) {
				if (normx > 0.0) {
					(*axex) = 0.0;
					(*axey) = -1.0;
				}
				if (normx < 0.0) {
					(*axex) = 0.0;
					(*axey) = 1.0;
				}
			}
			if (fabs(normy) > 0.0) {
				if (normy > 0.0) {
					(*axex) = 1.0;
					(*axey) = 0.0;
				}
				if (normy < 0.0) {
					(*axex) = -1.0;
					(*axey) = 0.0;
				}
			}
		}
	}
	else {
		(*axex) = 0.0;
		(*axey) = 0.0;
	}

	// Finally, return the bond length	
	return dist;
}


// Return the length of a spin / liquid crystal / pore  foot (it is 
// actually already know at this stage), and 
// find the appropriate rotation angles
GLdouble GLCanvasArea::findSpinDistanceAngle( float footposx, float footposy, float footposz, float tipposx, float tipposy, float tipposz, GLdouble * alpha, GLdouble * axex, GLdouble * axey)
{
	double normx, normy, normz;

	double distx = (double)(tipposx-footposx);
	double disty = (double)(tipposy-footposy);
	double distz = (double)(tipposz-footposz);

        double dist2 = distx*distx+ disty*disty + distz*distz;
	double dist = (GLdouble)sqrt( dist2 );

	// Find the angle between the z-axis and the
	// foot direction.  This is given by the arccos of the 
	// scalar product of the appropriate unit vectors
	if (dist > 0.0) {
		normx = distx/dist;
		normy = disty/dist;
		normz = distz/dist;
		(*alpha) = 180.0-180.0/PI*acos( normz );
	}
	else {
		normx = normy = normz = 1.0;
		(*alpha) = 0.0;
	}

	// Find the x- and y-components of the rotation axis	
	if (dist > 0.0) {
		if (fabs(normx) > 0.0 && fabs(normy) > 0.0) {
			// Standard case: rotate about an axis with x- and 
			// y-components
			if (normy > 0.0) {
				(*axex) = 1.0;
				(*axey) = -normx/normy;
			}
			if (normy < 0.0) {
				(*axex) = -1.0;
				(*axey) = normx/normy;
			}
		}
		else {
			// Pathologic case: rotate about one of the coordinate
			// axis
			if (fabs(normx) > 0.0) {
				if (normx > 0.0) {
					(*axex) = 0.0;
					(*axey) = -1.0;
				}
				if (normx < 0.0) {
					(*axex) = 0.0;
					(*axey) = 1.0;
				}
			}
			if (fabs(normy) > 0.0) {
				if (normy > 0.0) {
					(*axex) = 1.0;
					(*axey) = 0.0;
				}
				if (normy < 0.0) {
					(*axex) = -1.0;
					(*axey) = 0.0;
				}
			}
		}
	}
	else {
		(*axex) = 0.0;
		(*axey) = 0.0;
	}

	// Finally, return the foot length	
	return dist;
}


// Find out if bonds between a given combination of particle types have 
// already been drawn
bool GLCanvasArea::bondCombinationNew( tag * fParticleTypes, tag * tParticleTypes, int * count, char * fAtom, char * tAtom )
{
	int nb = (*count);

	// Check all drawn particle type combinations so far
	for (int i=0;i<nb;i++) {
		if (typeCmp( (char *)&fParticleTypes[i], fAtom ) == TRUE && typeCmp( (char *)&tParticleTypes[i], tAtom ) == TRUE) {
			return FALSE;
		}
		if (typeCmp( (char *)&fParticleTypes[i], tAtom ) == TRUE && typeCmp( (char *)&tParticleTypes[i], fAtom ) == TRUE) {
			return FALSE;
		}
	}

	// The combination is new; make a new entry in the list
	// of drawn combinations
	typeCopy( fAtom, (char *)&fParticleTypes[nb]);
	typeCopy( tAtom, (char *)&tParticleTypes[nb]);
	nb++;
	
	(*count) = nb;

	return TRUE;
}


// Set the color, quality, and thickness that is appropriate for a given bond 
// type
void GLCanvasArea::setBondProperty( fixedColor color, fixedThickness thickness, GLfloat emission, GLdouble * relRadius, GLint * qual )
{
	GLfloat wqual = 1.0;
	GLfloat emissionR, emissionG, emissionB;

	switch (color) {
		case WHITE: 
			glColor3f( white_color );
			emissionR = 1.0*emission;
			emissionG = 1.0*emission;
			emissionB = 1.0*emission;
		break;
		case BLACK: 
			glColor3f( black_color );
			emissionR = 0.0*emission;
			emissionG = 0.0*emission;
			emissionB = 0.0*emission;
		break;
		case RED: 
			glColor3f( red_color );
			emissionR = 1.0*emission;
			emissionG = 0.0*emission;
			emissionB = 0.0*emission;
		break;
		case YELLOW: 
			glColor3f( yellow_color );
			emissionR = 1.0*emission;
			emissionG = 1.0*emission;
			emissionB = 0.0*emission;
		break;
		case GREEN: 
			glColor3f( green_color );
			emissionR = 0.0*emission;
			emissionG = 1.0*emission;
			emissionB = 0.0*emission;
		break;
		case BLUE: 
			glColor3f( blue_color );
			emissionR = 0.0*emission;
			emissionG = 0.0*emission;
			emissionB = 1.0*emission;
		break;
		case GOLD: 
			glColor3f( gold_color );
			emissionR = 0.83*emission;
			emissionG = 0.61*emission;
			emissionB = 0.21*emission;
		break;
		case SILVER: 
			glColor3f( silver_color );
			emissionR = 1.0*emission;
			emissionG = 1.0*emission;
			emissionB = 1.0*emission;
		break;
		case BRONZE: 
			glColor3f( bronze_color );
			emissionR = 0.83*emission;
			emissionG = 0.56*emission;
			emissionB = 0.45*emission;
		break;
		default: 
			glColor3f( red_color );
			emissionR = 1.0*emission;
			emissionG = 0.0*emission;
			emissionB = 0.0*emission;
		break;
	}

	// Enable glowing if needed
	if (thickness != LINE && emission > 0.0) {
		glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
		GLfloat bond_mat[] = { emissionR, emissionG, emissionB, 1.0 };
		glMaterialfv( GL_FRONT, GL_EMISSION, bond_mat );
	}

	switch (thickness) {
		case THIN: 
			// For line renderings, change the line
			// thickness
			if (vp.renderQuality == LOW) {
				wqual = 1.0;
			}
			// For cylinder renderings, change the radius
			if (vp.renderQuality == HIGH || vp.renderQuality == FINAL) {
				(*relRadius) = 0.5;
			}
		break;
		case MEDIUM: 
			// For line renderings, change the line
			// thickness
			if (vp.renderQuality == LOW) {
				wqual = 2.0;
			}
			// For cylinder renderings, change the radius
			if (vp.renderQuality == HIGH || vp.renderQuality == FINAL) {
				(*relRadius) = 1.0;
			}
		break;
		case THICK: 
			// For line renderings, change the line
			// thickness
			if (vp.renderQuality == LOW) {
				wqual = 3.0;
			}
			// For cylinder renderings, change the radius
			if (vp.renderQuality == HIGH || vp.renderQuality == FINAL) {
				(*relRadius) = 1.5;
			}
		break;
		case VARYING_LINEAR:
		case VARYING_QUADRATIC:
			// This mode is only relevant for Cylinder rendering.
			// Set this default value which will not matter bacause the radius will depend on the bond length
			(*relRadius) = 1;
		break;
		case FAT: 
			// For line renderings, change the line
			// thickness
			if (vp.renderQuality == LOW) {
				wqual = 4.0;
			}
			// For cylinder renderings, change the radius
			if (vp.renderQuality == HIGH || vp.renderQuality == FINAL) {
				(*relRadius) = 2.0;
			}
		break;
		case LINE: 
			// The standard case anyway...
			wqual = 1.0;
		break;
	}

	// Set the bond render quality
	switch (vp.renderQuality) {
               case FINAL:
                        (*qual) = 20;
                        if (ad.numberOfParticles > MANY_ATOMS)
                                (*qual) = 15;
                        if (ad.numberOfParticles > VERY_MANY_ATOMS)
                                (*qual) = 12;
                        if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS)
                                (*qual) = 10;
			wqual = 2.0;
                break;
                case HIGH:
                        (*qual) = 10;
                        if (ad.numberOfParticles > MANY_ATOMS)
                                (*qual) = 9;
                        if (ad.numberOfParticles > VERY_MANY_ATOMS)
                                (*qual) = 6;
                        if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS)
                                (*qual) = 5;
			wqual = 2.0;
                break;
		case LOW:
			(*qual) = 3;
			wqual = 1.0;
		break;
	}
			
	// Set line width
	glLineWidth( wqual );
}


// Set the color, quality, and thickness that is appropriate for a given line
// type 
void GLCanvasArea::setLineFootProperty( fixedThickness thickness, GLfloat emission, GLdouble * relRadius, GLint * qual )
{
	GLfloat wqual = 1.0;
	GLfloat emissionR, emissionG, emissionB;

	// Emit white color
	emissionR = 1.0*emission;
	emissionG = 1.0*emission;
	emissionB = 1.0*emission;

	// Enable glowing if needed
	if (thickness != LINE && emission > 0.0) {
		glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
		GLfloat foot_mat[] = { emissionR, emissionG, emissionB, 1.0 };
		glMaterialfv( GL_FRONT, GL_EMISSION, foot_mat );
	}

	switch (thickness) {
		case THIN: 
			// The standard case anyway...
			wqual = 1.0;
			// For cone/cylinder renderings, change the radius
			(*relRadius) = 0.5;
		break;
		case VARYING_LINEAR: // default like the MEDIUM case
		case VARYING_QUADRATIC: // default like the MEDIUM case
		case MEDIUM: 
			wqual = 2.0;
			// For cone/cylinder renderings, change the radius
			(*relRadius) = 1.0;
		break;
		case THICK: 
			wqual = 3.0;
			// For cone/cylinder renderings, change the radius
			(*relRadius) = 1.5;
		break;
		case FAT: 
			wqual = 4.0;
			// For cone/cylinder renderings, change the radius
			(*relRadius) = 2.0;
		break;
		case LINE: 
			wqual = 1.0;
			// For cone/cylinder renderings, change the radius
			(*relRadius) = 0.5;
		break;
	}

	// Set the render quality
	switch (vp.renderQuality) {
               case FINAL:
			(*qual) = 20;
                        if (ad.numberOfParticles > MANY_ATOMS)
				(*qual) = 15;
                        if (ad.numberOfParticles > VERY_MANY_ATOMS)
				(*qual) = 12;
                        if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS)
				(*qual) = 10;
                break;
                case HIGH:
			(*qual) = 10;
                        if (ad.numberOfParticles > MANY_ATOMS)
				(*qual) = 9;
                        if (ad.numberOfParticles > VERY_MANY_ATOMS)
				(*qual) = 6;
                        if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS)
				(*qual) = 5;
                break;
		case LOW:
			(*qual) = 7;
                        if (ad.numberOfParticles > MANY_ATOMS)
				(*qual) = 6;
                        if (ad.numberOfParticles > VERY_MANY_ATOMS)
				(*qual) = 5;
                        if (ad.numberOfParticles > VERY_VERY_MANY_ATOMS)
				(*qual) = 3;
                break;
		break;
	}
			
	// Set line width
	glLineWidth( wqual );

}


// Find the index that relates tag list and particle property list
int GLCanvasArea::findPdListIndex(  char * thisType )
{
	int j = 0;

	// Search for the specified type
       	while (typeCmp( thisType, (char *)&pd.type[j] ) != TRUE && j < pd.numberOfParticleTypes) {
                        j++;
	}
	if (j >= pd.numberOfParticleTypes) {
		// Panic if no correct type was identified
		j = pd.numberOfParticleTypes-1;
	}

	return j;
}


// Set the color that is appropriate for a given particle type
// (overloaded function) 
void GLCanvasArea::setTypeColor( int pdIndex )
{
	// Set the color if coloring should
	// be according to type.  Otherwise the color
	// is later set individually for each atom
	if (pdIndex >= 0 && pdIndex < ATOMS_MAX) {

	 	GLfloat red = pd.particleColors[pdIndex].red;
       		GLfloat green = pd.particleColors[pdIndex].green;
		GLfloat blue = pd.particleColors[pdIndex].blue;

		// Set the type color
		glColor3f( red, green, blue );
	}
}


// Set a relative size that is appropriate for a given particle type
// Set the color that is appropriate for a given particle type
// (overloaded function) 
void GLCanvasArea::setTypeColor( GLfloat * red, GLfloat * green, GLfloat * blue, GLfloat * redTwo, GLfloat * greenTwo, GLfloat * blueTwo, GLfloat * redThree, GLfloat * greenThree, GLfloat * blueThree, int pdIndex )
{
	// Set the color if coloring should
	// be according to type.  Otherwise the color
	// is later set individually for each atom
 	(*red) = pd.particleColors[pdIndex].red;
       	(*green) = pd.particleColors[pdIndex].green;
	(*blue) = pd.particleColors[pdIndex].blue;
 	(*redTwo) = pd.particleColors[pdIndex].redTwo;
       	(*greenTwo) = pd.particleColors[pdIndex].greenTwo;
	(*blueTwo) = pd.particleColors[pdIndex].blueTwo;
 	(*redThree) = pd.particleColors[pdIndex].redThree;
       	(*greenThree) = pd.particleColors[pdIndex].greenThree;
	(*blueThree) = pd.particleColors[pdIndex].blueThree;

	// Set the type color
	glColor3f( (*red), (*green), (*blue) );
}


// Set a relative size that is appropriate for a given particle type
// (overloaded function)
void GLCanvasArea::setTypeRelSize( int pdIndex, relativeSize * relSize )
{
        // Now we have the correct relative size
	if (pdIndex >= 0 && pdIndex < ATOMS_MAX)
	        (* relSize) = pd.relSize[pdIndex];
}


// Set the color that is appropriate for a given particle
void GLCanvasArea::setRampColorPos( float posx, float posy, float posz, 
								int pdIndex )
{
	GLfloat red, green, blue;
	GLfloat redMin = pd.particleColors[pdIndex].redMin;
	GLfloat redMax = pd.particleColors[pdIndex].redMax;
	GLfloat greenMin = pd.particleColors[pdIndex].greenMin;
	GLfloat greenMax = pd.particleColors[pdIndex].greenMax;
	GLfloat blueMin = pd.particleColors[pdIndex].blueMin;
	GLfloat blueMax = pd.particleColors[pdIndex].blueMax;

	// Depending on the position, choose a color
	colorCriterionPos colorCritPos = pd.colorCritPos[pdIndex];
	switch (colorCritPos) {
		case XPOS:
			red = (vo.xmax-posx)/(vo.deltax)*(redMax-redMin)+redMin;
			green = (vo.xmax-posx)/(vo.deltax)*(greenMax-greenMin)+greenMin;
			blue = (vo.xmax-posx)/(vo.deltax)*(blueMax-blueMin)+blueMin;
		break;
		case YPOS:
			red = (posy-vo.ymin)/(vo.deltay)*(redMax-redMin)+redMin;
			green = (posy-vo.ymin)/(vo.deltay)*(greenMax-greenMin)+greenMin;
			blue = (posy-vo.ymin)/(vo.deltay)*(blueMax-blueMin)+blueMin;
		break;
		case ZPOS:
			red = (posz-vo.zmin)/(vo.deltaz)*(redMax-redMin)+redMin;
			green = (posz-vo.zmin)/(vo.deltaz)*(greenMax-greenMin)+greenMin;
			blue= (posz-vo.zmin)/(vo.deltaz)*(blueMax-blueMin)+blueMin;
		break;
		default:
			red = (vo.xmax-posx)/(vo.deltax)*(redMax-redMin)+redMin;
			green = (vo.xmax-posx)/(vo.deltax)*(greenMax-greenMin)+greenMin;
			blue = (vo.xmax-posx)/(vo.deltax)*(blueMax-blueMin)+blueMin;
		break;
	}
			
	// Set now the position-dependent color
	glColor3f( red, green, blue );
}


// Set the color that is appropriate for a given particle property
void GLCanvasArea::setRampColorProp( float p1, float p2, float p3, float p4, 
			float p5, float p6, float p7, float p8, int pdIndex )
{
	GLfloat red, green, blue;
	GLfloat redMin = pd.particleColors[pdIndex].redMin;
	GLfloat redMax = pd.particleColors[pdIndex].redMax;
	GLfloat greenMin = pd.particleColors[pdIndex].greenMin;
	GLfloat greenMax = pd.particleColors[pdIndex].greenMax;
	GLfloat blueMin = pd.particleColors[pdIndex].blueMin;
	GLfloat blueMax = pd.particleColors[pdIndex].blueMax;

	// Depending on the property, choose a color
	colorCriterionColumn colorCritProp = pd.colorCritProp[pdIndex];
	switch (colorCritProp) {
		case PROP1:
			red = (p1-vo.p1min)/(vo.deltap1)*(redMax-redMin)+redMin;
			green = (p1-vo.p1min)/(vo.deltap1)*(greenMax-greenMin)+greenMin;
			blue = (p1-vo.p1min)/(vo.deltap1)*(blueMax-blueMin)+blueMin;
		break;
		case PROP2:
			red = (p2-vo.p2min)/(vo.deltap2)*(redMax-redMin)+redMin;
			green = (p2-vo.p2min)/(vo.deltap2)*(greenMax-greenMin)+greenMin;
			blue = (p2-vo.p2min)/(vo.deltap2)*(blueMax-blueMin)+blueMin;
		break;
		case PROP3:
			red = (p3-vo.p3min)/(vo.deltap3)*(redMax-redMin)+redMin;
			green = (p3-vo.p3min)/(vo.deltap3)*(greenMax-greenMin)+greenMin;
			blue = (p3-vo.p3min)/(vo.deltap3)*(blueMax-blueMin)+blueMin;
		break;
		case PROP4:
			red = (p4-vo.p4min)/(vo.deltap4)*(redMax-redMin)+redMin;
			green = (p4-vo.p4min)/(vo.deltap4)*(greenMax-greenMin)+greenMin;
			blue = (p4-vo.p4min)/(vo.deltap4)*(blueMax-blueMin)+blueMin;
		break;
		case PROP5:
			red = (p5-vo.p5min)/(vo.deltap5)*(redMax-redMin)+redMin;
			green = (p5-vo.p5min)/(vo.deltap5)*(greenMax-greenMin)+greenMin;
			blue = (p5-vo.p5min)/(vo.deltap5)*(blueMax-blueMin)+blueMin;
		break;
		case PROP6:
			red = (p6-vo.p6min)/(vo.deltap6)*(redMax-redMin)+redMin;
			green = (p6-vo.p6min)/(vo.deltap6)*(greenMax-greenMin)+greenMin;
			blue = (p6-vo.p6min)/(vo.deltap6)*(blueMax-blueMin)+blueMin;
		break;
		case PROP7:
			red = (p7-vo.p7min)/(vo.deltap7)*(redMax-redMin)+redMin;
			green = (p7-vo.p7min)/(vo.deltap7)*(greenMax-greenMin)+greenMin;
			blue = (p7-vo.p7min)/(vo.deltap7)*(blueMax-blueMin)+blueMin;
		break;
		case PROP8:
			red = (p8-vo.p8min)/(vo.deltap8)*(redMax-redMin)+redMin;
			green = (p8-vo.p8min)/(vo.deltap8)*(greenMax-greenMin)+greenMin;
			blue = (p8-vo.p8min)/(vo.deltap8)*(blueMax-blueMin)+blueMin;
		break;
		default:
			red = (p1-vo.p1min)/(vo.deltap1)*(redMax-redMin)+redMin;
			green = (p1-vo.p1min)/(vo.deltap1)*(greenMax-greenMin)+greenMin;
			blue = (p1-vo.p1min)/(vo.deltap1)*(blueMax-blueMin)+blueMin;
		break;
	}
			
	// Set now the property-dependent color
	glColor3f( red, green, blue );
}


// Set the color that is appropriate for a given particle
void GLCanvasArea::setRampColorCode( float p1, float p2, float p3, float p4, 
			float p5, float p6, float p7, float p8, int pdIndex )
{
	GLfloat red, green, blue;
	colorCriterionColumn colorCritCodeRed = pd.colorCritCodeRed[pdIndex];
	colorCriterionColumn colorCritCodeGreen = pd.colorCritCodeGreen[pdIndex];
	colorCriterionColumn colorCritCodeBlue = pd.colorCritCodeBlue[pdIndex];

	// Depending on the coding, choose a color
	switch (colorCritCodeRed) {
		case PROP1:
			red = p1;
		break;
		case PROP2:
			red = p2;
		break;
		case PROP3:
			red = p3;
		break;
		case PROP4:
			red = p4;
		break;
		case PROP5:
			red = p5;
		break;
		case PROP6:
			red = p6;
		break;
		case PROP7:
			red = p7;
		break;
		case PROP8:
			red = p8;
		break;
		default:
			red = (p1);
		break;
	}

	switch (colorCritCodeGreen) {
		case PROP1:
			green = p1;
		break;
		case PROP2:
			green = p2;
		break;
		case PROP3:
			green = p3;
		break;
		case PROP4:
			green = p4;
		break;
		case PROP5:
			green = p5;
		break;
		case PROP6:
			green = p6;
		break;
		case PROP7:
			green = p7;
		break;
		case PROP8:
			green = p8;
		break;
		default:
			green = (p1);
		break;
	}
	switch (colorCritCodeBlue) {
		case PROP1:
			blue = p1;
		break;
		case PROP2:
			blue = p2;
		break;
		case PROP3:
			blue = p3;
		break;
		case PROP4:
			blue = p4;
		break;
		case PROP5:
			blue = p5;
		break;
		case PROP6:
			blue = p6;
		break;
		case PROP7:
			blue = p7;
		break;
		case PROP8:
			blue = p8;
		break;
		default:
			blue = (p1);
		break;
	}

	// Clamp the values
	if (red < 0.0) 
		red = 0.0;
	if (red > 1.0) 
		red = 1.0;

	if (green < 0.0) 
		green = 0.0;
	if (green > 1.0) 
		green = 1.0;
			
	if (blue < 0.0) 
		blue = 0.0;
	if (blue > 1.0) 
		blue = 1.0;
			
	// Set now the code-dependent color
	glColor3f( red, green, blue );
}
