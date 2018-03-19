/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_policy_120318170542
#define KSERGEY_policy_120318170542

#include "exception.hpp"

namespace TinyConfig {

/// Field required policy
struct PolicyRequired
{
    template< class Field >
    void operator()(const std::string& fieldName, Field&) const
    {
        throw PolicyError{"Mandatory field \"" + fieldName + "\" not exists"};
    }
};

/// Field optional policy
template< class FieldDefault >
class PolicyOptional
{
private:
    FieldDefault defaultValue_;

public:
    PolicyOptional(FieldDefault&& value)
        : defaultValue_{std::forward< FieldDefault >(value)}
    {}

    template< class Field >
    void operator()(const std::string&, Field& field) const
    {
        // TODO: static_assert is_convertible ?
        field = defaultValue_;
    }
};

} /* namespace TinyConfig */

#endif /* KSERGEY_policy_120318170542 */
