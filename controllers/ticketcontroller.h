#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include "modelcontroller.h"

class TicketController : public ModelController
{
public:
    TicketController();
    virtual void index(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &) override;
    virtual void store(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response) override;
    virtual void show(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &) override;
    virtual void update(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &) override;

private:
    static const QByteArray USER_NAME_PARAM;
    static const QByteArray ACTION_PARAM;
    static const QString TICKET_NUMBER;
    static const QString WINDOW_NUMBER;
    bool validateRequest(stefanfrings::HttpRequest &);

};

#endif // TICKETCONTROLLER_H
