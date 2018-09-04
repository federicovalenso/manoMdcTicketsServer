#ifndef USER_H
#define USER_H

#include "entities/entity.h"

struct User : public Entity
{
    QString name;
};

#endif // USER_H
