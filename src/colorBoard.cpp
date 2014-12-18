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

#include "atomBoard.h"
#include "lcBoard.h"
#include "polymerBoard.h"
#include "poreBoard.h"
#include "spinBoard.h"

// Private pointers are defined in colorBoard.h, but the corresponding
// header files cannot be included there -- it causes problems regarding
// the mutual inclusion of  header files.  Here the pointers are of
// the general class QWiget and must be cast into the specific
// class

// Make a popup dialog box. Note:
// It is the responsibility of the calling widget 
// (atomBoard, etc.) to build the layout by calling
// buildLayout.  The argument indicates how many 
// colors should be specified (1, 2, or 3)	
ColorBoard::ColorBoard( QWidget * parent, const char * name )
    : QDialog( parent, name, FALSE, Qt::WType_TopLevel )
{
	setWindowTitle( "AViz: Set Colors" );

	// Create a hboxlayout that will contain the top
	// set of color sliders
	numCols = 6;
	canvCol = 5;
	hb0 = new Q3HBox( this, "hb0" );
	hb0->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
	hb0->setMargin( SPACE );
	hb0->setSpacing( SPACE );
	hb0->setMinimumWidth( COLOR_MIN_WIDTH );
	hb0->setFixedHeight( COLOR_MIN_HEIGHT_ROW );
		
	// Create a hboxlayout that will contain the center 
	// set of color sliders
	hb1 = new Q3HBox( this, "hb1" );
	hb1->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
	hb1->setMargin( SPACE );
	hb1->setSpacing( SPACE );
	hb1->setMinimumWidth( COLOR_MIN_WIDTH );
	hb1->setFixedHeight( COLOR_MIN_HEIGHT_ROW );
	
	// Create a hboxlayout that will contain the center 
	// set of color sliders
	hb2 = new Q3HBox( this, "hb2" );
	hb2->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
	hb2->setMargin( SPACE );
	hb2->setSpacing( SPACE );
	hb2->setMinimumWidth( COLOR_MIN_WIDTH );
	hb2->setFixedHeight( COLOR_MIN_HEIGHT_ROW );

	// Create labels that are shown when
	// color ramps are being specified
	topL = new QLabel( hb0, "topL" );
	centerL = new QLabel( hb1, "topL" );
	bottomL = new QLabel( hb2, "bottomL" );

	// Create a vertical box to contain the top set of 
	// slider labels
	Q3VBox * vb0 = new Q3VBox( hb0, "vb0" );
	
	// Create a vertical box to contain the center set of 
	// slider labels
	Q3VBox * vb3 = new Q3VBox( hb1, "vb3" );
	
	// Create a vertical box to contain the bottom set of 
	// slider labels
	Q3VBox * vb6 = new Q3VBox( hb2, "vb6" );
	
	// Create three labels in the next column
	QLabel * redL0 = new QLabel( vb0, "redL0" );
	redL0->setText( " Red: ");	

	QLabel * greenL0 = new QLabel( vb0, "greenL0" );
	greenL0->setText( " Green: ");	

	QLabel * blueL0 = new QLabel( vb0, "blueL0" );
	blueL0->setText( " Blue: ");	
	
	// Create again three labels in the next column
	QLabel * redL1 = new QLabel( vb3, "redL1" );
	redL1->setText( " Red: ");	

	QLabel * greenL1 = new QLabel( vb3, "greenL1" );
	greenL1->setText( " Green: ");	

	QLabel * blueL1 = new QLabel( vb3, "blueL1" );
	blueL1->setText( " Blue: ");	
	
	// Create again three labels in the next column
	QLabel * redL2 = new QLabel( vb6, "redL2" );
	redL2->setText( " Red: ");	

	QLabel * greenL2 = new QLabel( vb6, "greenL2" );
	greenL2->setText( " Green: ");	

	QLabel * blueL2 = new QLabel( vb6, "blueL2" );
	blueL2->setText( " Blue: ");	
	
	// Create a vertical box to contain the top set of color
	// sliders 
	Q3VBox * vb1 = new Q3VBox( hb0, "vb1" );

	// Create a vertical box to contain the center set of color
	// sliders
	Q3VBox * vb4 = new Q3VBox( hb1, "vb4" );

	// Create a vertical box to contain the bottom set of color
	// sliders
	Q3VBox * vb7 = new Q3VBox( hb2, "vb7" );\

	// Create three sliders
	redS0 = new QSlider( Qt::Horizontal, vb1, "redS" );
	redS0->setMinimum( COLOR_MIN );
        redS0->setMaximum( COLOR_MAX );
        redS0->setTickInterval( 10 );
        redS0->setTickPosition( QSlider::TicksAbove );
	redS0->setValue( COLOR_MAX );
	redS0->setFixedWidth( SLIDER_WIDTH );
        redS0->setFixedHeight( SLIDER_HEIGHT );

	greenS0 = new QSlider(  Qt::Horizontal, vb1, "greenS" );
	greenS0->setMinimum( COLOR_MIN );
        greenS0->setMaximum( COLOR_MAX );
        greenS0->setTickInterval( 10 );
        greenS0->setTickPosition( QSlider::TicksAbove );
	greenS0->setValue( COLOR_MAX );
	greenS0->setFixedWidth( SLIDER_WIDTH );
        greenS0->setFixedHeight( SLIDER_HEIGHT );

	blueS0 = new QSlider( Qt::Horizontal, vb1, "blueS" );
	blueS0->setMinimum( COLOR_MIN );
        blueS0->setMaximum( COLOR_MAX );
        blueS0->setTickInterval( 10 );
        blueS0->setTickPosition( QSlider::TicksAbove );
	blueS0->setValue( COLOR_MAX );
	blueS0->setFixedWidth( SLIDER_WIDTH );
        blueS0->setFixedHeight( SLIDER_HEIGHT );

	// Connect the sliders to callbacks 
	connect( redS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );
	connect( greenS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );
	connect( blueS0, SIGNAL(valueChanged(int)), this, SLOT(adjustColor0( )) );

	// Create again three sliders
	redS1 = new QSlider( Qt::Horizontal, vb4, "redS" );
	redS1->setMinimum( COLOR_MIN );
        redS1->setMaximum( COLOR_MAX );
        redS1->setTickInterval( 10 );
        redS1->setTickPosition( QSlider::TicksAbove );
	redS1->setValue( COLOR_MAX );
	redS1->setFixedWidth( SLIDER_WIDTH );
        redS1->setFixedHeight( SLIDER_HEIGHT );

	greenS1 = new QSlider(  Qt::Horizontal, vb4, "greenS" );
	greenS1->setMinimum( COLOR_MIN );
        greenS1->setMaximum( COLOR_MAX );
        greenS1->setTickInterval( 10 );
        greenS1->setTickPosition( QSlider::TicksAbove );
	greenS1->setValue( COLOR_MAX );
	greenS1->setFixedWidth( SLIDER_WIDTH );
        greenS1->setFixedHeight( SLIDER_HEIGHT );

	blueS1 = new QSlider( Qt::Horizontal, vb4, "blueS" );
	blueS1->setMinimum( COLOR_MIN );
        blueS1->setMaximum( COLOR_MAX );
        blueS1->setTickInterval( 10 );
        blueS1->setTickPosition( QSlider::TicksAbove );
	blueS1->setValue( COLOR_MAX );
	blueS1->setFixedWidth( SLIDER_WIDTH );
        blueS1->setFixedHeight( SLIDER_HEIGHT );

	// Connect the sliders to callbacks 
	connect( redS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );
	connect( greenS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );
	connect( blueS1, SIGNAL(valueChanged(int)), this, SLOT(adjustColor1( )) );

	// Create again three sliders
	redS2 = new QSlider( Qt::Horizontal, vb7, "redS" );
	redS2->setMinimum( COLOR_MIN );
        redS2->setMaximum( COLOR_MAX );
        redS2->setTickInterval( 10 );
        redS2->setTickPosition( QSlider::TicksAbove );
	redS2->setValue( COLOR_MAX );
	redS2->setFixedWidth( SLIDER_WIDTH );
        redS2->setFixedHeight( SLIDER_HEIGHT );

	greenS2 = new QSlider(  Qt::Horizontal, vb7, "greenS" );
	greenS2->setMinimum( COLOR_MIN );
        greenS2->setMaximum( COLOR_MAX );
        greenS2->setTickInterval( 10 );
        greenS2->setTickPosition( QSlider::TicksAbove );
	greenS2->setValue( COLOR_MAX );
	greenS2->setFixedWidth( SLIDER_WIDTH );
        greenS2->setFixedHeight( SLIDER_HEIGHT );

	blueS2 = new QSlider( Qt::Horizontal, vb7, "blueS" );
	blueS2->setMinimum( COLOR_MIN );
        blueS2->setMaximum( COLOR_MAX );
        blueS2->setTickInterval( 10 );
        blueS2->setTickPosition( QSlider::TicksAbove );
	blueS2->setValue( COLOR_MAX );
	blueS2->setFixedWidth( SLIDER_WIDTH );
        blueS2->setFixedHeight( SLIDER_HEIGHT );

	// Connect the sliders to callbacks 
	connect( redS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );
	connect( greenS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );
	connect( blueS2, SIGNAL(valueChanged(int)), this, SLOT(adjustColor2( )) );

	// Create a vertical box to contain the top color canvas
	Q3VBox * vb2 = new Q3VBox( hb0, "vb2" );
	vb2->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );

	// Create a vertical box to contain the bottom color canvas
	Q3VBox * vb5 = new Q3VBox( hb1, "vb5" );
	vb5->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );

	// Create a vertical box to contain the bottom color canvas
	Q3VBox * vb8 = new Q3VBox( hb2, "vb8" );
	vb5->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );

	// Create the top color label canvas 
    colorLabel0 = new ColorLabel(vb2);
	colorLabel0->setFixedHeight( COLOR_MIN_HEIGHT_ROW );

	// Create the bottom color label canvas 
    colorLabel1 = new ColorLabel(vb5);
	colorLabel1->setFixedHeight( COLOR_MIN_HEIGHT_ROW );

	// Create the bottom color label canvas 
    colorLabel2 = new ColorLabel(vb8);
	colorLabel2->setFixedHeight( COLOR_MIN_HEIGHT_ROW );

	// Create a hboxlayout that will fill the lowest row
	hb9 = new Q3HBox( this, "hb9" );

	// Create a placeholder 
	QLabel * emptyL = new QLabel( hb9, "emptyL" );

	// Create pushbuttons that will go into the lowest row
	QPushButton * done = new QPushButton( hb9, "done" );
	done->setText( "Done" ); 

	 // Define a callback for that button
        QObject::connect( done, SIGNAL(clicked()), this, SLOT(bdone()) );

	QPushButton * apply = new QPushButton( hb9, "apply" );
	apply->setText( "Apply" ); 

	 // Define a callback for that button
        QObject::connect( apply, SIGNAL(clicked()), this, SLOT(bapply()) );

	QPushButton * cancel = new QPushButton( hb9, "cancel" );
	cancel->setText( "Cancel" ); 

	 // Define a callback for that button
        QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(bcancel()) );

	hb2->setStretchFactor( emptyL, 10 );

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
void ColorBoard::setAtomBoardAddress( char * thisAB )
{
	ab = (AtomBoard * )thisAB;
}

