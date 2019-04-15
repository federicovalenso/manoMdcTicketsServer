#include "windowrule.h"

WindowRule::WindowRule(const QByteArray &parameter)
    : ValidationRule(parameter) {}

bool WindowRule::Validate() const {
  bool result = false;
  if (parameter_.isEmpty() == false) {
    bool conversion_res = false;
    auto value = parameter_.toInt(&conversion_res);
    if (conversion_res && value >= 1 && value <= 8) {
      result = true;
    }
  }
  return result;
}
