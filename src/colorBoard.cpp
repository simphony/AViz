//
// Implementation of color board dialog widget class
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

#include "colorBoard.h"

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include "defaults.h"
#include "atomBoard.h"
#include "lcBoard.h"
#include "polymerBoard.h"
#include "poreBoard.h"
#include "spinBoard.h"
#include "colorLabel.h"
#include "data.h"
#include "widgets/doneapplycancelwidget.h"

namespace {

QHBoxLayout* createHboxLayout(QWidget *widget) {
    QHBoxLayout *box = new QHBoxLayout(widget);
    box->setSpacing(SPACE);
    return box;
}

QVBoxLayout* createVboxLayout(QWidget *widget) {
    QVBoxLayout *box = new QVBoxLayout(widget);
    box->setSpacing(SPACE);
    return box;
}

// creates widget containing Red, Green and Blue
// labels in a vertical layout
QWidget* createRedGreenBlueLabels() {
    QWidget *w = new QWidget();
    QVBoxLayout *layout = createVboxLayout(w);
    layout->addWidget(new QLabel(" Red: "));
    layout->addWidget(new QLabel(" Green: "));
    layout->addWidget(new QLabel(" Blue: "));
    return w;
}

QFrame* createFrame() {
    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    frame->setContentsMargins(SPACE, SPACE, SPACE, SPACE );
    frame->setMinimumWidth( COLOR_MIN_WIDTH );
    return frame;
}

QSlider* createHorizontalSlider() {
    QSlider *s = new QSlider(Qt::Horizontal);
    s->setMinimum( COLOR_MIN );
    s->setMaximum( COLOR_MAX );
    s->setTickInterval( 10 );
    s->setTickPosition( QSlider::TicksAbove );
    s->setValue( COLOR_MAX );
    s->setFixedWidth( SLIDER_WIDTH );
    return s;
}

}

