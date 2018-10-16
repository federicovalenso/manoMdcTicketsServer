#ifndef TICKET_H
#define TICKET_H

#include "entity.h"

struct Ticket : public Entity
{
    int number_by_action = 0;
    QString action;
    int user_id = -1;
    qint64 created_at = 0;
    qint64 updated_at = 0;
    bool on_service = false;
    bool is_done = false;
    bool is_voiced = false;
    bool is_manual = false;
    int window_number = -1;
};

#endif // TICKET_H
