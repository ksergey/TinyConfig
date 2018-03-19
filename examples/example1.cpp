/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <string_view>
#include <TinyConfig/config.hpp>

using namespace std::string_view_literals;

struct SubValue
{
    std::string key;
    std::string value;

    template< class Ar >
    void serialize(Ar& ar)
    {
        ar
            & TinyConfig::required("key", key, TinyConfig::nonEmpty())
            & TinyConfig::optional("value", value, ""sv);
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
            & TinyConfig::required("value1", value1, TinyConfig::endsWith("lo"))
            & TinyConfig::required("value2", value2, TinyConfig::oneOf({100, 101, 150}))
            & TinyConfig::optional("value3", value3, 5.1)
            & TinyConfig::required("value4", value4)
            & TinyConfig::optional("value5", value5, SubValue{"demo", "none"})
            & TinyConfig::required("value6", value6, TinyConfig::nonEmpty())
            & TinyConfig::required("value7", value7);
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
        TinyConfig::fromJsonString(text, entry);
        std::cout << TinyConfig::toJsonString(entry) << '\n';

        entry.value5.key = "-_-_-_-_-_";
        std::cout << TinyConfig::toJsonString(entry) << '\n';

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
