#ifndef DLGDBCONNECTION_H
#define DLGDBCONNECTION_H

#include "ui_dlgdbconnection.h"

#include <QDialog>
#include <QString>

#include "appsettings.h"
#include "database.h"

class DlgDbConnection : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDbConnection(QWidget *parent = 0);
    ~DlgDbConnection();

signals:
    void connectionSuccess();

private slots:
//    void on_btnCheckConnection_clicked();
//    void on_buttonBox_accepted();
//    void on_buttonBox_rejected();

private:
    Ui::dlgdbconnection *ui;
    AppSettings& mSettings = AppSettings::getInstance();
//    Database& mDb = Database::getInstance();
    void saveSettings();
};

#endif // DLGDBCONNECTION_H
