//
// Implementation of customized file dialog class
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

#include "customFileDialog.h"

#include <QPushButton>
#include <QLayout>
#include <QDir>
#include <QTextStream>

const QString generalFilter = "General (*)";
const QString xyzFilter = "XYZ coordinate files (*.xyz)";


// Define a file dialog with a generate file list button
CustomFileDialog::CustomFileDialog( )
    :  QFileDialog () {
    setFileMode( QFileDialog::ExistingFile );

    QStringList filts;
    filts << xyzFilter << generalFilter;
    setNameFilters(filts);
    selectNameFilter(xyzFilter);

    /// @todo this needs to be changed/reevaulated
    /// in qt3 this was added with addToolButton
    /// however, this is deprecated
    QPushButton * genFileListPb = new QPushButton(this);
	genFileListPb->setText( "Generate File List" );
    layout()->addWidget(genFileListPb);
    connect(genFileListPb, SIGNAL(clicked()), SLOT(generateFileList()));
}

CustomFileDialog::~CustomFileDialog() {
}



// Generate a file list
void CustomFileDialog::generateFileList() {
    QStringList xzyFileNames = directory().entryList(QStringList() << "*.xyz",
                                                   QDir::Files, /*filter: only files*/
                                                   QDir::QDir::Name /*sort*/
                                                   );


    QFile file(directory().absoluteFilePath("filelist.dat"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        QStringList::const_iterator iter;
        for (iter = xzyFileNames.constBegin(); iter != xzyFileNames.constEnd(); ++iter)
            out << (*iter).toLocal8Bit().constData() << endl;
        file.close();

        // Re-read the directory
        selectNameFilter( generalFilter );
    }
}
