#ifndef VALIDATIONRULE_H
#define VALIDATIONRULE_H

#include <QByteArray>

template <class... Rules>
inline static bool Validate(Rules... rules) {
  return (... && rules);
}

class ValidationRule {
 public:
  ValidationRule(const QByteArray& parameter = "");
  virtual ~ValidationRule() = default;
  virtual bool Validate() const = 0;
  operator bool();
  inline QByteArray getParameter() const noexcept { return parameter_; }

 protected:
  QByteArray parameter_;
};

class TrueRule : public ValidationRule {
 public:
  TrueRule() = default;
  virtual bool Validate() const override;
};

class FalseRule : public ValidationRule {
 public:
  FalseRule() = default;
  virtual bool Validate() const override;
};

#endif  // VALIDATIONRULE_H
