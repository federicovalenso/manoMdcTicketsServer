#include <stdexcept>
#include <QDebug>
#include "model.h"

const QString Model::ID_COL = "id";
const QString Model::CREATED_AT_COL = "created_at";
const QString Model::UPDATED_AT_COL = "updated_at";
const QByteArray Model::ID_COL_PARAM = ID_COL.toUtf8();

Model::Model(const QString &owner) :
    mDb(owner)
{
    mColumns.insert(Model::ID_COL);
    mColumns.insert(Model::CREATED_AT_COL);
    mColumns.insert(Model::UPDATED_AT_COL);
}

Model::~Model()
{}

bool Model::isValid()
{
    return mDb.isValid();
}
