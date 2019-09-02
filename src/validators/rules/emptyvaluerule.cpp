#include "emptyvaluerule.h"

EmptyValueRule::EmptyValueRule(const QByteArray& parameter) :
    ValidationRule (parameter)
{}

bool EmptyValueRule::Validate() const
{
    return parameter_.isEmpty();
}
