#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QObject>
#include <QString>
#include <QDebug>

#include "httpsessionstore.h"
#include "httprequesthandler.h"

class ModelController : public QObject
{
public:
    ModelController();
    virtual void store(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void index(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void create(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void destroy(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void update(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void show(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);
    virtual void edit(stefanfrings::HttpRequest&, stefanfrings::HttpResponse&);

protected:
    void setClientError(stefanfrings::HttpResponse&) noexcept;
    void setServerError(stefanfrings::HttpResponse&) noexcept;
};

#endif // MODELCONTROLLER_H
