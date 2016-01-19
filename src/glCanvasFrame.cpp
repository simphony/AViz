//
// Implementation of the OpenGL drawing canvas frame
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

#include "glCanvasFrame.h"

#include <cmath>
#include <limits>

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QToolButton>
#include <QTimer>

#include "defaults.h"
#include "SoQtThumbWheel.h"
#include "mainForm.h"
#include "glCanvasArea.h"
#include "parameterLimits.h" //DOLLY_MAX, DOLLY_MIN

#include "./pixmaps/home.xpm"
#include "./pixmaps/homenew.xpm"
#include "./pixmaps/parallel.xpm"
#include "./pixmaps/perspective.xpm"
#include "./pixmaps/auto.xpm"
#include "./pixmaps/autoRed.xpm"
#include "./pixmaps/left.xpm"
#include "./pixmaps/leftRed.xpm"
#include "./pixmaps/right.xpm"
#include "./pixmaps/rightRed.xpm"
#include "./pixmaps/spinLeft.xpm"
#include "./pixmaps/spinLeftRed.xpm"
#include "./pixmaps/spinRight.xpm"
#include "./pixmaps/spinRightRed.xpm"
#include "./pixmaps/up.xpm"
#include "./pixmaps/upRed.xpm"
#include "./pixmaps/down.xpm"
#include "./pixmaps/downRed.xpm"
#include "./pixmaps/dollyIn.xpm"
#include "./pixmaps/dollyInRed.xpm"
#include "./pixmaps/dollyOut.xpm"
#include "./pixmaps/dollyOutRed.xpm"
#include "./pixmaps/blackBackground.xpm"
#include "./pixmaps/whiteBackground.xpm"
#include "./pixmaps/stereoVisionEn.xpm"
#include "./pixmaps/eyeSeparationPlus.xpm"
#include "./pixmaps/eyeSeparationMinus.xpm"

