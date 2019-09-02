#include "route.h"

Route::Route(ControllerPtr controller_ptr, Action action)
    : controller_ptr_(controller_ptr), action_(action) {}

void Route::execute(stefanfrings::HttpRequest& request,
                    stefanfrings::HttpResponse& response) const {
  action_(controller_ptr_, request, response);
}
