#ifndef TICKETCOUNTER_H
#define TICKETCOUNTER_H

#include <atomic>

class TicketCounter {
  std::atomic<uint16_t> counter_ = 0;

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
  inline void clear() { counter_ = 0; }
  inline uint16_t value() const { return counter_; }
};

#endif  // TICKETCOUNTER_H
