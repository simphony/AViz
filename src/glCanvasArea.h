//
// Define the OpenGL drawing canvas area
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

#ifndef GLCANVAR_H
#define GLCANVAR_H

#include "data.h"

#include <QGLWidget>
#include <QMouseEvent>

#include <GL/glx.h>
#include <GL/glu.h>

class MainForm;

//  Open GL drawing area
class GLCanvasArea: public QGLWidget
{
    Q_OBJECT

public:
    GLCanvasArea(QWidget* parent);
    ~GLCanvasArea();

    void setFormAddress( MainForm * );

public slots:
    void setViewObjectAndRecompile( viewObject );
    void setViewParam( viewParam );

    void snapRendering();
    void snapRendering( const char * filename );

    aggregateData * getAggregateData();
    viewParam * getViewParam();
    viewObject * getViewObject();
    particleData * getParticleData();
    trackData * getTrackData();

protected:
    virtual void initializeGL();
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void paintGL();
    virtual void resizeGL( int w, int h );

private:
    void recompileObjects();
    void completeParticleData();
    void makeLights();
    void antiAliasing( bool );
    void drawContourBox();
    void drawAxes();
    void drawAxesAnnotation();
    void drawAnnotation();
    bool simpleRendering();
    GLuint makeRasterFont( int );
    void printString(char *string, GLuint base );
    void drawSlicingPlanes();
    GLuint makeBondsObject( bool );
    GLuint makeSpheresObject();
    GLuint makeCubesObject();
    GLuint makeDotsObject();
    GLuint makePinsObject();
    GLuint makeConesObject( bool );
    GLuint makeLinesObject( bool );
    GLuint makeCylindersObject( bool );
    GLuint makeTracksObject();
    void renderDot( int, int );
    void renderSphere( int, int, GLUquadricObj * , GLint, relativeSize );
    void renderCube( int, int, relativeSize );
    void renderBondCylinder( int, int, GLUquadricObj * , GLint, GLdouble, fixedThickness );
    void renderLineCylinder( GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLUquadricObj *, GLint, GLdouble, relativeSize, fixedThickness, bool, bool );
    void renderCone( GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLUquadricObj *, GLint, GLdouble, relativeSize, fixedThickness, bool );
    void renderCone( GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLUquadricObj *, GLint, GLdouble, GLdouble, relativeSize, fixedThickness, bool );
    void renderSpinTip( GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLUquadricObj *, GLint, GLdouble );
    bool particleIsInSlice( int );
    bool trackIsInSlice( int );
    bool isBond( int, int, float, float, bool, bool , double *);
    GLdouble findBondDistanceAngle( int, int, GLdouble *, GLdouble * , GLdouble * );
    GLdouble findSpinDistanceAngle( float, float, float, float, float, float, GLdouble *, GLdouble * , GLdouble * );
    bool bondCombinationNew( tag *, tag *, int *, char *, char * );
    void setBondProperty( fixedColor, fixedThickness, GLfloat, GLdouble *, GLint * );
    void setLineFootProperty( fixedThickness, GLfloat, GLdouble *, GLint * );
    int findPdListIndex( char * );
    void setTypeRelSize( int, relativeSize * );
    void setTypeColor( int );
    void setTypeColor( GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, int );
    void setRampColorPos( float, float, float, int );
    void setRampColorProp( float, float, float, float, float, float, float, float, int );
    void setRampColorCode( float, float, float, float, float, float, float, float, int );

private:
    MainForm * mainForm;
    GLuint singleAtoms, spins, lcs, pores, polymerAtoms;
    GLuint atomBonds, polymerBonds;
    GLuint tracks;
    aggregateData ad;
    particleData pd;
    trackData td;
    viewObject vo;
    viewParam vp;
    int mouseX, mouseY, mouseXPre, mouseYPre, mouseButton;
    bool mouseOn;
};


#endif // GLCANVAR_H
