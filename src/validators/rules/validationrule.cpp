#include "validationrule.h"
#include <QDebug>

ValidationRule::ValidationRule(const QByteArray& parameter)
    : parameter_(parameter) {}

ValidationRule::operator bool() { return Validate(); }

bool TrueRule::Validate() const { return true; }

bool FalseRule::Validate() const { return false; }
