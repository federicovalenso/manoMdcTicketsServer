#include "requestvalidator.h"

bool RequestValidator::Validate() const noexcept
{
    bool result = true;
    for (const auto& rule : rules) {
        result = result && rule->Validate();
    }
    return result;
}
