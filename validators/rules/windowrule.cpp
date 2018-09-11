#include <QIntValidator>
#include "windowrule.h"

WindowRule::WindowRule(const QByteArray &parameter) :
    ValidationRule (parameter)
{}

bool WindowRule::Validate() const
{
    bool result = false;
    if (parameter_.isEmpty() == false) {
        QIntValidator int_validator(1, 8);
        QString window(parameter_);
        int pos = 0;
        if (int_validator.validate(window, pos) == QValidator::State::Acceptable) {
            result = true;
        }
    }
    return result;
}
