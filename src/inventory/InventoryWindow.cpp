#include "inventory/InventoryWindow.h"
#include "ProductManager.h"
#include "InventoryManager.h"
#include "barcode/BarcodeScanner.h"
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

namespace {
enum ProductRoles {
    IdRole = Qt::UserRole,
    PriceRole,
    DiscountRole
};
}

InventoryWindow::InventoryWindow(ProductManager *pm, InventoryManager *im,
                                 BarcodeScanner *scanner, QWidget *parent)
    : QWidget(parent), m_pm(pm), m_im(im), m_scanner(scanner)
{
    setWindowTitle(tr("Inventory"));

    m_productBox = new QComboBox(this);
    m_productBox->setObjectName("productBox");

    m_qtySpin = new QSpinBox(this);
    m_qtySpin->setObjectName("qtySpin");
    m_qtySpin->setMinimum(1);
    m_qtySpin->setMaximum(1000000);

    m_addBtn = new QPushButton(tr("Add"), this);
    m_removeBtn = new QPushButton(tr("Remove"), this);

    connect(m_addBtn, &QPushButton::clicked, this, &InventoryWindow::onAdd);
    connect(m_removeBtn, &QPushButton::clicked, this, &InventoryWindow::onRemove);

    if (m_scanner)
        connect(m_scanner, &BarcodeScanner::codeScanned,
                this, &InventoryWindow::onBarcodeScanned);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Product"), m_productBox);
    form->addRow(tr("Quantity"), m_qtySpin);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_addBtn);
    layout->addWidget(m_removeBtn);

    loadProducts();
}

void InventoryWindow::loadProducts()
{
    m_productBox->clear();
    if (!m_pm)
        return;
    const QList<QVariantMap> products = m_pm->listProducts();
    for (const QVariantMap &p : products) {
        QString label = QString("%1 (%2)")
                             .arg(p.value("name").toString())
                             .arg(p.value("price").toDouble());
        m_productBox->addItem(label);
        int row = m_productBox->count() - 1;
        m_productBox->setItemData(row, p.value("id"), IdRole);
        m_productBox->setItemData(row, p.value("price"), PriceRole);
        m_productBox->setItemData(row, p.value("discount"), DiscountRole);
    }
}

void InventoryWindow::onAdd()
{
    if (!m_im)
        return;
    int row = m_productBox->currentIndex();
    int pid = m_productBox->itemData(row, IdRole).toInt();
    if (!m_im->addStock(pid, m_qtySpin->value()))
        QMessageBox::warning(this, tr("Error"), m_im->lastError());
}

void InventoryWindow::onRemove()
{
    if (!m_im)
        return;
    int row = m_productBox->currentIndex();
    int pid = m_productBox->itemData(row, IdRole).toInt();
    if (!m_im->removeStock(pid, m_qtySpin->value()))
        QMessageBox::warning(this, tr("Error"), m_im->lastError());
}

void InventoryWindow::onBarcodeScanned(const QString &code)
{
    if (!m_pm)
        return;
    QVariantMap prod = m_pm->productByBarcode(code);
    int pid = prod.value("id").toInt();
    for (int i = 0; i < m_productBox->count(); ++i) {
        if (m_productBox->itemData(i, IdRole).toInt() == pid) {
            m_productBox->setCurrentIndex(i);
            break;
        }
    }
    m_qtySpin->setValue(m_qtySpin->value() + 1);
}


