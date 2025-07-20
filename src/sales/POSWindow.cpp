#include "POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "payments/PaymentProcessor.h"
#include "invoices/InvoicePrinter.h"
#include "returns/ReturnManager.h"

namespace {
enum ProductRoles {
    IdRole = Qt::UserRole,
    PriceRole,
    DiscountRole
};
}

#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QInputDialog>

POSWindow::POSWindow(ProductManager *pm, SalesManager *sm, QWidget *parent)
    : QWidget(parent),
      m_pm(pm),
    m_sm(sm),
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

    m_sellBtn = new QPushButton(tr("Sell"), this);
    m_sellBtn->setObjectName("sellBtn");
    connect(m_sellBtn, &QPushButton::clicked, this, &POSWindow::onSell);

    m_paymentBox->addItems({tr("Cash"), tr("Card"), tr("Mobile Money"), tr("QR Code")});

    connect(m_returnBtn, &QPushButton::clicked, this, &POSWindow::onReturn);
    connect(m_invoiceBtn, &QPushButton::clicked, this, &POSWindow::onPrintInvoice);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Product"), m_productBox);
    form->addRow(tr("Quantity"), m_qtySpin);

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

    loadProducts();
    QMessageBox::information(this, tr("Return"), tr("Return processed."));
}

void POSWindow::onPrintInvoice()
{
    QMessageBox::information(this, tr("Invoice"), tr("Invoice printed."));
}

