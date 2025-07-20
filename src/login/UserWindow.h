#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>
#include <QList>
#include <QVariantMap>

class QTableWidget;
class QLineEdit;
class QPushButton;
class UserManager;

class UserWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UserWindow(UserManager *um, QWidget *parent = nullptr);

public slots:
    void loadUsers();

private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onRowSelected();

private:
    UserManager *m_um;
    QTableWidget *m_table;
    QLineEdit *m_userEdit;
    QLineEdit *m_passEdit;
    QLineEdit *m_roleEdit;
    QPushButton *m_addBtn;
    QPushButton *m_editBtn;
    QPushButton *m_deleteBtn;
};

#endif // USERWINDOW_H
