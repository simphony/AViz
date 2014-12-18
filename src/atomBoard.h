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

#include <QDialog>

#include "data.h"

class Q3GridLayout;
class QCheckBox;
class QPushButton;
class QComboBox;
class QRadioButton;

class MainForm;
class ColorLabel;
class ColorBoard;
class CodeBox;
class SizeBox;
class PositionBox;
class PropertyBox;

// Atom board dialog widget
class AtomBoard: public QDialog
{
    Q_OBJECT
public:
    AtomBoard(MainForm *mainForm, QWidget * parent=0);

public slots:
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
    void setAtom();
    void adjustAtom();
    void adjustCriterion();
    void setColorCb();
    void bbonds();
    void bdone();
    void bapply();
    void bcancel();

private:
    void buildLayout( colorCriterion );
    void setColors();
    void readToggles();

private:
    MainForm * mainForm;
    CodeBox * codeBox;
    PositionBox * positionBox;
    PropertyBox * propertyBox;
    SizeBox * sizeBox;
    Q3GridLayout * atomBox;
    QWidget *m_atomWidget;
    QWidget *m_colorWidget;
    QWidget *m_colorCriterionWidget;
    QWidget *m_BondsDoneApplyWidget;
    QComboBox * atomCob;
    QComboBox * styleCob;
    QCheckBox * showAtomCb;
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
