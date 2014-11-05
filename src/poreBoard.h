//
// Declaration of pore board class
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

#ifndef POREB_H
#define POREB_H 

#include "codeBox.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "data.h"
#include "defaultParticles.h"
#include "defaults.h"
#include "fileFunctions.h"
#include "floatSpin.h"
#include "lineTypeBox.h"
#include "mainForm.h"
#include "memoryFunctions.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "sizeBox.h"
#include "typeColorNumberBox.h"

#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qdialog.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qsizepolicy.h>
#include <qspinbox.h>
#include <qwidget.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>

// Pore board dialog widget
class PoreBoard: public QDialog
{
	Q_OBJECT
public:
	PoreBoard( QWidget * parent=0, const char * name=0 );

public slots:
	void setMainFormAddress( MainForm * );
	void setData();
	void getColors( float, float, float, float, float, float, float, float, float );
	void getColorBoardPos( int, int );
	void setDotStyle();
        void setLineStyle();
        void setCubeStyle();
        void setCylinderStyle();
        void setConeStyle();
        void setSphereStyle();
        void setLowQuality();
        void setHighQuality();
        void setFinalQuality();
        void closeColorBoard();

private slots:
	void buildLayout( colorCriterion );
	void setPore();
        void adjustPore();
        void adjustCriterion( void );
	void setColorCb();
	void setColors();
	void readToggles();
	void bdone();
	void bapply();
	void bcancel();

private:
	MainForm * mainForm;
	CodeBox * codeBox;
	LineTypeBox * lineTypeBox;
	PositionBox * positionBox;
        PropertyBox * propertyBox;
        SizeBox * sizeBox;
        TypeColorNumberBox * typeColorNumberBox;
	QHBox * hb1;
	QHBox * hb2;
	QHBox * hb4;
	QHBox * hb5;
	QGridLayout * poreBox;
	QComboBox * poreCob;
	QCheckBox * showPoreCb;
        QLabel * modeL;
        QLabel * colorL;
	ColorLabel * colorLabel0;
        ColorLabel * colorLabel1;
        ColorLabel * colorLabel2;
        ColorLabel * colorLabel3;
        ColorLabel * colorLabel4;
        ColorLabel * colorLabel5;
	QPushButton * colorButton;
        QButtonGroup * colorMode;
        QRadioButton * colorMode0;
        QRadioButton * colorMode1;
        QRadioButton * colorMode2;
        QRadioButton * colorMode3;
	ColorBoard * cb;
        particleData * thisPd;
        int thisPoreIndex;
	char colors;
        int colorPosX, colorPosY;
        bool showColorBoard;
	poreRStyle poreRenderStyle;
        quality renderQuality;
};

#endif // POREB_H
