#include "model.h"
#include <QDebug>
#include <stdexcept>
#include "database.h"

const QString Model::kIdCol = "id";
const QString Model::kCreatedAtCol = "created_at";
const QString Model::kUpdatedAtCol = "updated_at";
const QByteArray Model::kIdColParam = Model::kIdCol.toUtf8();

Model::Model(const QString &owner)
    : kConnectionName(Database::initConnection(owner)) {
  columns_.insert(kIdCol);
  columns_.insert(kCreatedAtCol);
  columns_.insert(kUpdatedAtCol);
}

Model::~Model() { Database::closeConnection(kConnectionName); }
