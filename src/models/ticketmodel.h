#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QJsonArray>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <memory>
#include <optional>
#include "entities/ticket.h"
#include "model.h"
#include "ticketactionmodel.h"

class TicketModel : public Model {
 public:
  TicketModel();
  ~TicketModel() = default;

  std::optional<Ticket> save(const TableOptions& options) noexcept;
  std::optional<Ticket> updateTicket(const TableOptions& options) noexcept;
  std::optional<Ticket> getOldestNonVoicedTicket() noexcept;
  std::optional<Ticket> getById(int id) noexcept;
  std::optional<Ticket> getByFilter(QString filter) noexcept;

  QVector<Ticket> getAll() noexcept;
  QVector<Ticket> getAvailableTickets(bool on_service = false,
                                      bool is_manual = false) noexcept;

  static const QString TABLE_NAME;
  static const int ACTION_ID_COL_NUMBER = 2;
  static const int CREATED_AT_COL_NUMBER = 4;
  static const QString NUMBER_BY_ACTION_COL;
  static const QString TICKET_ID_COL;
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
  std::unique_ptr<QSqlRelationalTableModel> model_;

  bool validateOptions(const TableOptions& options) const;
  Ticket extractFromRecord(const QSqlRecord& record) const;
};

#endif  // TICKETMODEL_H
