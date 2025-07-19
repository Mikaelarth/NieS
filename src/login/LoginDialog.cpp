#include "LoginDialog.h"
#include "UserManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

LoginDialog::LoginDialog(UserManager *userManager, QWidget *parent, bool showErrors)
    : QDialog(parent),
      m_userManager(userManager),
      m_showErrors(showErrors)
{
    setWindowTitle(tr("Login"));

    m_usernameEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton(tr("Login"), this);
    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("Username"), m_usernameEdit);
    form->addRow(tr("Password"), m_passwordEdit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_loginButton);
}

void LoginDialog::onLoginClicked()
{
    attemptLogin(m_usernameEdit->text(), m_passwordEdit->text());
}

bool LoginDialog::attemptLogin(const QString &username, const QString &password)
{
    if (m_userManager->authenticate(username, password)) {
        emit loginSuccessful();
        accept();
        return true;
    }

    if (m_showErrors) {
        QMessageBox::warning(this, tr("Login failed"), m_userManager->lastError());
    }
    return false;
}

