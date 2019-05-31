#include "validationrule.h"
#include <QDebug>

ValidationRule::ValidationRule(const QByteArray& parameter)
    : parameter_(parameter) {}

ValidationRule::operator bool() { return Validate(); }

AlwaysTrueRule::AlwaysTrueRule(const QByteArray&) : ValidationRule() {}

bool AlwaysTrueRule::Validate() const { return true; }

AlwaysFalseRule::AlwaysFalseRule(const QByteArray&) : ValidationRule() {}

bool AlwaysFalseRule::Validate() const { return false; }
