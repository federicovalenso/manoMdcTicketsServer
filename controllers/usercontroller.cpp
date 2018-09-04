#include <stdexcept>
#include <QTime>
#include "usercontroller.h"
#include "models/usermodel.h"
#include "entities/user.h"

UserController::UserController()
{}

const QByteArray UserController::SESSION_PARAM_USER = "user";
const QByteArray UserController::SESSION_PARAM_LOGINTIME = "logintime";

extern stefanfrings::HttpSessionStore* sessionStore;

void UserController::show(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response)
{
    response.setHeader("Content-Type", "text/html; charset=utf-8");
    auto name = request.getParameter("name");
    auto password = request.getParameter("password");

    try {
        User user = UserModel().authorize(name, password);
        if (user.id != -1) {
            response.setStatus(200, "Correct login pair");
            stefanfrings::HttpSession session = sessionStore->getSession(request, response, true);
            session.set(SESSION_PARAM_USER, name);
            session.set(SESSION_PARAM_LOGINTIME, QTime::currentTime());
        } else {
            response.setStatus(401, "Incorrect login pair");
        }
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}