// Make a popup dialog box. Note:
// It is the responsibility of the calling widget 
// (atomBoard, etc.) to build the layout by calling
// buildLayout.  The argument indicates how many 
// colors should be specified (1, 2, or 3)	
ColorBoard::ColorBoard(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle( "AViz: Set Colors" );

    // Create three widgets to contain the center,
    // middle and bottom set of color sliders
    hb0 = createFrame();
    hb1 = createFrame();
    hb2 = createFrame();

    QHBoxLayout *hb0Layout = createHboxLayout(hb0);
    QHBoxLayout *hb1Layout = createHboxLayout(hb1);
    QHBoxLayout *hb2Layout = createHboxLayout(hb2);

    // Create labels that are shown when
    // color ramps are being specified
    topL = new QLabel();
    centerL = new QLabel();
    bottomL = new QLabel();

    hb0Layout->addWidget(topL);
    hb1Layout->addWidget(centerL);
    hb2Layout->addWidget(bottomL);

    // Create a vertical box to contain the top set of
    // slider labels
    QWidget *vb0 = createRedGreenBlueLabels();
    // Create a vertical box to contain the center set of
    // slider labels
    QWidget *vb3 = createRedGreenBlueLabels();

    // Create a vertical box to contain the bottom set of
    // slider labels
    QWidget *vb6 = createRedGreenBlueLabels();

    hb0Layout->addWidget(vb0);
    hb1Layout->addWidget(vb3);
    hb2Layout->addWidget(vb6);

    // Create a vertical box to contain the top set of color
    // sliders
    QWidget *vb1 = new QWidget();
    QVBoxLayout *vb1Layout = createVboxLayout(vb1);

    // Create a vertical box to contain the center set of color
    // sliders
    QWidget *vb4 = new QWidget();
    QVBoxLayout *vb4Layout = createVboxLayout(vb4);

    // Create a vertical box to contain the bottom set of color
    // sliders
    QWidget *vb7 = new QWidget();
    QVBoxLayout *vb7Layout = createVboxLayout(vb7);

    hb0Layout->addWidget(vb1);
    hb1Layout->addWidget(vb4);
    hb2Layout->addWidget(vb7);

    // Create three sliders
    redS0 = createHorizontalSlider();
    greenS0 = createHorizontalSlider();
    blueS0 = createHorizontalSlider();

    vb1Layout->addWidget(redS0);
    vb1Layout->addWidget(greenS0);
    vb1Layout->addWidget(blueS0);

    // Connect the sliders to callbacks
    connect(redS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );
    connect(greenS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );
    connect(blueS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );

    // Create again three sliders
    redS1 = createHorizontalSlider();
    greenS1 = createHorizontalSlider();
    blueS1 = createHorizontalSlider();

    vb4Layout->addWidget(redS1);
    vb4Layout->addWidget(greenS1);
    vb4Layout->addWidget(blueS1);

    // Connect the sliders to callbacks
    connect(redS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );
    connect(greenS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );
    connect(blueS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );

    // Create again three sliders
    redS2 = createHorizontalSlider();
    greenS2 = createHorizontalSlider();
    blueS2 = createHorizontalSlider();

    vb7Layout->addWidget(redS2);
    vb7Layout->addWidget(greenS2);
    vb7Layout->addWidget(blueS2);

    // Connect the sliders to callbacks
    connect(redS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );
    connect(greenS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );
    connect(blueS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );

    // Create the top color label canvas
    colorLabel0 = new ColorLabel();
    colorLabel0->setFixedHeight( COLOR_MIN_HEIGHT_ROW );
    hb0Layout->addWidget(colorLabel0);

    // Create the bottom color label canvas
    colorLabel1 = new ColorLabel();
    colorLabel1->setFixedHeight( COLOR_MIN_HEIGHT_ROW );
    hb1Layout->addWidget(colorLabel1);

    // Create the bottom color label canvas
    colorLabel2 = new ColorLabel();
    colorLabel2->setFixedHeight( COLOR_MIN_HEIGHT_ROW );
    hb2Layout->addWidget(colorLabel2);

    // Create a hboxlayout that will fill the lowest row
    DoneApplyCancelWidget *doneApplyCancelW = new DoneApplyCancelWidget(this);
    connect(doneApplyCancelW, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancelW, SIGNAL(applied()), this, SLOT(bapply()) );
    connect(doneApplyCancelW, SIGNAL(canceled()), this, SLOT(bcancel()));

    QVBoxLayout *colorBox = new QVBoxLayout(this);
    colorBox->setSpacing(SPACE);
    colorBox->addWidget(hb0);
    colorBox->addWidget(hb1);
    colorBox->addWidget(hb2);
    colorBox->addWidget(doneApplyCancelW);

    // Set defaults
    red0 = red1 = red2 = red0Org = red1Org = red2Org = 1.0;
    green0 = green1 = green2 = green0Org = green1Org = green2Org = 1.0;
    blue0 = blue1 = blue2 = blue0Org = blue1Org = blue2Org = 1.0;

    // Clear pointers
    ab = NULL;
    sb = NULL;
    lcb = NULL;
    pb = NULL;
    pob = NULL;
}


// Set a pointer to the calling class
void ColorBoard::setAtomBoardAddress( AtomBoard * thisAB )
{
    ab = thisAB;
}

// Set a pointer to the calling class
void ColorBoard::setSpinBoardAddress( SpinBoard * thisSB )
{
    sb = thisSB;
}


// Set a pointer to the calling class
void ColorBoard::setLcBoardAddress( LcBoard * thisLcB )
{	
    lcb = thisLcB;
}


// Set a pointer to the calling class
void ColorBoard::setPolymerBoardAddress( PolymerBoard * thisPoB )
{	
    pob = thisPoB;
}

// Set a pointer to the calling class
void ColorBoard::setPoreBoardAddress( PoreBoard * thisPB )
{	
    pb = thisPB;
}


// Build the layout of color labels and slider elements
void ColorBoard::buildLayout( char colors ) 
{
    switch (colors) {
    case 1:
        hb1->hide();
        hb2->hide();
        break;
    case 2:
        hb1->show();
        hb2->hide();
        break;
    case 3:
        hb1->show();
        hb2->show();
        break;
    }
}


