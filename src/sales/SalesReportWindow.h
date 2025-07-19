#ifndef SALESREPORTWINDOW_H
#define SALESREPORTWINDOW_H

#include <QWidget>

class QTableWidget;
class SalesManager;

class SalesReportWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SalesReportWindow(SalesManager *sm, QWidget *parent = nullptr);

public slots:
    void loadReport();

private:
    SalesManager *m_sm;
    QTableWidget *m_table;
};

#endif // SALESREPORTWINDOW_H
