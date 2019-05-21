#include <QTime>

#include "entities/user.h"
#include "models/usermodel.h"
#include "requestmapper.h"
#include "usercontroller.h"

UserController::UserController() {}

const QByteArray UserController::SESSION_PARAM_USER_ID = "user_id";
const QByteArray UserController::SESSION_PARAM_USER_NAME = "user_name";
const QByteArray UserController::SESSION_PARAM_LOGINTIME = "logintime";

void UserController::show(stefanfrings::HttpRequest& request,
                          stefanfrings::HttpResponse& response) {
  response.setHeader("Content-Type", "text/html; charset=utf-8");
  qDebug() << request.getHeaderMap();
  auto name = request.getParameter("name");
  auto password = request.getParameter("password");

  if (auto user = UserModel().authorize(name, password); user) {
    response.setStatus(200, "Correct login pair");
    stefanfrings::HttpSession session =
        RequestMapper::sessionStore->getSession(request, response, true);
    session.set(SESSION_PARAM_USER_ID, user->id);
    session.set(SESSION_PARAM_USER_NAME, user->name);
    session.set(SESSION_PARAM_LOGINTIME, QTime::currentTime());
  } else {
    setUnauthorizedError(response);
  }
}
