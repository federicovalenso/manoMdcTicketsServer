#ifndef IDRULE_H
#define IDRULE_H

#include "validationrule.h"

class IdRule : public ValidationRule
{
public:
    IdRule(const QByteArray &parameter);
    virtual bool Validate() const override;
};

#endif // IDRULE_H
