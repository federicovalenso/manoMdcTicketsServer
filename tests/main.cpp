#include "ctstest.h"

//  auto* cts_test = new CtsTest(&app);
//  QObject::connect(cts_test, SIGNAL(TestsFinished()), &app, SLOT(quit()),
//                   Qt::QueuedConnection);

//  QTimer::singleShot(0, cts_test, SLOT(onEventLoopStarted()));

QTEST_GUILESS_MAIN(CtsTest)
