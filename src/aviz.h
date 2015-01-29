//
// Declaration of main panel
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

#ifndef AVIZ_H
#define AVIZ_H

#include "animationBoard.h"
#include "customFileDialog.h"
#include "data.h"
#include "defaults.h"
#include "defaultParticles.h"
#include "fileFunctions.h"
#include "license.h"
#include "mainForm.h"
#include "messageBox.h"
#include "version.h"

#include <QMainWindow>
#include <QTimer>

#include <sys/stat.h>
#include <time.h>

class QAction;

class AViz: public QMainWindow
{
	Q_OBJECT
public:
	AViz();
	~AViz();

    fileType isFileType();

public slots:
	void setFileType( fileType );
	void readFile( const QString & );
	void setDefaults();
	void setDefaults( char * );
	void startAutoRot();
	void startAutoTilt();
	void startAutoSpin();
	void startAutoZoom();
	void startAutoSnap();
	void useFileList( char * );
	void setRenderMode( char * );
	void setRenderStyle( char * );
	void setRenderQuality( char * );
	void setRenderSize( int );
	void setAnnotationString( char * );
	void savePNGFile();
	void setSnap();
	void setSnapq();
	void setWatchMode();
	void setAtomMenus();
	void setSpinMenus();
	void setLcMenus();
	void setPolymerMenus();
	void setPoreMenus();

private slots:
	void openXYZ();
	void openList();
	void openViewParam();
	void saveViewParam();
	void writeFile( const QString & );
	void setDefaultView();
	void viewXYPlus();
	void viewXYMinus();
	void viewXZPlus();
	void viewXZMinus();
	void viewYZPlus();
	void viewYZMinus();
	void swapXY();
	void swapXZ();
	void swapYZ();
	void mirrorX();
	void mirrorY();
	void mirrorZ();
	void showHideAxesCB();
	void showHideContourCB();
	void onlyContourCB();
	void launchAnimation();
	void launchAnnotation();
	void launchAtoms();
	void launchClip();
	void launchBonds();
	void launchExplicit();
	void launchLights();
	void launchLiquidCrystals();
	void launchPores();
	void launchPolymers();
	void launchSlice();
	void launchSpins();
	void launchStretch();
	void launchTranslation();
	void launchFileList();
	void watchFile();
	void about();
	void license();
	void distribute();
	void snapFile( const QString & );
	void setSnapNow();
	void setSnapqNow();
	void keepWatchNow();

private:
    enum ElementType {
        ET_ATOM,
        ET_SPIN,
        ET_LIQUID_CRYSTALS,
        ET_POLYMERS,
        ET_PORES
    };

private:
    /// enables menu items related to particlular element
    void enableElement(ElementType);

private:
    MainForm *m_mainForm;
    QTimer *m_watchTimer;
    QMenu *m_atomsMenu;
    QAction *m_spinsAction;
    QAction *m_liquidCrystalsAction;
    QMenu *m_polymersMenu;
    QAction *m_poresAction;
    QAction *m_usePerspectiveAction;
    QAction *m_useOrthoAction;
    QAction *m_showHideAxesAction;
    QAction *m_showHideContourAction;
    QAction *m_onlyContourAction;
    QAction *m_inOutWatchModelAction;
    bool m_inWatchMode;
};


#endif // AVIZ_H