// Set a pointer to the calling class
void ColorBoard::setSpinBoardAddress( char * thisSB )
{
	sb = (SpinBoard * )thisSB;
}


// Set a pointer to the calling class
void ColorBoard::setLcBoardAddress( char * thisLcB )
{	
	lcb = (LcBoard * )thisLcB;
}


// Set a pointer to the calling class
void ColorBoard::setPolymerBoardAddress( char * thisPoB )
{	
	pob = (PolymerBoard * )thisPoB;
}

// Set a pointer to the calling class
void ColorBoard::setPoreBoardAddress( char * thisPB )
{	
	pb = (PoreBoard * )thisPB;
}


// Build the layout of color labels and slider elements
void ColorBoard::buildLayout( char colors ) 
{
	numRows = colors+1; 

	switch (colors) {
		case 1:
	        	colorBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "colorBox" );

			colorBox->addMultiCellWidget( hb0, 0, 0, 1, -1);
			hb1->hide();
			hb2->hide();
		break;
		case 2:
	        	colorBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "colorBox" );

			colorBox->addMultiCellWidget( hb0, 0, 0, 1, -1);
			colorBox->addMultiCellWidget( hb1, 1, 1, 1, -1);
			hb1->show();
			hb2->hide();
		break;
		case 3:
	        	colorBox = new Q3GridLayout( this, numCols, numRows, SPACE, SPACE, "colorBox" );

			colorBox->addMultiCellWidget( hb0, 0, 0, 1, -1);
			colorBox->addMultiCellWidget( hb1, 1, 1, 1, -1);
			colorBox->addMultiCellWidget( hb2, 2, 2, 1, -1);
			hb1->show();
			hb2->show();
		break;
	}

	colorBox->addMultiCellWidget( hb9, numRows-1, numRows-1, 0, -1);
       	colorBox->setColStretch( canvCol, 10 );
	this->adjustSize( );	
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
		((AtomBoard *)ab)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (sb) {
		((SpinBoard *)sb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (lcb) {
		((LcBoard *)lcb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (pb) {
		((PoreBoard *)pb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (pob) {
		((PolymerBoard *)pob)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}

	// Hide now
        hide();

	// Close yourself
	if (ab) 
		((AtomBoard *)ab)->closeColorBoard();
	if (sb) 
		((SpinBoard *)sb)->closeColorBoard();
	if (lcb) 
		((LcBoard *)lcb)->closeColorBoard();
	if (pb) 
		((PoreBoard *)pb)->closeColorBoard();
	if (pb) 
		((PolymerBoard *)pob)->closeColorBoard();
}


// Accept the color setting: ask the atom board to retrieve and 
// save the color data
void ColorBoard::bapply()
{
	if (ab) {
		((AtomBoard *)ab)->getColors( red0, green0, blue0, red1, green1, blue1 );
	}
	if (sb) {
		((SpinBoard *)sb)->getColors( red0, green0, blue0, red1, green1, blue1 );
	}
	if (lcb) {
		((LcBoard *)lcb)->getColors( red0, green0, blue0, red1, green1, blue1, red2, green2, blue2 );
	}
	if (pb) {
		((PoreBoard *)pb)->getColors( red0, green0, blue0, red1, green1, blue1, red2, green2, blue2 );
	}
	if (pob) {
		((PolymerBoard *)pob)->getColors( red0, green0, blue0, red1, green1, blue1 );
	}
}


// Cancel the color setting: hide the board
void ColorBoard::bcancel()
{
	if (ab) {
		((AtomBoard *)ab)->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
		((AtomBoard *)ab)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (sb) {
		((SpinBoard *)sb)->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
		((SpinBoard *)sb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (lcb) {
		((LcBoard *)lcb)->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org, red2Org, green2Org, blue2Org );
		((LcBoard *)lcb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (pb) {
		((PoreBoard *)pb)->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org, red2Org, green2Org, blue2Org );
		((PoreBoard *)pb)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}
	if (pob) {
		((PolymerBoard *)pob)->getColors( red0Org, green0Org, blue0Org, red1Org, green1Org, blue1Org );
		((PolymerBoard *)pob)->getColorBoardPos( this->pos().x(), this->pos().y() );
	}

	// Hide now
	hide();

	// Close yourself
	if (ab) 
		((AtomBoard *)ab)->closeColorBoard();
	if (sb) 
		((SpinBoard *)sb)->closeColorBoard();
	if (lcb) 
		((LcBoard *)lcb)->closeColorBoard();
	if (pb) 
		((PoreBoard *)pb)->closeColorBoard();
	if (pob) 
		((PolymerBoard *)pob)->closeColorBoard();
}
