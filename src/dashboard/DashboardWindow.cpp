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
#include <QStringList>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>

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

    auto *salesChart = new QtCharts::QChart();
    salesChart->setTitle(tr("Daily Revenue"));
    m_salesChart = new QtCharts::QChartView(salesChart, this);
    m_salesChart->setObjectName("salesChart");
    layout->addWidget(m_salesChart);

    auto *stockChart = new QtCharts::QChart();
    stockChart->setTitle(tr("Inventory Levels"));
    m_stockChart = new QtCharts::QChartView(stockChart, this);
    m_stockChart->setObjectName("stockChart");
    layout->addWidget(m_stockChart);

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

    if (m_salesChart) {
        auto chart = m_salesChart->chart();
        chart->removeAllSeries();
        QStringList categories;
        auto *set = new QtCharts::QBarSet(tr("Revenue"));
        QSqlQuery q("SELECT sale_date, SUM(total) FROM sales GROUP BY sale_date ORDER BY sale_date");
        while (q.next()) {
            *set << q.value(1).toDouble();
            categories << q.value(0).toString();
        }
        auto *series = new QtCharts::QBarSeries(chart);
        series->append(set);
        chart->addSeries(series);
        auto *axis = new QtCharts::QBarCategoryAxis(chart);
        axis->append(categories);
        chart->setAxisX(axis, series);
        chart->createDefaultAxes();
    }

    if (m_stockChart) {
        auto chart = m_stockChart->chart();
        chart->removeAllSeries();
        auto *series = new QtCharts::QPieSeries(chart);
        QSqlQuery q("SELECT product_id, quantity FROM inventory");
        while (q.next())
            series->append(QString::number(q.value(0).toInt()), q.value(1).toInt());
        chart->addSeries(series);
    }

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

