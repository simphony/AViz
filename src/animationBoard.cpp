//
// Implementation of animation board dialog widget class
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

#include "animationBoard.h"

#include <dirent.h>
#include <iostream>

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

#include "defaults.h"

const int MIN_FILE_LINE_WIDTH = 500;

// Make a popup dialog box
AnimationBoard::AnimationBoard(QWidget * parent)
    : QDialog(parent),
      haveTarget(false) {
    setWindowTitle("AViz: Animation Generator");

    // Create a label
    QLabel *targetL = new QLabel(" Target Directory: ");

    // Create a text window
    fileLine = new QLineEdit("--");

    // Create a pushbutton
    QPushButton * browsePb = new QPushButton("Browse...");

    // Create a label
    numberL = new QLabel(" Note: Image files in target directory must be named abc.0001.png, abc.0002.png, etc");

    // Create more pushbuttons that will go into the lowest row
    animGIF = new QPushButton("Create AnimatedGIF");
    animGIF->setEnabled(haveTarget);

    QPushButton *done = new QPushButton("Done");

    // layout widgets
    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(targetL, 0 /*row*/, 0 /*col*/);
    grid->addWidget(fileLine, 0 /*row*/, 1 /*col*/);
    grid->addWidget(browsePb, 0 /*row*/, 2 /*col*/);
    grid->addWidget(numberL, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, 3/*colSpan*/);
    grid->addWidget(animGIF, 2 /*row*/, 0 /*col*/);
    grid->addWidget(done, 2 /*row*/, 2 /*col*/);

    // create connections
    QObject::connect(browsePb, SIGNAL(clicked()), this, SLOT(browseCB()) );
    QObject::connect(animGIF, SIGNAL(clicked()), this, SLOT(createAGIF()) );
    QObject::connect(done, SIGNAL(clicked()), this, SLOT(close()) );

    this->setMinimumWidth(MIN_FILE_LINE_WIDTH);
}


// Create animatedGIF
void AnimationBoard::createAGIF() {
    if (haveTarget) {
        // Create an animated GIF
        QString command  = QString("cd %1; convert *.png* anim.gif").arg(targetDir);
        QString msg = QString("Executing command %1\n").arg(command);
        std::cout << msg.toStdString();
        system(qPrintable(command));
    }
}


// Launch a file browser
void AnimationBoard::browseCB() {
    DIR *dirp;
    struct dirent *direntp;
    char * filename = (char *)malloc(BUFSIZ);

    QString fn (QFileDialog::getExistingDirectory(this, QString(), "./") );
    // Register the selection
    if ( !fn.isEmpty() ) {
        targetDir = fn;
        fileLine->setText( targetDir );

        // Set a flag
        haveTarget = true;
        animGIF->setEnabled( true );
    }

    // Count number of PNG files
    int fileNumber = 0;
    dirp = opendir(qPrintable(targetDir));
    while ( (direntp = readdir( dirp )) != NULL ) {
        sprintf(filename, "%s", direntp->d_name );
        if (strstr(filename, ".png"))
            fileNumber++;
    }

    closedir( dirp );

    // Update the label
    numberL->setText(QString("Number of PNG files in target directory: %1").arg(fileNumber));

    free(filename);
}

