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
    QSqlTableModel *sqlTableModel = model.getSqlTableModel();
    sqlTableModel->setEditStrategy(QSqlTableModel::OnRowChange);
    sqlTableModel->select();
    sqlTableModel->setHeaderData(1, Qt::Horizontal, tr("ФИО"));
    ui->twUsers->setModel(sqlTableModel);
    ui->twUsers->hideColumn(0);
    ui->twUsers->hideColumn(2);
}

dlgUsers::~dlgUsers()
{
    delete ui;
}

void dlgUsers::resizeEvent(QResizeEvent *event)
{
    ui->twUsers->setColumnWidth(1, this->width());
    QDialog::resizeEvent(event);
}

void dlgUsers::updateListView()
{
}

void dlgUsers::on_twUsers_doubleClicked(const QModelIndex &index)
{
    dlgUserEditor editor;
    editor.setModel(ui->twUsers->model());
    editor.setCurrentModelIndex(index);
    editor.exec();
}

void dlgUsers::on_btnAdd_clicked()
{
    QSqlTableModel *sqlTableModel = model.getSqlTableModel();
    if (sqlTableModel->insertRow(sqlTableModel->rowCount()) == true) {
        dlgUserEditor editor;
        editor.setModel(ui->twUsers->model());
        editor.setCurrentModelIndex(
                    sqlTableModel->index(sqlTableModel->rowCount(), 1));
        editor.exec();
    }
}
