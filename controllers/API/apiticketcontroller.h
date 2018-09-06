#ifndef APITICKETCONTROLLER_H
#define APITICKETCONTROLLER_H

#include <controllers/modelcontroller.h>

class ApiTicketController : public ModelController
{
public:
    virtual void update(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response) override;

private:
    bool validateRequest(stefanfrings::HttpRequest &request);
};

#endif // APITICKETCONTROLLER_H
