#include <QApplication>
#include <QMessageBox>
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager db;
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", db.lastError());
        return -1;
    }

    QMessageBox::information(nullptr, "Success", "Connected to database!");

    db.close();
    return 0;
}
