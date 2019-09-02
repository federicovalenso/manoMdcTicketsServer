#ifndef WINDOWRULE_H
#define WINDOWRULE_H

#include "validationrule.h"

class WindowRule : public ValidationRule
{
public:
    WindowRule(const QByteArray& parameter);
    virtual bool Validate() const override;
};

#endif // WINDOWRULE_H
