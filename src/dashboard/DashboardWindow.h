#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>

class SalesManager;
class InventoryManager;

class QLabel;

class DashboardWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardWindow(SalesManager *sm, InventoryManager *im, QWidget *parent = nullptr);

public slots:
    void refresh();

private:
    SalesManager *m_sm;
    InventoryManager *m_im;
    QLabel *m_revenueLabel = nullptr;
    QLabel *m_unitsLabel = nullptr;
    QLabel *m_stockLabel = nullptr;
};

#endif // DASHBOARDWINDOW_H
