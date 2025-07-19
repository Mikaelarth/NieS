#ifndef INVOICEPRINTER_H
#define INVOICEPRINTER_H

#include <QObject>
#include <QString>

class InvoicePrinter : public QObject
{
    Q_OBJECT
public:
    explicit InvoicePrinter(QObject *parent = nullptr);

    bool printInvoice(int saleId, const QString &filePath);
    QString lastError() const;

private:
    QString m_lastError;
};

#endif // INVOICEPRINTER_H
