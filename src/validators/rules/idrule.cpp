#include "idrule.h"

IdRule::IdRule(const QByteArray& parameter) : ValidationRule(parameter) {}

bool IdRule::Validate() const {
  bool result = false;
  if (parameter_.isEmpty() == false) {
    bool conversion_res = false;
    auto value = parameter_.toInt(&conversion_res);
    if (conversion_res && value >= 1) {
      result = true;
    }
  }
  return result;
}
