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

  std::optional<QJsonObject> save(const TableOptions& options) noexcept;
  std::optional<QJsonObject> updateTicket(const TableOptions& options) noexcept;
  std::optional<QJsonObject> getOldestNonVoicedTicket() noexcept;
  std::optional<QJsonObject> getById(int id) noexcept;
  std::optional<QJsonObject> getByFilter(QString filter) noexcept;

  QVector<QJsonObject> getAll() noexcept;
  QVector<QJsonObject> getAvailableTickets(bool on_service = false,
                                           bool is_manual = false) noexcept;

  static const QString kTableName;
  static const int kActionIdColNumber = 2;
  static const int kCreatedAtColNumber = 4;
  static const QString kNumberByActionCol;
  static const QString kTicketIdCol;
  static const QString kActionIdCol;
  static const QString kUserIdCol;
  static const QString kOnServiceCol;
  static const QString kIsManualCol;
  static const QString kIsDoneCol;
  static const QString kIsVoicedCol;
  static const QString kWindowNumberCol;
  static const QString kTicketNumber;
  static const QByteArray kOnServiceParam;
  static const QByteArray kIsManualParam;
  static const QByteArray kIsDoneParam;
  static const QByteArray kIsVoicedParam;
  static const QByteArray kWindowNumberParam;

 private:
  std::unique_ptr<QSqlRelationalTableModel> model_;
  enum class ExtractionModes { INSERT, SELECT };

  bool validateOptions(const TableOptions& options) const;
  QJsonObject extractFromRecord(
      const QSqlRecord& record,
      ExtractionModes mode = ExtractionModes::SELECT) const;
};

#endif  // TICKETMODEL_H
