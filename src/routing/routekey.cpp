#include "routekey.h"

RouteKey::RouteKey(const Path& path, const HttpMethod& method)
    : mPath(path), mMethod(method) {}

Path RouteKey::path() const { return mPath; }

HttpMethod RouteKey::method() const { return mMethod; }
