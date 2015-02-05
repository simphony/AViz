//
// Implementation of annotation board dialog widget class
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

#include "annotationBoard.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mainForm.h"
#include "widgets/doneapplycancelwidget.h"

namespace {
QSpinBox* createPositionSpinBox(QWidget* parent) {
    QSpinBox *sb = new QSpinBox(parent);
    sb->setMinimum(0);
    sb->setMaximum(1000);
    sb->setSingleStep(10);
    return sb;
}
}

// Make a popup dialog box that will hold a vertical row of buttons
AnnotationBoard::AnnotationBoard(QWidget * parent, MainForm *mainForm)
    : QDialog(parent), mainForm(mainForm) {
    setWindowTitle("AViz: Set Annotation");

    QVBoxLayout *vbox = new QVBoxLayout(this);

    {
        // Create a annotation line edit and check box
        annotationTextLe = new QLineEdit(this);
        showAnnotationCb = new QCheckBox("Show Annotation", this);

        QWidget *w = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(w);
        layout->addWidget(new QLabel("Annotation:"), 0 /*stretch*/, Qt::AlignRight);
        layout->addWidget(annotationTextLe, 1 /*stretch*/);
        layout->addWidget(showAnnotationCb, 0 /*stretch*/, Qt::AlignRight);
        vbox->addWidget(w);
    }

    {   // create size and position (x,y) input widgets
        annotationSizeSb = new QSpinBox(this);
        annotationSizeSb->setMinimum(1);
        annotationSizeSb->setMaximum(4);
        annotationSizeSb->setSingleStep(1);

        annotationXSb = createPositionSpinBox(this);
        annotationYSb = createPositionSpinBox(this);

        QWidget *w = new QWidget();
        QGridLayout *layout = new QGridLayout(w);
        layout->addWidget(new QLabel("Size:"), 0, 0, Qt::AlignRight);
        layout->addWidget(annotationSizeSb, 0, 1);
        layout->addWidget(new QLabel("X Position:"), 0, 2, Qt::AlignRight);
        layout->addWidget(annotationXSb, 0, 3);
        layout->addWidget(new QLabel("Y Position:"), 0, 4, Qt::AlignRight);
        layout->addWidget(annotationYSb, 0, 5);
        vbox->addWidget(w);
    }

    DoneApplyCancelWidget *doneApplyCancel = new DoneApplyCancelWidget(this);
    connect(doneApplyCancel, SIGNAL(done()), this, SLOT(bdone()) );
    connect(doneApplyCancel, SIGNAL(applied()), this, SLOT(applySettings()) );
    connect(doneApplyCancel, SIGNAL(canceled()), this, SLOT(hide()));
    vbox->addStretch(1);
    vbox->addWidget(doneApplyCancel);
}

// Get the current view settings from main form, register
// it using a local copy, and update the settings
void AnnotationBoard::setAnnotation(const viewParam& vp) {
    annotationTextLe->setText(vp.annotationText);
    showAnnotationCb->setChecked(vp.showAnnotation);
    annotationSizeSb->setValue(vp.annotationSize);
    annotationXSb->setValue(vp.annotationCoordX);
    annotationYSb->setValue(vp.annotationCoordY);
}


// Register the current settings
void AnnotationBoard::registerSettings() {
    // Update the view parameters with
    // the current settings

    viewParam * vp = mainForm->getViewParam();

    // Update the values
    QString text = annotationTextLe->text();
    sprintf((*vp).annotationText, "%s", qPrintable(text));
    (*vp).showAnnotation = showAnnotationCb->isChecked();
    (*vp).annotationSize = annotationSizeSb->value();
    (*vp).annotationCoordX = annotationXSb->value();
    (*vp).annotationCoordY = annotationYSb->value();
}


// Apply the annotation setting and hide dialog
void AnnotationBoard::bdone() {
    applySettings();
    hide();
}


// Accept the annotation setting
void AnnotationBoard::applySettings() {
    registerSettings();

    // Re-do the graphics, using the new view settings
    mainForm->updateRendering();
}
