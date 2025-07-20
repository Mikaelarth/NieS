#ifndef LOYALTYWINDOW_H
#define LOYALTYWINDOW_H

#include <QWidget>

class QTableWidget;
class QLineEdit;
class QSpinBox;
class QPushButton;
class LoyaltyManager;

class LoyaltyWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoyaltyWindow(LoyaltyManager *lm, QWidget *parent = nullptr);

public slots:
    void loadAccounts();

private slots:
    void onAddAccount();
    void onAddPoints();
    void onRowSelected();

private:
    LoyaltyManager *m_lm;
    QTableWidget *m_table;
    QLineEdit *m_phoneEdit;
    QSpinBox *m_pointsSpin;
    QPushButton *m_addAccountBtn;
    QPushButton *m_addPointsBtn;
};

#endif // LOYALTYWINDOW_H
