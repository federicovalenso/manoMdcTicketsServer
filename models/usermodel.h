#ifndef USERMODEL_H
#define USERMODEL_H

#include <QCryptographicHash>
#include <QSqlTableModel>
#include <memory>
#include "entities/user.h"
#include "model.h"

class UserModel : public Model {
 public:
  static const QString TABLE_NAME;
  static const QString NAME_COL;
  static const QString PASSWORD_COL;

  static QString hashPassword(const QByteArray& password);

  QSqlTableModel* getSqlTableModel() const;

  UserModel();
  ~UserModel();
  int save(const TableOptions& options) noexcept;
  virtual QList<std::unique_ptr<Entity>> getAll();
  std::optional<User> getByName(const QString& name) noexcept;
  std::optional<User> authorize(const QByteArray& name,
                                const QByteArray& password) noexcept;

 private:
  std::unique_ptr<QSqlTableModel> model_;
  std::optional<User> selectByFilter(QString filter);
};

#endif  // USERMODEL_H
