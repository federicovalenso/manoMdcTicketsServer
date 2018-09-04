#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgusers.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartServer_clicked()
{

}

void MainWindow::on_btnStopServer_clicked()
{

}

void MainWindow::on_actionCloseApp_triggered()
{
    close();
}

void MainWindow::on_actionDbSettings_triggered()
{
    DlgDbConnection dlgDbConnection;
    dlgDbConnection.exec();
}

void MainWindow::on_actionUsers_triggered()
{
    dlgUsers dlgUsers;
    dlgUsers.exec();
}
