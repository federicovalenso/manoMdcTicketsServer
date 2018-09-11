#include <QDebug>
#include "validationrule.h"

ValidationRule::ValidationRule(const QByteArray& parameter) :
    parameter_(parameter)
{}

AlwaysTrueRule::AlwaysTrueRule(const QByteArray&) :
    ValidationRule ()
{}

bool AlwaysTrueRule::Validate() const
{
    return true;
}

AlwaysFalseRule::AlwaysFalseRule(const QByteArray &) :
    ValidationRule ()
{}

bool AlwaysFalseRule::Validate() const
{
    return false;
}
