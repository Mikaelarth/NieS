#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QTimer>
#include <QVariantMap>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

DashboardWindow::DashboardWindow(SalesManager *sm, InventoryManager *im,
                                 int interval, QWidget *parent)
    : QWidget(parent), m_sm(sm), m_im(im)
{
    setWindowTitle(tr("Dashboard"));
    auto *layout = new QVBoxLayout(this);

    m_revenueLabel = new QLabel(this);
    m_revenueLabel->setObjectName("revenueLabel");
    layout->addWidget(m_revenueLabel);

    m_unitsLabel = new QLabel(this);
    m_unitsLabel->setObjectName("unitsLabel");
    layout->addWidget(m_unitsLabel);

    m_stockLabel = new QLabel(this);
    m_stockLabel->setObjectName("stockLabel");
    layout->addWidget(m_stockLabel);

    m_predictionList = new QListWidget(this);
    m_predictionList->setObjectName("predictionList");
    layout->addWidget(m_predictionList);

    connect(&m_timer, &QTimer::timeout, this, &DashboardWindow::refreshPredictions);
    m_timer.start(interval);

    if (m_sm)
        connect(m_sm, SIGNAL(saleRecorded(int,int)), this, SLOT(refreshPredictions()));
    if (m_im)
        connect(m_im, SIGNAL(stockChanged(int,int)), this, SLOT(refreshPredictions()));

    refresh();
}

void DashboardWindow::refresh()
{
    double revenue = 0.0;
    int units = 0;
    if (m_sm) {
        const QVariantMap report = m_sm->financialReport();
        revenue = report.value("revenue").toDouble();
        units = report.value("units").toInt();
    }

    int stock = 0;
    if (m_im) {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen()) {
            QSqlQuery q(db);
            q.prepare("SELECT SUM(quantity) FROM inventory");
            if (q.exec() && q.next())
                stock = q.value(0).toInt();
        }
    }

    if (m_revenueLabel)
        m_revenueLabel->setText(tr("Total revenue: %1").arg(QString::number(revenue)));
    if (m_unitsLabel)
        m_unitsLabel->setText(tr("Units sold: %1").arg(units));
    if (m_stockLabel)
        m_stockLabel->setText(tr("Stock on hand: %1").arg(stock));

    refreshPredictions();
}

void DashboardWindow::refreshPredictions()
{
    if (!m_predictionList)
        return;

    m_predictionList->clear();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
        return;

    QSqlQuery q(db);
    if (!q.exec("SELECT id, name FROM products"))
        return;

    while (q.next()) {
        int pid = q.value(0).toInt();
        QString name = q.value(1).toString();
        int pred = m_prediction.predict(pid);
        bool critical = m_prediction.isCritical(pid, 5);
        QString text = tr("%1: predicted %2").arg(name).arg(pred);
        if (critical)
            text += tr(" - Low stock");
        m_predictionList->addItem(text);
    }
}

