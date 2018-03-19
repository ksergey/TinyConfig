/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_checker_120318174207
#define KSERGEY_checker_120318174207

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <set>
#include <string>
#include "exception.hpp"

namespace TinyConfig {
namespace detail {

/// STL-based checker
template< class T, template< class > class C >
class StlComparator
{
private:
    T value_;

public:
    explicit StlComparator(const T& value = 0)
        : value_(value)
    {}

    bool operator()(const T& value) const noexcept
    {
        return C< T >{}(value, value_);
    }
};

/// Check value inside of a range.
template< class T, template< class > class Left, template< class > class Right >
class InsideChecker
{
private:
    const T left_;
    const T right_;

public:
    InsideChecker(const T& left, const T& right)
        : left_(left)
        , right_(right)
    {}

    bool operator()(const T& value) const noexcept
    {
        return Left< T >{}(value) && Right< T >{}(value);
    }
};

} /* namespace detail */

/// Checker which no checks
struct NullChecker
{
    template< class Field >
    constexpr bool operator()( const Field&) const
    {
        return true;
    }
};

template< class T >
auto greater(const T& value)
{
    return detail::StlComparator< T, std::greater >(value);
}

template< class T >
auto greaterEqual(const T& value)
{
    return detail::StlComparator< T, std::greater_equal >(value);
}

template< class T >
auto less(const T& value)
{
    return detail::StlComparator< T, std::less >(value);
}

template< class T >
auto lessEqual(const T& value)
{
    return detail::StlComparator< T, std::less_equal >(value);
}

template< class T >
auto notEqualTo(const T& value)
{
    return detail::StlComparator< T, std::not_equal_to >(value);
}

template< class T >
auto inside(const T& left, const T& right)
{
    return detail::InsideChecker< T, std::greater_equal, std::less_equal >(left, right);
}

template< class T >
auto strongInside(const T& left, const T& right)
{
    return detail::InsideChecker< T, std::greater, std::less >(left, right);
}

inline auto nonEmpty()
{
    return [](const auto& s) -> bool {
        return !s.empty();
    };
}

inline auto endsWith(std::string_view needle)
{
    return [needle](std::string_view haystack) -> bool {
        if (needle.size() > haystack.size()) {
            return false;
        }
        return std::equal(needle.rbegin(), needle.rend(), haystack.rbegin());
    };
}

inline auto maxLength(std::size_t count)
{
    return [count](const auto& value) -> bool {
        return value.size() <= count;
    };
}

template< class T >
auto oneOf(std::initializer_list< T > values)
{
    const std::set< T > storage{values};
    return [storage](const T& value) -> bool {
        return storage.count(value);
    };
}

} /* namespace TinyConfig */

#endif /* KSERGEY_checker_120318174207 */