// Set the current color (overloaded function)
void ColorBoard::setColor( float r, float g, float b )
{
    // Adjust the sliders and the color labels
    redS0->setValue( (int)(100.0*r) );
    greenS0->setValue( (int)(100.0*g) );
    blueS0->setValue( (int)(100.0*b) );

    colorLabel0->setColor( r, g, b);

    // Save the values in the global variables
    red0 = red1 = red2 = r;
    green0 = green1 = green2 = g;
    blue0 = blue1 = blue2 = b;

    // Save these values in case the setting is cancelled
    red0Org = red1Org = red2Org = r;
    green0Org = green1Org = green2Org = g;
    blue0Org = blue1Org = blue2Org = b;
}


// Set the current color (overloaded function)
void ColorBoard::setColor( float r0, float g0, float b0, float r1, float g1, float b1 )
{
    // Adjust the sliders and the color labels
    redS0->setValue( (int)(100.0*r0) );
    greenS0->setValue( (int)(100.0*g0) );
    blueS0->setValue( (int)(100.0*b0) );
    redS1->setValue( (int)(100.0*r1) );
    greenS1->setValue( (int)(100.0*g1) );
    blueS1->setValue( (int)(100.0*b1) );

    colorLabel0->setColor( r0, g0, b0 );
    colorLabel1->setColor( r1, g1, b1 );

    // Save the values in the global variables
    red0 = r0;
    green0 = g0;
    blue0 = b0;
    red1 = red2 = r1;
    green1 = green2 = g1;
    blue1 = blue2 = b1;

    // Save these values in case the setting is cancelled
    red0Org = r0;
    green0Org = g0;
    blue0Org = b0;
    red1Org = red2Org = r1;
    green1Org = green2Org = g1;
    blue1Org = blue2Org = b1;
}


// Set the current color (overloaded function)
void ColorBoard::setColor( float r0, float g0, float b0, float r1, float g1, float b1, float r2, float g2, float b2 )
{
    // Adjust the sliders and the color labels
    redS0->setValue( (int)(100.0*r0) );
    greenS0->setValue( (int)(100.0*g0) );
    blueS0->setValue( (int)(100.0*b0) );
    redS1->setValue( (int)(100.0*r1) );
    greenS1->setValue( (int)(100.0*g1) );
    blueS1->setValue( (int)(100.0*b1) );
    redS2->setValue( (int)(100.0*r2) );
    greenS2->setValue( (int)(100.0*g2) );
    blueS2->setValue( (int)(100.0*b2) );

    colorLabel0->setColor( r0, g0, b0 );
    colorLabel1->setColor( r1, g1, b1 );
    colorLabel2->setColor( r2, g2, b2 );

    // Save the values in the global variables
    red0 = r0;
    green0 = g0;
    blue0 = b0;
    red1 = r1;
    green1 = g1;
    blue1 = b1;
    red2 = r2;
    green2 = g2;
    blue2 = b2;

    // Save these values in case the setting is cancelled
    red0Org = r0;
    green0Org = g0;
    blue0Org = b0;
    red1Org = r1;
    green1Org = g1;
    blue1Org = b1;
    red2Org = r2;
    green2Org = g2;
    blue2Org = b2;
}


// Set the current labels (overloaded function)
void ColorBoard::setLabel(const QString& label1) {
    topL->setText(label1);
}


// Set the current labels (overloaded function)
void ColorBoard::setLabel(const QString& label1, const QString& label2) {
    topL->setText(label1);
    centerL->setText(label2);
}


// Set the current labels (overloaded function)
void ColorBoard::setLabel(const QString& label1, const QString& label2, const QString& label3) {
    topL->setText(label1);
    centerL->setText(label2);
    bottomL->setText(label3);
}


