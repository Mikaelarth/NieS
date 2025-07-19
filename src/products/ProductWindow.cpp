#include "ProductWindow.h"
#include "ProductManager.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>

ProductWindow::ProductWindow(ProductManager *pm, QWidget *parent)
    : QWidget(parent),
      m_pm(pm)
{
    setWindowTitle(tr("Products"));

    m_table = new QTableWidget(this);
    m_table->setObjectName("productTable");
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({tr("ID"), tr("Name"), tr("Price"), tr("Discount")});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &ProductWindow::onRowSelected);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setObjectName("nameEdit");
    m_priceEdit = new QDoubleSpinBox(this);
    m_priceEdit->setObjectName("priceEdit");
    m_priceEdit->setMaximum(999999);
    m_discountEdit = new QDoubleSpinBox(this);
    m_discountEdit->setObjectName("discountEdit");
    m_discountEdit->setMaximum(100000);

    m_addBtn = new QPushButton(tr("Add"), this);
    m_editBtn = new QPushButton(tr("Edit"), this);
    m_deleteBtn = new QPushButton(tr("Delete"), this);

    connect(m_addBtn, &QPushButton::clicked, this, &ProductWindow::onAdd);
    connect(m_editBtn, &QPushButton::clicked, this, &ProductWindow::onEdit);
    connect(m_deleteBtn, &QPushButton::clicked, this, &ProductWindow::onDelete);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Name"), m_nameEdit);
    form->addRow(tr("Price"), m_priceEdit);
    form->addRow(tr("Discount"), m_discountEdit);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(m_addBtn);
    buttons->addWidget(m_editBtn);
    buttons->addWidget(m_deleteBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addLayout(form);
    layout->addLayout(buttons);

    loadProducts();
}

void ProductWindow::loadProducts()
{
    m_table->setRowCount(0);
    const QList<QVariantMap> products = m_pm->listProducts();
    for (int i = 0; i < products.size(); ++i) {
        m_table->insertRow(i);
        const QVariantMap &p = products[i];
        m_table->setItem(i, 0, new QTableWidgetItem(p.value("id").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(p.value("name").toString()));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(p.value("price").toDouble())));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(p.value("discount").toDouble())));
    }
    if (m_table->rowCount() > 0)
        m_table->selectRow(0);
}

void ProductWindow::onAdd()
{
    if (m_nameEdit->text().isEmpty())
        return;
    if (!m_pm->addProduct(m_nameEdit->text(), m_priceEdit->value(), m_discountEdit->value())) {
        QMessageBox::warning(this, tr("Error"), m_pm->lastError());
        return;
    }
    m_nameEdit->clear();
    m_priceEdit->setValue(0);
    m_discountEdit->setValue(0);
    loadProducts();
}

void ProductWindow::onEdit()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    int id = m_table->item(row, 0)->text().toInt();
    if (!m_pm->updateProduct(id, m_nameEdit->text(), m_priceEdit->value(), m_discountEdit->value())) {
        QMessageBox::warning(this, tr("Error"), m_pm->lastError());
        return;
    }
    loadProducts();
}

void ProductWindow::onDelete()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    int id = m_table->item(row, 0)->text().toInt();
    if (!m_pm->deleteProduct(id)) {
        QMessageBox::warning(this, tr("Error"), m_pm->lastError());
        return;
    }
    loadProducts();
}

void ProductWindow::onRowSelected()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    m_nameEdit->setText(m_table->item(row, 1)->text());
    m_priceEdit->setValue(m_table->item(row, 2)->text().toDouble());
    m_discountEdit->setValue(m_table->item(row, 3)->text().toDouble());
}

