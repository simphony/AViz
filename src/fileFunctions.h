//
// Declaration of functions used in file handling
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

#ifndef FILE_H
#define FILE_H

#include "data.h"

class QString;
class AggregateData;

void checkSuffix( const char *, const char * );
bool fileExists( const char *);

/*! Read a coordinate file in XYZ format.
 *
 * There can be up to eight additional property columns per line
 *
 */
bool openCoordinateFunction( const char *filename, AggregateData * );

bool openFileListFunction( const char *filename, fileList * );

bool generateTrackDataFunction(const fileList&, AggregateData *, trackData * );

/*! Save view parameters to file
 *  \return true if saved, false if file could not be opened
 */
bool saveViewParamFunction( const char *filename, viewParam * );

/*! Load view parameters from file
 *  \return true if saved, false if file could not be opened or incorrect version
 */
bool openViewParamFunction( const char *filename, viewParam * );

/*! Load particle data function (i.e. if and how to color what kind of particle) from file
 *  \return true if saved, false if file could not be opened or incorrect version
 */
bool openParticleDataFunction( const char *filename, particleData * );

/*! Save particle data function (i.e. if and how to color what kind of particle) to file
 *  \return true if saved, false if file could not be opened
*/
bool saveParticleDataFunction( const char *filename, particleData * );

/*! Parse line of xzy file
 *
 * Line consists of type, coordinates (x,y,z) and there can be up to eight additional
 * property columns per line.
 *
 * \throws aviz::parse_error
 * \sa openCoordinateFunction
 * \return particle contained in that line
 */
particle parseParticleLine(const QString& line);


#endif // FILE_H
