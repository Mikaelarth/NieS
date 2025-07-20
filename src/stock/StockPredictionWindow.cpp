#include "stock/StockPredictionWindow.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include "SalesManager.h"
#include "InventoryManager.h"

StockPredictionWindow::StockPredictionWindow(SalesManager *sm, InventoryManager *im, QWidget *parent)
    : QWidget(parent), m_sm(sm), m_im(im), m_list(new QListWidget(this))
{
    setWindowTitle(tr("Stock Predictions"));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_list);

    connect(&m_timer, &QTimer::timeout, this, &StockPredictionWindow::refreshPredictions);
    m_timer.start(60000);

    if (m_sm)
        connect(m_sm, SIGNAL(saleRecorded(int,int)), this, SLOT(refreshPredictions()));
    if (m_im)
        connect(m_im, SIGNAL(stockChanged(int,int)), this, SLOT(refreshPredictions()));

    refreshPredictions();
}

void StockPredictionWindow::refreshPredictions()
{
    if (!m_list)
        return;

    m_list->clear();

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
        bool crit = m_prediction.isCritical(pid, 5);
        QString text = tr("%1: predicted %2").arg(name).arg(pred);
        if (crit)
            text += tr(" - Low stock");
        m_list->addItem(text);
    }
}
