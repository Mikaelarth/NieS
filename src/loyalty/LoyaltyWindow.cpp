#include "loyalty/LoyaltyWindow.h"
#include "loyalty/LoyaltyManager.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

LoyaltyWindow::LoyaltyWindow(LoyaltyManager *lm, QWidget *parent)
    : QWidget(parent),
      m_lm(lm)
{
    setWindowTitle(tr("Loyalty Accounts"));

    m_table = new QTableWidget(this);
    m_table->setObjectName("loyaltyTable");
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({tr("ID"), tr("Phone"), tr("Points")});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &LoyaltyWindow::onRowSelected);

    m_phoneEdit = new QLineEdit(this);
    m_phoneEdit->setObjectName("phoneEdit");
    m_pointsSpin = new QSpinBox(this);
    m_pointsSpin->setObjectName("pointsSpin");
    m_pointsSpin->setMinimum(-1000000);
    m_pointsSpin->setMaximum(1000000);

    m_addAccountBtn = new QPushButton(tr("Add Account"), this);
    m_addPointsBtn = new QPushButton(tr("Apply Points"), this);
    connect(m_addAccountBtn, &QPushButton::clicked, this, &LoyaltyWindow::onAddAccount);
    connect(m_addPointsBtn, &QPushButton::clicked, this, &LoyaltyWindow::onAddPoints);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Phone"), m_phoneEdit);
    form->addRow(tr("Points"), m_pointsSpin);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_addAccountBtn);
    btnLayout->addWidget(m_addPointsBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addLayout(form);
    layout->addLayout(btnLayout);

    loadAccounts();
}

void LoyaltyWindow::loadAccounts()
{
    m_table->setRowCount(0);
    if (!m_lm)
        return;
    const QList<QVariantMap> accts = m_lm->listAccounts();
    for (int i = 0; i < accts.size(); ++i) {
        const QVariantMap &a = accts[i];
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(a.value("id").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(a.value("phone").toString()));
        m_table->setItem(i, 2, new QTableWidgetItem(a.value("points").toString()));
    }
    if (m_table->rowCount() > 0)
        m_table->selectRow(0);
}

void LoyaltyWindow::onAddAccount()
{
    if (!m_lm || m_phoneEdit->text().isEmpty())
        return;
    if (!m_lm->createAccount(m_phoneEdit->text())) {
        QMessageBox::warning(this, tr("Error"), m_lm->lastError());
        return;
    }
    loadAccounts();
}

void LoyaltyWindow::onAddPoints()
{
    if (!m_lm)
        return;
    int row = m_table->currentRow();
    QString phone = m_phoneEdit->text();
    if (row >= 0 && phone.isEmpty())
        phone = m_table->item(row, 1)->text();
    if (phone.isEmpty())
        return;
    if (!m_lm->changePoints(phone, m_pointsSpin->value())) {
        QMessageBox::warning(this, tr("Error"), m_lm->lastError());
        return;
    }
    m_pointsSpin->setValue(0);
    loadAccounts();
}

void LoyaltyWindow::onRowSelected()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    m_phoneEdit->setText(m_table->item(row, 1)->text());
}

