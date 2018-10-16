#ifndef EMPTYVALUERULE_H
#define EMPTYVALUERULE_H

#include "validationrule.h"

class EmptyValueRule : public ValidationRule
{
public:
    EmptyValueRule(const QByteArray& parameter);
    virtual bool Validate() const override;
};

#endif // EMPTYVALUERULE_H
