#include "LoginDialog.h"
#include "UserSession.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

LoginDialog::LoginDialog(UserSession *session, QWidget *parent, bool showErrors)
    : QDialog(parent),
      m_session(session),
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
    if (m_session && m_session->login(username, password)) {
        emit loginSuccessful();
        accept();
        return true;
    }

    if (m_showErrors) {
        QString err = m_session ? m_session->lastError() : QString();
        QMessageBox::warning(this, tr("Login failed"), err);
    }
    return false;
}

