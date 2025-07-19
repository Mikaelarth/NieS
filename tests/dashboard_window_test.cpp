#include <QtTest>
#include <QApplication>
#include <QLabel>

#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include "dashboard_window_test.h"

void DashboardWindowTest::initUi()
{
    SalesManager sm;
    InventoryManager im;
    DashboardWindow w(&sm, &im);
    QCOMPARE(w.windowTitle(), QString("Dashboard"));
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));
    w.refresh();
    auto label = w.findChild<QLabel*>();
    QVERIFY(label);
}

