#ifndef POSWINDOW_H
#define POSWINDOW_H

#include <QWidget>

class QComboBox;
class QSpinBox;
class QPushButton;
class ProductManager;
class SalesManager;

class POSWindow : public QWidget
{
    Q_OBJECT
public:
    explicit POSWindow(ProductManager *pm, SalesManager *sm, QWidget *parent = nullptr);

public slots:
    void loadProducts();

private slots:
    void onSell();

private:
    ProductManager *m_pm;
    SalesManager *m_sm;
    QComboBox *m_productBox;
    QSpinBox *m_qtySpin;
    QPushButton *m_sellBtn;
};

#endif // POSWINDOW_H
