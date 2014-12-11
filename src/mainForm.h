//
// Declaration of main form class 
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

#ifndef FORM_H
#define FORM_H

#include <QWidget>

#include "data.h"

class QLabel;
class Q3GridLayout;
class AnnotationBoard;

// Note: Pointers to composite widgets are defined in 
// mainForm.cpp and not here -- it causes problems 
// regarding the mutual inclusion of  header files 

// MainForm is a composite widget that implements
// the drawing canvas and associated control elements
class MainForm: public QWidget
{
    Q_OBJECT
public:
    MainForm( QWidget *parent=0, const char *name=0 );

    void setAVizAddress( char * );
    void setFileType( fileType );
    fileType isFileType();
    void setKeepViewObject( bool );
    bool isKeepViewObject();
    void readFile( const QString & );
    void writeFile( const QString & );
    QString readFileFromList( int, bool );

    void setDefaults();
    void setDefaults( viewParam, const char * );
    void setViewParam( viewParam );
    void setAutoAndStart( viewParam );
    void updateView();

    aggregateData * getAggregateData();
    viewParam * getViewParam();
    viewObject * getViewObject();
    particleData * getParticleData();

    void updateRendering();
    void updateExplicitBoard();
    void snapRendering();
    void snapRendering( const QString & );
    void setAtomMenus();
    void setSpinMenus();
    void setLcMenus();
    void setPolymerMenus();
    void setPoreMenus();
    void broadcastDotStyle();
    void broadcastLineStyle();
    void broadcastCubeStyle();
    void broadcastCylinderStyle();
    void broadcastConeStyle();
    void broadcastSphereStyle();
    void broadcastLowQuality();
    void broadcastHighQuality();
    void broadcastFinalQuality();
    void closeModeBoards();

    void setViewXYPlus();
    void setViewXYMinus();
    void setViewXZPlus();
    void setViewXZMinus();
    void setViewYZPlus();
    void setViewYZMinus();
    void computeTransformation( double, double, double, double, double *, double *, double * );

    void statusMessage( const char * );
    void statusMessage( const char *, const char * );

    void launchAnnotation( void );
    void launchAtoms( void );
    void launchBonds( void );
    void launchClip( void );
    void launchExplicit( void );
    void launchFileList( void );
    void launchFileList( const QString & );
    void launchLights( void );
    void launchLiquidCrystals( void );
    void launchPolymers( void );
    void launchPores( void );
    void launchSlice( void );
    void launchSpins( void );
    void launchStretch( void );
    void launchTrack( const QString & );
    void launchTranslation( void );

    void hideTrack( void );
    void generateTracks( void );

private:
    QWidget * aviz;
    QWidget * glCanvasFrame;
    QWidget * renderBox;
    QWidget * liveBox;
    QWidget * ab;
    AnnotationBoard * m_anb;
    QWidget * bb;
    QWidget * clb;
    QWidget * eb;
    QWidget * flb;
    QWidget * lcb;
    QWidget * lb;
    QWidget * plb;
    QWidget * pb;
    QWidget * sb;
    QWidget * slb;
    QWidget * stb;
    QWidget * tb;
    QWidget * trab;
    Q3GridLayout * mainBox;
    QLabel * status;
    fileList fl;
    fileType ft;
    bool keepViewObject;
    int numCols;
    int numRows;

    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;
};


#endif // FORM_H
