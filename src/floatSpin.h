//
// Declaration of floating point spin box class
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

#ifndef FSPIN_H
#define FSPIN_H 

#include <qspinbox.h>

// Define floating-point spin box
class QFSpinBox: public QSpinBox 
{ 
	Q_OBJECT
public:
	QFSpinBox( QWidget* parent = 0, const char *name = 0);

	QString mapValueToText( int value );
	int mapTextToValue( bool* ok );
};

#endif // FSPIN_H
