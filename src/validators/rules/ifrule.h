#ifndef IFRULE_H
#define IFRULE_H

#include "validationrule.h"

template <class _If, class _Then = AlwaysTrueRule, class _Else = AlwaysFalseRule>
class IfRule : public ValidationRule
{
public:
    IfRule(const QByteArray& ifParameter = "",
           const QByteArray& thenParameter = "",
           const QByteArray& elseParameter = "") :
        ValidationRule(),
        ifRule_(ifParameter),
        thenRule_(thenParameter),
        elseRule_(elseParameter)
    {}

    IfRule(_If ifRule, _Then thenRule = _Then(), _Else elseRule = _Else()) :
        ValidationRule(),
        ifRule_(std::move(ifRule)),
        thenRule_(std::move(thenRule)),
        elseRule_(std::move(elseRule))
    {}

    virtual bool Validate() const override
    {
        bool result = false;
        if (ifRule_.Validate()) {
            if (thenRule_.Validate()) {
                result = true;
            }
        } else {
            if (elseRule_.Validate()) {
                result = true;
            }
        }
        return result;
    }

private:
    _If ifRule_;
    _Then thenRule_;
    _Else elseRule_;
};

#endif // IFRULE_H
