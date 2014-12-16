//
// Declaration of message box class
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

#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <QDialog>

/*! @class MessageBox
    @brief Dialog that shows message

    Dialog shows a message and an "ok" button.  Once,
    the ok button is pushed, the dialog is closed (and destroyed).
    The dialog is non-modal and has a title ("AViz: Message").
*/
class MessageBox: public QDialog
{
public:
    MessageBox(const QString& message, QWidget *parent=0);
};

#endif // MESSAGE_BOX_H
