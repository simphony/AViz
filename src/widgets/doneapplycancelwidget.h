
#ifndef DONE_APPLY_CANCEL_WIDGET_H
#define DONE_APPLY_CANCEL_WIDGET_H

#include <QWidget>

/*! @class DoneApplyCancelWidget
    @brief Widget containing done, apply, cancel buttons

    Widget has done, apply and cancel buttons.  A signal
    is emitted for when each of these buttons are clicked.
    The buttons are laid out horizontally.
*/
class DoneApplyCancelWidget: public QWidget
{
    Q_OBJECT

public:
    DoneApplyCancelWidget(QWidget * parent);

signals:
    /// This signal is emitted whenever the done button is pushed
    void done();

    /// This signal is emitted whenever the apply button is pushed
    void applied();

    /// This signal is emitted whenever the cancel button is pushed
    void canceled();
};

#endif
