#ifndef CHECKRULE_H
#define CHECKRULE_H

#include "validationrule.h"

class IntRule : public ValidationRule
{
public:
    IntRule(const QByteArray& parameter, int value);
    virtual bool Validate() const override;

private:
    int value_;
};

#endif // CHECKRULE_H