//  Framed canvas widget constructor and destructor
GLCanvasFrame::GLCanvasFrame(MainForm *mainForm, QWidget* parent)
    : QWidget(parent), mainForm(mainForm)
{
    // Draw a frame in a new window
    drawFrame = new QFrame();
    drawFrame->setContentsMargins(0,0,0,0);
    drawFrame->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    drawFrame->setLineWidth( 2 );

    QVBoxLayout *drawFrameLayout = new QVBoxLayout(drawFrame);
    drawFrameLayout->setSpacing(0);
    drawFrameLayout->setContentsMargins(0,0,0,0);


    // Construct a horizontal box to contain the
    // drawing area and some controls
    QWidget * hb0 = new QWidget(drawFrame);
    hb0->setContentsMargins(0,0,0,0);
    drawFrameLayout->addWidget(hb0);

    QHBoxLayout *hb0Layout = new QHBoxLayout(hb0);
    hb0Layout->setSpacing(0);
    hb0Layout->setContentsMargins(0,0,0,0);


    // Construct a vertical box to contain controls
    QWidget * vb0 = new QWidget(hb0);
    vb0->setContentsMargins(0,0,0,0);
    hb0Layout->addWidget(vb0);


    // Construct a drawing area widget; decorate it with a frame
    QFrame * vb0Frame = new QFrame();
    vb0Frame->setContentsMargins(0,0,0,0);
    hb0Layout->addWidget(vb0Frame);
    vb0Frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    vb0Frame->setLineWidth( 2 );

    QVBoxLayout *vb0FrameLayout = new QVBoxLayout(vb0Frame);
    vb0FrameLayout->setSpacing(0);
    vb0FrameLayout->setContentsMargins(0,0,0,0);

    drawArea = new GLCanvasArea(vb0Frame);
    drawArea->setContentsMargins(0,0,0,0);
    drawArea->setFormAddress( mainForm );
    drawArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    vb0FrameLayout->addWidget(drawArea, 10 /*stretch*/);

    {

        QVBoxLayout *vb0Layout = new QVBoxLayout(vb0);
        vb0Layout->setSpacing(0);
        vb0Layout->setContentsMargins(0,0,0,0);

        // Add a control button that also serves as a label
        z1Button = new QToolButton();
        vb0Layout->addWidget(z1Button);
        z1Pixmap = QPixmap( spinRight_xpm );
        z1RedPixmap = QPixmap( spinRightRed_xpm );
        z1Button->setIcon( z1Pixmap );
        z1Button->setAutoRepeat( TRUE );
        connect( z1Button, SIGNAL(clicked()), SLOT(spin1()) );

        // Add spin control wheel
        spinWheel =new SoQtThumbWheel(vb0);
        vb0Layout->addWidget(spinWheel);
        spinWheel->setOrientation( SoQtThumbWheel::Vertical );
        spinWheel->setFixedWidth( FRAME_WIDTH );
        spinWheel->setRangeBoundaryHandling( SoQtThumbWheel::ACCUMULATE );
        connect(spinWheel, SIGNAL(wheelMoved(float)), this, SLOT(spinWheelMoved(float)));

        // Add a control button that also serves as a label
        z2Button = new QToolButton();
        vb0Layout->addWidget(z2Button);
        z2Pixmap = QPixmap( spinLeft_xpm );
        z2RedPixmap = QPixmap( spinLeftRed_xpm );
        z2Button->setIcon( z2Pixmap );
        z2Button->setAutoRepeat( TRUE );
        connect( z2Button, SIGNAL(clicked()), SLOT(spin2()) );

        // Add a placeholder
        vb0Layout->addStretch(10);

        // Add a control button that also serves as a label
        y1Button = new QToolButton();
        vb0Layout->addWidget(y1Button);
        y1Pixmap = QPixmap( up_xpm );
        y1RedPixmap = QPixmap( upRed_xpm );
        y1Button->setIcon( y1Pixmap );
        y1Button->setAutoRepeat( TRUE );
        connect( y1Button, SIGNAL(clicked()), SLOT(tilt1()) );

        // Add tilt control wheel
        tiltWheel =new SoQtThumbWheel();
        vb0Layout->addWidget(tiltWheel);
        tiltWheel->setOrientation( SoQtThumbWheel::Vertical );
        tiltWheel->setFixedWidth( FRAME_WIDTH );
        tiltWheel->setRangeBoundaryHandling( SoQtThumbWheel::ACCUMULATE );
        connect(tiltWheel, SIGNAL(wheelMoved(float)), this, SLOT(tiltWheelMoved(float)));

        // Add a control button that also serves as a label
        y2Button = new QToolButton();
        vb0Layout->addWidget(y2Button);
        y2Pixmap = QPixmap( down_xpm );
        y2RedPixmap = QPixmap( downRed_xpm );
        y2Button->setIcon( y2Pixmap );
        y2Button->setAutoRepeat( TRUE );
        connect( y2Button, SIGNAL(clicked()), SLOT(tilt2()) );
    }

    {

        // Now construct a horizontal box to contain more controls
        QWidget * hb1 = new QWidget(drawFrame);
        hb1->setContentsMargins(0,0,0,0);
        drawFrameLayout->addWidget(hb1);

        QHBoxLayout *hb1Layout = new QHBoxLayout(hb1);
        hb1Layout->setSpacing(0);
        hb1Layout->setContentsMargins(0,0,0,0);

        // Add a placeholder
        QLabel * emptyL2 = new QLabel("");
        hb1Layout->addWidget(emptyL2, 10 /*stretch*/);
        emptyL2->setFixedWidth( FRAME_WIDTH );

        // Add a control button that also serves as a label
        x1Button = new QToolButton();
        hb1Layout->addWidget(x1Button);
        x1Pixmap = QPixmap( left_xpm );
        x1RedPixmap = QPixmap( leftRed_xpm );
        x1Button->setIcon( x1Pixmap );
        x1Button->setAutoRepeat( TRUE );
        connect( x1Button, SIGNAL(clicked()), SLOT(rot1()) );

        // Add rotation control wheel
        rotWheel =new SoQtThumbWheel(hb1);
        hb1Layout->addWidget(rotWheel);
        rotWheel->setOrientation( SoQtThumbWheel::Horizontal );
        rotWheel->setFixedHeight( FRAME_WIDTH );
        rotWheel->setRangeBoundaryHandling( SoQtThumbWheel::ACCUMULATE );
        connect(rotWheel, SIGNAL(wheelMoved(float)), this, SLOT(rotWheelMoved(float)));

        // Add a control button that also serves as a label
        x2Button = new QToolButton();
        hb1Layout->addWidget(x2Button);
        x2Pixmap = QPixmap( right_xpm );
        x2RedPixmap = QPixmap( rightRed_xpm );
        x2Button->setIcon( x2Pixmap );
        x2Button->setAutoRepeat( TRUE );
        connect( x2Button, SIGNAL(clicked()), SLOT(rot2()) );

        // Add a placeholder
        hb1Layout->addStretch(1);

        // Insert a button to lock the controls and switch to
        // auto motion mode
        autoButton = new QToolButton();
        hb1Layout->addWidget(autoButton);
        autoPixmap = QPixmap( auto_xpm );
        autoRedPixmap = QPixmap( autoRed_xpm );
        autoButton->setIcon( autoPixmap );
        autoButton->setCheckable(true);
        connect( autoButton, SIGNAL(clicked()), this, SLOT(autoButtonCB()) );


        // Add a placeholder
        QLabel * emptyL1a = new QLabel("");
        emptyL1a->setFixedWidth(FRAME_WIDTH);
        hb1Layout->addWidget(emptyL1a);
    }


    {
        // Construct a vertical box to contain controls
        QWidget * vb1 = new QWidget();
        vb1->setContentsMargins(0,0,0,0);
        hb0Layout->addWidget(vb1);

        QVBoxLayout *vb1Layout = new QVBoxLayout(vb1);
        vb1Layout->setSpacing(0);
        vb1Layout->setContentsMargins(0,0,0,0);

        // Add home button
        setHomeButton = new QToolButton();
        vb1Layout->addWidget(setHomeButton);
        setHomePixmap = QPixmap( home_xpm );
        setHomeButton->setIcon( setHomePixmap );
        connect( setHomeButton, SIGNAL(clicked()), SLOT(setHome()) );

        // Add set home button
        setNewHomeButton = new QToolButton();
        vb1Layout->addWidget(setNewHomeButton);
        setNewHomePixmap = QPixmap( homenew_xpm );
        setNewHomeButton->setIcon( setNewHomePixmap );
        connect( setNewHomeButton, SIGNAL(clicked()), SLOT(setNewHome()) );

        // Add ortho perspective button
        parallelButton = new QToolButton();
        vb1Layout->addWidget(parallelButton);
        parallelPixmap = QPixmap( parallel_xpm );
        parallelButton->setIcon( parallelPixmap );
        parallelButton->setCheckable(true);
        connect( parallelButton, SIGNAL(clicked()), SLOT(setParallel()) );

        // Add 3D perspective button
        perspectiveButton = new QToolButton();
        vb1Layout->addWidget(perspectiveButton);
        perspectivePixmap = QPixmap( perspective_xpm );
        perspectiveButton->setIcon( perspectivePixmap );
        perspectiveButton->setCheckable(true);
        connect( perspectiveButton, SIGNAL(clicked()), SLOT(setPerspective()) );

        // Add background buttons
        blackBackgroundButton = new QToolButton();
        vb1Layout->addWidget(blackBackgroundButton);
        blackBackgroundPixmap = QPixmap( blackBackground_xpm );
        blackBackgroundButton->setIcon( blackBackgroundPixmap );
        blackBackgroundButton->setCheckable(true);
        connect( blackBackgroundButton, SIGNAL(clicked()), SLOT(setBlackBackground()) );

        whiteBackgroundButton = new QToolButton();
        vb1Layout->addWidget(whiteBackgroundButton);
        whiteBackgroundPixmap = QPixmap( whiteBackground_xpm );
        whiteBackgroundButton->setIcon( whiteBackgroundPixmap );
        whiteBackgroundButton->setCheckable(true);
        connect( whiteBackgroundButton, SIGNAL(clicked()), SLOT(setWhiteBackground()) );

        // Add stereo vision enable/disable button
        stereoVisionButton = new QToolButton();
        vb1Layout->addWidget(stereoVisionButton);
        stereoVisionPixmap = QPixmap( stereoVisionEn_xpm );
        stereoVisionButton->setIcon( stereoVisionPixmap );
        stereoVisionButton->setCheckable(true);
        connect( stereoVisionButton, SIGNAL(clicked()), SLOT(toggleStereoVision()) );

        // Add eyes separation "plus" button
        eyeSeparationPlusButton = new QToolButton();
        vb1Layout->addWidget(eyeSeparationPlusButton);
        eyeSeparationPlusPixmap = QPixmap( eyeSeparationPlus_xpm );
        eyeSeparationPlusButton->setIcon( eyeSeparationPlusPixmap );
        eyeSeparationPlusButton->setAutoRepeat( TRUE );
        connect( eyeSeparationPlusButton, SIGNAL(clicked()), SLOT(eyeSeparationPlus()) );

        // Add eyes separation "minus" button
        eyeSeparationMinusButton = new QToolButton();
        vb1Layout->addWidget(eyeSeparationMinusButton);
        eyeSeparationMinusPixmap = QPixmap( eyeSeparationMinus_xpm );
        eyeSeparationMinusButton->setIcon( eyeSeparationMinusPixmap );
        eyeSeparationMinusButton->setAutoRepeat( TRUE );
        connect( eyeSeparationMinusButton, SIGNAL(clicked()), SLOT(eyeSeparationMinus()) );

        // Add a placeholder
        vb1Layout->addStretch(10);

        // Add a control button that also serves as a label
        dolly2Button = new QToolButton();
        vb1Layout->addWidget(dolly2Button);
        dolly2Pixmap = QPixmap( dollyOut_xpm );
        dolly2RedPixmap = QPixmap( dollyOutRed_xpm );
        dolly2Button->setIcon( dolly2Pixmap );
        dolly2Button->setAutoRepeat( TRUE );
        connect( dolly2Button, SIGNAL(clicked()), SLOT(dolly2()) );

        // Add dolly control wheel
        dollyWheel =new SoQtThumbWheel(vb1);
        vb1Layout->addWidget(dollyWheel);
        dollyWheel->setOrientation( SoQtThumbWheel::Vertical );
        dollyWheel->setFixedWidth( FRAME_WIDTH );
        dollyWheel->setRangeBoundaryHandling( SoQtThumbWheel::ACCUMULATE );
        connect(dollyWheel, SIGNAL(wheelMoved(float)), this, SLOT(dollyWheelMoved(float)));

        // Add a control button that also serves as a label
        dolly1Button = new QToolButton();
        vb1Layout->addWidget(dolly1Button);
        dolly1Pixmap = QPixmap( dollyIn_xpm );
        dolly1RedPixmap = QPixmap( dollyInRed_xpm );
        dolly1Button->setIcon( dolly1Pixmap );
        dolly1Button->setAutoRepeat( TRUE );
        connect( dolly1Button, SIGNAL(clicked()), SLOT(dolly1()) );
    }

    // Construct timers
    rotTimer1 = new QTimer( this );
    rotTimer2 = new QTimer( this );
    tiltTimer1 = new QTimer( this );
    tiltTimer2 = new QTimer( this );
    spinTimer1 = new QTimer( this );
    spinTimer2 = new QTimer( this );
    zoomTimer1 = new QTimer( this );
    zoomTimer2 = new QTimer( this );

    // Reset a flag
    autoMode = FALSE;

    // Show the new window
    drawFrame->setGeometry( 0, 0, FRAME_SIZE, FRAME_SIZE );
    drawFrame->show();
    drawFrame->raise();
}


