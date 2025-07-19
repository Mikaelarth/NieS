#include <QApplication>
#include <QMessageBox>
#include "DatabaseManager.h"
#include "UserManager.h"
#include "login/LoginDialog.h"
#include "login/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager db;
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", db.lastError());
        return -1;
    }

    UserManager userManager;
    LoginDialog login(&userManager);
    if (login.exec() != QDialog::Accepted) {
        db.close();
        return 0;
    }

    MainWindow mainWin;
    mainWin.show();
    int ret = app.exec();

    db.close();
    return ret;
}
