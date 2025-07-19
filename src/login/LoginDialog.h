#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class UserManager;

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(UserManager *userManager, QWidget *parent = nullptr, bool showErrors = true);

    bool attemptLogin(const QString &username, const QString &password);

signals:
    void loginSuccessful();

private slots:
    void onLoginClicked();

private:
    UserManager *m_userManager;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    bool m_showErrors;
};

#endif // LOGINDIALOG_H
