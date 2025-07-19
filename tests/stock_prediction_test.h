#ifndef STOCK_PREDICTION_TEST_H
#define STOCK_PREDICTION_TEST_H

#include <QObject>

class StockPredictionTest : public QObject
{
    Q_OBJECT
private slots:
    void predictionAndCritical();
};

#endif // STOCK_PREDICTION_TEST_H
