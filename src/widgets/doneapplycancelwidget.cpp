#include <widgets/doneapplycancelwidget.h>

#include <QHBoxLayout>
#include <QPushButton>

DoneApplyCancelWidget::DoneApplyCancelWidget(QWidget * parent)
    : QWidget(parent) {
    QPushButton *done = new QPushButton("Done");
    QObject::connect(done, SIGNAL(clicked()), SIGNAL(done()));

    QPushButton *apply = new QPushButton("Apply");
    QObject::connect(apply, SIGNAL(clicked()), SIGNAL(applied()));

    QPushButton *cancel = new QPushButton("Cancel");
    QObject::connect(cancel, SIGNAL(clicked()), SIGNAL(canceled()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(done);
    layout->addWidget(apply);
    layout->addWidget(cancel);
}

