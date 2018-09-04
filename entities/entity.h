#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <cinttypes>

struct Entity
{
    int id = -1;

    inline bool isValid()
    {
        return id != -1 ? true : false;
    }
};

#endif // ENTITY_H
