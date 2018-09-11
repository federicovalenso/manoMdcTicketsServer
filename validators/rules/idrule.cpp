#include <QIntValidator>
#include "idrule.h"

IdRule::IdRule(const QByteArray& parameter) :
    ValidationRule (parameter)
{}

bool IdRule::Validate() const
{
    bool result = false;
    if (parameter_.isEmpty() == false) {
        QIntValidator int_validator;
        int_validator.setBottom(1);
        QString id(parameter_);
        int pos = 0;
        if (int_validator.validate(id, pos) == QValidator::State::Acceptable) {
            result = true;
        }
    }
    return result;
}
