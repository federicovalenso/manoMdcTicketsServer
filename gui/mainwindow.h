#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dlgdbconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnStartServer_clicked();
    void on_btnStopServer_clicked();
    void on_actionCloseApp_triggered();
    void on_actionDbSettings_triggered();
    void on_actionUsers_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
