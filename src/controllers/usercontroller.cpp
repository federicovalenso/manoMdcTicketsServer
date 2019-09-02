#include <QTime>

#include "entities/user.h"
#include "models/usermodel.h"
#include "requestmapper.h"
#include "usercontroller.h"

const QByteArray UserController::kUserId = "user_id";
const QByteArray UserController::kUserName = "user_name";
const QByteArray UserController::kLoginTime = "logintime";

void UserController::show(stefanfrings::HttpRequest& request,
                          stefanfrings::HttpResponse& response) const {
  response.setHeader("Content-Type", "text/html; charset=utf-8");
  qDebug() << request.getHeaderMap();
  auto name = request.getParameter("name");
  auto password = request.getParameter("password");

  if (auto user = UserModel().authorize(name, password); user) {
    response.setStatus(200, "Correct login pair");
    stefanfrings::HttpSession session =
        RequestMapper::sessionStore->getSession(request, response, true);
    session.set(kUserId, user->id);
    session.set(kUserName, user->name);
    session.set(kLoginTime, QTime::currentTime());
  } else {
    setUnauthorizedError(response);
  }
}
