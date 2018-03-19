/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <string_view>
#include <tinyconf/config.hpp>

using namespace std::string_view_literals;

struct SubValue
{
    std::string key;
    std::string value;

    template< class Ar >
    void serialize(Ar& ar)
    {
        ar
            & tinyconf::required("key", key, tinyconf::nonEmpty())
            & tinyconf::optional("value", value, ""sv);
    }
};

struct Entry
{
    std::string value1;
    int value2;
    double value3;
    SubValue value4;
    SubValue value5;
    std::vector< int > value6;
    std::array< int, 3 > value7;

    template< class Ar >
    void serialize(Ar& ar)
    {
        ar
            & tinyconf::required("value1", value1, tinyconf::endsWith("lo"))
            & tinyconf::required("value2", value2, tinyconf::oneOf({100, 101, 150}))
            & tinyconf::optional("value3", value3, 5.1)
            & tinyconf::required("value4", value4)
            & tinyconf::optional("value5", value5, SubValue{"demo", "none"})
            & tinyconf::required("value6", value6, tinyconf::nonEmpty())
            & tinyconf::required("value7", value7);
    }
};

static constexpr auto text = R"(
{
    "value1": "hello",
    "value2": 101,
    "value4": {
        "key": "test",
        "value": "123"
    },
    "value6": [ 1, 3, 4, 6 ],
    "value7": [ 255, 15, 1 ]
})"sv;

int main(int argc, char* argv[])
{
    try {
        Entry entry;
        tinyconf::fromJsonString(text, entry);
        std::cout << tinyconf::toJsonString(entry) << '\n';

        entry.value5.key = "-_-_-_-_-_";
        std::cout << tinyconf::toJsonString(entry) << '\n';

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
