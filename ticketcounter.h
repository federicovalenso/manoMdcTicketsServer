#ifndef TICKETCOUNTER_H
#define TICKETCOUNTER_H

#include <QDateTime>
#include <atomic>

class TicketCounter {
  std::atomic<uint16_t> counter_ = 0;
  uint16_t prev_count_ = 0;
  QDateTime last_clear_time_ = QDateTime::currentDateTime();

 public:
  static TicketCounter& getInstance() {
    static TicketCounter instance;
    return instance;
  }
  TicketCounter() = default;
  TicketCounter(const TicketCounter&) = delete;
  TicketCounter(TicketCounter&&) = delete;
  TicketCounter& operator=(const TicketCounter&) = delete;
  TicketCounter& operator=(TicketCounter&&) = delete;
  ~TicketCounter() = default;

  inline TicketCounter& operator++() {
    ++counter_;
    return *this;
  }
  inline void clear() {
    prev_count_ = counter_;
    counter_ = 0;
    last_clear_time_ = QDateTime::currentDateTime();
  }
  inline uint16_t value() const { return counter_; }
  inline uint16_t prev_value() const { return prev_count_; }
  inline QString last_clear_time() const {
    return last_clear_time_.toString("HH:mm dd-MMMM-yyyy");
  }
};

#endif  // TICKETCOUNTER_H
