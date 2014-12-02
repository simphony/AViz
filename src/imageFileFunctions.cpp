//
// Implementation of functions used in image file handling
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

#include "imageFileFunctions.h"

#include <png.h>	// Best kept away from header file!

// Return a suitable filename
void getImageFilename( char * datafile, char * filename )
{	
	sprintf(filename, "%s", datafile);
	char * ptr = filename+strlen(filename);

	// Find the last '.' in the data filename
	do {
		ptr--;
	}
	while ( (*ptr) != '.' && strlen(ptr) >0);

	if (strlen(ptr) < 1) {
		sprintf(filename, "%s.png", datafile);
	}
	else {
		(*(ptr+1)) = 'p';
		(*(ptr+2)) = 'n';
		(*(ptr+3)) = 'g';
		(*(ptr+4)) = 0;
	} 
}


// Modify the filename to avoid overwriting
void modifyImageFilename( char * filename )
{
	struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
        char * newFilename = (char *)malloc( BUFSIZ );
	
	// Add an appendix and try again.  Cut off the 
	// filename
	int count = 1;
	char * ptr = strstr(filename, ".png" );
	if (ptr)
		(*ptr) = 0;

	// Add a new suffix
	do {
		// The filename should be of the sort 
		// file.XXXX.png
		if (count > 999)
			sprintf( newFilename, "%s.%d.png", filename, count);
		else if (count > 99)
			sprintf( newFilename, "%s.0%d.png", filename, count);
		else if (count > 9)
			sprintf( newFilename, "%s.00%d.png", filename, count);
		else 
			sprintf( newFilename, "%s.000%d.png", filename, count);
		count++;	
	} while (!stat( (const char *)newFilename, buf ) );

	// Found a good name finally
	strcpy ( filename, newFilename );

	free (newFilename);
}


// Write a PNG image file
bool writePNGImage( const char * filename, char * comment, int thisWidth, int thisHeight, char * imageRed, char * imageGreen, char * imageBlue )
{
        FILE * out;
	png_structp png_ptr;
	png_infop info_ptr;
	png_text text_ptr[3];
	png_uint_32 i, j, k, l, m, height, width, bit_depth, bytes_per_pixel;
	png_byte * img;
	png_bytep * row_pointers;

	width = thisWidth;
	height = thisHeight;
	bit_depth = 8;
	bytes_per_pixel = 3;	

	// Allocate memory
 	img = (png_byte *)malloc( thisWidth*thisHeight*bytes_per_pixel*sizeof(png_byte) );
	row_pointers = (png_bytep *)malloc( thisHeight*sizeof(png_bytep) );

	// Copy the image data into the PNG data structure
	for (i=0;i<width;i++) {
		for (j=0;j<height;j++) {
			k = i*bytes_per_pixel;
			l = j*width*bytes_per_pixel+k+0;
			m = (height-j-1)*width+i;
			img[l] = imageRed[m];
			l++;
			img[l] = imageGreen[m];
			l++;
			img[l] = imageBlue[m];
		}
	}

        // Open the file
        if ((out = fopen( (char *)filename, "w" ))) {
		// Create and initialize the png_struct 
   		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fclose(out);
			free(row_pointers);
			free(img);
			return FALSE;
   		}

		// Allocate/initialize the image information data
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fclose(out);
			png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
			free(row_pointers);
			free(img);
			return FALSE;
   		}

		// Set error handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			// If we get here, we had a problem reading the file 
			fclose(out);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			free(row_pointers);
			free(img);
			return FALSE;
   		}

		// I/O initialization function
		png_init_io(png_ptr, out);

		// Set the image information here
		png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		// Write comments about the image 
		text_ptr[0].key = "Title";
		text_ptr[0].text = "AViz screen shot";
		text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[1].key = "Author";
		text_ptr[1].text = "AViz";
		text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[2].key = "Description";
		text_ptr[2].text = (char *)comment;
		text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#ifdef PNG_iTXt_SUPPORTED
		text_ptr[0].lang = NULL;
		text_ptr[1].lang = NULL;
		text_ptr[2].lang = NULL;
#endif
		png_set_text(png_ptr, info_ptr, text_ptr, 3);

		// Write the file header information
		png_write_info(png_ptr, info_ptr);

		// Set image data pointers
		for (m=0;m<height;m++) 
			row_pointers[m] = (png_bytep)img + m*width*bytes_per_pixel;

		// Write image data now
		png_write_image(png_ptr, row_pointers);

		// Finish up
		png_write_end(png_ptr, info_ptr);

		// Clean up after the write, and free any memory allocated 
		png_destroy_write_struct(&png_ptr, &info_ptr);
		free(row_pointers);
		free(img);

		// Close the file 
		fclose(out);

		return TRUE;
        }
        else {
		return FALSE;
        }
}
