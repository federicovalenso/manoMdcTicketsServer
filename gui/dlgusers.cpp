#include "dlgusers.h"
#include "ui_dlgusers.h"
#include "gui/dlgusereditor.h"
#include "models/usermodel.h"

#include <QAbstractListModel>

dlgUsers::dlgUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgUsers)
{
    ui->setupUi(this);
    QSqlTableModel *sqlTableModel = model_.getSqlTableModel();
    sqlTableModel->setEditStrategy(QSqlTableModel::OnRowChange);
    sqlTableModel->select();
    sqlTableModel->setHeaderData(1, Qt::Horizontal, tr("Пользователь"));
    sqlTableModel->setHeaderData(2, Qt::Horizontal, tr("ФИО"));
    ui->twUsers->setModel(sqlTableModel);
    ui->twUsers->hideColumn(0);
    ui->twUsers->hideColumn(3);
}

dlgUsers::~dlgUsers()
{
    delete ui;
}

void dlgUsers::resizeEvent(QResizeEvent *event)
{
    ui->twUsers->setColumnWidth(1, this->width() / 3);
    ui->twUsers->setColumnWidth(2, this->width() / 3 * 2);
    QDialog::resizeEvent(event);
}

void dlgUsers::updateListView()
{
}

void dlgUsers::editUser(const QModelIndex &index)
{
    UserEditDialog editor;
    editor.setModel(ui->twUsers->model());
    editor.setCurrentModelIndex(index);
    editor.exec();
}

void dlgUsers::on_twUsers_doubleClicked(const QModelIndex &index)
{
    editUser(index);
}

void dlgUsers::on_btnAdd_clicked()
{
    auto* sql_model = ui->twUsers->model();
    if (sql_model->insertRow(sql_model->rowCount()) == true) {
        UserEditDialog editor;
        editor.setModel(sql_model);
        editor.setCurrentModelIndex(
                    sql_model->index(sql_model->rowCount() - 1, 1));
        editor.exec();
    }
}

void dlgUsers::on_btnEdit_clicked()
{

}
