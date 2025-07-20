#include "LoginDialog.h"
#include "UserSession.h"
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPixmap>

LoginDialog::LoginDialog(UserSession *session, QWidget *parent, bool showErrors,
                         const QPixmap &logo)
    : QDialog(parent),
      m_session(session),
      m_showErrors(showErrors)
{
    setWindowTitle(tr("Login"));

    m_logoLabel = new QLabel(this);
    if (!logo.isNull()) {
        m_logoLabel->setPixmap(logo);
        m_logoLabel->setAlignment(Qt::AlignCenter);
    } else {
        m_logoLabel->setVisible(false);
    }

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setObjectName("usernameEdit");
    m_usernameEdit->setPlaceholderText(tr("Username"));
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setObjectName("passwordEdit");
    m_passwordEdit->setPlaceholderText(tr("Password"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_showPassCheck = new QCheckBox(tr("Show Password"), this);
    m_showPassCheck->setObjectName("showPassCheck");
    connect(m_showPassCheck, &QCheckBox::toggled,
            this, &LoginDialog::onTogglePassword);

    m_loginButton = new QPushButton(tr("Login"), this);
    m_loginButton->setObjectName("loginButton");
    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);

    QFormLayout *form = new QFormLayout;
    form->setSpacing(10);
    form->addRow(tr("Username"), m_usernameEdit);
    form->addRow(tr("Password"), m_passwordEdit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    if (!logo.isNull())
        layout->addWidget(m_logoLabel);
    layout->addLayout(form);
    layout->addWidget(m_showPassCheck);
    layout->addWidget(m_loginButton);
}

void LoginDialog::onLoginClicked()
{
    attemptLogin(m_usernameEdit->text(), m_passwordEdit->text());
}

void LoginDialog::onTogglePassword(bool checked)
{
    m_passwordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
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