GLCanvasFrame::~GLCanvasFrame()
{
}


// Find new view object data and recompile aggregated data
void GLCanvasFrame::updateView()
{
    viewObject thisVo = this->getObjectData();

    // Cause a recompile of the drawing lists
    if (drawArea) {
        drawArea->setViewObjectAndRecompile( thisVo );
    }
}


// Recompile aggregate data without changing the view object data
// (useful for sequences)
void GLCanvasFrame::updateViewWithoutViewObjectChange()
{

    if (drawArea) {
        // Read current view object...
        viewObject * thisVo = drawArea->getViewObject();

        // ..and cause a recompile of the drawing lists
        drawArea->setViewObjectAndRecompile( (*thisVo) );
    }
}


// Produce information on the aggregate data that is useful 
// for rendering: the extensions and the center location, 
// and maxima and minima of properties
viewObject GLCanvasFrame::getObjectData()
{
    viewObject vo;

    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float zmin = std::numeric_limits<float>::max();
    float xmax = -std::numeric_limits<float>::max();
    float ymax = -std::numeric_limits<float>::max();
    float zmax = -std::numeric_limits<float>::max();

    float p1min = std::numeric_limits<float>::max();
    float p2min = std::numeric_limits<float>::max();
    float p3min = std::numeric_limits<float>::max();
    float p4min = std::numeric_limits<float>::max();
    float p5min = std::numeric_limits<float>::max();
    float p6min = std::numeric_limits<float>::max();
    float p7min = std::numeric_limits<float>::max();
    float p8min = std::numeric_limits<float>::max();
    float p1max = -std::numeric_limits<float>::max();
    float p2max = -std::numeric_limits<float>::max();
    float p3max = -std::numeric_limits<float>::max();
    float p4max = -std::numeric_limits<float>::max();
    float p5max = -std::numeric_limits<float>::max();
    float p6max = -std::numeric_limits<float>::max();
    float p7max = -std::numeric_limits<float>::max();
    float p8max = -std::numeric_limits<float>::max();

    // Get the current aggregate data
    if (drawArea) {
        aggregateData * ad = drawArea->getAggregateData();

        // Find the min and max extensions
        for (int i=0;i<(*ad).numberOfParticles;i++) {
            if ((*ad).particles[i].x < xmin)
                xmin = (*ad).particles[i].x;
            if ((*ad).particles[i].y < ymin)
                ymin = (*ad).particles[i].y;
            if ((*ad).particles[i].z < zmin)
                zmin = (*ad).particles[i].z;
            if ((*ad).particles[i].x > xmax)
                xmax = (*ad).particles[i].x;
            if ((*ad).particles[i].y > ymax)
                ymax = (*ad).particles[i].y;
            if ((*ad).particles[i].z > zmax)
                zmax = (*ad).particles[i].z;
            
             if ((*ad).particles[i].prop1 < p1min)
                p1min = (*ad).particles[i].prop1;
            if ((*ad).particles[i].prop2 < p2min)
                p2min = (*ad).particles[i].prop2;
            if ((*ad).particles[i].prop3 < p3min)
                p3min = (*ad).particles[i].prop3;
            if ((*ad).particles[i].prop4 < p4min)
                p4min = (*ad).particles[i].prop4;
            if ((*ad).particles[i].prop5 < p5min)
                p5min = (*ad).particles[i].prop5;
            if ((*ad).particles[i].prop6 < p6min)
                p6min = (*ad).particles[i].prop6;
            if ((*ad).particles[i].prop7 < p7min)
                p7min = (*ad).particles[i].prop7;
            if ((*ad).particles[i].prop8 < p8min)
                p8min = (*ad).particles[i].prop8;

            if ((*ad).particles[i].prop1 > p1max)
                p1max = (*ad).particles[i].prop1;
            if ((*ad).particles[i].prop2 > p2max)
                p2max = (*ad).particles[i].prop2;
            if ((*ad).particles[i].prop3 > p3max)
                p3max = (*ad).particles[i].prop3;
            if ((*ad).particles[i].prop4 > p4max)
                p4max = (*ad).particles[i].prop4;
            if ((*ad).particles[i].prop5 > p5max)
                p5max = (*ad).particles[i].prop5;
            if ((*ad).particles[i].prop6 > p6max)
                p6max = (*ad).particles[i].prop6;
            if ((*ad).particles[i].prop7 > p7max)
                p7max = (*ad).particles[i].prop7;
            if ((*ad).particles[i].prop8 > p8max)
                p8max = (*ad).particles[i].prop8;
        }
    }

    vo.xmin = xmin;
    vo.ymin = ymin;
    vo.zmin = zmin;
    vo.xmax = xmax;
    vo.ymax = ymax;
    vo.zmax = zmax;

    vo.deltax = vo.xmax-vo.xmin;
    vo.deltay = vo.ymax-vo.ymin;
    vo.deltaz = vo.zmax-vo.zmin;

    vo.p1min = p1min;
    vo.p2min = p2min;
    vo.p3min = p3min;
    vo.p4min = p4min;
    vo.p5min = p5min;
    vo.p6min = p6min;
    vo.p7min = p7min;
    vo.p8min = p7min;
    vo.p1max = p1max;
    vo.p2max = p2max;
    vo.p3max = p3max;
    vo.p4max = p4max;
    vo.p5max = p5max;
    vo.p6max = p6max;
    vo.p7max = p7max;
    vo.p8max = p8max;

    vo.deltap1 = vo.p1max-vo.p1min;
    vo.deltap2 = vo.p2max-vo.p2min;
    vo.deltap3 = vo.p3max-vo.p3min;
    vo.deltap4 = vo.p4max-vo.p4min;
    vo.deltap5 = vo.p5max-vo.p5min;
    vo.deltap6 = vo.p6max-vo.p6min;
    vo.deltap7 = vo.p7max-vo.p7min;
    vo.deltap8 = vo.p8max-vo.p8min;

    if (vo.deltax < DELTAEPS)
        vo.deltax = DELTAEPS;
    if (vo.deltay < DELTAEPS)
        vo.deltay = DELTAEPS;
    if (vo.deltaz < DELTAEPS)
        vo.deltaz = DELTAEPS;

    if (vo.deltap1 < DELTAEPS)
        vo.deltap1 = DELTAEPS;
    if (vo.deltap2 < DELTAEPS)
        vo.deltap2 = DELTAEPS;
    if (vo.deltap3 < DELTAEPS)
        vo.deltap3 = DELTAEPS;
    if (vo.deltap4 < DELTAEPS)
        vo.deltap4 = DELTAEPS;
    if (vo.deltap5 < DELTAEPS)
        vo.deltap5 = DELTAEPS;
    if (vo.deltap6 < DELTAEPS)
        vo.deltap6 = DELTAEPS;
    if (vo.deltap7 < DELTAEPS)
        vo.deltap7 = DELTAEPS;
    if (vo.deltap8 < DELTAEPS)
        vo.deltap8 = DELTAEPS;

    vo.xyzmin = vo.xmin;
    if (vo.ymin < vo.xyzmin)
        vo.xyzmin = vo.ymin;
    if (vo.zmin < vo.xyzmin)
        vo.xyzmin = vo.zmin;

    vo.xyzmax = vo.xmax;
    if (vo.ymax > vo.xyzmax)
        vo.xyzmax = vo.ymax;
    if (vo.zmax > vo.xyzmax)
        vo.xyzmax = vo.zmax;

    // Find the max extension of the faces
    vo.maxXYExtension = (vo.xmax-vo.xmin);
    if ((vo.ymax-vo.ymin) > vo.maxXYExtension)
        vo.maxXYExtension = (vo.ymax-vo.ymin);
    vo.maxXZExtension = (vo.xmax-vo.xmin);
    if ((vo.zmax-vo.zmin) > vo.maxXZExtension)
        vo.maxXZExtension = (vo.zmax-vo.zmin);
    vo.maxYZExtension = (vo.ymax-vo.ymin);
    if ((vo.zmax-vo.zmin) > vo.maxYZExtension)
        vo.maxYZExtension = (vo.zmax-vo.zmin);
    vo.maxExtension = vo.maxXYExtension;
    if (vo.maxXZExtension > vo.maxExtension)
        vo.maxExtension = vo.maxXZExtension;
    if (vo.maxYZExtension > vo.maxExtension)
        vo.maxExtension = vo.maxYZExtension;

    // Find the center of the system
    vo.centerx = (vo.xmax+vo.xmin)/2.0;
    vo.centery = (vo.ymax+vo.ymin)/2.0;
    vo.centerz = (vo.zmax+vo.zmin)/2.0;

    return vo;
}


