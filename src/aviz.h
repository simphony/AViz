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
#include "memoryFunctions.h"
#include "messageBox.h"
#include "version.h"

#include <qapplication.h>
#include <q3filedialog.h>
#include <q3mainwindow.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <qstatusbar.h>
#include <q3popupmenu.h>
#include <qtimer.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

class AViz: public Q3MainWindow 
{
	Q_OBJECT
public:
	AViz();
	~AViz();

public slots:
	void setFileType( fileType );
	fileType isFileType();
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

protected:

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
	MainForm * mf;
	Q3PopupMenu * file;
	Q3PopupMenu * elements;
	Q3PopupMenu * view;
	Q3PopupMenu * settings;
	Q3PopupMenu * data;
	Q3PopupMenu * help;
	Q3PopupMenu * openfile;
	Q3PopupMenu * elementsAtoms;
	Q3PopupMenu * elementsPolymers;
	Q3PopupMenu * viewpoint;
	Q3PopupMenu * viewmode;
	Q3PopupMenu * background;
	QTimer * watchTimer; 
	int atomsId;
	int spinsId;
	int liquidCrystalsId;
	int polymersId;
	int poresId;
	int usePerspectiveId, useOrthoId;
	int backgroundBlackId, backgroundWhiteId;
	int showHideAxesId;
	int showHideContourId;
	int onlyContourId;
	int inOutWatchModeId;
	bool inWatchMode;
};


#endif // AVIZ_H
