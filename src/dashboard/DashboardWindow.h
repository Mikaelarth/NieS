#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>

class QListWidget;
#include "stock/StockPrediction.h"

class SalesManager;
class InventoryManager;

class QLabel;

class DashboardWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardWindow(SalesManager *sm, InventoryManager *im,
                             int interval = 60000, QWidget *parent = nullptr);

public slots:
    void refresh();
    void refreshPredictions();

private:
    SalesManager *m_sm;
    InventoryManager *m_im;
    StockPrediction m_prediction;
    QTimer m_timer;
    QListWidget *m_predictionList = nullptr;
    QLabel *m_revenueLabel = nullptr;
    QLabel *m_unitsLabel = nullptr;
    QLabel *m_stockLabel = nullptr;
    QtCharts::QChartView *m_salesChart = nullptr;
    QtCharts::QChartView *m_stockChart = nullptr;
};

#endif // DASHBOARDWINDOW_H
