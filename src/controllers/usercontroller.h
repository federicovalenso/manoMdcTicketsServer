#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "modelcontroller.h"

class UserController : public ModelController {
 public:
  static const QByteArray kUserId;
  static const QByteArray kUserName;
  static const QByteArray kLoginTime;

  UserController() = default;

  virtual void show(stefanfrings::HttpRequest &,
                    stefanfrings::HttpResponse &) const override;
};

#endif  // USERCONTROLLER_H
