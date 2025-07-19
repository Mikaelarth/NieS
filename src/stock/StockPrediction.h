#ifndef STOCKPREDICTION_H
#define STOCKPREDICTION_H

#include <QObject>

class StockPrediction : public QObject
{
    Q_OBJECT
public:
    explicit StockPrediction(QObject *parent = nullptr);

    int predict(int productId) const;
    bool isCritical(int productId, int threshold) const;
};

#endif // STOCKPREDICTION_H
