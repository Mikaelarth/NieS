#ifndef LOGIN_TEST_H
#define LOGIN_TEST_H

#include <QObject>

class LoginDialogTest : public QObject
{
    Q_OBJECT
private slots:
    void invalidCredentials();
    void validCredentials();
    void togglePasswordVisibility();
};

#endif // LOGIN_TEST_H
