#ifndef USER_WINDOW_TEST_H
#define USER_WINDOW_TEST_H

#include <QObject>

class UserWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void createUserUI();
    void changeRoleUI();
};

#endif // USER_WINDOW_TEST_H
