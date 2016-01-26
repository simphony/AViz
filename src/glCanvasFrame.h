//
// Define the OpenGL drawing canvas frame
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

#ifndef GLCANVFR_H
#define GLCANVFR_H

#include "data.h"

#include <QWidget>
#include <QPixmap>

class QTimer;
class QToolButton;
class GLCanvasArea;
class QFrame;

class MainForm;
class SoQtThumbWheel;

class AggregateData;

// Frame widget containing the Open GL drawing area
class GLCanvasFrame: public QWidget
{
    Q_OBJECT

public:
    GLCanvasFrame(MainForm *, QWidget* parent);
    ~GLCanvasFrame();

public slots:
    void updateView();
    void updateViewWithoutViewObjectChange();
    void setViewParam( viewParam );

    void updateRendering();
    void snapRendering();
    void snapRendering( const char * );

    void setAuto( viewParam );
    void startAutoRot1();
    void startAutoRot2();
    void startAutoTilt1();
    void startAutoTilt2();
    void startAutoSpin1();
    void startAutoSpin2();
    void startAutoZoom1();
    void startAutoZoom2();
    void computeTransformation( double, double, double, double, double *, double *, double * );

    AggregateData * getAggregateData();
    viewParam * getViewParam();
    viewObject * getViewObject();
    particleData * getParticleData();
    trackData * getTrackData();

private slots:
    viewObject getObjectData();

    void rotWheelMoved( float );
    void rot1();
    void rot1SingleStep();
    void rot2();
    void rot2SingleStep();
    void tiltWheelMoved( float );
    void tilt1();
    void tilt1SingleStep();
    void tilt2();
    void tilt2SingleStep();
    void spinWheelMoved( float );
    void spin1();
    void spin1SingleStep();
    void spin2();
    void spin2SingleStep();
    void dollyWheelMoved( float );
    void dolly1();
    void dolly1SingleStep();
    void dolly2();
    void dolly2SingleStep();

    void rotStep( char );
    void rotStep( char, int );
    void tiltStep( char );
    void tiltStep( char, int );
    void spinStep( char );
    void spinStep( char, int );
    void zoomStep( char );
    void zoomStep( char, int );

    void setHome();
    void setNewHome();
    void setParallel();
    void setPerspective();
    void setBlackBackground();
    void setWhiteBackground();
    void toggleStereoVision();
    void eyeSeparationPlus();
    void eyeSeparationMinus();
    void adjustButtons();
    void autoButtonCB();

private:
    MainForm * mainForm;
    GLCanvasArea * drawArea;
    QFrame * drawFrame;
    SoQtThumbWheel * tiltWheel;
    SoQtThumbWheel * rotWheel;
    SoQtThumbWheel * spinWheel;
    SoQtThumbWheel * dollyWheel;
    QToolButton * x1Button;
    QToolButton * x2Button;
    QToolButton * y1Button;
    QToolButton * y2Button;
    QToolButton * z1Button;
    QToolButton * z2Button;
    QToolButton * autoButton;
    QToolButton * setHomeButton;
    QToolButton * setNewHomeButton;
    QToolButton * perspectiveButton;
    QToolButton * parallelButton;
    QToolButton * blackBackgroundButton;
    QToolButton * whiteBackgroundButton;
    QToolButton * stereoVisionButton;
    QToolButton * eyeSeparationPlusButton;
    QToolButton * eyeSeparationMinusButton;
    QToolButton * dolly1Button;
    QToolButton * dolly2Button;
    QPixmap x1Pixmap;
    QPixmap x1RedPixmap;
    QPixmap x2Pixmap;
    QPixmap x2RedPixmap;
    QPixmap y1Pixmap;
    QPixmap y1RedPixmap;
    QPixmap y2Pixmap;
    QPixmap y2RedPixmap;
    QPixmap z1Pixmap;
    QPixmap z1RedPixmap;
    QPixmap z2Pixmap;
    QPixmap z2RedPixmap;
    QPixmap autoPixmap;
    QPixmap autoRedPixmap;
    QPixmap setHomePixmap;
    QPixmap setNewHomePixmap;
    QPixmap parallelPixmap;
    QPixmap perspectivePixmap;
    QPixmap blackBackgroundPixmap;
    QPixmap whiteBackgroundPixmap;
    QPixmap stereoVisionPixmap;
    QPixmap eyeSeparationPlusPixmap;
    QPixmap eyeSeparationMinusPixmap;
    QPixmap dolly1Pixmap;
    QPixmap dolly1RedPixmap;
    QPixmap dolly2Pixmap;
    QPixmap dolly2RedPixmap;
    QTimer * rotTimer1;
    QTimer * rotTimer2;
    QTimer * tiltTimer1;
    QTimer * tiltTimer2;
    QTimer * spinTimer1;
    QTimer * spinTimer2;
    QTimer * zoomTimer1;
    QTimer * zoomTimer2;
    bool autoMode;
};

#endif // GLCANVFR_H
