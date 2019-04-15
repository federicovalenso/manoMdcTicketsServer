#include <QTime>

#include "entities/user.h"
#include "models/usermodel.h"
#include "requestmapper.h"
#include "usercontroller.h"

UserController::UserController() {}

const QByteArray UserController::SESSION_PARAM_USER = "user";
const QByteArray UserController::SESSION_PARAM_LOGINTIME = "logintime";

void UserController::show(stefanfrings::HttpRequest& request,
                          stefanfrings::HttpResponse& response) {
  response.setHeader("Content-Type", "text/html; charset=utf-8");
  qDebug() << request.getHeaderMap();
  auto name = request.getParameter("name");
  auto password = request.getParameter("password");

  if (UserModel().authorize(name, password)) {
    response.setStatus(200, "Correct login pair");
    stefanfrings::HttpSession session =
        RequestMapper::sessionStore->getSession(request, response, true);
    session.set(SESSION_PARAM_USER, name);
    session.set(SESSION_PARAM_LOGINTIME, QTime::currentTime());
  } else {
    setUnauthorizedError(response);
  }
}
