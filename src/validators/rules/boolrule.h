#ifndef BOOLRULE_H
#define BOOLRULE_H

#include "validationrule.h"

class BoolRule : public ValidationRule
{
public:
    BoolRule(const QByteArray& parameter);
    virtual bool Validate() const override;
};

#endif // BOOLRULE_H
