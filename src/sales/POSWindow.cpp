#include "POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "payments/PaymentProcessor.h"
#include "invoices/InvoicePrinter.h"
#include "returns/ReturnManager.h"
#include "loyalty/LoyaltyManager.h"
#include "barcode/BarcodeScanner.h"

namespace {
enum ProductRoles {
    IdRole = Qt::UserRole,
    PriceRole,
    DiscountRole
};
}

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

POSWindow::POSWindow(ProductManager *pm, SalesManager *sm, LoyaltyManager *lm,
                     QWidget *parent, BarcodeScanner *scanner)
    : QWidget(parent),
      m_pm(pm),
    m_sm(sm),
    m_loyalty(lm),
    m_scanner(scanner),
    m_paymentBox(new QComboBox(this)),
    m_returnBtn(new QPushButton(tr("Return"), this)),
    m_invoiceBtn(new QPushButton(tr("Print Invoice"), this)),
    m_payments(new PaymentProcessor(nullptr, nullptr, this)),
    m_printer(new InvoicePrinter(this)),
    m_returns(new ReturnManager(this))
{
    setWindowTitle(tr("Point of Sale"));

    m_productBox = new QComboBox(this);
    m_productBox->setObjectName("productBox");

    m_qtySpin = new QSpinBox(this);
    m_qtySpin->setObjectName("qtySpin");
    m_qtySpin->setMinimum(1);
    m_qtySpin->setMaximum(1000000);

    m_loyaltyEdit = new QLineEdit(this);
    m_loyaltyEdit->setObjectName("loyaltyEdit");

    m_sellBtn = new QPushButton(tr("Sell"), this);
    m_sellBtn->setObjectName("sellBtn");
    connect(m_sellBtn, &QPushButton::clicked, this, &POSWindow::onSell);

    m_paymentBox->addItems({tr("Cash"), tr("Card"), tr("Mobile Money"), tr("QR Code")});

    connect(m_returnBtn, &QPushButton::clicked, this, &POSWindow::onReturn);
    connect(m_invoiceBtn, &QPushButton::clicked, this, &POSWindow::onPrintInvoice);
    if (m_scanner)
        connect(m_scanner, &BarcodeScanner::codeScanned,
                this, &POSWindow::onBarcodeScanned);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Product"), m_productBox);
    form->addRow(tr("Quantity"), m_qtySpin);
    form->addRow(tr("Loyalty Phone"), m_loyaltyEdit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_paymentBox);
    layout->addWidget(m_sellBtn);
    layout->addWidget(m_returnBtn);
    layout->addWidget(m_invoiceBtn);

    loadProducts();
}

void POSWindow::loadProducts()
{
    m_productBox->clear();
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

void POSWindow::onSell()
{
    int row = m_productBox->currentIndex();
    int productId = m_productBox->itemData(row, IdRole).toInt();
    double price = m_productBox->itemData(row, PriceRole).toDouble();
    double discount = m_productBox->itemData(row, DiscountRole).toDouble();
    int qty = m_qtySpin->value();
    if (productId <= 0 || qty <= 0)
        return;
    double total = price * (1.0 - discount) * qty;
    if (!m_sm->recordSale(productId, qty)) {
        QMessageBox::warning(this, tr("Error"), m_sm->lastError());
        return;
    }
    const QString method = m_paymentBox->currentText();
    if (method == tr("Card"))
        m_payments->processCard(total);
    else if (method == tr("Mobile Money"))
        m_payments->processMobileMoney(total);
    else if (method == tr("QR Code"))
        m_payments->processQrCode(total);
    if (m_loyalty && !m_loyaltyEdit->text().isEmpty())
        m_loyalty->changePoints(m_loyaltyEdit->text(), qty, tr("sale"));
    m_qtySpin->setValue(1);
}

void POSWindow::onReturn()
{
    bool ok = false;
    int saleId = QInputDialog::getInt(this, tr("Return"), tr("Sale ID"), 1, 1, 1000000, 1, &ok);
    if (!ok)
        return;

    int qty = QInputDialog::getInt(this, tr("Return"), tr("Quantity"), 1, 1, 1000000, 1, &ok);
    if (!ok)
        return;

    if (!m_returns->recordReturn(saleId, qty)) {
        QMessageBox::warning(this, tr("Error"), m_returns->lastError());
        return;
    }

    if (m_loyalty && !m_loyaltyEdit->text().isEmpty())
        m_loyalty->changePoints(m_loyaltyEdit->text(), -qty, tr("return"));

    loadProducts();
    QMessageBox::information(this, tr("Return"), tr("Return processed."));
}

void POSWindow::onPrintInvoice()
{
    QSqlQuery q;
    if (!q.exec("SELECT MAX(id) FROM sales") || !q.next()) {
        QMessageBox::warning(this, tr("Error"), q.lastError().text());
        return;
    }
    int saleId = q.value(0).toInt();
    if (saleId <= 0) {
        QMessageBox::warning(this, tr("Error"), tr("No sales recorded."));
        return;
    }

    const QString path = askInvoicePath();
    if (path.isEmpty())
        return;

    if (!m_printer->printInvoice(saleId, path)) {
        QMessageBox::warning(this, tr("Error"), m_printer->lastError());
        return;
    }

    QMessageBox::information(this, tr("Invoice"),
                             tr("Invoice printed to %1").arg(path));
}

void POSWindow::onBarcodeScanned(const QString &code)
{
    QVariantMap prod = m_pm->productByBarcode(code);
    if (prod.isEmpty())
        return;
    int pid = prod.value("id").toInt();
    for (int i = 0; i < m_productBox->count(); ++i) {
        if (m_productBox->itemData(i, IdRole).toInt() == pid) {
            m_productBox->setCurrentIndex(i);
            break;
        }
    }
    m_qtySpin->setValue(m_qtySpin->value() + 1);
}

QString POSWindow::askInvoicePath()
{
    return QFileDialog::getSaveFileName(this, tr("Save Invoice"), QString(),
                                        tr("Text Files (*.txt);;All Files (*)"));
}

