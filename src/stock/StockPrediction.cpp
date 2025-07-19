#include "stock/StockPrediction.h"
#include <QtSql/QSqlQuery>
#include <QVariant>

StockPrediction::StockPrediction(QObject *parent)
    : QObject(parent)
{
}

int StockPrediction::predict(int productId) const
{
    // Dummy prediction using simple moving average of last 5 sales
    QSqlQuery query;
    query.prepare("SELECT quantity FROM sales WHERE product_id = :pid ORDER BY sale_date DESC LIMIT 5");
    query.bindValue(":pid", productId);
    if (!query.exec())
        return 0;
    int count = 0;
    int total = 0;
    while (query.next()) {
        total += query.value(0).toInt();
        ++count;
    }
    if (count == 0)
        return 0;
    return total / count;
}

bool StockPrediction::isCritical(int productId, int threshold) const
{
    QSqlQuery q;
    q.prepare("SELECT quantity FROM inventory WHERE product_id = :pid");
    q.bindValue(":pid", productId);
    if (!q.exec() || !q.next())
        return false;
    int qty = q.value(0).toInt();
    return qty < threshold;
}

