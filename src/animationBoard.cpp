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

#include <iostream>

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDir>
#include <QFileDialog>

const int MIN_FILE_LINE_WIDTH = 500;

// Make a popup dialog box
AnimationBoard::AnimationBoard(QWidget * parent)
    : QDialog(parent) {
    setWindowTitle("AViz: Animation Generator");

    setModal(true);

    // Create a text window
    m_fileLine = new QLineEdit("--");
    m_fileLine->setReadOnly(true);

    // Create a pushbutton
    QPushButton * browsePb = new QPushButton("Browse...");

    // Create a label
    m_numberL = new QLabel("Note: Image files in target directory must be named abc.0001.png, abc.0002.png, etc");

    // Create more pushbuttons that will go into the lowest row
    m_animGIF = new QPushButton("Create AnimatedGIF");
    m_animGIF->setEnabled(false);

    QPushButton *done = new QPushButton("Done");

    // layout widgets
    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(new QLabel(" Target Directory: "), 0 /*row*/, 0 /*col*/);
    grid->addWidget(m_fileLine, 0 /*row*/, 1 /*col*/);
    grid->addWidget(browsePb, 0 /*row*/, 2 /*col*/);
    grid->addWidget(m_numberL, 1 /*fromRow*/, 0 /*fromCol*/, 1 /*rowSpan*/, 3/*colSpan*/);
    grid->addWidget(m_animGIF, 2 /*row*/, 0 /*col*/);
    grid->addWidget(done, 2 /*row*/, 2 /*col*/);

    // create connections
    QObject::connect(browsePb, SIGNAL(clicked()), this, SLOT(selectImageDirectory()) );
    QObject::connect(m_animGIF, SIGNAL(clicked()), this, SLOT(createAGIF()) );
    QObject::connect(done, SIGNAL(clicked()), this, SLOT(close()) );

    this->setMinimumWidth(MIN_FILE_LINE_WIDTH);
}


// Create animatedGIF
void AnimationBoard::createAGIF() {
    // Create an animated GIF
    QString command  = QString("cd %1; convert *.png* anim.gif").arg(m_targetDir);
    QString msg = QString("Executing command %1\n").arg(command);
    std::cout << msg.toStdString();
    int exitCode = system(qPrintable(command));

    if(exitCode == 0) {
        m_numberL->setText(QString("anim.gif has been created"));
    } else {
        m_numberL->setText(QString("Error occured when trying to create anim.gif (Return code %1)").arg(exitCode));
    }
}

namespace {

// count number of png files in the directory
int getNumberOfPngFiles(const QString& directoryName) {

    QDir directory(directoryName);
    QStringList namedFilter;
    namedFilter << "*.png";
    QStringList pngFileNames = directory.entryList(namedFilter,
                                               QDir::Files /*filter: only files*/
                                               );
    return pngFileNames.size();
}
}

void AnimationBoard::selectImageDirectory() {
    QFileDialog fd(this,
                   QString("Select target directory containing image files"));
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ReadOnly);

    if (fd.exec()) {
        QString directoryName = fd.selectedFiles()[0];

        m_targetDir = directoryName;
        m_fileLine->setText( m_targetDir );

        int nFiles = getNumberOfPngFiles(m_targetDir);
        m_animGIF->setEnabled(nFiles > 0  ? true : false);

        m_numberL->setText(QString("Number of PNG files in target directory: %1").arg(nFiles));
    }
}

