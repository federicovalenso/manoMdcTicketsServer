#ifndef USERMODEL_H
#define USERMODEL_H

#include <memory>
#include <QCryptographicHash>
#include <QSqlTableModel>
#include "model.h"
#include "entities/user.h"

class UserModel : public Model
{
public:
    UserModel();
    ~UserModel();
    int save(const TableOptions &options) noexcept;
    virtual QList<Entity> getAll();
    User getByName(const QString &name) noexcept;
    User authorize(const QByteArray& name, const QByteArray& password) noexcept;
    static QString hashPassword(const QByteArray& password);
    static const QString TABLE_NAME;
    static const QString NAME_COL;
    static const QString PASSWORD_COL;
    QSqlTableModel *getSqlTableModel() const;

private:
    std::unique_ptr<QSqlTableModel> model_;
};

#endif // USERMODEL_H
