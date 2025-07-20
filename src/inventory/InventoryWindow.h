#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QWidget>

class QComboBox;
class QSpinBox;
class QPushButton;
class ProductManager;
class InventoryManager;
class BarcodeScanner;

class InventoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InventoryWindow(ProductManager *pm, InventoryManager *im,
                             BarcodeScanner *scanner = nullptr, QWidget *parent = nullptr);
public slots:
    void loadProducts();

private slots:
    void onAdd();
    void onRemove();
    void onBarcodeScanned(const QString &code);

private:
    ProductManager *m_pm;
    InventoryManager *m_im;
    BarcodeScanner *m_scanner;
    QComboBox *m_productBox;
    QSpinBox *m_qtySpin;
    QPushButton *m_addBtn;
    QPushButton *m_removeBtn;
};

#endif // INVENTORYWINDOW_H
