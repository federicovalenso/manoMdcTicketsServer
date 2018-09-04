#include "model.h"

#include <stdexcept>
#include <QDebug>

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

const QString Model::ID_COL = "id";
const QString Model::CREATED_AT_COL = "created_at";
const QString Model::UPDATED_AT_COL = "updated_at";
