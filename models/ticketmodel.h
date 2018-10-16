#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QJsonArray>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include "model.h"
#include "entities/ticket.h"
#include "ticketactionmodel.h"

class TicketModel : public Model
{
public:
    TicketModel();
    ~TicketModel();
    Ticket save(const TableOptions& options) noexcept;
    virtual QVector<Ticket> getAll();
    Ticket getById(int id) noexcept;
    QVector<Ticket> getAvailableTickets(bool on_service = false, bool is_manual = false);
    Ticket getOldestNonVoicedTicket();
    Ticket updateTicket(const TableOptions& options);

    static const QString TABLE_NAME;
    static const int ACTION_ID_COL_NUMBER = 2;
    static const int CREATED_AT_COL_NUMBER = 4;
    static const QString NUMBER_BY_ACTION_COL;
    static const QString ACTION_ID_COL;
    static const QString USER_ID_COL;
    static const QString ON_SERVICE_COL;
    static const QString IS_MANUAL_COL;
    static const QString IS_DONE_COL;
    static const QString IS_VOICED_COL;
    static const QString WINDOW_NUMBER_COL;
    static const QByteArray ON_SERVICE_PARAM;
    static const QByteArray IS_MANUAL_PARAM;
    static const QByteArray IS_DONE_PARAM;
    static const QByteArray IS_VOICED_PARAM;
    static const QByteArray WINDOW_NUMBER_PARAM;

private:
    QSqlRelationalTableModel* mModel;

    bool validateOptions(const TableOptions& options);
    Ticket extractFromRecord(const QSqlRecord& record);
};

#endif // TICKETMODEL_H
