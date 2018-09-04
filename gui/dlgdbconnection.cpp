#include "dlgdbconnection.h"

#include <QSettings>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <stdexcept>

DlgDbConnection::DlgDbConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgdbconnection)
{
    ui->setupUi(this);
    ui->leServer->setText(mSettings.getServerAddr());
    ui->leBase->setText(mSettings.getBaseName());
    ui->leUser->setText(mSettings.getUserName());
    ui->lePass->setText(mSettings.getPassword());
}

DlgDbConnection::~DlgDbConnection() {
    delete ui;
}

void DlgDbConnection::saveSettings() {
    mSettings.setConnectionSettings(ui->leServer->text(),
                                    ui->leBase->text(),
                                    ui->leUser->text(),
                                    ui->lePass->text());
}

//void DlgDbConnection::on_btnCheckConnection_clicked() {
//    if (QMessageBox::question(this,
//                              tr("Сохранение настроек подключения"),
//                              tr("Сохранить настроки?"),
//                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
//        QMessageBox::information(this, tr("Операция отменена"), tr("Проверка подключения отменена"));
//        return;
//    }
//    saveSettings();
//    try {
//        if (mDb.setConnection() == true) {
//            QMessageBox::information(this, tr("Состояние подключения к БД"), tr("Соединение успешно установлено"));
//            emit connectionSuccess();
//        }
//        else {
//            QMessageBox::critical(this, tr("Состояние подключения к БД"), tr("Соединение не установлено"));
//        }
//    }
//    catch (QSqlError e) {
//        QMessageBox::critical(this, tr("Состояние подключения к БД"), e.text());
//    }
//}

//void DlgDbConnection::on_buttonBox_accepted() {
//    saveSettings();
//    QDialog::accept();
//}

//void DlgDbConnection::on_buttonBox_rejected() {
//    QDialog::reject();
//}
