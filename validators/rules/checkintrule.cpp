#include "checkintrule.h"

CheckIntRule::CheckIntRule(const QByteArray &parameter, int value) :
    ValidationRule (parameter),
    value_(value)
{}

bool CheckIntRule::Validate() const
{
    return QByteArray::number(value_) == parameter_;
}
