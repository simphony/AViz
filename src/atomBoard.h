//
// Declaration of atom board class
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

#ifndef ATOMB_H
#define ATOMB_H 

#include "mainForm.h"
#include "codeBox.h"
#include "colorBoard.h"
#include "colorLabel.h"
#include "defaultParticles.h"
#include "defaults.h"
#include "fileFunctions.h"
#include "positionBox.h"
#include "propertyBox.h"
#include "sizeBox.h"
#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <sys/stat.h>
#include <unistd.h>

#include <q3buttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qdialog.h>
#include <q3hbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qsizepolicy.h>
#include <q3vbox.h>
#include <qwidget.h>

class Q3GridLayout;

// Atom board dialog widget
class AtomBoard: public QDialog
{
	Q_OBJECT
public:
    AtomBoard( QWidget * parent=0 );

public slots:
	void setMainFormAddress( MainForm * );
	void setData();
	void getColors( float, float, float, float, float, float );
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
	void setAtom();
	void adjustAtom();
	void adjustCriterion();
	void setColorCb();
	void setColors();
	void readToggles();
	void bbonds();
	void bdone();
	void bapply();
	void bcancel();

private:
	MainForm * mainForm;
	CodeBox * codeBox;
	PositionBox * positionBox;
	PropertyBox * propertyBox;
	SizeBox * sizeBox;
	Q3GridLayout * atomBox;
	Q3HBox * hb1;
	Q3HBox * hb2;
	Q3HBox * hb3;
	Q3HBox * hb4;
	Q3HBox * hb5;
	QComboBox * atomCob;
	QComboBox * styleCob;
	QCheckBox * showAtomCb;
	QLabel * modeL;
	QLabel * colorL;
        Q3ButtonGroup * colorMode;
        QRadioButton * colorMode0;
        QRadioButton * colorMode1;
        QRadioButton * colorMode2;
        QRadioButton * colorMode3;
	ColorLabel * colorLabel0;
        ColorLabel * colorLabel1;
        ColorLabel * colorLabel2;
        ColorLabel * colorLabel3;
        ColorLabel * colorLabel4;
        ColorLabel * colorLabel5;
	QPushButton * colorButton;
	ColorBoard * cb;
	particleData * thisPd;
	int thisAtomIndex;
	char colors; 
	int colorPosX, colorPosY;
        atomRStyle atomRenderStyle;
        quality renderQuality;
	bool showColorBoard;
};

#endif // ATOMB_H
