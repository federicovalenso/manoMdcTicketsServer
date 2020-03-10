#ifndef TICKETCOUNTER_H
#define TICKETCOUNTER_H

#include <QCoreApplication>
#include <QDateTime>
#include <QMetaType>
#include <QSettings>
#include <QUrl>
#include <atomic>

class TicketCounter : public QObject {
  Q_OBJECT

  static const QByteArray kClassName;

  std::atomic<uint32_t> counter_ = 0;
  uint32_t prev_count_ = 0;
  QDateTime last_clear_time_ = QDateTime::currentDateTime();

  class Notificator {
    const uint32_t kCriticalCount;
    const QUrl kService;
    const QString kMail;

   public:
    Notificator(const uint32_t critical_count, QString&& service,
                QString&& mail);
    void send() const;
    uint32_t CriticalCount() const;
  };
  const Notificator kNotificator;

 public:
  static TicketCounter& instance();

  TicketCounter(QCoreApplication* parent, QSettings* settings);
  TicketCounter(const TicketCounter&) = delete;
  TicketCounter(TicketCounter&&) = delete;
  TicketCounter& operator=(const TicketCounter&) = delete;
  TicketCounter& operator=(TicketCounter&&) = delete;
  ~TicketCounter();

  TicketCounter& operator++();
  void clear();
  inline uint32_t value() const { return counter_; }
  inline uint32_t prev_value() const { return prev_count_; }
  inline QString last_clear_time() const {
    return last_clear_time_.toString("HH:mm dd-MMMM-yyyy");
  }
};

#endif  // TICKETCOUNTER_H