// Respond to thumb wheel movement 
// and do rotation steps
void GLCanvasFrame::rotWheelMoved( float r )
{
    static float oldValue = 0.0;

    float diff = r - oldValue;
    float adiff = fabs(diff);

    if (diff > 0.0)
        this->rotStep( 1, (int)floor(WHEEL_REPEAT*adiff+0.5) );
    else
        this->rotStep( 0, (int)floor(WHEEL_REPEAT*adiff+0.5) );


    this->updateRendering();

    oldValue = r;
}


// Respond to frame button click
// and do rotation steps
void GLCanvasFrame::rot1()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do rotation steps
    if (autoMode) {
        switch (x1Button->isChecked()) {
        case TRUE:
            x1Button->setIcon( x1RedPixmap );

            // Start the rotation
            connect(rotTimer1, SIGNAL(timeout()), this, SLOT(rot1SingleStep()) );
            rotTimer1->start( 40 );
            (*vp).autoRot1 = TRUE;
            break;
        case FALSE:
            x1Button->setIcon( x1Pixmap );

            // End the rotation
            rotTimer1->stop();
            (*vp).autoRot1 = FALSE;
            break;
        }
    }
    else {
        this->rot1SingleStep();
    }
}


// Do a single rotation step
void GLCanvasFrame::rot1SingleStep()
{	
    this->rotStep( 0, MIN_REPEAT );

    this->updateRendering();
}


// Respond to frame button click
// and do rotation steps
void GLCanvasFrame::rot2()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do rotation steps
    if (autoMode) {
        switch (x2Button->isChecked()) {
        case TRUE:
            x2Button->setIcon( x2RedPixmap );

            // Start the rotation
            connect(rotTimer2, SIGNAL(timeout()), this, SLOT(rot2SingleStep()) );
            rotTimer2->start( 40 );
            (*vp).autoRot2 = TRUE;
            break;
        case FALSE:
            x2Button->setIcon( x2Pixmap );

            // End the rotation
            rotTimer2->stop();
            (*vp).autoRot2 = FALSE;
            break;
        }
    }
    else {
        this->rot2SingleStep();
    }
}


