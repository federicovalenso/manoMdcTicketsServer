#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "modelcontroller.h"

class UserController : public ModelController
{
public:
    UserController();

    virtual void show(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &) override;

    static const QByteArray SESSION_PARAM_USER;
    static const QByteArray SESSION_PARAM_LOGINTIME;
};

#endif // USERCONTROLLER_H
