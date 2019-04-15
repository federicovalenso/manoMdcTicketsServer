#include "mainwindow.h"
#include "dlgusers.h"
#include "ui_mainwindow.h"

#include "httplistener.h"

extern stefanfrings::HttpListener *listener;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>()) {
  ui->setupUi(this);
  if (listener->isListening()) {
    ui->btnStartServer->setEnabled(false);
  } else {
    ui->btnStopServer->setEnabled(true);
  }
}

MainWindow::~MainWindow() {}

void MainWindow::on_btnStartServer_clicked() {
  if (!listener->isListening()) {
    listener->listen();
    ui->btnStartServer->setEnabled(false);
    ui->btnStopServer->setEnabled(true);
    qWarning("Server's been started...");
  }
}

void MainWindow::on_btnStopServer_clicked() {
  if (listener->isListening()) {
    listener->close();
    ui->btnStartServer->setEnabled(true);
    ui->btnStopServer->setEnabled(false);
    qWarning("Server's been stoped...");
  }
}

void MainWindow::on_actionCloseApp_triggered() { close(); }

void MainWindow::on_actionDbSettings_triggered() {
  DlgDbConnection dlgDbConnection;
  dlgDbConnection.exec();
}

void MainWindow::on_actionUsers_triggered() {
  dlgUsers dlgUsers;
  dlgUsers.exec();
}