// Do a single rotation step
void GLCanvasFrame::rot2SingleStep()
{	
    this->rotStep( 1, MIN_REPEAT );

    this->updateRendering();
}


// Respond to thumb wheel movement 
// and do tilt steps
void GLCanvasFrame::tiltWheelMoved( float r )
{
    static float oldValue = 0.0;

    float diff = r - oldValue;
    float adiff = fabs(diff);

    if (diff > 0.0)
        this->tiltStep( 0, (int)floor(WHEEL_REPEAT*adiff+0.5) );
    else
        this->tiltStep( 1, (int)floor(WHEEL_REPEAT*adiff+0.5) );

    this->updateRendering();

    oldValue = r;
}


// Respond to frame button click
// and do tilt steps
void GLCanvasFrame::tilt1()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do tilt steps
    if (autoMode) {
        switch (y1Button->isChecked()) {
        case TRUE:
            y1Button->setIcon( y1RedPixmap );

            // Start the tilt
            connect(tiltTimer1, SIGNAL(timeout()), this, SLOT(tilt1SingleStep()) );
            tiltTimer1->start( 40 );
            (*vp).autoTilt1 = TRUE;
            break;
        case FALSE:
            y1Button->setIcon( y1Pixmap );

            // End the tilt
            tiltTimer1->stop();
            (*vp).autoTilt1 = FALSE;
            break;
        }
    }
    else {
        this->tilt1SingleStep();
    }
}


// Do a single tilt step
void GLCanvasFrame::tilt1SingleStep()
{	
    this->tiltStep( 1, MIN_REPEAT );

    this->updateRendering();
}


// Respond to frame button click
// and do tilt steps
void GLCanvasFrame::tilt2()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do tilt steps
    if (autoMode) {
        switch (y2Button->isChecked()) {
        case TRUE:
            y2Button->setIcon( y2RedPixmap );

            // Start the tilt
            connect(tiltTimer2, SIGNAL(timeout()), this, SLOT(tilt2SingleStep()) );
            tiltTimer2->start( 40 );
            (*vp).autoTilt2 = TRUE;
            break;
        case FALSE:
            y2Button->setIcon( y2Pixmap );

            // End the tilt
            tiltTimer2->stop();
            (*vp).autoTilt2 = FALSE;
            break;
        }
    }
    else {
        this->tilt2SingleStep();
    }
}


// Do a single tilt step
void GLCanvasFrame::tilt2SingleStep()
{	
    this->tiltStep( 0, MIN_REPEAT );

    this->updateRendering();
}


// Respond to thumb wheel movement 
// and do spin steps
void GLCanvasFrame::spinWheelMoved( float r )
{
    static float oldValue = 0.0;

    float diff = r - oldValue;
    float adiff = fabs(diff);

    if (diff > 0.0)
        this->spinStep( 1, (int)floor(WHEEL_REPEAT*adiff+0.5) );
    else
        this->spinStep( 0, (int)floor(WHEEL_REPEAT*adiff+0.5) );

    this->updateRendering();

    oldValue = r;
}


// Respond to frame button click
// and do spin steps
void GLCanvasFrame::spin1()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do spin steps
    if (autoMode) {
        switch (z1Button->isChecked()) {
        case TRUE:
            z1Button->setIcon( z1RedPixmap );

            // Start the spin
            connect(spinTimer1, SIGNAL(timeout()), this, SLOT(spin1SingleStep()) );
            spinTimer1->start( 40 );
            (*vp).autoSpin1 = TRUE;
            break;
        case FALSE:
            z1Button->setIcon( z1Pixmap );

            // End the spin
            spinTimer1->stop();
            (*vp).autoSpin1 = FALSE;
            break;
        }
    }
    else {
        this->spin1SingleStep();
    }
}


// Do a single spin step
void GLCanvasFrame::spin1SingleStep()
{	
    this->spinStep( 0, MIN_REPEAT );

    this->updateRendering();
}


// Respond to frame button click
// and do spin steps
void GLCanvasFrame::spin2()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do spin steps
    if (autoMode) {
        switch (z2Button->isChecked()) {
        case TRUE:
            z2Button->setIcon( z2RedPixmap );

            // Start the spin
            connect(spinTimer2, SIGNAL(timeout()), this, SLOT(spin2SingleStep()) );
            spinTimer2->start( 40 );
            (*vp).autoSpin2 = TRUE;
            break;
        case FALSE:
            z2Button->setIcon( z2Pixmap );

            // End the spin
            spinTimer2->stop();
            (*vp).autoSpin2 = FALSE;
            break;
        }
    }
    else {
        this->spin2SingleStep();
    }
}


// Do a single spin step
void GLCanvasFrame::spin2SingleStep()
{	
    this->spinStep( 1, MIN_REPEAT );

    this->updateRendering();
}


// Respond to thumb wheel movement 
// and do zoom steps
void GLCanvasFrame::dollyWheelMoved( float r )
{
    static float oldValue = 0.0;

    float diff = r - oldValue;
    float adiff = fabs(diff);

    if (diff > 0.0)
        this->zoomStep( 0, (int)floor(WHEEL_REPEAT*adiff+0.5) );
    else
        this->zoomStep( 1, (int)floor(WHEEL_REPEAT*adiff+0.5) );

    this->updateRendering();

    oldValue = r;
}


// Respond to frame button click
// and do zoom steps
void GLCanvasFrame::dolly1()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do zoom steps
    if (autoMode) {
        switch (dolly1Button->isChecked()) {
        case TRUE:
            dolly1Button->setIcon( dolly1RedPixmap );

            // Start the zoom
            connect(zoomTimer1, SIGNAL(timeout()), this, SLOT(dolly1SingleStep()) );
            zoomTimer1->start( 40 );
            (*vp).autoZoom1 = TRUE;
            break;
        case FALSE:
            dolly1Button->setIcon( dolly1Pixmap );

            // End the zoom
            zoomTimer1->stop();
            (*vp).autoZoom1 = FALSE;
            break;
        }
    }
    else {
        this->dolly1SingleStep();
    }
}

// Do a single zoom step
void GLCanvasFrame::dolly1SingleStep()
{	
    this->zoomStep( 0, MIN_ZOOM_REPEAT );

    this->updateRendering();
}


