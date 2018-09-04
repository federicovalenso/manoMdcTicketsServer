#ifndef TICKETACTIONMODEL_H
#define TICKETACTIONMODEL_H

#include <QJsonArray>
#include <QVector>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "model.h"
#include "entities/ticketaction.h"

class TicketActionModel : public Model
{
public:
    TicketActionModel();
    QList<TicketAction> getAll();
    int save(const TableOptions &options) noexcept;
    TicketAction getByName(const QString& name) noexcept;
    QVector<TicketAction> getByNamesArray(const QJsonArray& names);

    static const QString TABLE_NAME;
    static const QString NAME_COL;
    static const QString PREFIX_COL;

private:
    QSqlTableModel* mSqlTableModel;
};

#endif // TICKETACTIONMODEL_H
