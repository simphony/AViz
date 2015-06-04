//
// Declaration of animation board class
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

#ifndef ANIMB_H
#define ANIMB_H 

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;

/*! @class AnimationBoard
    @brief Dialog for user to create animation for directory of images

    User selects directory of images and then can create an animation
    from these images.

    @todo user should have feedback that the animation is being created (progressbar)
*/
// File list board dialog widget
class AnimationBoard: public QDialog
{
    Q_OBJECT
public:
    AnimationBoard( QWidget * parent=0 );

private slots:
    /// create animation from images in selected directory
    void createAGIF();

    /// select directory containing images for animation
    void selectImageDirectory();

private:
    QLineEdit *m_fileLine;
    QPushButton *m_animGIF;
    QLabel *m_numberL;
    QString m_targetDir;
};

#endif // ANIMB_H
