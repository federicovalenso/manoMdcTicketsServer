#ifndef VALIDATIONRULE_H
#define VALIDATIONRULE_H

#include <memory>
#include <QByteArray>

class ValidationRule
{
public:
    ValidationRule(const QByteArray& parameter = "");
    virtual bool Validate() const = 0;
    inline QByteArray getParameter() const noexcept
    {
        return  parameter_;
    }

    template <class _Rule>
    static std::unique_ptr<_Rule> createRule(const QByteArray& parameter)
    {
        return std::make_unique<_Rule>(parameter);
    }

    template <class _Rule>
    static std::unique_ptr<_Rule> createRule(const _Rule& rule)
    {
        return std::make_unique<_Rule>(rule);
    }

protected:
    QByteArray parameter_;
};

class AlwaysTrueRule : public ValidationRule
{
public:
    AlwaysTrueRule(const QByteArray& parameter = "");
    virtual bool Validate() const override;
};

class AlwaysFalseRule : public ValidationRule
{
public:
    AlwaysFalseRule(const QByteArray& parameter = "");
    virtual bool Validate() const override;
};

#endif // VALIDATIONRULE_H
