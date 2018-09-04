#ifndef TICKETACTION_H
#define TICKETACTION_H

#include "entity.h"

struct TicketAction : public Entity
{
    QString name;
    QString prefix;
};

#endif // TICKETACTION_H
