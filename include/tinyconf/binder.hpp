/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_binder_120318164518
#define KSERGEY_binder_120318164518

#include <string>
#include "json.hpp"
#include "policy.hpp"
#include "read_write.hpp"

namespace tinyconf {

/// Field binder
/// Write value itno json and perform field value checks during i/o
template< class Field, class Policy, class Checker >
class Binder
{
private:
    const std::string fieldName_;
    bool secret_{false};
    Field& field_;
    Policy policy_;
    Checker checker_;

public:
    Binder(const Binder&) = delete;
    Binder& operator=(const Binder&) = delete;

    /// Constructor
    Binder(std::string fieldName, Field& field, Policy&& policy, Checker&& checker);

    /// Make field secret for writing
    Binder& setSecret() noexcept;

    /// Read field value
    void read(const nlohmann::json& json) const;

    /// Write field value
    void write(nlohmann::json& json) const;
};

template< class Field, class Policy, class Checker >
inline Binder< Field, Policy, Checker >::Binder(std::string fieldName, Field& field, Policy&& policy, Checker&& checker)
    : fieldName_{std::move(fieldName)}
    , field_{field}
    , policy_{std::move(policy)}
    , checker_{std::move(checker)}
{}

template< class Field, class Policy, class Checker >
inline Binder< Field, Policy, Checker >& Binder< Field, Policy, Checker >::setSecret() noexcept
{
    secret_ = true;
    return *this;
}

template< class Field, class Policy, class Checker >
inline void Binder< Field, Policy, Checker >::read(const nlohmann::json& json) const
{
    auto found = json.find(fieldName_);
    if (found != json.end()) {
        const nlohmann::json& value = found.value();
        if (!value.is_null()) {
            // TODO: make readValue return bool and thdow exception here
            // TODO: rename to fromJson?

            // readValue is free function
            readValue(value, field_);
        } else {
            throw TypeError{"JSON value is null (\"" + fieldName_ + "\")"};
        }
    } else {
        // Field not exists, what to do kwown policy
        policy_(fieldName_, field_);
    }

    if (!checker_(field_)) {
        throw CheckError{"Value for field \"" + fieldName_ + "\" not passed validation"};
    }
}

template< class Field, class Policy, class Checker >
inline void Binder< Field, Policy, Checker >::write(nlohmann::json& json) const
{
    if (!checker_(field_)) {
        throw CheckError{"Value for field \"" + fieldName_ + "\" not passed validation"};
    }

    if (!secret_) {
        nlohmann::json entry;
        // TODO: make writeValue return bool and thdow exception here
        // TODO: rename to toJson?
        // writeValue is free function
        writeValue(field_, entry);

        json[fieldName_] = entry;
    } else {
        json[fieldName_] = "<-- hidden -->";
    }
}

} /* namespace tinyconf */

#endif /* KSERGEY_binder_120318164518 */
