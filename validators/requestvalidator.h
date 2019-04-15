#ifndef REQUESTVALIDATOR_H
#define REQUESTVALIDATOR_H

#include <memory>
#include <vector>

#include <QByteArray>
#include <QMultiMap>

#include "rules/validationrule.h"

class RequestValidator {
 public:
  template <class _Rule>
  RequestValidator& AddRule(_Rule rule) {
    rules.push_back(ValidationRule::createRule<_Rule>(std::move(rule)));
    return *this;
  }
  bool Validate() const noexcept;

 private:
  std::vector<std::unique_ptr<ValidationRule>> rules;
};

#endif  // REQUESTVALIDATOR_H
