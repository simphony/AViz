//
// Definition of default file names and factory defaults
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

#ifndef DEFAULTS_H
#define DEFAULTS_H

#define particleDataFile "aviz.particle"
#define defaultViewParamFile "aviz.vpm"
#define defaultTrackDataFile "trackfile.dat"

#define FOVY 10.0
#define PHI 250.0
#define THETA 0.0
#define CHI 0.0
#define DOLLY 1.0
#define SIZE 1.0
#define BOND_MINLENGTH 0.0
#define BOND_MAXLENGTH 10.0
#define BDIAMETER 0.1

#define FALSE 0
#define TRUE 1

// Max number of atom types
#define ATOMS_MAX 256

// Max number of files in file list; max length of file names
#define MAXFILELIST 1000

// Max number of bonds to different atom types
#define BONDS_MAX 100

// Max number of lights 
#define LIGHTS_MAX 8

// Size of graphics window
#define FRAME_SIZE 400

// Define graphics window boundary
#define FRAME_WIDTH 24

// Define emergency size
#define MIN_SIZE 100

// Define spacer distances and dimensions
#define SPACE 5
#define ROW_HEIGHT 45 
#define LABEL_HEIGHT 30
#define COLORLABEL_WIDTH 30 
#define SPIN_WIDTH 30
#define SLIDER_WIDTH 150 
#define SLIDER_HEIGHT 15 
#define LABEL_WIDTH 45 
#define SMALL_LABEL_WIDTH 20

#define COLOR_MIN 0
#define COLOR_MAX 100
#define COLOR_MIN_WIDTH 350 
#define COLOR_MIN_HEIGHT_ROW 60 

#define LIGHT_MIN 0
#define LIGHT_MAX 100
#define SHINE_MIN 0
#define SHINE_MAX 128

#define RENDERS_MIN 5 
#define RENDERS_MAX 995
#define RENDERS_STEP 5
#define RENDERS0 100

// Define mouse operations
#define MOUSE_PAN_STEP 0.2
#define MOUSE_TRANSFORMATION_STEP 50.0

//Definition of default atom/spin types and colors, to be
// used if no default color file can be stated;
#define aluminium "Al 0.78 0.88 0.88" 
#define carbon "C 0.7 0.5 0.25"
#define copper "Cu 0.93 0.5 0.25"
#define germanium "Ge 0.8 1.0 0.9"
#define gold "Au 1.0 0.81 0.15"
#define helium "He 0.5 1.0 0.8"
#define hydrogen "H 0.0 0.9 0.0"
#define lead "Pb 0.0 0.2 0.2"
#define oxygen "O 0.0 0.0 0.77"
#define silicium "Si 1.0 0.4 0.0"
#define silver "Ag 0.8 0.8 0.7"
#define vanadium "Va 0.2 1.0 0.9"
#define unidentified "?? 0.5 0.5 0.5"   // Used if identification of type fails

// Miscellaneous definitions
#define FALSE 0
#define TRUE 1
#define SPACE_CHAR 32
#define PI 3.1415927
#define TWO_PI 6.28318530
#define INFINITY 10000.0
#define DELTAEPS 1.0E-10
#define SCEPS 0.0000001
#define SLICEEPS 0.1;
#define MIN_REPEAT 1
#define MIN_ZOOM_REPEAT 5
#define MAX_REPEAT 10
#define WHEEL_REPEAT 50

// Text sizes
#define TEXT_SIZE0 0
#define TEXT_SIZE1 1
#define TEXT_SIZE2 2
#define TEXT_SIZE3 3

// Colors
#define white_color 1.0, 1.0, 1.0 
#define yellow_color 1.0, 1.0, 0.0 
#define darkyellow_color 1.0, 0.5, 0.0 
#define red_color 1.0, 0.0, 0.0 
#define green_color 0.0, 1.0, 0.0 
#define blue_color 0.0, 0.0, 1.0 
#define black_color 0.0, 0.0, 0.0 
#define gold_color 0.83, 0.61, 0.21
#define silver_color 0.59, 0.59, 0.59
#define bronze_color 0.83, 0.56, 0.45
#define track_color 0.5, 0.95, 0.75
#define unknown_color "0.3, 0.15, 1.0" 
#define grey_color 0.3, 0.3, 0.3

// Particle numbers
#define MANY_ATOMS 1000
#define VERY_MANY_ATOMS 10000
#define VERY_VERY_MANY_ATOMS 50000

// Rendering constants
#define MAX_PARTICLE_TYPES 300
#define MAX_TRACK_TYPES 300
#define SAFETY_DIST 1.5
#define RELSIZE_FACTOR 1.5

#endif // DEFAULTS_H 
