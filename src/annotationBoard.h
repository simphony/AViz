//
// Declaration of annotation board class
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

#ifndef ANNOTATION_BOARD_H
#define ANNOTATION_BOARD_H

#include <QDialog>

#include "data.h"

class QCheckBox;
class QLineEdit;
class QSpinBox;

class MainForm;


/*! @class AnnotationBoard
    @brief Annotation dialog where annotation text can be configured
*/
class AnnotationBoard: public QDialog {
    Q_OBJECT
public:
    AnnotationBoard(QWidget *parent, MainForm *);

public slots:
    void setAnnotation(const viewParam&);

private slots:
    /// handle when done is clicked (apply and close)
    void bdone();

    /// apply the settings
    void applySettings();

private:
    /// register the current annotation-related view parameters
    void registerSettings();

private:
    MainForm * mainForm;
    QCheckBox * showAnnotationCb;
    QLineEdit * annotationTextLe;
    QSpinBox * annotationXSb;
    QSpinBox * annotationYSb;
    QSpinBox * annotationSizeSb;
};

#endif // ANNOTB_H
