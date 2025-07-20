#ifndef POSWINDOW_H
#define POSWINDOW_H

#include <QWidget>

class QComboBox;
class QSpinBox;
class QPushButton;
class ProductManager;
class SalesManager;
class PaymentProcessor;
class InvoicePrinter;
class ReturnManager;
class LoyaltyManager;
class QLineEdit;

class POSWindow : public QWidget
{
    Q_OBJECT
public:
    explicit POSWindow(ProductManager *pm, SalesManager *sm,
                       LoyaltyManager *lm = nullptr, QWidget *parent = nullptr);

public slots:
    void loadProducts();

private slots:
    void onSell();
    void onReturn();
    void onPrintInvoice();

protected:
    virtual QString askInvoicePath();

private:
    ProductManager *m_pm;
    SalesManager *m_sm;
    QComboBox *m_productBox;
    QSpinBox *m_qtySpin;
    QPushButton *m_sellBtn;
    QLineEdit *m_loyaltyEdit;
    QComboBox *m_paymentBox;
    QPushButton *m_returnBtn;
    QPushButton *m_invoiceBtn;
    PaymentProcessor *m_payments;
    InvoicePrinter *m_printer;
    ReturnManager *m_returns;
    LoyaltyManager *m_loyalty;
};

#endif // POSWINDOW_H