// Respond to frame button click
// and do zoom steps
void GLCanvasFrame::dolly2()
{	
    viewParam * vp = this->getViewParam();

    // Change pixmap in auto mode while button is pressed,
    // and do zoom steps
    if (autoMode) {
        switch (dolly2Button->isChecked()) {
        case TRUE:
            dolly2Button->setIcon( dolly2RedPixmap );

            // Start the zoom
            connect(zoomTimer2, SIGNAL(timeout()), this, SLOT(dolly2SingleStep()) );
            zoomTimer2->start( 40 );
            (*vp).autoZoom2 = TRUE;
            break;
        case FALSE:
            dolly2Button->setIcon( dolly2Pixmap );

            // End the zoom
            zoomTimer2->stop();
            (*vp).autoZoom2 = FALSE;
            break;
        }
    }
    else {
        this->dolly2SingleStep();
    }
}


// Do a single zoom step
void GLCanvasFrame::dolly2SingleStep()
{	
    this->zoomStep( 1, MIN_ZOOM_REPEAT );

    this->updateRendering();
}


// Do one rotation step forward or backward
// (overloaded function)
void GLCanvasFrame::rotStep( char mdir )
{
    viewParam * vp = this->getViewParam();
    double delta, deltaPhi, deltaTheta, deltaChi;

    if (mdir) {
        delta = 1.0;
    }
    else {
        delta = -1.0;
    }

    // The components of the angular velocity along the fixed
    // space axis must be (0 0 1) -- this corresponds to rotating.
    computeTransformation( delta, 0.0, 0.0, 1.0, &deltaPhi, &deltaTheta, &deltaChi );

    (*vp).phi += deltaPhi;
    (*vp).theta += deltaTheta;
    (*vp).chi += deltaChi;
}


// Do one rotation step forward or backward
// (overloaded function)
void GLCanvasFrame::rotStep( char mdir, int n )
{
    for (int i=0;i<n;i++)
        this->rotStep( mdir );
}


// Do one tilt step forward or backward
// (overloaded function)
void GLCanvasFrame::tiltStep( char mdir )
{
    viewParam * vp = this->getViewParam();
    double delta, deltaPhi, deltaTheta, deltaChi;

    if (mdir) {
        delta = 0.01;
    }
    else {
        delta = -0.01;
    }

    // The components of the angular velocity along the fixed
    // space axis must be (1 0 0) -- this corresponds to tilting.
    computeTransformation( delta, 1.0, 0.0, 0.0, &deltaPhi, &deltaTheta, &deltaChi );

    (*vp).phi += deltaPhi;
    (*vp).theta += deltaTheta;
    (*vp).chi += deltaChi;
}


// Do one tilt step forward or backward
// (overloaded function)
void GLCanvasFrame::tiltStep( char mdir, int n )
{
    for (int i=0;i<n;i++)
        this->tiltStep( mdir );
}

// Do one spin step forward or backward
// (overoaded function)
void GLCanvasFrame::spinStep( char mdir )
{
    viewParam * vp = this->getViewParam();
    double delta, deltaPhi, deltaTheta, deltaChi;

    if (mdir) {
        delta = 0.01;
    }
    else {
        delta = -0.01;
    }

    // The components of the angular velocity along the fixed
    // space axis must be (0 1 0) -- this corresponds to spinning.
    computeTransformation( delta, 0.0, 1.0, 0.0, &deltaPhi, &deltaTheta, &deltaChi );

    (*vp).phi += deltaPhi;
    (*vp).theta += deltaTheta;
    (*vp).chi += deltaChi;
}


// Do one spin step forward or backward
// (overloaded function)
void GLCanvasFrame::spinStep( char mdir, int n )
{
    for (int i=0;i<n;i++)
        this->spinStep( mdir );
}


// Do one zoom step forward or backward
// (overloaded function)
void GLCanvasFrame::zoomStep( char mdir )
{
    viewParam * vp = this->getViewParam();

    if (mdir)
        (*vp).dolly -= 1.0;
    else
        (*vp).dolly += 1.0;

    if ((*vp).dolly > DOLLY_MAX)
        (*vp).dolly = DOLLY_MAX;
    if ((*vp).dolly < DOLLY_MIN)
        (*vp).dolly = DOLLY_MIN;
}


// Do one zoom step forward or backward
// (overloaded function)
void GLCanvasFrame::zoomStep( char mdir, int n )
{
    for (int i=0;i<n;i++)
        this->zoomStep( mdir );
}


// Compute the phi/theta/chi transformation that corresponds to 
// a small rotation about a given axis.  The axis direction is 
// given in the arguments and is expected to be a unit vector
void GLCanvasFrame::computeTransformation( double delta, double w1, double w2, double w3, double * deltaPhi, double * deltaTheta, double * deltaChi)
{
    viewParam * vp = this->getViewParam();
    double norm2, factor = 1.0;
    double phiRad = (double)(*vp).phi*PI/180.0;
    double thetaRad = (double)(*vp).theta*PI/180.0;
    //	double chiRad = (double)(*vp).chi*PI/180.0;

    // The components of the angular velocity along the fixed
    // space axis are
    //      w1 = cos(phi)(theta') + sin(theta)sin(phi)(chi')
    //      w2 = sin(phi)(theta') - sin(theta)cos(phi)(chi')
    //      w3 = (phi') + cos(theta)(chi')
    // where (x') is the time derivative (i.e., the delta)

    // Tilting (direction vector [1 0 0] and Spinning
    // (directon vector [0 1 0]) is achieved by changing all three angles
    // simultaneously.  For rotation (direction vector [0 0 1]), it is
    // sufficient to change only phi
    if (fabs(tan(thetaRad)) > SCEPS)
        (*deltaPhi) = -w1*delta/tan(thetaRad)*sin(phiRad) + w2*delta/tan(thetaRad)*cos(phiRad) + w3*delta;
    else
        (*deltaPhi) = -w1*delta/SCEPS*sin(phiRad) + w2*delta/SCEPS*cos(phiRad) + w3*delta;

    (*deltaTheta) = w1*delta*cos(phiRad) + w2*delta*sin(phiRad);

    if (fabs(sin(thetaRad)) > SCEPS)
        (*deltaChi) = w1*delta/sin(thetaRad)*sin(phiRad) - w2*delta/sin(thetaRad)*cos(phiRad);
    else
        (*deltaChi) = w1*delta/SCEPS*sin(phiRad) - w2*delta/SCEPS*cos(phiRad);

    if ((*deltaPhi) > 0.0) {
        double phiMod = floor((*deltaPhi)/TWO_PI);
        (*deltaPhi) -= phiMod*TWO_PI;
    }
    else {
        double phiMod = floor(-(*deltaPhi)/TWO_PI);
        (*deltaPhi) += phiMod*TWO_PI;
    }
    if ((*deltaTheta) > 0.0) {
        double thetaMod = floor((*deltaTheta)/TWO_PI);
        (*deltaTheta) -= thetaMod*TWO_PI;
    }
    else {
        double thetaMod = floor(-(*deltaTheta)/TWO_PI);
        (*deltaTheta) += thetaMod*TWO_PI;
    }
    if ((*deltaChi) > 0.0) {
        double chiMod = floor((*deltaChi)/TWO_PI);
        (*deltaChi) -= chiMod*TWO_PI;
    }
    else {
        double chiMod = floor(-(*deltaChi)/TWO_PI);
        (*deltaChi) += chiMod*TWO_PI;
    }

    norm2 = (*deltaPhi)*(*deltaPhi) + (*deltaTheta)*(*deltaTheta) + (*deltaChi)*(*deltaChi);
    if (norm2 > 0.0) {
        // The components should be constant
        if (fabs(w1) > fabs(w2) && fabs(w1) > fabs(w3)) {
            double neww1 = cos(phiRad)*(*deltaTheta)/sqrt(norm2) + sin(thetaRad)*sin(phiRad)*(*deltaChi)/sqrt(norm2);

            if (fabs(neww1) > SCEPS)
                factor = 1.0/fabs(neww1);
            else
                factor = 1.0/SCEPS;
        }

        if (fabs(w2) > fabs(w1) && fabs(w2) > fabs(w3)) {
            double neww2 = sin(phiRad)*(*deltaTheta)/sqrt(norm2) - sin(thetaRad)*cos(phiRad)*(*deltaChi)/sqrt(norm2);
            if (fabs(neww2) > SCEPS)
                factor = 1.0/fabs(neww2);
            else
                factor = 1.0/SCEPS;
        }

        if (fabs(w3) > fabs(w1) && fabs(w3) > fabs(w2)) {
            double neww3 = (*deltaPhi)/sqrt(norm2) + cos(thetaRad)*(*deltaChi)/sqrt(norm2);

            if (fabs(neww3) > SCEPS)
                factor = 1.0/fabs(neww3);
            else
                factor = 1.0/SCEPS;
        }

        (*deltaPhi) /= (sqrt(norm2)/factor);
        (*deltaTheta) /= (sqrt(norm2)/factor);
        (*deltaChi) /= (sqrt(norm2)/factor);
    }
}


