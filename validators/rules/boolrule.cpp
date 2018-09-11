#include <QIntValidator>
#include "boolrule.h"

BoolRule::BoolRule(const QByteArray &parameter) :
    ValidationRule (parameter)
{}

bool BoolRule::Validate() const
{
    bool result = false;
    if (parameter_.isEmpty() == false) {
        QIntValidator int_validator(0, 1);
        QString value(parameter_);
        int pos = 0;
        if (int_validator.validate(value, pos) == QValidator::State::Acceptable) {
            result = true;
        }
    }
    return result;
}
