#include "boolrule.h"

BoolRule::BoolRule(const QByteArray &parameter) : ValidationRule(parameter) {}

bool BoolRule::Validate() const {
  bool result = false;
  if (!parameter_.isEmpty()) {
    bool conversion_res = false;
    auto value = parameter_.toInt(&conversion_res);
    if (conversion_res) {
      if (value == 0 || value == 1) {
        result = true;
      }
    }
  }
  return result;
}
