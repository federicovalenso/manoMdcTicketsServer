#include "dlgusereditor.h"
#include "ui_dlgusereditor.h"
#include "models/usermodel.h"

#include <QMessageBox>

UserEditDialog::UserEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserEditDialog)
{
    ui->setupUi(this);
    ui->leHashedPass->setVisible(false);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void UserEditDialog::setModel(QAbstractItemModel *model) {
    mapper->setModel(model);
    mapper->addMapping(ui->leUser, 1);
    mapper->addMapping(ui->leName, 2);
    mapper->addMapping(ui->leHashedPass, 3);
}

void UserEditDialog::setCurrentModelIndex(const QModelIndex &index) {
    mapper->setCurrentModelIndex(index);
}

UserEditDialog::~UserEditDialog()
{
    delete ui;
}

void UserEditDialog::on_cbChangePass_stateChanged(int state)
{
    if (state == Qt::CheckState::Checked) {
        ui->lePass1->setEnabled(true);
        ui->lePass2->setEnabled(true);
    } else {
        ui->lePass1->setEnabled(false);
        ui->lePass1->clear();
        ui->lePass2->setEnabled(false);
        ui->lePass2->clear();
    }
}

void UserEditDialog::done(int result)
{
    bool submit_flag = true;
    if (result == QDialog::Accepted) {
        if (ui->cbChangePass->checkState() == Qt::CheckState::Checked) {
            if (ui->lePass1->text() == ui->lePass2->text()) {
                ui->leHashedPass->setText(UserModel::hashPassword(ui->lePass1->text().toUtf8()));
            } else {
                QMessageBox::critical(this,
                                      tr("Ошибка при изменении пароля"),
                                      tr("Пароли не совпадают"));
                return;
            }
        }
        if (submit_flag == true) {
            mapper->submit();
        }
    }
    QDialog::done(result);
}

void UserEditDialog::on_buttonBox_accepted()
{

}
