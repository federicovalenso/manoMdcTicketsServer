#include <QSqlRecord>
#include <QSqlField>
#include "usermodel.h"

const QString UserModel::TABLE_NAME = "users";
const QString UserModel::NAME_COL = "user_name";
const QString UserModel::PASSWORD_COL = "password";

QString UserModel::hashPassword(const QByteArray &password)
{
    return QString(QCryptographicHash::hash(password, QCryptographicHash::Sha3_512).toHex());
}

UserModel::UserModel()
    : Model("UserModel")
{
    mColumns.insert(NAME_COL);
    mColumns.insert(PASSWORD_COL);
    mSqlTableModel = new QSqlTableModel(nullptr, QSqlDatabase::database(mDb.connectionName()));
    mSqlTableModel->setTable(TABLE_NAME);
}

int UserModel::save(const TableOptions &) noexcept
{
    return -1;
}

QList<Entity> UserModel::getAll()
{
    QList<Entity> result;
    return result;
}

User UserModel::getByName(const QString &name) noexcept
{
    QString filter = QString("%1='%2'").arg(NAME_COL).arg(name);
    mSqlTableModel->setFilter(filter);
    mSqlTableModel->select();
    User result;
    if (mSqlTableModel->rowCount() == 1) {
        auto record = mSqlTableModel->record(0);
        result.id = record.field(Model::ID_COL).value().toInt();
        result.name = record.field(NAME_COL).value().toString();
    }
    return result;
}

User UserModel::authorize(const QByteArray &name, const QByteArray &password) noexcept
{
    QString filter = QString(NAME_COL + "='%1' AND " + PASSWORD_COL +"='%2'")
                     .arg(QString(name))
                     .arg(hashPassword(password));
    mSqlTableModel->setFilter(filter);
    mSqlTableModel->select();
    User result;
    if (mSqlTableModel->rowCount() == 1) {
        auto record = mSqlTableModel->record(0);
        result.id = record.field(Model::ID_COL).value().toInt();
        result.name = record.field(NAME_COL).value().toString();
    }
    return result;
}

QSqlTableModel *UserModel::getSqlTableModel() const
{
    return mSqlTableModel;
}
