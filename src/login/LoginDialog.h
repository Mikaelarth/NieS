#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPixmap>

class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;
class QPixmap;
class UserSession;

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(UserSession *session, QWidget *parent = nullptr,
                         bool showErrors = true, const QPixmap &logo = QPixmap());

    bool attemptLogin(const QString &username, const QString &password);

signals:
    void loginSuccessful();

private slots:
    void onLoginClicked();
    void onTogglePassword(bool checked);

private:
    UserSession *m_session;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QCheckBox *m_showPassCheck;
    QLabel *m_logoLabel;
    bool m_showErrors;
};

#endif // LOGINDIALOG_H
