#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include <list>
#include <mutex>
#include "entities/ticket.h"
#include "modelcontroller.h"

class TicketController : public ModelController {
 public:
  TicketController();
  virtual void index(stefanfrings::HttpRequest&,
                     stefanfrings::HttpResponse&) const override;
  virtual void store(stefanfrings::HttpRequest&,
                     stefanfrings::HttpResponse&) override;
  virtual void show(stefanfrings::HttpRequest&,
                    stefanfrings::HttpResponse&) const override;
  virtual void update(stefanfrings::HttpRequest&,
                      stefanfrings::HttpResponse&) override;

 private:
  static const QByteArray ACTION_PARAM;
  static const QString TICKET_NUMBER;
  static const QString WINDOW_NUMBER;
  std::list<int> locked_ticket_ids_;
  std::mutex ids_locker_;

  bool validateIndexRequest(stefanfrings::HttpRequest& request) const noexcept;
  bool validateUpdateRequest(stefanfrings::HttpRequest& request) const noexcept;
  void unlockTicket(int id);
};

#endif  // TICKETCONTROLLER_H
