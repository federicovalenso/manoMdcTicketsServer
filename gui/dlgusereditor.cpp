#include "dlgusereditor.h"
#include "ui_dlgusereditor.h"
#include "models/usermodel.h"

#include <QMessageBox>

dlgUserEditor::dlgUserEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgUserEditor)
{
    ui->setupUi(this);
    ui->leHashedPass->setVisible(false);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

void dlgUserEditor::setModel(QAbstractItemModel *model) {
    mapper->setModel(model);
    mapper->addMapping(ui->leName, 1);
    mapper->addMapping(ui->leHashedPass, 2);
}

void dlgUserEditor::setCurrentModelIndex(const QModelIndex &index) {
    mapper->setCurrentModelIndex(index);
}

dlgUserEditor::~dlgUserEditor()
{
    delete ui;
}

void dlgUserEditor::on_cbChangePass_stateChanged(int state)
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

void dlgUserEditor::done(int result)
{
    if (result == QDialog::Accepted) {
        if (ui->cbChangePass->checkState() == Qt::CheckState::Checked) {
            if (ui->lePass1->text() == ui->lePass2->text()) {
                ui->leHashedPass->setText(UserModel::hashPassword(ui->lePass1->text().toUtf8()));
                mapper->submit();
            } else {
                QMessageBox::critical(this,
                                      tr("Ошибка при изменении пароля"),
                                      tr("Пароли не совпадают"));
                return;
            }
        }
    }

    QDialog::done(result);
}
