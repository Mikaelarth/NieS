#ifndef STOCKPREDICTIONWINDOW_H
#define STOCKPREDICTIONWINDOW_H

#include <QWidget>
#include <QTimer>
#include "stock/StockPrediction.h"

class QListWidget;
class SalesManager;
class InventoryManager;

class StockPredictionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StockPredictionWindow(SalesManager *sm = nullptr,
                                   InventoryManager *im = nullptr,
                                   QWidget *parent = nullptr);

public slots:
    void refreshPredictions();

private:
    StockPrediction m_prediction;
    SalesManager *m_sm;
    InventoryManager *m_im;
    QListWidget *m_list;
    QTimer m_timer;
};

#endif // STOCKPREDICTIONWINDOW_H
