#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>

class SalesManager;
class InventoryManager;

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
};

#endif // DASHBOARDWINDOW_H
