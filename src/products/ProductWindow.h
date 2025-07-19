#ifndef PRODUCTWINDOW_H
#define PRODUCTWINDOW_H

#include <QWidget>

class QTableWidget;
class QLineEdit;
class QDoubleSpinBox;
class QPushButton;
class ProductManager;

class ProductWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ProductWindow(ProductManager *pm, QWidget *parent = nullptr);

public slots:
    void loadProducts();

private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onRowSelected();

private:
    ProductManager *m_pm;
    QTableWidget *m_table;
    QLineEdit *m_nameEdit;
    QDoubleSpinBox *m_priceEdit;
    QDoubleSpinBox *m_discountEdit;
    QPushButton *m_addBtn;
    QPushButton *m_editBtn;
    QPushButton *m_deleteBtn;
};

#endif // PRODUCTWINDOW_H
