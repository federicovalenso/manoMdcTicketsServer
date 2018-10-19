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
    columns_.insert(NAME_COL);
    columns_.insert(PASSWORD_COL);
    model_ = std::make_unique<QSqlTableModel>(nullptr, QSqlDatabase::database(database_.connectionName()));
    model_->setTable(TABLE_NAME);
}

UserModel::~UserModel()
{
    model_.reset();
    QSqlDatabase::database(database_.connectionName()).close();
    QSqlDatabase::removeDatabase(database_.connectionName());
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
    model_->setFilter(filter);
    model_->select();
    User result;
    if (model_->rowCount() == 1) {
        auto record = model_->record(0);
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
    model_->setFilter(filter);
    model_->select();
    User result;
    if (model_->rowCount() == 1) {
        auto record = model_->record(0);
        result.id = record.field(Model::ID_COL).value().toInt();
        result.name = record.field(NAME_COL).value().toString();
    }
    return result;
}

QSqlTableModel *UserModel::getSqlTableModel() const
{
    return model_.get();
}
