/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_config_190318112033
#define KSERGEY_config_190318112033

#include <iostream>
#include <string_view>
#include "binder.hpp"
#include "checker.hpp"

namespace TinyConfig {

/// Read/write field as required
template< class Field, class Checker = NullChecker >
inline auto required(std::string fieldName, Field& field, Checker&& checker = {})
{
    return Binder< Field, PolicyRequired, Checker >{
        std::move(fieldName), field, PolicyRequired{}, std::forward< Checker >(checker)
    };
}

/// Read/write field as optional
template< class Field, class FieldDefault, class Checker = NullChecker >
inline auto optional(std::string fieldName, Field& field, FieldDefault&& defaultValue, Checker&& checker = {})
{
    using Policy = PolicyOptional< FieldDefault >;
    return Binder< Field, Policy, Checker >{
        std::move(fieldName), field, Policy{std::forward< FieldDefault >(defaultValue)}, std::forward< Checker >(checker)
    };
}

/// Read an object from JSON
template< class T >
inline void fromJsonString(std::string_view str, T& object)
{
    auto json = nlohmann::json::parse(str);
    Input in{json};
    serialize(in, object);
}

/// Read an object from std::istream
template< class T >
inline void fromJsonStream(std::istream& is, T& object)
{
    nlohmann::json json;
    is >> json;
    Input in{json};
    serialize(in, object);
}

/// Write an object to JSON
template< class T >
inline std::string toJsonString(const T& object, int indent = 2)
{
    nlohmann::json json;
    Output output{json};
    serialize(output, const_cast< T& >(object));
    return json.dump(indent);
}

} /* namespace TinyConfig */

#endif /* KSERGEY_config_190318112033 */
