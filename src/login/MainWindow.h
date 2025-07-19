#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProductWindow;
class POSWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void openProducts();
    void openPOS();

private:
    ProductWindow *m_productWindow = nullptr;
    POSWindow *m_posWindow = nullptr;
};

#endif // MAINWINDOW_H
