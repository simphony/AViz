//
// Declaration of file list board class
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

#ifndef FILELB_H
#define FILELB_H 

#include "data.h"
#include "defaults.h"
#include "fileFunctions.h"
#include "mainForm.h"

#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qsizepolicy.h>
#include <qstring.h>
#include <qtimer.h>
#include <qwidget.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// File list board dialog widget
class FileListBoard: public QDialog
{
	Q_OBJECT
public:
	FileListBoard( QWidget * parent=0, const char * name=0 );

        enum listDirection{ FORWARD, BACKWARD }; // Define directions

public slots:
	void setMainFormAddress( MainForm * );
	void setFileListParam( int, int, const QString & );
	void resetKeepViewScale( void );

private slots:
	void showCurrentFile();
	void keepViewScaleChanged();
	void launchTrack();
	void bsingleStep();
	void bfastStep();
	void bcycle();
	void start();
	void end();
	void bdirection( int );
	listDirection getDirection();
	void bdone();

private:
	MainForm * mainForm;
	QButtonGroup * direction;
	QRadioButton * forwardRb;
	QRadioButton * backwardRb;
	QLineEdit * fileLine;
	QCheckBox * keepViewScaleCb;
	QTimer * cycleTimer;
	int numberOfFiles;
	int currentFile;
	listDirection thisDirection;
	bool cycleMode;
	QString fileListName;
	bool haveFileList;
};

#endif // FILELB_H
