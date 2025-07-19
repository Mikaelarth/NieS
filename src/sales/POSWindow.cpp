#include "POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "payments/PaymentProcessor.h"
#include "invoices/InvoicePrinter.h"
#include "returns/ReturnManager.h"

#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

POSWindow::POSWindow(ProductManager *pm, SalesManager *sm, QWidget *parent)
    : QWidget(parent),
      m_pm(pm),
    m_sm(sm),
    m_paymentBox(new QComboBox(this)),
    m_returnBtn(new QPushButton(tr("Return"), this)),
    m_invoiceBtn(new QPushButton(tr("Print Invoice"), this)),
    m_payments(new PaymentProcessor(this)),
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
        m_productBox->addItem(label, p.value("id"));
    }
}

void POSWindow::onSell()
{
    int productId = m_productBox->currentData().toInt();
    int qty = m_qtySpin->value();
    if (productId <= 0 || qty <= 0)
        return;
    if (!m_sm->recordSale(productId, qty)) {
        QMessageBox::warning(this, tr("Error"), m_sm->lastError());
        return;
    }
    const QString method = m_paymentBox->currentText();
    double total = qty; // total value not known; placeholder
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
    // In a real application we would select a sale to return
    QMessageBox::information(this, tr("Return"), tr("Return processed."));
}

void POSWindow::onPrintInvoice()
{
    QMessageBox::information(this, tr("Invoice"), tr("Invoice printed."));
}

