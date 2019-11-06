#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include <list>
#include <mutex>
#include <rules>
#include "entities/ticket.h"
#include "modelcontroller.h"

class TicketController : public ModelController {
 public:
  TicketController() = default;
  void index(stefanfrings::HttpRequest&,
             stefanfrings::HttpResponse&) const override;
  void store(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&) override;
  void show(stefanfrings::HttpRequest&,
            stefanfrings::HttpResponse&) const override;
  void voiceTicket(stefanfrings::HttpRequest&,
                   stefanfrings::HttpResponse&) const;
  void takeTicket(stefanfrings::HttpRequest&,
                  stefanfrings::HttpResponse&) const;
  void returnTicket(stefanfrings::HttpRequest&,
                    stefanfrings::HttpResponse&) const;
  void finishTicket(stefanfrings::HttpRequest&,
                    stefanfrings::HttpResponse&) const;

 private:
  static const QByteArray kAction;
  static const QString kWindowNumber;
  mutable std::mutex ids_locker_;

  bool validateIndexRequest(stefanfrings::HttpRequest& request) const noexcept;
  bool validateUpdateRequest(stefanfrings::HttpRequest& request) const noexcept;
};

#endif  // TICKETCONTROLLER_H
