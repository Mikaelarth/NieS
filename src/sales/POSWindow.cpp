#include "POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"

#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

POSWindow::POSWindow(ProductManager *pm, SalesManager *sm, QWidget *parent)
    : QWidget(parent),
      m_pm(pm),
      m_sm(sm)
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

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Product"), m_productBox);
    form->addRow(tr("Quantity"), m_qtySpin);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_sellBtn);

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
    m_qtySpin->setValue(1);
}

