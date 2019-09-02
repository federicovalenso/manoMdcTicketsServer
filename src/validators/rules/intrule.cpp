#include "intrule.h"

IntRule::IntRule(const QByteArray &parameter, int value) :
    ValidationRule (parameter),
    value_(value)
{}

bool IntRule::Validate() const
{
    return QByteArray::number(value_) == parameter_;
}
