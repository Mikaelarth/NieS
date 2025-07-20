#include "login/UserWindow.h"
#include "UserManager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

UserWindow::UserWindow(UserManager *um, QWidget *parent)
    : QWidget(parent), m_um(um)
{
    setWindowTitle(tr("Users"));

    m_table = new QTableWidget(this);
    m_table->setObjectName("userTable");
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({tr("Username"), tr("Role")});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &UserWindow::onRowSelected);

    m_userEdit = new QLineEdit(this);
    m_userEdit->setObjectName("userEdit");
    m_passEdit = new QLineEdit(this);
    m_passEdit->setObjectName("passEdit");
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_roleEdit = new QLineEdit(this);
    m_roleEdit->setObjectName("roleEdit");

    m_addBtn = new QPushButton(tr("Add"), this);
    m_editBtn = new QPushButton(tr("Change Role"), this);
    m_deleteBtn = new QPushButton(tr("Delete"), this);

    connect(m_addBtn, &QPushButton::clicked, this, &UserWindow::onAdd);
    connect(m_editBtn, &QPushButton::clicked, this, &UserWindow::onEdit);
    connect(m_deleteBtn, &QPushButton::clicked, this, &UserWindow::onDelete);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Username"), m_userEdit);
    form->addRow(tr("Password"), m_passEdit);
    form->addRow(tr("Role"), m_roleEdit);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(m_addBtn);
    buttons->addWidget(m_editBtn);
    buttons->addWidget(m_deleteBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addLayout(form);
    layout->addLayout(buttons);

    loadUsers();
}

void UserWindow::loadUsers()
{
    m_table->setRowCount(0);
    if (!m_um)
        return;
    const QList<QVariantMap> users = m_um->listUsers();
    for (int i = 0; i < users.size(); ++i) {
        m_table->insertRow(i);
        const QVariantMap &u = users[i];
        m_table->setItem(i, 0, new QTableWidgetItem(u.value("username").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(u.value("role").toString()));
    }
    if (m_table->rowCount() > 0)
        m_table->selectRow(0);
}

void UserWindow::onAdd()
{
    if (!m_um || m_userEdit->text().isEmpty() || m_passEdit->text().isEmpty())
        return;
    if (!m_um->createUser(m_userEdit->text(), m_passEdit->text(), m_roleEdit->text())) {
        QMessageBox::warning(this, tr("Error"), m_um->lastError());
        return;
    }
    m_userEdit->clear();
    m_passEdit->clear();
    m_roleEdit->clear();
    loadUsers();
}

void UserWindow::onEdit()
{
    int row = m_table->currentRow();
    if (row < 0 || !m_um)
        return;
    QString username = m_table->item(row, 0)->text();
    if (!m_um->updateUserRole(username, m_roleEdit->text())) {
        QMessageBox::warning(this, tr("Error"), m_um->lastError());
        return;
    }
    loadUsers();
}

void UserWindow::onDelete()
{
    int row = m_table->currentRow();
    if (row < 0 || !m_um)
        return;
    QString username = m_table->item(row, 0)->text();
    if (!m_um->deleteUser(username)) {
        QMessageBox::warning(this, tr("Error"), m_um->lastError());
        return;
    }
    loadUsers();
}

void UserWindow::onRowSelected()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    m_userEdit->setText(m_table->item(row, 0)->text());
    m_roleEdit->setText(m_table->item(row, 1)->text());
    m_passEdit->clear();
}

