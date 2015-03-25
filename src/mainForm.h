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

#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>

#include "data.h"

class QLabel;

class AnnotationBoard;
class AViz;
class AtomBoard;
class GLCanvasFrame;
class RenderBox;
class LiveBox;
class BondBoard;
class ClipBoard;
class ExplicitBoard;
class FileListBoard;
class LcBoard;
class LightsBoard;
class PolymerBoard;
class PoreBoard;
class SpinBoard;
class SliceBoard;
class StretchBoard;
class TranslationBoard;
class TrackBoard;

// MainForm is a composite widget that implements
// the drawing canvas and associated control elements
class MainForm: public QWidget
{
    Q_OBJECT
public:
    MainForm(QWidget *parent, AViz*);

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
    void snapRendering(const QString &);
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
    void computeTransformation(double, double, double, double, double *, double *, double *);

    void statusMessage(const QString &);
    void statusMessage(const QString &, const QString &);

    void launchAnnotation();
    void launchAtoms();
    void launchBonds();
    void launchClip();
    void launchExplicit();
    void launchFileList();
    void launchFileList(const QString &);
    void launchLights();
    void launchLiquidCrystals();
    void launchPolymers();
    void launchPores();
    void launchSlice();
    void launchSpins();
    void launchStretch();
    void launchTrack(const QString &);
    void launchTranslation();

    void hideTrack();
    void generateTracks();

private:
    AViz *m_aviz;
    GLCanvasFrame * glCanvasFrame;
    RenderBox * renderBox;
    LiveBox *m_liveBox;
    AtomBoard * ab;
    AnnotationBoard * m_anb;
    BondBoard * bb;
    ClipBoard * clb;
    ExplicitBoard * eb;
    FileListBoard * flb;
    LcBoard * lcb;
    LightsBoard * lb;
    PolymerBoard * plb;
    PoreBoard * pb;
    SpinBoard * sb;
    SliceBoard * slb;
    StretchBoard * stb;
    TranslationBoard * tb;
    TrackBoard * trab;
    QLabel * m_status;
    fileList fl;
    fileType ft;
    bool m_keepViewObject;
};


#endif // FORM_H
