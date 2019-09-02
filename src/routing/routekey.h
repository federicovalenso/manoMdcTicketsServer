#ifndef ROUTEKEY_H
#define ROUTEKEY_H

#include <QHash>

using Path = QString;
using HttpMethod = QString;

class RouteKey {
 public:
  RouteKey(const Path& path, const HttpMethod& method);

  inline bool operator<(const RouteKey& other) { return other.path() < mPath; }

  Path path() const;
  HttpMethod method() const;

 private:
  Path mPath;
  HttpMethod mMethod;
};

inline bool operator==(const RouteKey& lhs, const RouteKey& rhs) {
  return (lhs.method() == rhs.method()) && (lhs.path() == rhs.path());
}

inline uint qHash(const RouteKey& key, uint seed) {
  return qHash(key.method().left(1) + key.path(), seed);
}

#endif  // ROUTEKEY_H
