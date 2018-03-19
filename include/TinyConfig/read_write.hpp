/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_read_write_120318173256
#define KSERGEY_read_write_120318173256

#include <type_traits>
#include <vector>
#include <array>
#include "io.hpp"

namespace TinyConfig {
namespace detail {

template< class T, class = std::void_t<> >
struct HasImplicitConversion
    : std::false_type
{};

template< class T >
struct HasImplicitConversion< T,
    std::void_t< decltype(nlohmann::detail::from_json(std::declval< const nlohmann::json& >(), std::declval< T& >())) >
>
    : std::true_type
{};

template< class T, class DTO, class = std::void_t<> >
struct HasSerialize
    : std::false_type
{};

template< class T, class DTO >
struct HasSerialize< T, DTO, std::void_t< decltype(std::declval< T >().serialize(std::declval< DTO& >())) > >
    : std::true_type
{};

} /* namespace detail */

template< class T >
inline void readValue(const nlohmann::json& json, T& value,
        std::enable_if_t< detail::HasImplicitConversion< T >::value >* = nullptr)
{
    try {
        value = json;
    } catch (const nlohmann::detail::type_error& error) {
        throw TypeError{error.what()};
    }
}

template< class T >
inline void writeValue(const T& value, nlohmann::json& json,
        std::enable_if_t< detail::HasImplicitConversion< T >::value >* = nullptr)
{
    try {
        json = value;
    } catch (const nlohmann::detail::type_error& error) {
        throw TypeError{error.what()};
    }
}

template< class T >
inline void readValue(const nlohmann::json& json, T& value,
        std::enable_if_t< detail::HasSerialize< T, Input >::value >* = nullptr)
{
    Input input{json};
    serialize(input, value);
}

template< class T >
inline void writeValue(const T& value, nlohmann::json& json,
        std::enable_if_t< detail::HasSerialize< T, Output >::value >* = nullptr)
{
    Output output{json};
    serialize(output, const_cast< T& >(value));
}

template< class T, class Allocator >
inline void readValue(const nlohmann::json& json, std::vector< T, Allocator >& value)
{
    if (json.is_array()) {
        value.clear();
        value.reserve(json.size());
        for (auto& node: json) {
            value.emplace_back();
            readValue(node, value.back());
        }
    } else {
        throw TypeError{"JSON node is not an array"};
    }
}

template< class T, class Allocator >
inline void writeValue(const std::vector< T, Allocator >& value, nlohmann::json& json)
{
    json = nlohmann::json::array();

    for (auto& entry: value) {
        nlohmann::json node;
        writeValue(entry, node);
        json.push_back(node);
    }
}

template< class T, std::size_t N >
inline void readValue(const nlohmann::json& json, std::array< T, N >& value)
{
    if (json.is_array()) {
        if (N != json.size()) {
            throw TypeError{"JSON node is not an array of required size"};
        }

        for (std::size_t i = 0; i < N; ++i) {
            readValue(json[i], value[i]);
        }
    } else {
        throw TypeError{"JSON node is not an array"};
    }
}

template< class T, std::size_t N >
inline void writeValue(const std::array< T, N >& value, nlohmann::json& json)
{
    json = nlohmann::json::array();

    for (std::size_t i = 0; i < N; ++i) {
        nlohmann::json node;
        writeValue(value[i], node);
        json.push_back(node);
    }
}

} /* namespace TinyConfig */

#endif /* KSERGEY_read_write_120318173256 */
