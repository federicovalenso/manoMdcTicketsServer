#ifndef TICKETCOUNTER_H
#define TICKETCOUNTER_H

#include <QCoreApplication>
#include <QDateTime>
#include <QMetaType>
#include <atomic>

class TicketCounter : public QObject {
  Q_OBJECT
  std::atomic<uint32_t> counter_ = 0;
  uint32_t prev_count_ = 0;
  QDateTime last_clear_time_ = QDateTime::currentDateTime();

  static const QString kGroup;
  static const QString kCounter;
  static const QString kPrevCount;
  static const QString kLastTimeClear;

 public:
  static const QByteArray kClassName;
  static TicketCounter& instance();

  TicketCounter(QCoreApplication* parent);
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
