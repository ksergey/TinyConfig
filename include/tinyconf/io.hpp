/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_io_120318172355
#define KSERGEY_io_120318172355

#include "json.hpp"

namespace tinyconf {

/// Read from JSON helper
class Input
{
private:
    const nlohmann::json& json_;

public:
    Input(const nlohmann::json& json)
        : json_{json}
    {}

    template< class Binder >
    Input& operator&(const Binder& binder)
    {
        binder.read(json_);
        return *this;
    }
};

/// Write to JSON helper
class Output
{
private:
    nlohmann::json& json_;

public:
    Output(nlohmann::json& json)
        : json_{json}
    {}

    template< class Binder >
    Output& operator&(const Binder& binder)
    {
        binder.write(json_);
        return *this;
    }
};

template< class IO, class DTO >
inline void serialize(IO& io, DTO& dto)
{
    dto.serialize(io);
}

} /* namespace tinyconf */

#endif /* KSERGEY_io_120318172355 */