// Adjust the color settings
void ColorBoard::adjustColor0( void )
{
    int redi, greeni, bluei;

    // Read the slider values
    if (redS0)
        redi = (int)redS0->value( );
    else
        redi = 0;
    if (greenS0)
        greeni = (int)greenS0->value( );
    else
        greeni = 0;
    if (blueS0)
        bluei = (int)blueS0->value( );
    else
        bluei = 0;

    // Update the color data entry
    red0 = redi/100.0;
    green0 = greeni/100.0;
    blue0 = bluei/100.0;

    colorLabel0->setColor( red0, green0, blue0 );
}


// Adjust the color settings, using the local
// copy of the atom/spin color data structure
void ColorBoard::adjustColor1( void )
{
    int redi, greeni, bluei;

    // Read the slider values
    if (redS1)
        redi = (int)redS1->value( );
    else
        redi = 0;
    if (greenS1)
        greeni = (int)greenS1->value( );
    else
        greeni = 0;
    if (blueS1)
        bluei = (int)blueS1->value( );
    else
        bluei = 0;

    // Update the color data entry
    red1 = redi/100.0;
    green1 = greeni/100.0;
    blue1 = bluei/100.0;

    colorLabel1->setColor( red1, green1, blue1 );
}


// Adjust the color settings, using the local
// copy of the atom/spin color data structure
void ColorBoard::adjustColor2( void )
{
    int redi, greeni, bluei;

    // Read the slider values
    if (redS2)
        redi = (int)redS2->value( );
    else
        redi = 0;
    if (greenS2)
        greeni = (int)greenS2->value( );
    else
        greeni = 0;
    if (blueS2)
        bluei = (int)blueS2->value( );
    else
        bluei = 0;

    // Update the color data entry
    red2 = redi/100.0;
    green2 = greeni/100.0;
    blue2 = bluei/100.0;

    colorLabel2->setColor( red2, green2, blue2 );
}


// Complete the color setting: save the atom/spin data and hide the board
void ColorBoard::bdone()
{
    this->bapply();

    if (ab) {
        ab->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (sb) {
        sb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (lcb) {
        lcb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (pb) {
        pb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (pob) {
        pob->getColorBoardPos( this->pos().x(), this->pos().y() );
    }

    // Hide now
    hide();

    // Close yourself
    if (ab)
        ab->closeColorBoard();
    if (sb)
        sb->closeColorBoard();
    if (lcb)
        lcb->closeColorBoard();
    if (pb)
        pb->closeColorBoard();
    if (pob)
        pob->closeColorBoard();
}


// Accept the color setting: ask the atom board to retrieve and 
// save the color data
void ColorBoard::bapply()
{
    if (ab) {
        ab->getColors( red0, green0, blue0, red1, green1, blue1 );
    }
    if (sb) {
        sb->getColors( red0, green0, blue0, red1, green1, blue1 );
    }
    if (lcb) {
        lcb->getColors( red0, green0, blue0, red1, green1, blue1, red2, green2, blue2 );
    }
    if (pb) {
        pb->getColors( red0, green0, blue0, red1, green1, blue1, red2, green2, blue2 );
    }
    if (pob) {
        pob->getColors( red0, green0, blue0, red1, green1, blue1 );
    }
}


// Cancel the color setting: hide the board
void ColorBoard::bcancel()
{
    if (ab) {
        ab->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
        ab->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (sb) {
        sb->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
        sb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (lcb) {
        lcb->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org, red2Org, green2Org, blue2Org );
        lcb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (pb) {
        pb->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org, red2Org, green2Org, blue2Org );
        pb->getColorBoardPos( this->pos().x(), this->pos().y() );
    }
    if (pob) {
        pob->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
        pob->getColorBoardPos( this->pos().x(), this->pos().y() );
    }

    // Hide now
    hide();

    // Close yourself
    if (ab)
        ab->closeColorBoard();
    if (sb)
        sb->closeColorBoard();
    if (lcb)
        lcb->closeColorBoard();
    if (pb)
        pb->closeColorBoard();
    if (pob)
        pob->closeColorBoard();
}
