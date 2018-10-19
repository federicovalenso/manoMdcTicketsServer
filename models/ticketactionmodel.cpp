#include <stdexcept>
#include <QDebug>
#include <QSqlField>
#include "ticketactionmodel.h"

const QString TicketActionModel::TABLE_NAME = "ticketActions";
const QString TicketActionModel::NAME_COL = "action_name";
const QString TicketActionModel::PREFIX_COL = "prefix";

TicketActionModel::TicketActionModel()
    : Model("TicketActionModel")
{
    columns_.insert(TicketActionModel::NAME_COL);
    mSqlTableModel = new QSqlTableModel(nullptr, QSqlDatabase::database(database_.connectionName()));
    mSqlTableModel->setTable(TicketActionModel::TABLE_NAME);
}

QList<TicketAction> TicketActionModel::getAll()
{
    QList<TicketAction> result;
    mSqlTableModel->setFilter("");
    mSqlTableModel->select();
    for (int i = 0; i < mSqlTableModel->rowCount(); i++) {
        auto record = mSqlTableModel->record(i);
        TicketAction ticketAction;
        ticketAction.id = record.field(Model::ID_COL).value().toInt();
        ticketAction.name = record.field(TicketActionModel::NAME_COL).value().toString();
        ticketAction.prefix = record.field(TicketActionModel::PREFIX_COL).value().toString();
        result.push_back(std::move(ticketAction));
    }
    return result;
}

int TicketActionModel::save(const TableOptions &) noexcept
{
    return -1;
}

TicketAction TicketActionModel::getByName(const QString &name) noexcept
{
    QString filter = QString("%1='%2'").arg(TicketActionModel::NAME_COL).arg(name);
    mSqlTableModel->setFilter(filter);
    mSqlTableModel->select();
    TicketAction result;
    if (mSqlTableModel->rowCount() == 1) {
        auto record = mSqlTableModel->record(0);
        result.id = record.field(Model::ID_COL).value().toInt();
        result.name = record.field(TicketActionModel::NAME_COL).value().toString();
        result.prefix = record.field(TicketActionModel::PREFIX_COL).value().toString();
    }
    return result;
}

QVector<TicketAction> TicketActionModel::getByNamesArray(const QJsonArray& names = {})
{
    QVector<TicketAction> result;
    QString filter;
    for (const auto& name : names) {
        QString nameCondition = QString("%1='%2' AND ").arg(TicketActionModel::NAME_COL).arg(name.toString());
        filter.append(nameCondition);
    }
    if (filter.size() > 0) {
      filter = filter.left(filter.size() - 5);
    }
    mSqlTableModel->setFilter(filter);
    mSqlTableModel->select();
    if (mSqlTableModel->rowCount() > 0) {
        for (int i = 0; i < mSqlTableModel->rowCount(); i++) {
            auto record = mSqlTableModel->record(i);
            TicketAction ticketAction;
            ticketAction.id = record.field(Model::ID_COL).value().toInt();
            ticketAction.name = record.field(TicketActionModel::NAME_COL).value().toString();
            result.push_back(std::move(ticketAction));
        }
    }
    return result;
}