// Respond to frame button click:
// Switch to default view point
void GLCanvasFrame::setHome()
{	
    viewParam * vp = this->getViewParam();

    (*vp).phi = (*vp).phiHome;
    (*vp).theta = (*vp).thetaHome;
    (*vp).chi = (*vp).chiHome;
    (*vp).dolly = (*vp).dollyHome;
    (*vp).fovy = (*vp).fovyHome;

    this->updateRendering();
}


// Respond to frame button click
// Define new default view point
void GLCanvasFrame::setNewHome()
{	
    viewParam * vp = this->getViewParam();

    (*vp).phiHome = (*vp).phi;
    (*vp).thetaHome = (*vp).theta;
    (*vp).chiHome = (*vp).chi;
    (*vp).dollyHome = (*vp).dolly;
    (*vp).fovyHome = (*vp).fovy;

    this->updateRendering();
}


// Respond to frame button click
void GLCanvasFrame::setParallel()
{	
    viewParam * vp = this->getViewParam();

    (*vp).viewMode = ORTHO;
    this->setHome();
    this->updateRendering();

    // Adjust button settings
    this->adjustButtons();
}

// Respond to frame button click
void GLCanvasFrame::setPerspective()
{	
    viewParam * vp = this->getViewParam();

    (*vp).viewMode = PERSPECTIVE;
    this->setHome();
    this->updateRendering();

    // Adjust button settings
    this->adjustButtons();
}


// Respond to frame button click
void GLCanvasFrame::setBlackBackground()
{	
    viewParam * vp = this->getViewParam();

    (*vp).background = BGBLACK;
    this->updateRendering();

    // Adjust button settings
    this->adjustButtons();
}


// Respond to frame button click
void GLCanvasFrame::setWhiteBackground()
{	
    viewParam * vp = this->getViewParam();

    (*vp).background = BGWHITE;
    this->updateRendering();

    // Adjust button settings
    this->adjustButtons();
}

// Respond to stereo vision button click
void GLCanvasFrame::toggleStereoVision()
{	
    viewParam * vp = this->getViewParam();

    (*vp).stereoVisionEn = !(*vp).stereoVisionEn;
    this->updateRendering();

    // Adjust button settings
    this->adjustButtons();
}


// Respond to eye separaion plus button click
void GLCanvasFrame::eyeSeparationPlus()
{	
    viewParam * vp = this->getViewParam();

    (*vp).eyeSeparation += 0.01;
    this->updateRendering();
}

// Respond to eye separaion minus button click
void GLCanvasFrame::eyeSeparationMinus()
{	
    viewParam * vp = this->getViewParam();

    if ((*vp).eyeSeparation > 0.01)
        (*vp).eyeSeparation -= 0.01;
    else
        (*vp).eyeSeparation = 0.01;
    this->updateRendering();
}


// Prepare auto mode
void GLCanvasFrame::autoButtonCB()
{
    viewParam * vp = this->getViewParam();

    // Set a flag
    if (autoMode)
        autoMode = FALSE;
    else
        autoMode = TRUE;

    // Set default pixmaps when switching off auto mode;
    // release control buttons
    if (!autoMode) {
        autoButton->setIcon( autoPixmap );
        if (autoButton->isChecked())
            autoButton->toggle();

        if (x1Button->isChecked()) {
            x1Button->toggle();
            x1Button->setIcon( x1Pixmap );
        }
        if (x2Button->isChecked()) {
            x2Button->toggle();
            x2Button->setIcon( x2Pixmap );
        }
        if (y1Button->isChecked()) {
            y1Button->toggle();
            y1Button->setIcon( y1Pixmap );
        }
        if (y2Button->isChecked()) {
            y2Button->toggle();
            y2Button->setIcon( y2Pixmap );
        }
        if (z1Button->isChecked()) {
            z1Button->toggle();
            z1Button->setIcon( z1Pixmap );
        }
        if (z2Button->isChecked()) {
            z2Button->toggle();
            z2Button->setIcon( z2Pixmap );
        }
        if (dolly1Button->isChecked()) {
            dolly1Button->toggle();
            dolly1Button->setIcon( dolly1Pixmap );
        }
        if (dolly2Button->isChecked()) {
            dolly2Button->toggle();
            dolly2Button->setIcon( dolly2Pixmap );
        }

        // Stop all timers
        rotTimer1->stop();
        (*vp).autoRot1 = FALSE;
        rotTimer2->stop();
        (*vp).autoRot2 = FALSE;
        tiltTimer1->stop();
        (*vp).autoTilt1 = FALSE;
        tiltTimer2->stop();
        (*vp).autoTilt2 = FALSE;
        spinTimer1->stop();
        (*vp).autoSpin1 = FALSE;
        spinTimer2->stop();
        (*vp).autoSpin2 = FALSE;
        zoomTimer1->stop();
        (*vp).autoZoom1 = FALSE;
        zoomTimer2->stop();
        (*vp).autoZoom2 = FALSE;
    }
    else {
        // Indicate auto mode by coloring the auto mode button
        autoButton->setIcon( autoRedPixmap );
    }

    // Lock or unlock all control buttons
    x1Button->setCheckable(autoMode);
    x2Button->setCheckable(autoMode);
    y1Button->setCheckable(autoMode);
    y2Button->setCheckable(autoMode);
    z1Button->setCheckable(autoMode);
    z2Button->setCheckable(autoMode);
    dolly1Button->setCheckable(autoMode);
    dolly2Button->setCheckable(autoMode);
}


