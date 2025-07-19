#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class UserSession;

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(UserSession *session, QWidget *parent = nullptr, bool showErrors = true);

    bool attemptLogin(const QString &username, const QString &password);

signals:
    void loginSuccessful();

private slots:
    void onLoginClicked();

private:
    UserSession *m_session;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    bool m_showErrors;
};

#endif // LOGINDIALOG_H
