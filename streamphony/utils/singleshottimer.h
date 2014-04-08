#ifndef SINGLESHOTTIMER_H
#define SINGLESHOTTIMER_H

#include <QTimer>

namespace utils {

template <typename Func>
inline void singleShotTimer(int interval, Func lambda, QObject *parent)
{
    //! NOTE: parent is mandatory, for the case the parent is destroyed *before* the timer has fired.
    //!       In that case the timer should be stopped and the lambda not called any more.
    QTimer *timer = new QTimer(parent);
    timer->setSingleShot(true);
    timer->setInterval(interval);
    QObject::connect(timer, &QTimer::timeout, [timer, lambda] {
        timer->deleteLater();
        lambda();
    });
    timer->start();
}

}

#endif // SINGLESHOTTIMER_H