// Set the auto motion state buttons according to 
// command line arguments
void GLCanvasFrame::setAuto( viewParam vp )
{
    if (vp.autoRot1 == TRUE) {
        this->autoButtonCB();
        x1Button->setChecked(true);
        x1Button->setIcon( x1RedPixmap );
    }
    if (vp.autoRot2 == TRUE) {
        this->autoButtonCB();
        x2Button->setChecked(true);
        x2Button->setIcon( x2RedPixmap );
    }
    if (vp.autoTilt1 == TRUE) {
        this->autoButtonCB();
        y1Button->setChecked(true);
        y1Button->setIcon( y1RedPixmap );
    }
    if (vp.autoTilt2 == TRUE) {
        this->autoButtonCB();
        y2Button->setChecked(true);
        y2Button->setIcon( y2RedPixmap );
    }
    if (vp.autoSpin1 == TRUE) {
        this->autoButtonCB();
        z1Button->setChecked(true);
        z1Button->setIcon( z1RedPixmap );
    }
    if (vp.autoSpin2 == TRUE) {
        this->autoButtonCB();
        z2Button->setChecked(true);
        z2Button->setIcon( z2RedPixmap );
    }
    if (vp.autoZoom1 == TRUE) {
        this->autoButtonCB();
        dolly1Button->setChecked(true);
        dolly1Button->setIcon( dolly1RedPixmap );
    }
    if (vp.autoZoom2 == TRUE) {
        this->autoButtonCB();
        dolly2Button->setChecked(true);
        dolly2Button->setIcon( dolly2RedPixmap );
    }
}


// Start auto motion state
void GLCanvasFrame::startAutoRot1()
{
    // Start the rotation
    connect(rotTimer1, SIGNAL(timeout()), this, SLOT(rot1SingleStep()) );
    rotTimer1->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoRot2()
{
    // Start the rotation
    connect(rotTimer2, SIGNAL(timeout()), this, SLOT(rot2SingleStep()) );
    rotTimer2->start( 40 );
}

// Start auto motion state
void GLCanvasFrame::startAutoTilt1()
{
    // Start the tilting
    connect(tiltTimer1, SIGNAL(timeout()), this, SLOT(tilt1SingleStep()) );
    tiltTimer1->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoTilt2()
{
    // Start the tilting
    connect(tiltTimer2, SIGNAL(timeout()), this, SLOT(tilt2SingleStep()) );
    tiltTimer2->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoSpin1()
{
    // Start the spinning
    connect(spinTimer1, SIGNAL(timeout()), this, SLOT(spin1SingleStep()) );
    spinTimer1->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoSpin2()
{
    // Start the spinning
    connect(spinTimer2, SIGNAL(timeout()), this, SLOT(spin1SingleStep()) );
    spinTimer2->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoZoom1()
{
    // Start the zooming
    connect(zoomTimer1, SIGNAL(timeout()), this, SLOT(dolly1SingelStep()) );
    zoomTimer1->start( 40 );
}


// Start auto motion state
void GLCanvasFrame::startAutoZoom2()
{
    // Start the zooming
    connect(zoomTimer2, SIGNAL(timeout()), this, SLOT(dolly2SingleStep()) );
    zoomTimer2->start( 40 );
}


// Adjust the button settings
void GLCanvasFrame::adjustButtons()
{
    viewParam * vp = this->getViewParam();

    switch ((*vp).viewMode) {
    case PERSPECTIVE:
        perspectiveButton->setChecked(true);
        parallelButton->setChecked(false);
        break;
    case ORTHO:
        perspectiveButton->setChecked(false);
        parallelButton->setChecked(true);
        break;
    }

    switch ((*vp).background) {
    case BGBLACK:
        blackBackgroundButton->setChecked(true);
        whiteBackgroundButton->setChecked(false);
        break;
    case BGWHITE:
        blackBackgroundButton->setChecked(false);
        whiteBackgroundButton->setChecked(true);
        break;
    }

    switch ((*vp).stereoVisionEn) {
    case TRUE:
        stereoVisionButton->setChecked(true);
        break;
    case FALSE:
        stereoVisionButton->setChecked(false);
        break;
    }

}


// Update the drawing without recompiling
void GLCanvasFrame::updateRendering()
{
    // Refresh the graphics
    drawArea->updateGL();

    // Update boards
    mainForm->updateExplicitBoard();
}


// Change the view settings without requesting update of the
// drawing
void GLCanvasFrame::setViewParam( viewParam vp )
{
    drawArea->setViewParam( vp );

    // Adjust button settings
    this->adjustButtons();

    // Update boards
    mainForm->updateExplicitBoard();
}


// Create an image file from the current rendering
// (overloaded function)
void GLCanvasFrame::snapRendering()
{
    drawArea->snapRendering();
}


// Create an image file from the current rendering
// (overloaded function)
void GLCanvasFrame::snapRendering( const char * filename )
{
    drawArea->snapRendering( filename );
}


// Return a pointer to the current aggregate data 
aggregateData * GLCanvasFrame::getAggregateData()
{
    return drawArea->getAggregateData();
}


// Return a pointer to the view parameters currently used
viewParam * GLCanvasFrame::getViewParam() {
   return drawArea->getViewParam();
}


// Return a pointer to the view object structure currently used
viewObject * GLCanvasFrame::getViewObject() {
    return drawArea->getViewObject();
}


// Return a pointer to the particle data currently used 
particleData * GLCanvasFrame::getParticleData() {
    return drawArea->getParticleData();
}


// Return a pointer to the track data currently used 
trackData * GLCanvasFrame::getTrackData()
{
    return drawArea->getTrackData();
}
